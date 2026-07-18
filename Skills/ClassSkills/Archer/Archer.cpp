#include "Archer.hpp"
#include "../../../Characters/Character.hpp"

PiercingShot::PiercingShot() : Skill("Piercing Shot", 10, 1, 40, 1)
{
        description = "Precise shot: 10 base + DEX/2 + weapon damage";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void PiercingShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Poison, 3 + GetTotalEffectDurationBonus(), 3 + caster.GetStats().dexterity / 5 + GetTotalEffectDamageBonus(), caster.GetName());
    GainXP(1);
    ResetCooldown();
}

MultiShot::MultiShot() : Skill("Multi Shot", 20, 3, 30, 3)
{
        description = "8 base + DEX/3 + weapon damage (2 hits)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void MultiShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

RainOfArrows::RainOfArrows() : Skill("Rain of Arrows", 30, 4, 60, 5)
{
        description = "15 base + DEX + weapon damage (2 hits)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void RainOfArrows::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

QuickShot::QuickShot() : Skill("Quick Shot", 8, 0, 30, 5)
{
        description = "Fast shot: 8 base + DEX/3 + weapon damage (no cooldown)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void QuickShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Snipe::Snipe() : Skill("Snipe", 25, 3, 120, 15)
{
        description = "30 base + DEX + weapon damage (req. Lv.15)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void Snipe::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

ArrowStorm::ArrowStorm() : Skill("Arrow Storm", 35, 4, 60, 25)
{
        description = "15 base + DEX/3 + weapon damage (hits twice, req. Lv.25)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void ArrowStorm::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

DeathMark::DeathMark() : Skill("Death Mark", 55, 5, 220, 40)
{
        description = "55 base + DEX*2 + weapon damage (req. Lv.40)";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void DeathMark::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}

std::string PiercingShot::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon, Poison 3 turns"; }
int PiercingShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

std::string MultiShot::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (2 hits, 2nd at 50%)"; }
int MultiShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit + hit / 2;
}

std::string RainOfArrows::GetDamageFormula() const { return "base/4 + DEX + Weapon (2 hits, 2nd at 50%)"; }
int RainOfArrows::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + stats.dexterity + weaponDamage + elementalBonus);
    return hit + hit / 2;
}

std::string QuickShot::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (no CD)"; }
int QuickShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

std::string Snipe::GetDamageFormula() const { return "base/4 + DEX + Weapon"; }
int Snipe::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.dexterity + weaponDamage + elementalBonus);
}

std::string ArrowStorm::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (2 full-damage hits)"; }
int ArrowStorm::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit * 2;
}

std::string DeathMark::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int DeathMark::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}
