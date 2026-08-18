#pragma once
#include "ShaderCompileRequest.h"
#include "Luma/Containers/Array.h"
#include <cstdint>


namespace slang
{
    struct IGlobalSession;
    struct ISession;
}

namespace Luma
{
    struct FShaderCompiledData
    {
        EShaderStageBits stage;
        TArray<uint8_t> blob;
        bool operator==(const FShaderCompiledData& other) const = default;
    };

    struct FShaderCompileResult
    {
        bool succeeded = false;
        TArray<FShaderCompiledData> compiledData;
        TArray<FShaderReflectionData> reflectionData;
        bool operator==(const FShaderCompileResult& other) const = default;
    };

    class FShaderCompiler
    {
    public:
        FShaderCompiler(slang::IGlobalSession* globalSession);
        TArray<FShaderCompileResult> compile();

        void addCompileRequest(const FShaderCompileRequest& compileRequest);
    private:
        slang::IGlobalSession* m_GlobalSession = nullptr;
        TArray<FShaderCompileRequest> m_Requests;
    };
}