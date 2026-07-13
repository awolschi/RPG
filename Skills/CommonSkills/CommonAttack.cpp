#include "CommonAttack.hpp"
#include "../../Characters/Character.hpp"

CommonAttack::CommonAttack() : Skill("Attack", 0, 0, 25)
{
        description = "Basic attack dealing 6 base + STR/3 + weapon damage";
}

void CommonAttack::Use(Character& caster, Character& target)
{
    // Basic attack is always available
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
}
