#pragma once
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/Character.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Containers/StringHash.h"
#include "Luma/Containers/StringView.h"


namespace Luma
{
    template<Character T>
    struct TArgumentOption
    {
        T shortName;
        StringBase<T> longName;
    };

    template<Character T>
    struct THasher<TArgumentOption<T>>
    {
        uint64_t operator()(const TArgumentOption<T>& option) const
        {
            uint64_t hash = THasher<T>{}(option.shortName);
            hash ^= THasher<StringBase<T>>{}(option.longName);
            return hash;
        }
    };

    template<Character T>
    class TArgumentParser final
    {
    public:
        TArgumentParser(StringBase<T> name, const Array<TArgumentOption<T>>& options);

        bool parse(TBufferView<const T*> arguments);

        bool isUsed(const TArgumentOption<T>& option) const;
        bool isUsed(StringViewBase<T> name);
        bool isUsed(T name);

        bool getString(const TArgumentOption<T>& option);
        bool getString(StringViewBase<T> name);
        bool getString(T name);

    private:
        StringViewBase<T> getArg(uint32_t index);
    private:
        StringBase<T> m_Name;
        TBufferView<const T*> m_Arguments;
        Array<TArgumentOption<T>> m_Options;
        THashMap<TArgumentOption<T>, StringBase<T>> m_Values;
    };

    using FArgumentOption = TArgumentOption<char>;
    using FWideArgumentOption = TArgumentOption<wchar_t>;
    using FArgumentParser = TArgumentParser<char>;
    using FWideArgumentParser = TArgumentParser<wchar_t>;
}
