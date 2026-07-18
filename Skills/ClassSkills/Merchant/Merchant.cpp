#include "Merchant.hpp"
#include "../../../Characters/Character.hpp"

ThrowCoin::ThrowCoin() : Skill("Throw Coin", 5, 1, 35, 1)
{
        description = "9 base + DEX/3 + weapon damage";
        InitializeUpgrades();
}

void ThrowCoin::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

Appraise::Appraise() : Skill("Appraise", 0, 3, 0, 3)
{
        description = "+3 DEF & restore 15+WIS/2 mana (self)";
        InitializeUpgrades();
}

void Appraise::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(3 + GetTotalDefenseBonus());
    caster.RestoreMana(15 + caster.GetStats().wisdom / 2 + GetTotalHealBonus());
    GainXP(1);
    ResetCooldown();
}

Haggle::Haggle() : Skill("Haggle", 15, 4, 0, 3)
{
        description = "Heal 25+VIT HP, drain 10 enemy mana";
        InitializeUpgrades();
}

void Haggle::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int heal = 25 + caster.GetStats().vitality + GetTotalHealBonus();
    caster.RestoreHealth(heal);
    target.ReduceMana(10);
    GainXP(2);
    ResetCooldown();
}

Bribery::Bribery() : Skill("Bribery", 20, 2, 60, 10)
{
        description = "15 base + WIS/2 + weapon damage (req. Lv.10)";
        InitializeUpgrades();
}

void Bribery::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().wisdom / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

Invest::Invest() : Skill("Invest", 25, 4, 0, 15)
{
        description = "+5 DEF & restore 30+INT/2 mana (self, req. Lv.15)";
        InitializeUpgrades();
}

void Invest::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(5 + GetTotalDefenseBonus());
    caster.RestoreMana(30 + caster.GetStats().intelligence / 2 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

CoinStorm::CoinStorm() : Skill("Coin Storm", 40, 4, 35, 25)
{
        description = "9 base + DEX/4 + weapon damage (hits 3 times, req. Lv.25)";
        InitializeUpgrades();
}

void CoinStorm::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 4) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

GoldenHurricane::GoldenHurricane() : Skill("Golden Hurricane", 55, 5, 180, 40)
{
        description = "45 base + DEX*2 + weapon damage (req. Lv.40)";
        InitializeUpgrades();
}

void GoldenHurricane::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(4);
    ResetCooldown();
}

std::string ThrowCoin::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon"; }
int ThrowCoin::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

std::string Appraise::GetDamageFormula() const { return "+DEF & Restore mana (self)"; }

std::string Haggle::GetDamageFormula() const { return "Heal: 25 + VIT, drain 10 enemy mana"; }
int Haggle::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return 25 + stats.vitality + GetTotalHealBonus();
}

std::string Bribery::GetDamageFormula() const { return "base/4 + WIS/2 + Weapon"; }
int Bribery::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.wisdom / 2) + weaponDamage + elementalBonus);
}

std::string Invest::GetDamageFormula() const { return "+DEF & Restore mana (self)"; }

std::string CoinStorm::GetDamageFormula() const { return "base/4 + DEX/4 + Weapon (3 full-damage hits)"; }
int CoinStorm::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 4) + weaponDamage + elementalBonus);
    return hit * 3;
}

std::string GoldenHurricane::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int GoldenHurricane::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}
