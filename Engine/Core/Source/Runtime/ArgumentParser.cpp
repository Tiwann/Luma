#include "Luma/Runtime/ArgumentParser.h"

namespace Luma
{
    template <Character T>
    TArgumentParser<T>::TArgumentParser(TString<T> name, const TArray<TArgumentOption<T>>& options)
        : m_Name(std::move(name)), m_Options(options)
    {
    }

    template <Character T>
    bool TArgumentParser<T>::parse(TBufferView<const T*> arguments)
    {
        m_Arguments = std::move(arguments);

    }

    template <Character T>
    bool TArgumentParser<T>::isUsed(const TArgumentOption<T>& option) const
    {

    }

    template <Character T>
    bool TArgumentParser<T>::isUsed(TStringView<T> name)
    {
    }

    template <Character T>
    bool TArgumentParser<T>::isUsed(T name)
    {
    }

    template <Character T>
    bool TArgumentParser<T>::getString(const TArgumentOption<T>& option)
    {
    }

    template <Character T>
    bool TArgumentParser<T>::getString(TStringView<T> name)
    {
    }

    template <Character T>
    bool TArgumentParser<T>::getString(T name)
    {
    }

    template <Character T>
    TStringView<T> TArgumentParser<T>::getArg(uint32_t index)
    {
    }

    template class TArgumentParser<char>;
    template class TArgumentParser<wchar_t>;
}