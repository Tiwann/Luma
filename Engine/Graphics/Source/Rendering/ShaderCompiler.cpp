#include "Luma/Rendering/ShaderCompiler.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Utility/SlangCommon.h"
#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

namespace Luma
{
    static void collectResourceBindings(
    slang::VariableLayoutReflection* varLayout,
    uint32_t                         accBinding,
    uint32_t                         accSpace,
    TArray<FShaderReflectionData::FShaderBinding>&    out)
{
    if (!varLayout) return;

    slang::TypeLayoutReflection* typeLayout = varLayout->getTypeLayout();
    if (!typeLayout) return;

    slang::TypeReflection::Kind kind = typeLayout->getKind();

    // ── Determine the "resource" layout unit for this variable ──────────────
    //
    // For Vulkan/SPIR-V targets we look for DescriptorTableSlot (a binding).
    // For D3D targets we look for ShaderResource / UnorderedAccess /
    // ConstantBuffer / SamplerState as appropriate.
    // getCategoryCount()/getCategoryByIndex() tells us which units are used.

    const int catCount = varLayout->getCategoryCount();

    // ── Recurse into structs ─────────────────────────────────────────────────
    if (kind == slang::TypeReflection::Kind::Struct)
    {
        int fieldCount = typeLayout->getFieldCount();
        for (int f = 0; f < fieldCount; ++f)
        {
            slang::VariableLayoutReflection* fieldLayout =
                typeLayout->getFieldByIndex(f);

            // Accumulate the field's own relative offsets
            uint32_t fieldBinding = accBinding;
            uint32_t fieldSpace   = accSpace;

            for (int c = 0; c < fieldLayout->getCategoryCount(); ++c)
            {
                auto cat = fieldLayout->getCategoryByIndex(c);
                if (cat == slang::ParameterCategory::DescriptorTableSlot ||
                    cat == slang::ParameterCategory::ShaderResource       ||
                    cat == slang::ParameterCategory::UnorderedAccess      ||
                    cat == slang::ParameterCategory::ConstantBuffer       ||
                    cat == slang::ParameterCategory::SamplerState)
                {
                    fieldBinding += (uint32_t)fieldLayout->getOffset(cat);
                    fieldSpace   += (uint32_t)fieldLayout->getBindingSpace(cat);
                }
            }

            collectResourceBindings(fieldLayout, fieldBinding, fieldSpace, out);
        }
        return;
    }

    // ── Recurse into ConstantBuffer<T> / ParameterBlock<T> ──────────────────
    if (kind == slang::TypeReflection::Kind::ConstantBuffer ||
        kind == slang::TypeReflection::Kind::ParameterBlock)
    {
        // The "container" part occupies a binding at the accumulated offset.
        // The element type's resources start inside a (possibly new) space.
        uint32_t containerBinding = accBinding;
        uint32_t containerSpace   = accSpace;

        for (int c = 0; c < catCount; ++c)
        {
            auto cat = varLayout->getCategoryByIndex(c);

            if (cat == slang::ParameterCategory::SubElementRegisterSpace)
            {
                containerSpace += (uint32_t)varLayout->getOffset(cat);
            }
            else if (cat == slang::ParameterCategory::DescriptorTableSlot ||
                     cat == slang::ParameterCategory::ConstantBuffer)
            {
                containerBinding += (uint32_t)varLayout->getOffset(cat);
            }
        }

        // Emit a record for the container itself (the CB/PB binding)
        {
            FShaderReflectionData::FShaderBinding binding;
            binding.name = varLayout->getName() ? varLayout->getName() : "<anon>";
            binding.bindingIndex = containerBinding;
            binding.setIndex = containerSpace;
            //binding.bindingType = getBindingType(varLayout->getType()->getElementType)
            out.addUnique(binding);
        }

        // Recurse into the element type using a synthesised variable layout.
        // getElementVarLayout() gives the var-layout for the contained type
        // including its offset relative to the container.
        slang::VariableLayoutReflection* elemVarLayout =
            typeLayout->getElementVarLayout();
        if (elemVarLayout)
        {
            collectResourceBindings(
                elemVarLayout, containerBinding, containerSpace, out);
        }
        return;
    }

    // ── Leaf resource (Texture, Buffer, Sampler, …) ──────────────────────────
    if (kind == slang::TypeReflection::Kind::Resource   ||
        kind == slang::TypeReflection::Kind::SamplerState)
    {
        FShaderReflectionData::FShaderBinding binding;
        binding.name     = varLayout->getName() ? varLayout->getName() : "<anon>";
        binding.bindingIndex  = accBinding;
        binding.setIndex    = accSpace;

        // Pick the most descriptive category reported by the variable
        for (int c = 0; c < catCount; ++c)
        {
            auto cat = varLayout->getCategoryByIndex(c);
            if (cat != slang::ParameterCategory::Uniform)
            {
                // Apply the variable's own relative offset
                binding.bindingIndex  += (uint32_t)varLayout->getOffset(cat);
                binding.setIndex    += (uint32_t)varLayout->getBindingSpace(cat);
                break;
            }
        }

        out.addUnique(binding);
        return;
    }

    // ── Arrays: recurse into element, stepping the binding each time ─────────
    if (kind == slang::TypeReflection::Kind::Array)
    {
        slang::TypeLayoutReflection* elemTypeLayout =
            typeLayout->getElementTypeLayout();
        size_t elemCount = typeLayout->getElementCount();
        bool unbounded   = (elemCount == ~size_t(0));

        // Stride of one element (in bindings)
        uint32_t stride = 0;
        for (int c = 0; c < (int)elemTypeLayout->getCategoryCount(); ++c)
        {
            auto cat = elemTypeLayout->getCategoryByIndex(c);
            if (cat != slang::ParameterCategory::Uniform)
            {
                stride = (uint32_t)elemTypeLayout->getStride(cat);
                break;
            }
        }
        if (stride == 0) stride = 1;

        size_t limit = unbounded ? 1 : elemCount; // report first element for unbounded
        for (size_t i = 0; i < limit; ++i)
        {
            // Build a temporary name with index suffix
            // (real code might pass name down instead)
            collectResourceBindings(
                varLayout,
                accBinding + (uint32_t)(i * stride),
                accSpace,
                out);
        }
        return;
    }
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

            TArray<slang::PreprocessorMacroDesc> defines = request.getDefines().transform<slang::PreprocessorMacroDesc>(
                [](const FShaderCompileDefine& define)
                {
                    slang::PreprocessorMacroDesc desc;
                    desc.name = *define.key;
                    desc.value = *define.value;
                    return desc;
                });

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
                slang::BlobHandle entryPointCode = nullptr;
                if (SLANG_FAILED(linkedProgram->getEntryPointCode(entryPointIndex, 0, entryPointCode.writeRef(), errorBlob.writeRef())))
                {
                    std::cout << strfmt("Failed to get entry point code: {}", getErrorString(errorBlob)) << std::endl;
                    results.add({false});
                    continue;
                }

                const TArray<FShaderEntryPoint>& entryPointInfos = request.getEntryPointInfos();

                FShaderCompiledData compiledData;
                compiledData.blob = TArray(static_cast<const uint8_t*>(entryPointCode->getBufferPointer()), entryPointCode->getBufferSize());
                compiledData.stage = entryPointInfos[entryPointIndex].stage;
                compileResult.compiledData.emplace(std::move(compiledData));

                FShaderReflectionData reflectionData;
                reflectionData.stage = entryPointInfos[entryPointIndex].stage;
                compileResult.reflectionData.emplace(std::move(reflectionData));

                results.add(compileResult);
            }
        }

        return results;
    }

    void FShaderCompiler::addCompileRequest(const FShaderCompileRequest& compileRequest)
    {
        m_Requests.addUnique(compileRequest);
    }
}
