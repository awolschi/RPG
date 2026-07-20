#include "CommonAttack.hpp"
#include "../../Characters/Character.hpp"

CommonAttack::CommonAttack() : Skill("Attack", 0, 0, 25)
{
        description = "Basic attack dealing 8 base + STR/2 + weapon damage";
    InitializeUpgrades();
}

void CommonAttack::Use(Character& caster, Character& target)
{
    // Basic attack is always available
    int damage = (baseDamage / 3) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
}

std::string CommonAttack::GetDamageFormula() const
{
    return "base/3 + STR/2 + Weapon";
}

int CommonAttack::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int damage = (baseDamage / 3) + (stats.strength / 2) + weaponDamage + elementalBonus;
    return ApplyDamageBonus(damage);
}
