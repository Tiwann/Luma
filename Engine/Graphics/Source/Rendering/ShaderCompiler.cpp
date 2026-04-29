#include "Luma/Rendering/ShaderCompiler.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Utility/SlangCommon.h"
#include "Luma/Utility/SpirvReflectCommon.h"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>
#include <spirv_reflect.h>



namespace Luma
{
    FStringView getErrorString(slang::IBlob* blob)
    {
        const FStringView errorString = {(const char*)blob->getBufferPointer(), blob->getBufferSize()};
        return errorString;
    }

    FShaderCompiler::FShaderCompiler(slang::IGlobalSession* globalSession)
        : m_GlobalSession(globalSession)
    {
    }

    TArray<FShaderCompileResult> FShaderCompiler::compile()
    {
        if (m_Requests.isEmpty()) return {};
        TArray<FShaderCompileResult> results;

        for (const auto& request : m_Requests)
        {
            slang::TargetDesc shaderTargetDesc;
            shaderTargetDesc.format = getCompileTarget(request.getTarget());
            shaderTargetDesc.floatingPointMode = SLANG_FLOATING_POINT_MODE_DEFAULT;
            shaderTargetDesc.lineDirectiveMode = SLANG_LINE_DIRECTIVE_MODE_DEFAULT;
            shaderTargetDesc.profile = m_GlobalSession->findProfile("spirv_1_5");

            const auto toConstChar = [](const FString& includeDir) -> const char* { return *includeDir; };
            TArray<const char*> includes = request.getIncludeDirectories().transform<const char*>(toConstChar);

            TArray<slang::PreprocessorMacroDesc> defines = request.getDefines().transform<slang::PreprocessorMacroDesc>(toPreprocessorMacroDesc);

            slang::SessionDesc sessionDesc;
            sessionDesc.targets = &shaderTargetDesc;
            sessionDesc.targetCount = 1;
            sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
            sessionDesc.searchPaths = includes.data();
            sessionDesc.searchPathCount = includes.count();
            sessionDesc.preprocessorMacros = defines.data();
            sessionDesc.preprocessorMacroCount = defines.count();

            slang::ISession* session = nullptr;
            if (SLANG_FAILED(m_GlobalSession->createSession(sessionDesc, &session)))
            {
                results.add({false});
                continue;
            }

            slang::BlobHandle errorBlob = nullptr;
            const auto& [moduleName, filepath] = request.getModuleInfo();
            slang::ModuleHandle module = slang::ModuleHandle(session->loadModuleFromSource(
                    *moduleName,
                    *filepath,
                    nullptr,
                    errorBlob.writeRef()));

            if (!module)
            {
                results.add({false});
                continue;
            }

            TArray<slang::EntryPointHandle> entryPoints;
            for (const auto& [name, stage] : request.getEntryPointInfos())
            {
                slang::EntryPointHandle entryPoint = nullptr;
                if (SLANG_FAILED(module->findEntryPointByName(*name, entryPoint.writeRef())))
                {
                    results.add({false});
                    continue;
                }
                entryPoints.addUnique(entryPoint);
            }

            slang::ComponentHandle program = nullptr;
            TArray<slang::IComponentType*> entryPointsAsComponent = entryPoints.transform<slang::IComponentType*>(
                [](const auto& ep) { return ep.get(); });
            if (SLANG_FAILED(session->createCompositeComponentType(entryPointsAsComponent.data(), entryPointsAsComponent.count(), program.writeRef(), errorBlob.writeRef())))
            {
                std::cout << strfmt("Failed to create shader program: {}", getErrorString(errorBlob)) << std::endl;
                results.add({false});
                continue;
            }

            slang::ComponentHandle linkedProgram = nullptr;
            if (SLANG_FAILED(program->link(linkedProgram.writeRef(), errorBlob.writeRef())))
            {
                std::cout << strfmt("Failed to link shader program: {}", getErrorString(errorBlob)) << std::endl;
                results.add({false});
                continue;
            }

            FShaderCompileResult compileResult{true};

            THashMap<uint32_t, THashMap<uint32_t, FShaderBinding>> flattenedBindings;

            for (uint32_t entryPointIndex = 0; entryPointIndex < entryPointsAsComponent.count(); entryPointIndex++)
            {
                const TArray<FShaderEntryPoint>& entryPointInfos = request.getEntryPointInfos();
                const EShaderStageBits stage = entryPointInfos[entryPointIndex].stage;

                slang::BlobHandle entryPointCode = nullptr;
                if (SLANG_FAILED(linkedProgram->getEntryPointCode(entryPointIndex, 0, entryPointCode.writeRef(), errorBlob.writeRef())))
                {
                    std::cout << strfmt("Failed to get entry point code: {}", getErrorString(errorBlob)) << std::endl;
                    results.add({false});
                    continue;
                }

                
                FShaderCompiledData compiledData;
                compiledData.blob = TArray(static_cast<const uint8_t*>(entryPointCode->getBufferPointer()), entryPointCode->getBufferSize());
                compiledData.stage = stage;
                compileResult.compiledData.emplace(std::move(compiledData));

                SpvReflectShaderModule reflectModule;
                spvReflectCreateShaderModule(entryPointCode->getBufferSize(), entryPointCode->getBufferPointer(), &reflectModule);

                TBufferView<SpvReflectDescriptorSet> sets(reflectModule.descriptor_sets, reflectModule.descriptor_binding_count);
                for (const auto& set : sets)
                {
                    auto& bindingMap = flattenedBindings[set.set];

                    TBufferView<SpvReflectDescriptorBinding*> bindings(set.bindings, set.binding_count);
                    for (const auto* binding : bindings)
                    {
                        auto& shaderBinding = bindingMap.emplace(binding->binding);
                        shaderBinding.name = FString(binding->name);
                        shaderBinding.bindingType = getBindingType(binding->descriptor_type);
                        shaderBinding.stageFlags = stage;
                        shaderBinding.bindingIndex = binding->binding;
                        shaderBinding.arrayCount = binding->count;
                        shaderBinding.stageFlags |= stage;
                    }
                }

                TBufferView<SpvReflectInterfaceVariable> interfaceVariables(reflectModule.interface_variables, reflectModule.interface_variable_count);
                for (const auto& interfaceVar : interfaceVariables)
                {

                }
            }

            FShaderReflectionData reflectionData;
            for (auto& [set, bindingMap] : flattenedBindings)
            {
                auto* out = reflectionData.setLayoutDescs.single([&set](const auto& s) { return s.setIndex == set; });
                if (!out)
                {
                    reflectionData.setLayoutDescs.add({});
                    out = &reflectionData.setLayoutDescs.last();
                }

                for (auto& b : bindingMap)
                {
                    out->bindings.add(b.value);
                }
            }

            compileResult.reflectionData.add(reflectionData);
            results.add(compileResult);
        }

        return results;
    }

    void FShaderCompiler::addCompileRequest(const FShaderCompileRequest& compileRequest)
    {
        m_Requests.addUnique(compileRequest);
    }
}
