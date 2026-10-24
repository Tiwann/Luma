#include "Components.h"
#include <Luma/Memory/Ref.h>
#include <Luma/Runtime/RTTI.h>
#include <Luma/Runtime/TypeName.h>
#include <cstdio>

using namespace Luma;

static void printHierarchy(const Class& cls)
{
    std::printf("  %-14s depth=%u size=%-3u%s%s%s  chain: ",
        cls.getShortName().data(), cls.getDepth(), cls.getSize(),
        cls.isAbstract() ? " abstract" : "",
        cls.isFinal() ? " final" : "",
        cls.canCreate() ? " creatable" : "");

    for (const Class* current = &cls; current; current = current->getParent())
        std::printf("%s%s", current->getName().data(), current->getParent() ? " -> " : "\n");
}

int main()
{
    // 1. Compile-time type names and ids, for any type, no macro required.
    std::printf("== Type names\n");
    std::printf("  %s\n", typeNameOf<Game::Sword>().data());
    std::printf("  %s\n", typeNameOf<Ref<Game::Health>>().data());
    static_assert(typeIdOf<Game::Sword>() == TypeId::fromName("Game::Sword"));
    static_assert(typeIdOf<Game::Sword>() != typeIdOf<Game::Bow>());

    // 2. Class metadata and the reflected inheritance chain.
    std::printf("\n== Classes\n");
    printHierarchy(Game::Health::staticClass());
    printHierarchy(Game::Shield::staticClass());
    printHierarchy(Game::Weapon::staticClass());
    printHierarchy(Game::Sword::staticClass());
    printHierarchy(Game::Bow::staticClass());

    // 3. Casting through a base pointer, no dynamic_cast.
    std::printf("\n== Casting\n");
    Ref<Component> components[] = { new Game::Health(), new Game::Sword(), new Game::Bow(), new Game::Shield() };
    for (const Ref<Component>& component : components)
    {
        const Class& cls = component->getClass();
        std::printf("  %-7s isA<Weapon>=%d", cls.getShortName().data(), isA<Game::Weapon>(component.get()));

        if (const Game::Weapon* weapon = cast<Game::Weapon>(component.get()))
            std::printf("  attack=%.1f", weapon->attack());
        if (const Game::Bow* bow = cast<Game::Bow>(component.get())) // final class: single id compare
            std::printf("  arrows=%d", bow->arrows);
        std::printf("\n");
    }

    // Ref overload keeps the reference count.
    Ref<Game::Sword> sword = cast<Game::Sword>(components[1]);
    std::printf("  Ref cast: sword refcount=%u, health as sword=%s\n",
        sword->getRefCount(), cast<Game::Sword>(components[0]) ? "valid" : "null");

    // 4. Lookup by name and create instances, e.g. when loading a scene file.
    std::printf("\n== Registry and factory\n");
    for (const char* name : { "Game::Bow", "Game::Weapon", "Game::Dragon" })
    {
        const Class* cls = ClassRegistry::find(name);
        if (!cls)
        {
            std::printf("  %-12s -> not registered\n", name);
            continue;
        }

        Ref<Component> created = cls->createInstance<Component>();
        std::printf("  %-12s -> found, created=%s\n", name,
            created ? created->getClass().getName().data() : "null (abstract)");
    }

    // 5. Enumerate subclasses, e.g. to build an editor "Add Component" menu.
    std::printf("\n== Weapon subclasses\n");
    for (const Class* cls : ClassRegistry::getDerivedClasses(Game::Weapon::staticClass()))
        std::printf("  %s\n", cls->getName().data());

    std::printf("\n== Creatable components\n");
    for (const Class* cls : ClassRegistry::getDerivedClasses(Component::staticClass()))
        if (cls->canCreate())
            std::printf("  %s\n", cls->getName().data());

    return 0;
}
