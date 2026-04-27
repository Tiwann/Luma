#pragma once

struct ISlangBlob;

namespace Slang
{
    template<typename T>
    class ComPtr;
}

namespace slang
{
    struct IGlobalSession;
    struct ISession;
    struct IModule;
    struct IEntryPoint;
    struct IComponentType;
    typedef ISlangBlob IBlob;
}

namespace slang
{
    using GlobalSessionHandle = Slang::ComPtr<IGlobalSession>;
    using SessionHandle = Slang::ComPtr<ISession>;
    using ModuleHandle = Slang::ComPtr<IModule>;
    using EntryPointHandle = Slang::ComPtr<IEntryPoint>;
    using ComponentHandle = Slang::ComPtr<IComponentType>;
    using BlobHandle = Slang::ComPtr<IBlob>;
    enum class BindingType : uint32_t;
    struct PreprocessorMacroDesc;
}
