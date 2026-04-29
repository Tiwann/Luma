#pragma once
#include "BindingSetLayout.h"
#include "ShaderStage.h"
#include "ShadingLanguage.h"
#include "ShaderCompileTarget.h"
#include "ShaderCompileDefine.h"
#include "ShaderPushConstantRange.h"
#include "Luma/Containers/String.h"

namespace Luma
{
    struct FShaderModuleInfo
    {
        FString name;
        FString filepath;
        bool operator==(const FShaderModuleInfo& other) const = default;
    };

    struct FShaderEntryPoint
    {
        FString name;
        EShaderStageBits stage;
        bool operator==(const FShaderEntryPoint& other) const = default;
    };

    struct FInterfaceVariable
    {
        FString name;

    };

    struct FShaderReflectionData
    {
        TArray<FBindingSetLayoutDesc> setLayoutDescs;
        TArray<FShaderPushConstantRange> pushConstantRanges;
    };

    class FShaderCompileRequest
    {
    public:
        void setModuleInfo(const FString& moduleName, const FString& filepath);
        void setCompileTarget(EShaderCompileTarget target);
        void setLanguage(EShadingLanguage language);
        void addDefine(const FString& key, const FString& value);
        void addIncludeDirectory(const FString& includeDirectory);
        void addEntryPoint(const FString& name, EShaderStageBits stage);

        EShadingLanguage getLanguage() const;
        EShaderCompileTarget getTarget() const;
        const TArray<FShaderCompileDefine>& getDefines() const;
        const TArray<FString>& getIncludeDirectories() const;
        const TArray<FShaderEntryPoint>& getEntryPointInfos() const;
        const FShaderModuleInfo& getModuleInfo() const;

        bool operator==(const FShaderCompileRequest& compileRequest) const = default;
    private:
        EShadingLanguage m_Language = EShadingLanguage::Slang;
        EShaderCompileTarget m_Target = EShaderCompileTarget::Auto;
        TArray<FShaderCompileDefine> m_Defines;
        TArray<FString> m_IncludeDirectories;
        FShaderModuleInfo m_ModuleInfo;
        TArray<FShaderEntryPoint> m_EntryPointInfos;
    };
}
