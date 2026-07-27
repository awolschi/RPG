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
    damage = ApplyCharacterMasteryBonus(damage, caster);
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
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
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}

PrayerOfMending::PrayerOfMending() : Skill("Prayer of Mending", 10, 1, 0, 2)
{
    description = "Restore 25 + WIS/3 HP";
    InitializeUpgrades();
}

void PrayerOfMending::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 25 + (caster.GetStats().wisdom / 3) + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(1);
    ResetCooldown();
}

CleansingLight::CleansingLight() : Skill("Cleansing Light", 15, 2, 0, 4)
{
    description = "Restore 35 + WIS/2 HP";
    InitializeUpgrades();
}

void CleansingLight::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 35 + (caster.GetStats().wisdom / 2) + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(1);
    ResetCooldown();
}

HolySmiteII::HolySmiteII() : Skill("Holy Smite II", 22, 2, 55, 6)
{
    description = "Holy damage: base/4 + WIS/2 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void HolySmiteII::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Radiance::Radiance() : Skill("Radiance", 20, 3, 0, 7)
{
    description = "+5 DEF & heal 20 + WIS/3";
    InitializeUpgrades();
}

void Radiance::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(5 + GetTotalDefenseBonus());
    caster.RestoreHealth(20 + (caster.GetStats().wisdom / 3) + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

SmiteUndead::SmiteUndead() : Skill("Smite Undead", 25, 2, 65, 10)
{
    description = "Holy damage: base/4 + WIS/2 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void SmiteUndead::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

Barrier::Barrier() : Skill("Barrier", 28, 4, 0, 12)
{
    description = "+8 DEF & heal 30 + WIS/2";
    InitializeUpgrades();
}

void Barrier::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    caster.RestoreHealth(30 + (caster.GetStats().wisdom / 2) + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

PrayerOfHealing::PrayerOfHealing() : Skill("Prayer of Healing", 40, 3, 0, 14)
{
    description = "Restore 70 + WIS HP";
    InitializeUpgrades();
}

void PrayerOfHealing::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 70 + caster.GetStats().wisdom + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(2);
    ResetCooldown();
}

Purify::Purify() : Skill("Purify", 30, 3, 50, 16)
{
    description = "Holy damage + heal self: base/4 + WIS/3 + weapon; heal 25 + WIS/3";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void Purify::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(25 + (caster.GetStats().wisdom / 3) + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

Benediction::Benediction() : Skill("Benediction", 35, 4, 0, 18)
{
    description = "+10 DEF & restore 30 + WIS/3 mana";
    InitializeUpgrades();
}

void Benediction::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(10 + GetTotalDefenseBonus());
    caster.RestoreMana(30 + (caster.GetStats().wisdom / 3));
    GainXP(2);
    ResetCooldown();
}

HolyFire::HolyFire() : Skill("Holy Fire", 38, 3, 85, 20)
{
    description = "Fire damage: base/4 + WIS + weapon damage";
    element = ElementType::Fire;
    InitializeUpgrades();
}

void HolyFire::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + caster.GetStats().wisdom + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

ConsecratedGround::ConsecratedGround() : Skill("Consecrated Ground", 32, 3, 60, 22)
{
    description = "Holy damage + heal self: base/4 + WIS/2 + weapon; heal 35 + WIS/3";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void ConsecratedGround::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(35 + (caster.GetStats().wisdom / 3) + GetTotalHealBonus());
    GainXP(3);
    ResetCooldown();
}

Serenity::Serenity() : Skill("Serenity", 42, 4, 0, 24)
{
    description = "Restore 100 + WIS HP";
    InitializeUpgrades();
}

void Serenity::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 100 + caster.GetStats().wisdom + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(3);
    ResetCooldown();
}

ArchangelsBlessing::ArchangelsBlessing() : Skill("Archangel's Blessing", 60, 5, 0, 26)
{
    description = "Restore 140 + WIS*1.5 HP";
    InitializeUpgrades();
}

void ArchangelsBlessing::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 140 + (caster.GetStats().wisdom * 3 / 2) + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(3);
    ResetCooldown();
}

Judgement::Judgement() : Skill("Judgement", 45, 3, 100, 28)
{
    description = "Holy damage: base/4 + WIS*1.5 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void Judgement::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

Ascension::Ascension() : Skill("Ascension", 50, 5, 0, 30)
{
    description = "+12 DEF & heal 60 + WIS & restore 40 mana";
    InitializeUpgrades();
}

void Ascension::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(12 + GetTotalDefenseBonus());
    caster.RestoreHealth(60 + caster.GetStats().wisdom + GetTotalHealBonus());
    caster.RestoreMana(40);
    GainXP(3);
    ResetCooldown();
}

Exorcism::Exorcism() : Skill("Exorcism", 55, 4, 130, 32)
{
    description = "Holy damage: base/4 + WIS*1.5 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void Exorcism::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

DivineHurricane::DivineHurricane() : Skill("Divine Hurricane", 70, 5, 180, 34)
{
    description = "Holy damage: base/4 + WIS*2 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void DivineHurricane::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

Penance::Penance() : Skill("Penance", 55, 4, 110, 36)
{
    description = "Holy damage + heal self: base/4 + WIS*1.5 + weapon; heal 50 + WIS/2";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void Penance::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    caster.RestoreHealth(50 + (caster.GetStats().wisdom / 2) + GetTotalHealBonus());
    GainXP(3);
    ResetCooldown();
}

Rapture::Rapture() : Skill("Rapture", 60, 5, 0, 38)
{
    description = "+15 DEF & heal 80 + WIS & restore 50 mana";
    InitializeUpgrades();
}

void Rapture::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(15 + GetTotalDefenseBonus());
    caster.RestoreHealth(80 + caster.GetStats().wisdom + GetTotalHealBonus());
    caster.RestoreMana(50);
    GainXP(3);
    ResetCooldown();
}

HolyPrism::HolyPrism() : Skill("Holy Prism", 75, 5, 200, 42)
{
    description = "Holy damage: base/4 + WIS*2 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void HolyPrism::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

GraceOfGod::GraceOfGod() : Skill("Grace of God", 80, 6, 0, 44)
{
    description = "Restore 200 + WIS*2 HP";
    InitializeUpgrades();
}

void GraceOfGod::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int healAmount = 200 + (caster.GetStats().wisdom * 2) + GetTotalHealBonus();
    caster.RestoreHealth(healAmount);
    GainXP(3);
    ResetCooldown();
}

CelestialJudgment::CelestialJudgment() : Skill("Celestial Judgment", 90, 6, 250, 46)
{
    description = "Holy damage: base/4 + WIS*2.5 + weapon damage";
    element = ElementType::Holy;
    InitializeUpgrades();
}

void CelestialJudgment::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom * 5 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

Apotheosis::Apotheosis() : Skill("Apotheosis", 100, 8, 0, 50)
{
    description = "+20 DEF & full HP & full mana";
    InitializeUpgrades();
}

void Apotheosis::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(20 + GetTotalDefenseBonus());
    caster.RestoreHealth(caster.GetMaxHealth() - caster.GetCurrentHealth());
    caster.RestoreMana(9999);
    GainXP(3);
    ResetCooldown();
}

std::string HolySmite::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon"; }
int HolySmite::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Heal::GetDamageFormula() const { return "Heal: 40 + WIS/2"; }
int Heal::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 40 + (stats.wisdom / 2) + GetTotalHealBonus();
}

std::string MassHeal::GetDamageFormula() const { return "Heal: 60 + WIS"; }
int MassHeal::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 60 + stats.wisdom + GetTotalHealBonus();
}

std::string DivineShield::GetDamageFormula() const { return "+DEF & Heal (self)"; }

std::string Smite::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon"; }
int Smite::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Renew::GetDamageFormula() const { return "Heal: 90 + WIS"; }
int Renew::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 90 + stats.wisdom + GetTotalHealBonus();
}

std::string HolyNova::GetDamageFormula() const { return "base/4 + WIS + Weapon, heals self"; }
int HolyNova::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.wisdom + weaponDamage + elementalBonus);
}

std::string DivineWrath::GetDamageFormula() const { return "base/4 + WIS*2 + Weapon"; }
int DivineWrath::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 2) + weaponDamage + elementalBonus);
}

std::string PrayerOfMending::GetDamageFormula() const { return "Heal: 25 + WIS/3"; }
int PrayerOfMending::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 25 + (stats.wisdom / 3) + GetTotalHealBonus();
}

std::string CleansingLight::GetDamageFormula() const { return "Heal: 35 + WIS/2"; }
int CleansingLight::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 35 + (stats.wisdom / 2) + GetTotalHealBonus();
}

std::string HolySmiteII::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon"; }
int HolySmiteII::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Radiance::GetDamageFormula() const { return "+DEF & Heal (self)"; }
int Radiance::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

std::string SmiteUndead::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon"; }
int SmiteUndead::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Barrier::GetDamageFormula() const { return "+DEF & Heal (self)"; }
int Barrier::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

std::string PrayerOfHealing::GetDamageFormula() const { return "Heal: 70 + WIS"; }
int PrayerOfHealing::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 70 + stats.wisdom + GetTotalHealBonus();
}

std::string Purify::GetDamageFormula() const { return "base/4 + WIS/3 + Weapon, heals self"; }
int Purify::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 3) + weaponDamage + elementalBonus);
}

std::string Benediction::GetDamageFormula() const { return "+DEF & Restore Mana"; }
int Benediction::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

std::string HolyFire::GetDamageFormula() const { return "base/4 + WIS + Weapon"; }
int HolyFire::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + stats.wisdom + weaponDamage + elementalBonus);
}

std::string ConsecratedGround::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon, heals self"; }
int ConsecratedGround::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Serenity::GetDamageFormula() const { return "Heal: 100 + WIS"; }
int Serenity::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 100 + stats.wisdom + GetTotalHealBonus();
}

std::string ArchangelsBlessing::GetDamageFormula() const { return "Heal: 140 + WIS*1.5"; }
int ArchangelsBlessing::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 140 + (stats.wisdom * 3 / 2) + GetTotalHealBonus();
}

std::string Judgement::GetDamageFormula() const { return "base/4 + WIS*1.5 + Weapon"; }
int Judgement::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 3 / 2) + weaponDamage + elementalBonus);
}

std::string Ascension::GetDamageFormula() const { return "+DEF & Heal (self) & Restore Mana"; }
int Ascension::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

std::string Exorcism::GetDamageFormula() const { return "base/4 + WIS*1.5 + Weapon"; }
int Exorcism::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 3 / 2) + weaponDamage + elementalBonus);
}

std::string DivineHurricane::GetDamageFormula() const { return "base/4 + WIS*2 + Weapon"; }
int DivineHurricane::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 2) + weaponDamage + elementalBonus);
}

std::string Penance::GetDamageFormula() const { return "base/4 + WIS*1.5 + Weapon, heals self"; }
int Penance::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 3 / 2) + weaponDamage + elementalBonus);
}

std::string Rapture::GetDamageFormula() const { return "+DEF & Heal (self) & Restore Mana"; }
int Rapture::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

std::string HolyPrism::GetDamageFormula() const { return "base/4 + WIS*2 + Weapon"; }
int HolyPrism::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 2) + weaponDamage + elementalBonus);
}

std::string GraceOfGod::GetDamageFormula() const { return "Heal: 200 + WIS*2"; }
int GraceOfGod::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 200 + (stats.wisdom * 2) + GetTotalHealBonus();
}

std::string CelestialJudgment::GetDamageFormula() const { return "base/4 + WIS*2.5 + Weapon"; }
int CelestialJudgment::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom * 5 / 2) + weaponDamage + elementalBonus);
}

std::string Apotheosis::GetDamageFormula() const { return "+DEF & Full Heal (self) & Full Mana"; }
int Apotheosis::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}
