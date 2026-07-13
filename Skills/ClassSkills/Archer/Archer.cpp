#include "Archer.hpp"
#include "../../../Characters/Character.hpp"

PiercingShot::PiercingShot() : Skill("Piercing Shot", 10, 1, 40, 1)
{
        description = "Precise shot: 10 base + DEX/2 + weapon damage";
    element = ElementType::Poison;
}

void PiercingShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Poison, 3, 3 + caster.GetStats().dexterity / 5, caster.GetName());
    GainXP(1);
    ResetCooldown();
}

MultiShot::MultiShot() : Skill("Multi Shot", 20, 3, 30, 3)
{
        description = "8 base + DEX/3 + weapon damage (2 hits)";
    element = ElementType::Poison;
}

void MultiShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

RainOfArrows::RainOfArrows() : Skill("Rain of Arrows", 30, 4, 60, 5)
{
        description = "15 base + DEX + weapon damage (2 hits)";
    element = ElementType::Poison;
}

void RainOfArrows::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

QuickShot::QuickShot() : Skill("Quick Shot", 8, 0, 30, 5)
{
        description = "Fast shot: 8 base + DEX/3 + weapon damage (no cooldown)";
    element = ElementType::Poison;
}

void QuickShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Snipe::Snipe() : Skill("Snipe", 25, 3, 120, 15)
{
        description = "30 base + DEX + weapon damage (req. Lv.15)";
    element = ElementType::Poison;
}

void Snipe::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

ArrowStorm::ArrowStorm() : Skill("Arrow Storm", 35, 4, 60, 25)
{
        description = "15 base + DEX/3 + weapon damage (hits twice, req. Lv.25)";
    element = ElementType::Poison;
}

void ArrowStorm::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

DeathMark::DeathMark() : Skill("Death Mark", 55, 5, 220, 40)
{
        description = "55 base + DEX*2 + weapon damage (req. Lv.40)";
    element = ElementType::Poison;
}

void DeathMark::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}
