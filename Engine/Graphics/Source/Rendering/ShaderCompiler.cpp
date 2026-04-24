#include "Luma/Rendering/ShaderCompiler.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Containers/BufferView.h"
#include "Luma/Utility/SlangCommon.h"
#include "Luma/Utility/SpirvReflectCommon.h"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>
#include <spirv_reflect.h>



namespace Luma
{
    static TArray<FShaderPushConstantRange> getPushConstantRangesReflection(const SpvReflectShaderModule& module)
    {
        TArray<FShaderPushConstantRange> ranges;

        uint32_t pcBlockCount = 0;
        spvReflectEnumeratePushConstantBlocks(&module, &pcBlockCount, nullptr);
        TArray<SpvReflectBlockVariable*> pushConstantBlocks(pcBlockCount);
        spvReflectEnumeratePushConstantBlocks(&module, &pcBlockCount, pushConstantBlocks.data());

        for (const SpvReflectBlockVariable* block : pushConstantBlocks)
        {
            FShaderPushConstantRange range;
            range.offset = block->offset;
            range.size = block->size;
            range.stage = getShaderStage(module.shader_stage);
            ranges.add(range);
        }

        return ranges;
    }

    static TArray<FBindingSetLayoutDesc> getBindingSetLayoutDescReflection(const SpvReflectShaderModule& module)
    {
        uint32_t setLayoutCount = 0;
        spvReflectEnumerateDescriptorSets(&module, &setLayoutCount, nullptr);
        TArray<SpvReflectDescriptorSet*> sets(setLayoutCount);
        spvReflectEnumerateDescriptorSets(&module, &setLayoutCount, sets.data());

        TArray<FBindingSetLayoutDesc> result;

        for (const SpvReflectDescriptorSet* set : sets)
        {
            TBufferView<SpvReflectDescriptorBinding*> bindings(set->bindings, set->binding_count);

            FBindingSetLayoutDesc setLayoutDesc;
            setLayoutDesc.setIndex = set->set;

            for (const SpvReflectDescriptorBinding* binding : bindings)
            {
                FShaderBinding bindingInfo;
                bindingInfo.bindingIndex = binding->binding;
                bindingInfo.bindingType = getBindingType(binding->descriptor_type);
                bindingInfo.descriptorCount = binding->count;
                bindingInfo.name = binding->name;
                bindingInfo.stageFlags = getShaderStage(module.shader_stage);
                setLayoutDesc.bindings.add(bindingInfo);
            }

            result.add(setLayoutDesc);
        }

        return result;
    }

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

                FShaderReflectionData reflectionData;
                reflectionData.stage = stage;
                reflectionData.pushConstantRanges = getPushConstantRangesReflection(reflectModule);
                reflectionData.setLayoutDescs = getBindingSetLayoutDescReflection(reflectModule);
                compileResult.reflectionData.emplace(std::move(reflectionData));
            }

            results.add(compileResult);
        }

        return results;
    }

    void FShaderCompiler::addCompileRequest(const FShaderCompileRequest& compileRequest)
    {
        m_Requests.addUnique(compileRequest);
    }
}
