#include "Warrior.hpp"
#include "../../../Characters/Character.hpp"

PowerStrike::PowerStrike() : Skill("Power Strike", 15, 1, 60, 1)
{
        description = "Powerful strike: 15 base + STR/2 + weapon damage";
        InitializeUpgrades();
}

void PowerStrike::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Whirlwind::Whirlwind() : Skill("Whirlwind", 30, 3, 50, 1)
{
        description = "Spin attack: 12 base + STR/3 + weapon damage (2 hits)";
        InitializeUpgrades();
}

void Whirlwind::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

DefensiveStance::DefensiveStance() : Skill("Defensive Stance", 0, 2, 0, 3)
{
        description = "+3 DEF & restore 5 + VIT/2 HP (self)";
        InitializeUpgrades();
}

void DefensiveStance::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(3 + GetTotalDefenseBonus());
    caster.RestoreHealth(5 + caster.GetStats().vitality / 2 + GetTotalHealBonus());
    GainXP(1);
    ResetCooldown();
}

WarCry::WarCry() : Skill("War Cry", 20, 4, 0, 3)
{
        description = "Shout: +5 DEF & heal 20 + VIT HP (self)";
        InitializeUpgrades();
}

void WarCry::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(5 + GetTotalDefenseBonus());
    caster.RestoreHealth(20 + caster.GetStats().vitality + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

ShieldBash::ShieldBash() : Skill("Shield Bash", 20, 2, 80, 10)
{
        description = "20 base + STR/2 + weapon damage";
        InitializeUpgrades();
}

void ShieldBash::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

BattleCry::BattleCry() : Skill("Battle Cry", 30, 4, 0, 20)
{
        description = "Mighty shout: +8 DEF & heal 40 + VIT HP (self)";
        InitializeUpgrades();
}

void BattleCry::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    caster.RestoreHealth(40 + caster.GetStats().vitality + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

Execute::Execute() : Skill("Execute", 50, 4, 180, 30)
{
        description = "Finisher: 45 base + STR*2 + weapon damage (req. Lv.30)";
        InitializeUpgrades();
}

void Execute::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

WarStomp::WarStomp() : Skill("War Stomp", 60, 5, 120, 40)
{
        description = "30 base + STR + weapon damage, drains 25 enemy mana (req. Lv.40)";
        InitializeUpgrades();
}

void WarStomp::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(25);
    GainXP(3);
    ResetCooldown();
}
