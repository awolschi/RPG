#include "Merchant.hpp"
#include "../../../Characters/Character.hpp"
#include "../../../Characters/Player.hpp"

ThrowCoin::ThrowCoin() : Skill("Throw Coin", 0, 1, 35, 1)
{
        description = "9 base + DEX/3 + weapon damage (costs 5 gold)";
        InitializeUpgrades();
}

void ThrowCoin::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;

    auto* player = dynamic_cast<Player*>(&caster);
    if (!player || player->GetInventory().GetGold() < goldCost) return;
    player->GetInventory().RemoveGold(goldCost);

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

std::string ThrowCoin::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon (5 gold)"; }
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

PurchaseOrder::PurchaseOrder() : Skill("Purchase Order", 5, 1, 30, 2)
{
    description = "7 base + DEX/3 + weapon damage";
    InitializeUpgrades();
}

void PurchaseOrder::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string PurchaseOrder::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon"; }
int PurchaseOrder::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

AssessWeakness::AssessWeakness() : Skill("Assess Weakness", 8, 2, 40, 4)
{
    description = "10 base + DEX/3 + weapon damage";
    InitializeUpgrades();
}

void AssessWeakness::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string AssessWeakness::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon"; }
int AssessWeakness::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

PriceCheck::PriceCheck() : Skill("Price Check", 10, 3, 0, 5)
{
    description = "+4 DEF & restore 10+INT/3 mana (self)";
    InitializeUpgrades();
}

void PriceCheck::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(4 + GetTotalDefenseBonus());
    caster.RestoreMana(10 + caster.GetStats().intelligence / 3);
    GainXP(1);
    ResetCooldown();
}

std::string PriceCheck::GetDamageFormula() const { return "+4 DEF & Restore mana (self)"; }

MarketSwing::MarketSwing() : Skill("Market Swing", 15, 2, 50, 6)
{
    description = "12 base + DEX/3 + weapon damage";
    InitializeUpgrades();
}

void MarketSwing::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string MarketSwing::GetDamageFormula() const { return "base/4 + DEX/3 + Weapon"; }
int MarketSwing::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 3) + weaponDamage + elementalBonus);
}

BulkDeal::BulkDeal() : Skill("Bulk Deal", 18, 2, 35, 8)
{
    description = "8 base + DEX/4 + weapon damage (2 hits)";
    InitializeUpgrades();
}

void BulkDeal::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 4) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(1);
    ResetCooldown();
}

std::string BulkDeal::GetDamageFormula() const { return "base/4 + DEX/4 + Weapon (2 full-damage hits)"; }
int BulkDeal::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int hit = ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 4) + weaponDamage + elementalBonus);
    return hit * 2;
}

Liquidate::Liquidate() : Skill("Liquidate", 20, 3, 60, 10)
{
    description = "15 base + DEX/2 + weapon damage";
    InitializeUpgrades();
}

void Liquidate::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string Liquidate::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon"; }
int Liquidate::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

HostileTakeover::HostileTakeover() : Skill("Hostile Takeover", 25, 3, 70, 12)
{
    description = "17 base + DEX/2 + weapon damage, drains 15 mana";
    InitializeUpgrades();
}

void HostileTakeover::Use(Character& caster, Character& target)
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

std::string HostileTakeover::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon, drain 15 mana"; }
int HostileTakeover::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

InsurancePolicy::InsurancePolicy() : Skill("Insurance Policy", 20, 4, 0, 14)
{
    description = "+6 DEF & heal 25+VIT/2 HP";
    InitializeUpgrades();
}

void InsurancePolicy::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(6 + GetTotalDefenseBonus());
    caster.RestoreHealth(25 + caster.GetStats().vitality / 2 + GetTotalHealBonus());
    GainXP(2);
    ResetCooldown();
}

std::string InsurancePolicy::GetDamageFormula() const { return "+6 DEF & Heal HP"; }

ShortSell::ShortSell() : Skill("Short Sell", 22, 2, 75, 16)
{
    description = "18 base + DEX/2 + weapon damage";
    InitializeUpgrades();
}

void ShortSell::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(2);
    ResetCooldown();
}

std::string ShortSell::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon"; }
int ShortSell::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

PredatoryLending::PredatoryLending() : Skill("Predatory Lending", 28, 3, 80, 18)
{
    description = "20 base + DEX/2 + weapon damage, drains 20 mana";
    InitializeUpgrades();
}

void PredatoryLending::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(20);
    GainXP(2);
    ResetCooldown();
}

std::string PredatoryLending::GetDamageFormula() const { return "base/4 + DEX/2 + Weapon, drain 20 mana"; }
int PredatoryLending::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity / 2) + weaponDamage + elementalBonus);
}

GoldReserves::GoldReserves() : Skill("Gold Reserves", 25, 4, 0, 20)
{
    description = "+8 DEF & restore 25+INT/2 mana (self)";
    InitializeUpgrades();
}

void GoldReserves::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(8 + GetTotalDefenseBonus());
    caster.RestoreMana(25 + caster.GetStats().intelligence / 2);
    GainXP(2);
    ResetCooldown();
}

std::string GoldReserves::GetDamageFormula() const { return "+8 DEF & Restore mana (self)"; }

MarginCall::MarginCall() : Skill("Margin Call", 30, 3, 90, 22)
{
    description = "22 base + DEX*1.5 + weapon damage";
    InitializeUpgrades();
}

void MarginCall::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3) / 2 + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string MarginCall::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon"; }
int MarginCall::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3) / 2 + weaponDamage + elementalBonus);
}

Monopoly::Monopoly() : Skill("Monopoly", 35, 4, 0, 24)
{
    description = "+10 DEF & restore 35+INT/2 mana (self)";
    InitializeUpgrades();
}

void Monopoly::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(10 + GetTotalDefenseBonus());
    caster.RestoreMana(35 + caster.GetStats().intelligence / 2);
    GainXP(3);
    ResetCooldown();
}

std::string Monopoly::GetDamageFormula() const { return "+10 DEF & Restore mana (self)"; }

MarketCrash::MarketCrash() : Skill("Market Crash", 38, 4, 100, 26)
{
    description = "25 base + DEX*1.5 + weapon damage";
    InitializeUpgrades();
}

void MarketCrash::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3) / 2 + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string MarketCrash::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon"; }
int MarketCrash::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3) / 2 + weaponDamage + elementalBonus);
}

Taxation::Taxation() : Skill("Taxation", 40, 4, 110, 28)
{
    description = "27 base + DEX*1.5 + weapon damage, drains 25 mana";
    InitializeUpgrades();
}

void Taxation::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3) / 2 + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    target.ReduceMana(25);
    GainXP(3);
    ResetCooldown();
}

std::string Taxation::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon, drain 25 mana"; }
int Taxation::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3) / 2 + weaponDamage + elementalBonus);
}

Plunder::Plunder() : Skill("Plunder", 45, 4, 120, 30)
{
    description = "30 base + DEX*1.5 + weapon damage";
    InitializeUpgrades();
}

void Plunder::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3) / 2 + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Plunder::GetDamageFormula() const { return "base/4 + DEX*1.5 + Weapon"; }
int Plunder::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3) / 2 + weaponDamage + elementalBonus);
}

CornerMarket::CornerMarket() : Skill("Corner Market", 42, 5, 0, 32)
{
    description = "+12 DEF & heal 50+VIT HP, restore 40 mana";
    InitializeUpgrades();
}

void CornerMarket::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    caster.IncreaseTempDefense(12 + GetTotalDefenseBonus());
    caster.RestoreHealth(50 + caster.GetStats().vitality + GetTotalHealBonus());
    caster.RestoreMana(40);
    GainXP(3);
    ResetCooldown();
}

std::string CornerMarket::GetDamageFormula() const { return "+12 DEF & Heal HP, Restore mana"; }

FortunesFavor::FortunesFavor() : Skill("Fortune's Favor", 50, 4, 130, 34)
{
    description = "32 base + DEX*2 + weapon damage";
    InitializeUpgrades();
}

void FortunesFavor::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string FortunesFavor::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int FortunesFavor::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

HostileBid::HostileBid() : Skill("Hostile Bid", 52, 4, 140, 36)
{
    description = "35 base + DEX*2 + weapon damage";
    InitializeUpgrades();
}

void HostileBid::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string HostileBid::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int HostileBid::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

EconomicCollapse::EconomicCollapse() : Skill("Economic Collapse", 55, 5, 150, 38)
{
    description = "37 base + DEX*2 + weapon damage";
    InitializeUpgrades();
}

void EconomicCollapse::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string EconomicCollapse::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int EconomicCollapse::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

Monopolize::Monopolize() : Skill("Monopolize", 60, 5, 160, 42)
{
    description = "40 base + DEX*2 + weapon damage";
    InitializeUpgrades();
}

void Monopolize::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string Monopolize::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int Monopolize::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

HostileMerger::HostileMerger() : Skill("Hostile Merger", 65, 5, 180, 44)
{
    description = "45 base + DEX*2 + weapon damage";
    InitializeUpgrades();
}

void HostileMerger::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string HostileMerger::GetDamageFormula() const { return "base/4 + DEX*2 + Weapon"; }
int HostileMerger::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 2) + weaponDamage + elementalBonus);
}

GoldenHandshake::GoldenHandshake() : Skill("Golden Handshake", 70, 6, 200, 46)
{
    description = "50 base + DEX*2.5 + weapon damage";
    InitializeUpgrades();
}

void GoldenHandshake::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 5) / 2 + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string GoldenHandshake::GetDamageFormula() const { return "base/4 + DEX*2.5 + Weapon"; }
int GoldenHandshake::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 5) / 2 + weaponDamage + elementalBonus);
}

LegendaryBargain::LegendaryBargain() : Skill("Legendary Bargain", 80, 8, 300, 50)
{
    description = "75 base + DEX*3 + weapon damage";
    InitializeUpgrades();
}

void LegendaryBargain::Use(Character& caster, Character& target)
{
    if (!IsReady()) return;
    int damage = (baseDamage / 4) + (caster.GetStats().dexterity * 3) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));
    GainXP(3);
    ResetCooldown();
}

std::string LegendaryBargain::GetDamageFormula() const { return "base/4 + DEX*3 + Weapon"; }
int LegendaryBargain::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    return ApplyDamageBonus((baseDamage / 4) + (stats.dexterity * 3) + weaponDamage + elementalBonus);
}
