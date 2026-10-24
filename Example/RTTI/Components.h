#pragma once
#include <Luma/Runtime/Component.h>

// Example reflected component hierarchy:
//
//   Luma::Object                (engine root, LUMA_CLASS_ROOT)
//   └── Luma::Component         (engine)
//       ├── Game::Health
//       ├── Game::Shield        (header-only, LUMA_CLASS_INLINE)
//       └── Game::Weapon        (abstract)
//           ├── Game::Sword
//           └── Game::Bow       (final)

namespace Game
{
    class Health : public Luma::Component
    {
        LUMA_CLASS(Health, Luma::Component)
    public:
        float current = 100.0f;
        float maximum = 100.0f;
    };

    // No .cpp needed: the inline variant defines everything in the header.
    class Shield : public Luma::Component
    {
        LUMA_CLASS_INLINE(Shield, Luma::Component)
    public:
        float absorb = 0.5f;
    };

    class Weapon : public Luma::Component
    {
        LUMA_CLASS(Weapon, Luma::Component)
    public:
        virtual float attack() const = 0;
        int level = 1;
    };

    class Sword : public Weapon
    {
        LUMA_CLASS(Sword, Weapon)
    public:
        float attack() const override { return 12.0f * level; }
    };

    class Bow final : public Weapon
    {
        LUMA_CLASS(Bow, Weapon)
    public:
        float attack() const override { return 8.0f * level + arrows; }
        int arrows = 10;
    };
}
