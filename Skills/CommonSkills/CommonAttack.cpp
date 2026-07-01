#include "CommonAttack.hpp"
#include "../../Characters/Character.hpp"

CommonAttack::CommonAttack() : Skill("Attack", 0, 0, 25)
{
}

void CommonAttack::Use(Character& caster, Character& target)
{
    // Basic attack is always available
    int damage = baseDamage + (caster.GetStats().strength / 3);
    target.TakeDamage(damage);
    GainXP(1);
}
