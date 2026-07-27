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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 150 / 100; // 1.5x multiplier for skill vs basic attack
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 130 / 100; // 1.3x multiplier per hit
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage / 2, caster.GetEffectiveElement(element)); // 2nd hit at 50%
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 140 / 100; // 1.4x multiplier
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 170 / 100; // 1.7x multiplier
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 160 / 100; // 1.6x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(25);
    GainXP(3);
    ResetCooldown();
}

std::string PowerStrike::GetDamageFormula() const { return "base/4 + STR/2 + Weapon (x1.5)"; }
int PowerStrike::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 2) + weaponDamage + elementalBonus) * 150 / 100);
}

std::string Whirlwind::GetDamageFormula() const { return "base/4 + STR/3 + Weapon (x1.3, 2 hits, 2nd at 50%)"; }
int Whirlwind::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 3) + weaponDamage + elementalBonus) * 130 / 100);
    return hit + hit / 2;
}

std::string DefensiveStance::GetDamageFormula() const { return "+DEF & Heal (self)"; }

std::string WarCry::GetDamageFormula() const { return "+DEF & Heal (self)"; }

std::string ShieldBash::GetDamageFormula() const { return "base/4 + STR/2 + Weapon"; }
int ShieldBash::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.strength / 2) + weaponDamage + elementalBonus);
}

std::string BattleCry::GetDamageFormula() const { return "+DEF & Heal (self)"; }

std::string Execute::GetDamageFormula() const { return "base/4 + STR*2 + Weapon (x1.7)"; }
int Execute::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 2) + weaponDamage + elementalBonus) * 170 / 100);
}

std::string WarStomp::GetDamageFormula() const { return "base/4 + STR + Weapon (x1.6), drains 25 mana"; }
int WarStomp::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + stats.strength + weaponDamage + elementalBonus) * 160 / 100);
}

Cleave::Cleave() : Skill("Cleave", 10, 1, 45, 2)
{
    description = "Sweeping slash: base/4 + STR/3 + weapon damage";
    InitializeUpgrades();
}

void Cleave::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 135 / 100; // 1.35x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string Cleave::GetDamageFormula() const { return "base/4 + STR/3 + Weapon (x1.35)"; }
int Cleave::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 3) + weaponDamage + elementalBonus) * 135 / 100);
}

ShieldWall::ShieldWall() : Skill("Shield Wall", 12, 3, 0, 5)
{
    description = "+6 DEF & heal 15 + VIT/2 HP (self)";
    InitializeUpgrades();
}

void ShieldWall::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(6 + GetTotalDefenseBonus());
    caster.RestoreHealth(15 + caster.GetStats().vitality / 2 + GetTotalHealBonus());
    GainXP(1);
    ResetCooldown();
}

std::string ShieldWall::GetDamageFormula() const { return "+DEF & Heal (self)"; }

RallyingCry::RallyingCry() : Skill("Rallying Cry", 18, 4, 0, 7)
{
    description = "+4 DEF & heal 25 + VIT HP (self)";
    InitializeUpgrades();
}

void RallyingCry::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(4 + GetTotalDefenseBonus());
    caster.RestoreHealth(25 + caster.GetStats().vitality + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

std::string RallyingCry::GetDamageFormula() const { return "+DEF & Heal (self)"; }

HammerSlam::HammerSlam() : Skill("Hammer Slam", 25, 2, 70, 8)
{
    description = "Crushing blow: base/4 + STR/2 + weapon damage";
    InitializeUpgrades();
}

void HammerSlam::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 140 / 100; // 1.4x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string HammerSlam::GetDamageFormula() const { return "base/4 + STR/2 + Weapon (x1.4)"; }
int HammerSlam::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 2) + weaponDamage + elementalBonus) * 140 / 100);
}

VictoryRush::VictoryRush() : Skill("Victory Rush", 15, 2, 50, 12)
{
    description = "Strike and heal: base/4 + STR/3 + weapon damage; heal 20 + VIT/3";
    InitializeUpgrades();
}

void VictoryRush::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 140 / 100; // 1.4x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(20 + caster.GetStats().vitality / 3 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

std::string VictoryRush::GetDamageFormula() const { return "base/4 + STR/3 + Weapon (x1.4) + Heal"; }
int VictoryRush::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 3) + weaponDamage + elementalBonus) * 140 / 100);
}

Charge::Charge() : Skill("Charge", 18, 3, 65, 14)
{
    description = "Rushing attack: base/4 + STR/2 + weapon damage";
    InitializeUpgrades();
}

void Charge::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 155 / 100; // 1.55x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string Charge::GetDamageFormula() const { return "base/4 + STR/2 + Weapon (x1.55)"; }
int Charge::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 2) + weaponDamage + elementalBonus) * 155 / 100);
}

CleaveStrike::CleaveStrike() : Skill("Cleave Strike", 22, 2, 75, 16)
{
    description = "Powerful cleave: base/4 + STR/2 + weapon damage";
    InitializeUpgrades();
}

void CleaveStrike::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 150 / 100; // 1.5x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string CleaveStrike::GetDamageFormula() const { return "base/4 + STR/2 + Weapon (x1.5)"; }
int CleaveStrike::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength / 2) + weaponDamage + elementalBonus) * 150 / 100);
}

Intimidate::Intimidate() : Skill("Intimidate", 20, 4, 0, 18)
{
    description = "Drains 20 enemy mana & +5 DEF self";
    InitializeUpgrades();
}

void Intimidate::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    target.ReduceMana(20);
    caster.IncreaseTempDefense(5 + GetTotalDefenseBonus());
    GainXP(2);
    ResetCooldown();
}

std::string Intimidate::GetDamageFormula() const { return "Drain Mana & +DEF (self)"; }

BerserkerRage::BerserkerRage() : Skill("Berserker Rage", 35, 4, 90, 22)
{
    description = "Furious attack: base/4 + STR*1.5 + weapon damage";
    InitializeUpgrades();
}

void BerserkerRage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 180 / 100; // 1.8x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string BerserkerRage::GetDamageFormula() const { return "base/4 + STR*1.5 + Weapon (x1.8)"; }
int BerserkerRage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 3 / 2) + weaponDamage + elementalBonus) * 180 / 100);
}

Intercept::Intercept() : Skill("Intercept", 25, 3, 80, 24)
{
    description = "Swift strike: base/4 + STR + weapon damage";
    InitializeUpgrades();
}

void Intercept::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + caster.GetStats().strength + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 165 / 100; // 1.65x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string Intercept::GetDamageFormula() const { return "base/4 + STR + Weapon (x1.65)"; }
int Intercept::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + stats.strength + weaponDamage + elementalBonus) * 165 / 100);
}

Fortress::Fortress() : Skill("Fortress", 30, 5, 0, 26)
{
    description = "+10 DEF & heal 40 + VIT HP (self)";
    InitializeUpgrades();
}

void Fortress::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(10 + GetTotalDefenseBonus());
    caster.RestoreHealth(40 + caster.GetStats().vitality + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

std::string Fortress::GetDamageFormula() const { return "+DEF & Heal (self)"; }

ShieldCharge::ShieldCharge() : Skill("Shield Charge", 28, 3, 95, 28)
{
    description = "Shield bash: base/4 + STR + weapon damage";
    InitializeUpgrades();
}

void ShieldCharge::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + caster.GetStats().strength + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 170 / 100; // 1.7x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string ShieldCharge::GetDamageFormula() const { return "base/4 + STR + Weapon (x1.7)"; }
int ShieldCharge::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + stats.strength + weaponDamage + elementalBonus) * 170 / 100);
}

Devastate::Devastate() : Skill("Devastate", 40, 4, 120, 32)
{
    description = "Devastating blow: base/4 + STR*1.5 + weapon damage";
    InitializeUpgrades();
}

void Devastate::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 190 / 100; // 1.9x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Devastate::GetDamageFormula() const { return "base/4 + STR*1.5 + Weapon (x1.9)"; }
int Devastate::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 3 / 2) + weaponDamage + elementalBonus) * 190 / 100);
}

Warbanner::Warbanner() : Skill("Warbanner", 35, 5, 0, 34)
{
    description = "+8 DEF & heal 50 + VIT HP (self)";
    InitializeUpgrades();
}

void Warbanner::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    caster.RestoreHealth(50 + caster.GetStats().vitality + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

std::string Warbanner::GetDamageFormula() const { return "+DEF & Heal (self)"; }

ColossusStrike::ColossusStrike() : Skill("Colossus Strike", 45, 4, 140, 36)
{
    description = "Colossal hit: base/4 + STR*1.5 + weapon damage";
    InitializeUpgrades();
}

void ColossusStrike::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 200 / 100; // 2.0x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string ColossusStrike::GetDamageFormula() const { return "base/4 + STR*1.5 + Weapon (x2.0)"; }
int ColossusStrike::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 3 / 2) + weaponDamage + elementalBonus) * 200 / 100);
}

Rampage::Rampage() : Skill("Rampage", 50, 5, 160, 38)
{
    description = "Unstoppable fury: base/4 + STR*2 + weapon damage";
    InitializeUpgrades();
}

void Rampage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 220 / 100; // 2.2x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Rampage::GetDamageFormula() const { return "base/4 + STR*2 + Weapon (x2.2)"; }
int Rampage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 2) + weaponDamage + elementalBonus) * 220 / 100);
}

TitansGrip::TitansGrip() : Skill("Titan's Grip", 65, 6, 250, 50)
{
    description = "Ultimate power: base/4 + STR*3 + weapon damage";
    InitializeUpgrades();
}

void TitansGrip::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().strength * 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    damage = damage * 250 / 100; // 2.5x multiplier
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string TitansGrip::GetDamageFormula() const { return "base/4 + STR*3 + Weapon (x2.5)"; }
int TitansGrip::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus(((baseDamage / 4) + (stats.strength * 3) + weaponDamage + elementalBonus) * 250 / 100);
}
