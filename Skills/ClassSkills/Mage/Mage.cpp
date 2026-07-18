#include "Mage.hpp"
#include "../../../Characters/Character.hpp"

// ============================================================
//  LEVEL 1 — STARTER
// ============================================================

Fireball::Fireball() : Skill("Fireball", 15, 2, 50, 1)
{
        description = "12 base + INT/2 + weapon damage + elemental";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void Fireball::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Burn, 2 + GetTotalEffectDurationBonus(), 5 + caster.GetStats().intelligence / 5 + GetTotalEffectDamageBonus(), caster.GetName());
    GainXP(1);
    ResetCooldown();
}

IceBolt::IceBolt() : Skill("Ice Bolt", 12, 1, 35, 1)
{
        description = "9 base + INT/3 + weapon damage";
    element = ElementType::Ice;
    InitializeUpgrades();
}

void IceBolt::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

// ============================================================
//  LEVEL 2
// ============================================================

ArcaneBolt::ArcaneBolt() : Skill("Arcane Bolt", 6, 0, 20, 2)
{
        description = "5 base + INT/4 + weapon damage (no cooldown)";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ArcaneBolt::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence / 4) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

// ============================================================
//  LEVEL 3
// ============================================================

Meteor::Meteor() : Skill("Meteor", 40, 4, 90, 3)
{
        description = "22 base + INT + weapon damage";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void Meteor::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 4
// ============================================================

FrostWard::FrostWard() : Skill("Frost Ward", 14, 3, 0, 4)
{
        description = "+4 DEF & heal 15 + INT/4 HP (self)";
    InitializeUpgrades();
}

void FrostWard::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(4 + GetTotalDefenseBonus());
    caster.RestoreHealth(15 + caster.GetStats().intelligence / 4 + GetTotalHealBonus());
    GainXP(1);
    ResetCooldown();
}

// ============================================================
//  LEVEL 5
// ============================================================

ArcaneSurge::ArcaneSurge() : Skill("Arcane Surge", 35, 5, 100, 5)
{
        description = "25 base + INT*2 + weapon damage";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ArcaneSurge::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 6
// ============================================================

ManaSiphon::ManaSiphon() : Skill("Mana Siphon", 10, 2, 30, 6)
{
        description = "8 base + INT/2 + weapon damage, restore 10+INT/5 mana";
    InitializeUpgrades();
}

void ManaSiphon::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreMana(10 + caster.GetStats().intelligence / 5 + GetTotalHealBonus());
    GainXP(1);
    ResetCooldown();
}

// ============================================================
//  LEVEL 8
// ============================================================

ChainLightning::ChainLightning() : Skill("Chain Lightning", 20, 3, 55, 8)
{
        description = "14 base + INT + weapon damage";
    element = ElementType::Lightning;
    InitializeUpgrades();
}

void ChainLightning::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 10
// ============================================================

LightningBolt::LightningBolt() : Skill("Lightning Bolt", 18, 2, 60, 10)
{
        description = "15 base + INT/2 + weapon damage";
    element = ElementType::Lightning;
    InitializeUpgrades();
}

void LightningBolt::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Stun, 1, 0, caster.GetName());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 12
// ============================================================

ArcaneMissiles::ArcaneMissiles() : Skill("Arcane Missiles", 24, 3, 70, 12)
{
        description = "17 base + INT*1.5 + weapon damage";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ArcaneMissiles::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 14
// ============================================================

FrostShield::FrostShield() : Skill("Frost Shield", 18, 4, 0, 14)
{
        description = "+7 DEF & restore 25+INT/3 mana (self)";
    InitializeUpgrades();
}

void FrostShield::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(7 + GetTotalDefenseBonus());
    caster.RestoreMana(25 + caster.GetStats().intelligence / 3 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 16
// ============================================================

ManaBomb::ManaBomb() : Skill("Mana Bomb", 35, 4, 90, 16)
{
        description = "22 base + INT*2 + weapon damage, +3 dmg per extra mana spent";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ManaBomb::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int bonus = std::min(20, caster.GetCurrentMana() - 35);
    if (bonus > 0)
    {
        caster.ReduceMana(bonus);
    }
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + (bonus * 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 18
// ============================================================

BlazingOrb::BlazingOrb() : Skill("Blazing Orb", 26, 3, 65, 18)
{
        description = "16 base + INT + weapon damage, heal 10+INT/6 HP";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void BlazingOrb::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(10 + caster.GetStats().intelligence / 6 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 20
// ============================================================

FrostNova::FrostNova() : Skill("Frost Nova", 28, 3, 70, 20)
{
        description = "17 base + INT + weapon damage, drains 20 enemy mana";
    element = ElementType::Ice;
    InitializeUpgrades();
}

void FrostNova::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Freeze, 1 + GetTotalEffectDurationBonus(), 0, caster.GetName());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 22
// ============================================================

ArcaneBarrage::ArcaneBarrage() : Skill("Arcane Barrage", 30, 3, 85, 22)
{
        description = "21 base + INT*2 + weapon damage";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ArcaneBarrage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 26
// ============================================================

TimeWarp::TimeWarp() : Skill("Time Warp", 28, 4, 55, 26)
{
        description = "14 base + INT + weapon damage, self +6 DEF";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void TimeWarp::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.IncreaseTempDefense(6 + GetTotalDefenseBonus());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 28
// ============================================================

GlacialSpike::GlacialSpike() : Skill("Glacial Spike", 32, 4, 105, 28)
{
        description = "26 base + INT*1.5 + weapon damage, 1.5x vs low-HP foes";
    element = ElementType::Ice;
    InitializeUpgrades();
}

void GlacialSpike::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    double multiplier = 1.0;
    if (target.GetCurrentHealth() < target.GetStats().health / 2)
        multiplier = 1.5;
    target.TakeDamage(static_cast<int>(damage * multiplier), caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 30
// ============================================================

ManaShield::ManaShield() : Skill("Mana Shield", 0, 5, 0, 30)
{
        description = "+8 DEF & restore 40+INT/2 mana (self)";
    InitializeUpgrades();
}

void ManaShield::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    caster.RestoreMana(40 + caster.GetStats().intelligence / 2 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

// ============================================================
//  LEVEL 32
// ============================================================

Pyroblast::Pyroblast() : Skill("Pyroblast", 42, 5, 135, 32)
{
        description = "34 base + INT*2 + weapon damage";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void Pyroblast::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 34
// ============================================================

Blizzard::Blizzard() : Skill("Blizzard", 48, 5, 145, 34)
{
        description = "36 base + INT*1.5 + weapon damage, drains 15 enemy mana";
    element = ElementType::Ice;
    InitializeUpgrades();
}

void Blizzard::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(15);
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 36
// ============================================================

ArcanePower::ArcanePower() : Skill("Arcane Power", 38, 6, 0, 36)
{
        description = "+12 DEF, restore 50+INT/2 mana & 30+INT/3 HP";
    InitializeUpgrades();
}

void ArcanePower::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(12 + GetTotalDefenseBonus());
    caster.RestoreMana(50 + caster.GetStats().intelligence / 2 + GetTotalHealBonus());
    caster.RestoreHealth(30 + caster.GetStats().intelligence / 3 + GetTotalHealBonus());
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 38
// ============================================================

IceLance::IceLance() : Skill("Ice Lance", 36, 4, 125, 38)
{
        description = "31 base + INT*2 + weapon damage, 1.6x vs low-mana foes";
    element = ElementType::Ice;
    InitializeUpgrades();
}

void IceLance::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    double multiplier = 1.0;
    if (target.GetCurrentMana() < target.GetStats().mana / 2)
        multiplier = 1.6;
    target.TakeDamage(static_cast<int>(damage * multiplier), caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

// ============================================================
//  LEVEL 40
// ============================================================

Inferno::Inferno() : Skill("Inferno", 70, 6, 220, 40)
{
        description = "55 base + INT*2 + weapon damage (req. Lv.40)";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void Inferno::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}

// ============================================================
//  LEVEL 44
// ============================================================

ElementalFury::ElementalFury() : Skill("Elemental Fury", 55, 6, 200, 44)
{
        description = "50 base + INT*3 + weapon damage (req. Lv.44)";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void ElementalFury::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}

// ============================================================
//  LEVEL 50 — ULTIMATE
// ============================================================

ArchmageCataclysm::ArchmageCataclysm() : Skill("Archmage's Cataclysm", 75, 8, 300, 50)
{
        description = "75 base + INT*4 + weapon damage, restore 20 mana (req. Lv.50)";
    element = ElementType::Arcane;
    InitializeUpgrades();
}

void ArchmageCataclysm::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().intelligence * 4) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreMana(20 + GetTotalHealBonus());
    GainXP(5);
    ResetCooldown();
}

std::string Fireball::GetDamageFormula() const { return "base/4 + INT/2 + Weapon, Burn 2 turns"; }
int Fireball::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence / 2) + weaponDamage + elementalBonus);
}

std::string IceBolt::GetDamageFormula() const { return "base/4 + INT/3 + Weapon"; }
int IceBolt::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence / 3) + weaponDamage + elementalBonus);
}

std::string ArcaneBolt::GetDamageFormula() const { return "base/4 + INT/4 + Weapon (no CD)"; }
int ArcaneBolt::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence / 4) + weaponDamage + elementalBonus);
}

std::string Meteor::GetDamageFormula() const { return "base/4 + INT + Weapon"; }
int Meteor::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.intelligence + weaponDamage + elementalBonus);
}

std::string FrostWard::GetDamageFormula() const { return "+DEF & Heal (self)"; }

std::string ArcaneSurge::GetDamageFormula() const { return "base/4 + INT*2 + Weapon"; }
int ArcaneSurge::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string ManaSiphon::GetDamageFormula() const { return "base/4 + INT/2 + Weapon, restores mana"; }
int ManaSiphon::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence / 2) + weaponDamage + elementalBonus);
}

std::string ChainLightning::GetDamageFormula() const { return "base/4 + INT + Weapon"; }
int ChainLightning::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.intelligence + weaponDamage + elementalBonus);
}

std::string LightningBolt::GetDamageFormula() const { return "base/4 + INT/2 + Weapon, Stun 1 turn"; }
int LightningBolt::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence / 2) + weaponDamage + elementalBonus);
}

std::string ArcaneMissiles::GetDamageFormula() const { return "base/4 + INT*1.5 + Weapon"; }
int ArcaneMissiles::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 3 / 2) + weaponDamage + elementalBonus);
}

std::string FrostShield::GetDamageFormula() const { return "+DEF & Restore mana (self)"; }

std::string ManaBomb::GetDamageFormula() const { return "base/4 + INT*2 + Weapon, +3 dmg per extra mana"; }
int ManaBomb::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string BlazingOrb::GetDamageFormula() const { return "base/4 + INT + Weapon, heals self"; }
int BlazingOrb::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.intelligence + weaponDamage + elementalBonus);
}

std::string FrostNova::GetDamageFormula() const { return "base/4 + INT + Weapon, Freeze 1 turn"; }
int FrostNova::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.intelligence + weaponDamage + elementalBonus);
}

std::string ArcaneBarrage::GetDamageFormula() const { return "base/4 + INT*2 + Weapon"; }
int ArcaneBarrage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string TimeWarp::GetDamageFormula() const { return "base/4 + INT + Weapon, +6 DEF (self)"; }
int TimeWarp::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.intelligence + weaponDamage + elementalBonus);
}

std::string GlacialSpike::GetDamageFormula() const { return "base/4 + INT*1.5 + Weapon (1.5x vs low HP)"; }
int GlacialSpike::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 3 / 2) + weaponDamage + elementalBonus);
}

std::string ManaShield::GetDamageFormula() const { return "+DEF & Restore mana (self)"; }

std::string Pyroblast::GetDamageFormula() const { return "base/4 + INT*2 + Weapon"; }
int Pyroblast::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string Blizzard::GetDamageFormula() const { return "base/4 + INT*1.5 + Weapon, drains 15 mana"; }
int Blizzard::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 3 / 2) + weaponDamage + elementalBonus);
}

std::string ArcanePower::GetDamageFormula() const { return "+DEF, restore mana & HP (self)"; }

std::string IceLance::GetDamageFormula() const { return "base/4 + INT*2 + Weapon (1.6x vs low mana)"; }
int IceLance::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string Inferno::GetDamageFormula() const { return "base/4 + INT*2 + Weapon"; }
int Inferno::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 2) + weaponDamage + elementalBonus);
}

std::string ElementalFury::GetDamageFormula() const { return "base/4 + INT*3 + Weapon"; }
int ElementalFury::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 3) + weaponDamage + elementalBonus);
}

std::string ArchmageCataclysm::GetDamageFormula() const { return "base/4 + INT*4 + Weapon, restores 20 mana"; }
int ArchmageCataclysm::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.intelligence * 4) + weaponDamage + elementalBonus);
}
