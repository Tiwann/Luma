#include "Luma/Runtime/RTTI.h"
#include "Luma/Containers/HashMap.h"
#include "Luma/Runtime/Hash.h"
#include <mutex>
#include <shared_mutex>
#include <string_view>

namespace Luma
{
    namespace
    {
        struct RegistryStorage
        {
            std::shared_mutex mutex;
            HashMap<TypeId, const Class*> classes;
        };

        // Intentionally leaked: Class destructors of other modules may still unregister during
        // static destruction, so the registry must outlive every Class.
        RegistryStorage& getStorage()
        {
            static RegistryStorage* storage = new RegistryStorage();
            return *storage;
        }

        StringView computeShortName(const StringView name)
        {
            const std::string_view view(name.data(), name.count());
            // Ignore namespaces inside template arguments: "A::Foo<B::Bar>" -> "Foo<B::Bar>"
            const size_t templateStart = view.find('<');
            const size_t separator = view.rfind("::", templateStart == std::string_view::npos ? std::string_view::npos : templateStart);
            if (separator == std::string_view::npos)
                return name;
            return StringView(name.data() + separator + 2, name.count() - separator - 2);
        }
    }

    Class::Class(const ClassDesc& desc)
        : m_Name(desc.name)
        , m_ShortName(computeShortName(desc.name))
        , m_Id(desc.id)
        , m_Parent(desc.parent)
        , m_Depth(desc.parent ? desc.parent->m_Depth + 1 : 0)
        , m_Size(desc.size)
        , m_Alignment(desc.alignment)
        , m_IsAbstract(desc.isAbstract)
        , m_IsFinal(desc.isFinal)
        , m_Create(desc.create)
        , m_UpcastToParent(desc.upcastToParent)
    {
        LUMA_ASSERT(m_Id.isValid(), "RTTI: class has an invalid TypeId");
        ClassRegistry::registerClass(*this);
    }

    Class::~Class()
    {
        ClassRegistry::unregisterClass(*this);
    }

    void* Class::upcast(void* instance, const Class& target) const
    {
        if (!instance)
            return nullptr;

        const Class* current = this;
        while (current)
        {
            if (current->m_Id == target.m_Id)
                return instance;
            if (!current->m_UpcastToParent)
                return nullptr;
            instance = current->m_UpcastToParent(instance);
            current = current->m_Parent;
        }
        return nullptr;
    }

    void ClassRegistry::registerClass(const Class& cls)
    {
        RegistryStorage& storage = getStorage();
        std::unique_lock lock(storage.mutex);

        if (const auto* existing = storage.classes.find(cls.getId()))
        {
            // The same class can be instantiated once per module (DLL); identity is by id so that is fine.
            // Two *different* names with the same id is a hash collision and must be resolved by renaming.
            LUMA_ASSERT(existing->value->getName() == cls.getName(), "RTTI: TypeId collision between two different classes");
            return;
        }
        storage.classes[cls.getId()] = &cls;
    }

    void ClassRegistry::unregisterClass(const Class& cls)
    {
        RegistryStorage& storage = getStorage();
        std::unique_lock lock(storage.mutex);

        // Only remove the entry if it is this exact instance (another module may own the registered one).
        if (const auto* existing = storage.classes.find(cls.getId()); existing && existing->value == &cls)
            storage.classes.remove(cls.getId());
    }

    const Class* ClassRegistry::find(const TypeId id)
    {
        RegistryStorage& storage = getStorage();
        std::shared_lock lock(storage.mutex);
        const auto* pair = storage.classes.find(id);
        return pair ? pair->value : nullptr;
    }

    const Class* ClassRegistry::find(const StringView qualifiedName)
    {
        const Class* cls = find(TypeId::fromName(std::string_view(qualifiedName.data(), qualifiedName.count())));
        // Guard against hash collisions with unregistered names.
        if (cls && cls->getName() == qualifiedName)
            return cls;
        return nullptr;
    }

    Array<const Class*> ClassRegistry::getAllClasses()
    {
        RegistryStorage& storage = getStorage();
        std::shared_lock lock(storage.mutex);

        Array<const Class*> result;
        for (const auto& [id, cls] : storage.classes)
            result.add(cls);
        return result;
    }

    Array<const Class*> ClassRegistry::getDerivedClasses(const Class& base, const bool includeBase)
    {
        RegistryStorage& storage = getStorage();
        std::shared_lock lock(storage.mutex);

        Array<const Class*> result;
        for (const auto& [id, cls] : storage.classes)
        {
            if (!includeBase && *cls == base)
                continue;
            if (cls->isA(base))
                result.add(cls);
        }
        return result;
    }
}
