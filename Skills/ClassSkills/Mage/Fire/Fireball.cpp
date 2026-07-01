#include "Fireball.hpp"
#include "../../../Characters/Character.hpp"

Fireball::Fireball()
{
    name = "Fireball";
    manaCost = 15;
    cooldown = 2;
}

void Fireball::Use(Character& caster,
                   Character& target)
{
    target.TakeDamage(50);
}