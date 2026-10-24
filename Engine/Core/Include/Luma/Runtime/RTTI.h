#pragma once
#include "Luma/Core/Export.h"
#include "Luma/Runtime/TypeName.h"
#include "Luma/Runtime/Assertion.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/Ref.h"
#include <type_traits>

// Luma RTTI
// ---------
// Single-inheritance class reflection that replaces dynamic_cast/typeid for engine objects.
//
//   struct Object : RefCounted<Object>
//   {
//       LUMA_CLASS_ROOT(Object)
//   public:
//       ...
//   };
//
//   class TransformComponent : public Component, public TTransform<float>
//   {
//       LUMA_CLASS(TransformComponent, Component)
//   public:
//       ...
//   };
//
//   // TransformComponent.cpp (exactly one source file)
//   LUMA_DEFINE_CLASS(Luma::TransformComponent)
//
//   if (TransformComponent* transform = cast<TransformComponent>(component)) { ... }
//   const Class* cls = ClassRegistry::find("Luma::TransformComponent");
//   Component* created = cls->createInstance<Component>();
//
// Rules:
// - Put the macro first in the class body. It ends with `private:`.
// - Super must be the direct, public, reflected base. Additional bases (interfaces, mixins like
//   TTransform<float>) are allowed but are not part of the reflected hierarchy.
// - LUMA_CLASS/LUMA_CLASS_ROOT need a LUMA_DEFINE_CLASS in a .cpp at global scope. That keeps the
//   factory (and so the class's ctor/dtor) out of every includer, so headers can still hold
//   Ref<ForwardDeclared> members. Class templates and header-only classes use the _INLINE
//   variants instead, which require the class to be complete-able wherever the header is included.
// - Class identity is the TypeId (hash of the fully qualified name), never the Class address, so
//   it stays correct across DLL boundaries and is stable for serialization.
// - Every reflected class registers itself in ClassRegistry during static initialization. With
//   static libraries, a class whose .cpp is never referenced can be dropped by the linker.

namespace Luma
{
    class Class;

    namespace Detail
    {
        // True only if T itself declares LUMA_CLASS/LUMA_CLASS_ROOT (not merely inherits one).
        template<typename T>
        concept Reflected = requires
        {
            typename T::ThisClass;
            { T::staticClass() } -> std::same_as<const Class&>;
        } && std::is_same_v<typename T::ThisClass, T>;

        template<typename From, typename To>
        using CopyConst = std::conditional_t<std::is_const_v<From>, const To, To>;
    }

    template<typename T>
    concept ReflectedClass = Detail::Reflected<std::remove_cv_t<T>>;

    struct ClassDesc
    {
        using CreateFunc = void* (*)();
        using UpcastFunc = void* (*)(void*);

        StringView name;
        TypeId id;
        const Class* parent = nullptr;
        uint32_t size = 0;
        uint32_t alignment = 0;
        bool isAbstract = false;
        bool isFinal = false;
        CreateFunc create = nullptr;            // Returns a new Self* as void*, null if not creatable
        UpcastFunc upcastToParent = nullptr;    // Self* -> Super* (handles base offsets)
    };

    class LUMA_CORE_API Class final
    {
    public:
        explicit Class(const ClassDesc& desc);
        ~Class();

        Class(const Class&) = delete;
        Class(Class&&) = delete;
        Class& operator=(const Class&) = delete;
        Class& operator=(Class&&) = delete;

        bool operator==(const Class& other) const { return m_Id == other.m_Id; }

        // Fully qualified name, e.g. "Luma::TransformComponent".
        StringView getName() const { return m_Name; }
        // Name without namespaces, e.g. "TransformComponent".
        StringView getShortName() const { return m_ShortName; }
        TypeId getId() const { return m_Id; }
        const Class* getParent() const { return m_Parent; }
        // Number of reflected ancestors (0 for a root).
        uint32_t getDepth() const { return m_Depth; }
        uint32_t getSize() const { return m_Size; }
        uint32_t getAlignment() const { return m_Alignment; }
        bool isAbstract() const { return m_IsAbstract; }
        bool isFinal() const { return m_IsFinal; }
        bool canCreate() const { return m_Create != nullptr; }

        // True if this class is `other` or derives from it.
        bool isA(const Class& other) const
        {
            if (other.m_Depth > m_Depth)
                return false;

            const Class* current = this;
            for (uint32_t steps = m_Depth - other.m_Depth; steps > 0; --steps)
                current = current->m_Parent;
            return current->m_Id == other.m_Id;
        }

        template<ReflectedClass T>
        bool isA() const { return isA(T::staticClass()); }

        // Adjusts a pointer to an instance of this exact class into a pointer to its `target` base
        // subobject. Returns nullptr if this class is not a `target`.
        void* upcast(void* instance, const Class& target) const;

        // Default-constructs an instance with `new`. Returns nullptr if the class is abstract, has no
        // public default constructor, or is not a T.
        template<ReflectedClass T>
        T* createInstance() const
        {
            if (!m_Create || !isA<T>())
                return nullptr;
            return static_cast<T*>(upcast(m_Create(), T::staticClass()));
        }

    private:
        StringView m_Name;
        StringView m_ShortName;
        TypeId m_Id;
        const Class* m_Parent = nullptr;
        uint32_t m_Depth = 0;
        uint32_t m_Size = 0;
        uint32_t m_Alignment = 0;
        bool m_IsAbstract = false;
        bool m_IsFinal = false;
        ClassDesc::CreateFunc m_Create = nullptr;
        ClassDesc::UpcastFunc m_UpcastToParent = nullptr;
    };

    // Global lookup of every reflected class. Thread safe.
    class LUMA_CORE_API ClassRegistry
    {
    public:
        static const Class* find(TypeId id);
        static const Class* find(StringView qualifiedName);

        template<ReflectedClass T>
        static const Class* find() { return &T::staticClass(); }

        static Array<const Class*> getAllClasses();
        // Every registered class that isA(base). Excludes `base` itself unless includeBase is set.
        static Array<const Class*> getDerivedClasses(const Class& base, bool includeBase = false);

    private:
        friend class Class;
        static void registerClass(const Class& cls);
        static void unregisterClass(const Class& cls);
    };

    template<ReflectedClass T>
    const Class& classOf() { return T::staticClass(); }

    // Checked downcast: returns nullptr if `object` is null or not a To. Upcasts are free.
    template<ReflectedClass To, typename From>
    Detail::CopyConst<From, To>* cast(From* object)
    {
        using Target = Detail::CopyConst<From, To>;
        if constexpr (std::is_base_of_v<To, std::remove_cv_t<From>>)
        {
            return object;
        }
        else
        {
            static_assert(std::is_base_of_v<std::remove_cv_t<From>, To>, "cast<To>(From*): To must derive from From (cross casts are not supported)");
            if (!object)
                return nullptr;

            const Class& objectClass = object->getClass();
            if constexpr (std::is_final_v<To>)
            {
                if (objectClass.getId() != typeIdOf<To>())
                    return nullptr;
            }
            else
            {
                if (!objectClass.isA(To::staticClass()))
                    return nullptr;
            }
            return static_cast<Target*>(object);
        }
    }

    template<ReflectedClass To, typename From>
    Ref<To> cast(const Ref<From>& object)
    {
        return Ref<To>(cast<To>(const_cast<From*>(object.get())));
    }

    // Like cast, but asserts on failure (null input still returns null).
    template<ReflectedClass To, typename From>
    Detail::CopyConst<From, To>* castChecked(From* object)
    {
        auto* result = cast<To>(object);
        LUMA_ASSERT(!object || result, "castChecked: object is not of the requested class");
        return result;
    }

    template<ReflectedClass To, typename From>
    bool isA(const From* object)
    {
        return cast<To>(object) != nullptr;
    }

    namespace Detail
    {
        template<typename Self, typename Super>
        ClassDesc makeClassDesc()
        {
            static_assert(std::is_class_v<Self>);
            if constexpr (!std::is_void_v<Super>)
            {
                static_assert(std::is_base_of_v<Super, Self>, "LUMA_CLASS(Self, Super): Super is not a base of Self");
                static_assert(std::is_convertible_v<Self*, Super*>, "LUMA_CLASS(Self, Super): Self must inherit publicly and unambiguously from Super");
                static_assert(Reflected<Super>, "LUMA_CLASS(Self, Super): Super does not declare LUMA_CLASS/LUMA_CLASS_ROOT");
            }

            ClassDesc desc;
            desc.name = typeNameOf<Self>();
            desc.id = typeIdOf<Self>();
            desc.size = sizeof(Self);
            desc.alignment = alignof(Self);
            desc.isAbstract = std::is_abstract_v<Self>;
            desc.isFinal = std::is_final_v<Self>;

            if constexpr (!std::is_abstract_v<Self> && std::is_default_constructible_v<Self>)
                desc.create = []() -> void* { return new Self(); };

            if constexpr (!std::is_void_v<Super>)
            {
                desc.parent = &Super::staticClass();
                desc.upcastToParent = [](void* instance) -> void*
                {
                    return static_cast<Super*>(static_cast<Self*>(instance));
                };
            }
            return desc;
        }

        // Forces registration at static-init time instead of on first staticClass() call.
        struct ClassRegistrar
        {
            explicit ClassRegistrar(const Class& (*getClass)()) { getClass(); }
        };
    }
}

#define LUMA_RTTI_CONCAT_IMPL_(a, b) a##b
#define LUMA_RTTI_CONCAT_(a, b) LUMA_RTTI_CONCAT_IMPL_(a, b)

#define LUMA_CLASS_TYPES_(Self, SuperType) \
public: \
    using ThisClass = Self; \
    using Super = SuperType;

#define LUMA_CLASS_GET_ROOT_ \
    virtual const ::Luma::Class& getClass() const { return staticClass(); }

#define LUMA_CLASS_GET_DERIVED_ \
    const ::Luma::Class& getClass() const override { return staticClass(); }

#define LUMA_CLASS_STATIC_INLINE_ \
    static const ::Luma::Class& staticClass() \
    { \
        static const ::Luma::Class s_Class(::Luma::Detail::makeClassDesc<ThisClass, Super>()); \
        return s_Class; \
    } \
private: \
    static inline const ::Luma::Detail::ClassRegistrar s_LumaClassRegistrar_{ &ThisClass::staticClass }; \
public:

// Declares the root of a reflected hierarchy. Pair with LUMA_DEFINE_CLASS in a .cpp.
#define LUMA_CLASS_ROOT(Self) \
    LUMA_CLASS_TYPES_(Self, void) \
    static const ::Luma::Class& staticClass(); \
    LUMA_CLASS_GET_ROOT_ \
private:

// Declares a reflected class deriving from the reflected class Super. Pair with LUMA_DEFINE_CLASS in a .cpp.
#define LUMA_CLASS(Self, SuperType) \
    LUMA_CLASS_TYPES_(Self, SuperType) \
    static const ::Luma::Class& staticClass(); \
    LUMA_CLASS_GET_DERIVED_ \
private:

// Header-only variants, for class templates and classes without a source file.
#define LUMA_CLASS_ROOT_INLINE(Self) \
    LUMA_CLASS_TYPES_(Self, void) \
    LUMA_CLASS_STATIC_INLINE_ \
    LUMA_CLASS_GET_ROOT_ \
private:

#define LUMA_CLASS_INLINE(Self, SuperType) \
    LUMA_CLASS_TYPES_(Self, SuperType) \
    LUMA_CLASS_STATIC_INLINE_ \
    LUMA_CLASS_GET_DERIVED_ \
private:

// Defines a class declared with LUMA_CLASS/LUMA_CLASS_ROOT. Use at global scope with the fully
// qualified name, in exactly one source file.
#define LUMA_DEFINE_CLASS(QualifiedSelf) \
    const ::Luma::Class& QualifiedSelf::staticClass() \
    { \
        static const ::Luma::Class s_Class(::Luma::Detail::makeClassDesc<QualifiedSelf, QualifiedSelf::Super>()); \
        return s_Class; \
    } \
    static const ::Luma::Detail::ClassRegistrar LUMA_RTTI_CONCAT_(s_LumaClassRegistrar_, __LINE__){ &QualifiedSelf::staticClass };
