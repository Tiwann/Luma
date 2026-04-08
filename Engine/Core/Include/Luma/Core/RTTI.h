#pragma once
#include "Luma/Core/Export.h"
#include <string>

#define NOVA_DECLARE_CLASS(className) \
public: \
static inline constexpr const Nova::RTTI::Class* StaticClass() \
{ \
return &m_StaticClass_Generated; \
} \
private: \
static constexpr Nova::RTTI::Class m_StaticClass_Generated{#className, nullptr}; \


#define NOVA_DECLARE_CLASS_WITH_PARENT(className, parentClass) \
public: \
static inline constexpr const Nova::RTTI::Class* StaticClass() \
{ \
return &m_StaticClass_Generated; \
} \
private: \
static constexpr Nova::RTTI::Class m_StaticClass_Generated{#className, parentClass::StaticClass()}; \

namespace Luma::RTTI
{
    class LUMA_CORE_API FClass
    {
    public:
        FClass() = delete;
        constexpr FClass(std::string_view className, const FClass* parentClass)
            : m_ClassName(className), m_ParentClass(parentClass){}

        FClass(const FClass&) = delete;
        FClass(FClass&&) = delete;
        FClass& operator=(const FClass&) = delete;
        FClass& operator=(FClass&&) = delete;
        virtual ~FClass() = default;

        constexpr bool operator==(const FClass& other) const
        {
            return this == &other;
        }

        constexpr bool IsA(const FClass* other) const
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
        constexpr bool IsA() const
        {
            return IsA(T::StaticClass());
        }

        constexpr const FClass* Cast(const FClass* other) const
        {
            return IsA(other) ? other : nullptr;
        }

        template<typename T>
        constexpr const FClass* Cast() const
        {
            return Cast(T::StaticClass());
        }

        constexpr std::string_view GetName() const { return m_ClassName; }
        constexpr const FClass* GetParent() const { return m_ParentClass; }


    private:
        std::string_view m_ClassName;
        const FClass* m_ParentClass;
    };
}