#pragma once
#include "Luma/Core/Export.h"
#include <string>

#define NOVA_DECLARE_CLASS(className) \
public: \
static inline constexpr const Nova::RTTI::Class* staticClass() \
{ \
return &m_StaticClass_Generated; \
} \
private: \
static constexpr Nova::RTTI::Class m_StaticClass_Generated{#className, nullptr}; \


#define NOVA_DECLARE_CLASS_WITH_PARENT(className, parentClass) \
public: \
static inline constexpr const Nova::RTTI::Class* staticClass() \
{ \
return &m_StaticClass_Generated; \
} \
private: \
static constexpr Nova::RTTI::Class m_StaticClass_Generated{#className, parentClass::staticClass()}; \

namespace luma::rtti
{
    class LUMA_CORE_API FClass final
    {
    public:
        FClass() = delete;
        constexpr FClass(std::string_view className, const FClass* parentClass)
            : m_ClassName(className), m_ParentClass(parentClass){}

        FClass(const FClass&) = delete;
        FClass(FClass&&) = delete;
        FClass& operator=(const FClass&) = delete;
        FClass& operator=(FClass&&) = delete;
        ~FClass() = default;

        constexpr bool operator==(const FClass& other) const
        {
            return this == &other;
        }

        constexpr bool isA(const FClass* other) const
        {
            const FClass* current = this;
            while (current)
            {
                if (current == other)
                    return true;
                current = current->m_ParentClass;
            }
            return false;
        }

        template<typename T>
        constexpr bool isA() const
        {
            return isA(T::staticClass());
        }

        constexpr const FClass* cast(const FClass* other) const
        {
            return isA(other) ? other : nullptr;
        }

        template<typename T>
        constexpr const FClass* cast() const
        {
            return cast(T::staticClass());
        }

        constexpr std::string_view getName() const { return m_ClassName; }
        constexpr const FClass* getParent() const { return m_ParentClass; }


    private:
        std::string_view m_ClassName;
        const FClass* m_ParentClass;
    };
}