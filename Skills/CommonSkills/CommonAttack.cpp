#include "CommonAttack.hpp"
#include "../../Characters/Character.hpp"

CommonAttack::CommonAttack() : Skill("Attack", 0, 0, 25)
{
        description = "Basic attack dealing 6 base + STR/3 + weapon damage";
    InitializeUpgrades();
}

void CommonAttack::Use(Character& caster, Character& target)
{
    // Basic attack is always available
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
}

std::string CommonAttack::GetDamageFormula() const
{
    return "base/4 + STR/3 + Weapon";
}

int CommonAttack::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int damage = (baseDamage / 4) + (stats.strength / 3) + weaponDamage + elementalBonus;
    return ApplyDamageBonus(damage);
}
