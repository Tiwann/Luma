#include "Luma/Rendering/ShaderCompileRequest.h"

namespace Luma
{
    void FShaderCompileRequest::addDefine(const FString& key, const FString& value)
    {
        m_Defines.addUnique({key, value});
    }

    void FShaderCompileRequest::setModuleInfo(const FString& moduleName, const FString& filepath)
    {
        m_ModuleInfo = FShaderModuleInfo(moduleName, filepath);
    }

    void FShaderCompileRequest::setCompileTarget(EShaderCompileTarget target)
    {
        m_Target = target;
    }

    void FShaderCompileRequest::setLanguage(EShadingLanguage language)
    {
        m_Language = language;
    }

    void FShaderCompileRequest::addIncludeDirectory(const FString& includeDirectory)
    {
        m_IncludeDirectories.addUnique(includeDirectory);
    }

    void FShaderCompileRequest::addEntryPoint(const FString& name, const EShaderStageBits stage)
    {
        m_EntryPointInfos.addUnique({name, stage});
    }

    EShadingLanguage FShaderCompileRequest::getLanguage() const
    {
        return m_Language;
    }

    EShaderCompileTarget FShaderCompileRequest::getTarget() const
    {
        return m_Target;
    }

    const TArray<FShaderCompileDefine>& FShaderCompileRequest::getDefines() const
    {
        return m_Defines;
    }

    const TArray<FString>& FShaderCompileRequest::getIncludeDirectories() const
    {
        return m_IncludeDirectories;
    }

    const TArray<FShaderEntryPoint>& FShaderCompileRequest::getEntryPointInfos() const
    {
        return m_EntryPointInfos;
    }

    const FShaderModuleInfo& FShaderCompileRequest::getModuleInfo() const
    {
        return m_ModuleInfo;
    }
}
