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

PoisonedArrow::PoisonedArrow() : Skill("Poisoned Arrow", 8, 1, 35, 2)
{
    description = "Poisoned arrow: base/4 + DEX/3 + weapon damage, Poison 2 turns";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void PoisonedArrow::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Poison, 2 + GetTotalEffectDurationBonus(), 3 + caster.GetStats().dexterity / 5 + GetTotalEffectDamageBonus(), caster.GetName());
    GainXP(1);
    ResetCooldown();
}

std::string PoisonedArrow::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon, Poison 2 turns"; }
int PoisonedArrow::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

Evade::Evade() : Skill("Evade", 10, 3, 0, 4)
{
    description = "Evasive stance: +5 DEF";
    InitializeUpgrades();
}

void Evade::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(5 + GetTotalDefenseBonus());
    GainXP(1);
    ResetCooldown();
}

std::string Evade::GetDamageFormula() const { return "+5 DEF self"; }
int Evade::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

AimedShot::AimedShot() : Skill("Aimed Shot", 15, 2, 55, 6)
{
    description = "Carefully aimed: base/4 + DEX/2 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void AimedShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string AimedShot::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon"; }
int AimedShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

Volley::Volley() : Skill("Volley", 22, 3, 40, 8)
{
    description = "Volley of arrows: base/4 + DEX/3 + weapon damage (2 hits)";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void Volley::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string Volley::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (2 hits)"; }
int Volley::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit * 2;
}

TrapShot::TrapShot() : Skill("Trap Shot", 18, 3, 50, 10)
{
    description = "Trapping shot: base/4 + DEX/3 + weapon damage, Poison 3 turns";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void TrapShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Poison, 3 + GetTotalEffectDurationBonus(), 3 + caster.GetStats().dexterity / 5 + GetTotalEffectDamageBonus(), caster.GetName());
    GainXP(2);
    ResetCooldown();
}

std::string TrapShot::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon, Poison 3 turns"; }
int TrapShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

HawkEye::HawkEye() : Skill("Hawk Eye", 15, 4, 0, 12)
{
    description = "Sharp focus: +6 DEF & +damage next attack";
    InitializeUpgrades();
}

void HawkEye::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(6 + GetTotalDefenseBonus());
    GainXP(1);
    ResetCooldown();
}

std::string HawkEye::GetDamageFormula() const { return "+6 DEF self"; }
int HawkEye::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

ExploitWeakness::ExploitWeakness() : Skill("Exploit Weakness", 20, 2, 65, 14)
{
    description = "Strike the gap: base/4 + DEX/2 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void ExploitWeakness::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string ExploitWeakness::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon"; }
int ExploitWeakness::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

ConcussiveShot::ConcussiveShot() : Skill("Concussive Shot", 25, 3, 60, 16)
{
    description = "Stunning impact: base/4 + DEX/2 + weapon damage, drains 15 enemy mana";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void ConcussiveShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(15);
    GainXP(2);
    ResetCooldown();
}

std::string ConcussiveShot::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon, drain 15 mana"; }
int ConcussiveShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

Camouflage::Camouflage() : Skill("Camouflage", 20, 4, 0, 18)
{
    description = "Blend into surroundings: +8 DEF";
    InitializeUpgrades();
}

void Camouflage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    GainXP(1);
    ResetCooldown();
}

std::string Camouflage::GetDamageFormula() const { return "+8 DEF self"; }
int Camouflage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

DoubleTap::DoubleTap() : Skill("Double Tap", 22, 2, 45, 20)
{
    description = "Quick double strike: base/4 + DEX/3 + weapon damage (2 hits)";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void DoubleTap::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string DoubleTap::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (2 hits)"; }
int DoubleTap::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit * 2;
}

ScatterShot::ScatterShot() : Skill("Scatter Shot", 28, 3, 70, 22)
{
    description = "Wide spread: base/4 + DEX/2 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void ScatterShot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string ScatterShot::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon"; }
int ScatterShot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

MarkTarget::MarkTarget() : Skill("Mark Target", 20, 4, 0, 24)
{
    description = "Mark for death: drains 25 enemy mana";
    InitializeUpgrades();
}

void MarkTarget::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    target.ReduceMana(25);
    GainXP(1);
    ResetCooldown();
}

std::string MarkTarget::GetDamageFormula() const { return "drains 25 enemy mana"; }
int MarkTarget::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

Headshot::Headshot() : Skill("Headshot", 35, 3, 100, 26)
{
    description = "Precise headshot: base/4 + DEX*1.5 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void Headshot::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string Headshot::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon"; }
int Headshot::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3 / 2) + weaponDamage + elementalBonus);
}

Barrage::Barrage() : Skill("Barrage", 38, 4, 55, 28)
{
    description = "Relentless barrage: base/4 + DEX/3 + weapon damage (3 hits)";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void Barrage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Barrage::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (3 hits)"; }
int Barrage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit * 3;
}

Shadowmeld::Shadowmeld() : Skill("Shadowmeld", 30, 5, 0, 30)
{
    description = "Merge with shadows: +10 DEF";
    InitializeUpgrades();
}

void Shadowmeld::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(10 + GetTotalDefenseBonus());
    GainXP(2);
    ResetCooldown();
}

std::string Shadowmeld::GetDamageFormula() const { return "+10 DEF self"; }
int Shadowmeld::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

StormOfBlades::StormOfBlades() : Skill("Storm of Blades", 42, 4, 130, 32)
{
    description = "Whirlwind of steel: base/4 + DEX*1.5 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void StormOfBlades::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string StormOfBlades::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon"; }
int StormOfBlades::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3 / 2) + weaponDamage + elementalBonus);
}

EvasiveManeuvers::EvasiveManeuvers() : Skill("Evasive Maneuvers", 35, 5, 0, 34)
{
    description = "Elusive defense: +12 DEF & heal 40+DEX/3";
    InitializeUpgrades();
}

void EvasiveManeuvers::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(12 + GetTotalDefenseBonus());
    int heal = 40 + (caster.GetStats().dexterity / 3) + GetTotalHealBonus();
    caster.RestoreHealth(heal);
    GainXP(2);
    ResetCooldown();
}

std::string EvasiveManeuvers::GetDamageFormula() const { return "+12 DEF self, heal 40+DEX/3"; }
int EvasiveManeuvers::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

RapidFire::RapidFire() : Skill("Rapid Fire", 45, 4, 60, 36)
{
    description = "Blazing speed: base/4 + DEX/3 + weapon damage (3 hits)";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void RapidFire::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string RapidFire::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (3 hits)"; }
int RapidFire::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
    return hit * 3;
}

Windrunner::Windrunner() : Skill("Windrunner", 48, 5, 150, 38)
{
    description = "Wind-guided arrow: base/4 + DEX*2 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void Windrunner::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Windrunner::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int Windrunner::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

PiercingBarrage::PiercingBarrage() : Skill("Piercing Barrage", 50, 4, 80, 42)
{
    description = "Piercing volley: base/4 + DEX/2 + weapon damage (3 hits), Poison 3 turns";
    element = ElementType::Poison;
    InitializeUpgrades();
}

void PiercingBarrage::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ApplyEffect(EffectType::Poison, 3 + GetTotalEffectDurationBonus(), 3 + caster.GetStats().dexterity / 5 + GetTotalEffectDamageBonus(), caster.GetName());
    GainXP(3);
    ResetCooldown();
}

std::string PiercingBarrage::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon (3 hits), Poison 3 turns"; }
int PiercingBarrage::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
    return hit * 3;
}

ShadowStep::ShadowStep() : Skill("Shadow Step", 40, 5, 0, 44)
{
    description = "Step through shadows: +15 DEF";
    InitializeUpgrades();
}

void ShadowStep::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(15 + GetTotalDefenseBonus());
    GainXP(2);
    ResetCooldown();
}

std::string ShadowStep::GetDamageFormula() const { return "+15 DEF self"; }
int ShadowStep::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 0;
}

WindArrow::WindArrow() : Skill("Wind Arrow", 55, 5, 180, 46)
{
    description = "Gale force: base/4 + DEX*2 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void WindArrow::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string WindArrow::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int WindArrow::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

EaglesWrath::EaglesWrath() : Skill("Eagle's Wrath", 60, 6, 200, 48)
{
    description = "Raptor fury: base/4 + DEX*2.5 + weapon damage";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void EaglesWrath::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 5 / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string EaglesWrath::GetDamageFormula() const { return "base/4 + DEX*2.5 + Weapon"; }
int EaglesWrath::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 5 / 2) + weaponDamage + elementalBonus);
}

PhantomVolley::PhantomVolley() : Skill("Phantom Volley", 70, 6, 100, 50)
{
    description = "Spectral barrage: base/4 + DEX*2 + weapon damage (3 hits)";
    element = ElementType::Physical;
    InitializeUpgrades();
}

void PhantomVolley::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string PhantomVolley::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon (3 hits)"; }
int PhantomVolley::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
    return hit * 3;
}
