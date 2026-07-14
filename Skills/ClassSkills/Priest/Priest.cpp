#include "Priest.hpp"
#include "../../../Characters/Character.hpp"

HolySmite::HolySmite() : Skill("Holy Smite", 20, 2, 45, 1)
{
        description = "Smite foe: 11 base + WIS/2 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void HolySmite::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Heal::Heal() : Skill("Heal", 25, 1, 0, 1)
{
        description = "Restore 40 + WIS/2 HP to target";
    InitializeUpgrades();
}

void Heal::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 40 + (caster.GetStats().wisdom / 2) + GetTotalHealBonus();
    target.RestoreHealth(healAmount);
    GainXP(1);
    ResetCooldown();
}

MassHeal::MassHeal() : Skill("Mass Heal", 50, 3, 0, 3)
{
        description = "Restore 60 + WIS HP to target";
    InitializeUpgrades();
}

void MassHeal::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 60 + (caster.GetStats().wisdom) + GetTotalHealBonus();
    target.RestoreHealth(healAmount);
    GainXP(2);
    ResetCooldown();
}

DivineShield::DivineShield() : Skill("Divine Shield", 30, 5, 0, 3)
{
        description = "+17 DEF & heal 50 + WIS HP (self, req. Lv.5)";
    InitializeUpgrades();
}

void DivineShield::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(17 + GetTotalDefenseBonus());
    caster.RestoreHealth(50 + caster.GetStats().wisdom + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

Smite::Smite() : Skill("Smite", 18, 2, 55, 8)
{
        description = "14 base + WIS/2 + weapon damage (req. Lv.8)";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void Smite::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

Renew::Renew() : Skill("Renew", 35, 3, 0, 15)
{
        description = "Restore 90 + WIS HP to target (req. Lv.15)";
    InitializeUpgrades();
}

void Renew::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 90 + (caster.GetStats().wisdom) + GetTotalHealBonus();
    target.RestoreHealth(healAmount);
    GainXP(2);
    ResetCooldown();
}

HolyNova::HolyNova() : Skill("Holy Nova", 45, 4, 80, 25)
{
        description = "20 base + WIS + weapon damage, heal 50 + WIS/2 (req. Lv.25)";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void HolyNova::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(50 + caster.GetStats().wisdom / 2 + GetTotalHealBonus());
    GainXP(3);
    ResetCooldown();
}

DivineWrath::DivineWrath() : Skill("Divine Wrath", 65, 5, 200, 40)
{
        description = "50 base + WIS*2 + weapon damage (req. Lv.40)";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void DivineWrath::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}
