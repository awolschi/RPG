#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../../Skill.hpp"

class ThrowCoin : public Skill
{
public:
    ThrowCoin();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
    int goldCost = 5;
};

class Appraise : public Skill
{
public:
    Appraise();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class Haggle : public Skill
{
public:
    Haggle();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Bribery : public Skill
{
public:
    Bribery();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Invest : public Skill
{
public:
    Invest();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class CoinStorm : public Skill
{
public:
    CoinStorm();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class GoldenHurricane : public Skill
{
public:
    GoldenHurricane();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class PurchaseOrder : public Skill
{
public:
    PurchaseOrder();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class AssessWeakness : public Skill
{
public:
    AssessWeakness();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class PriceCheck : public Skill
{
public:
    PriceCheck();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class MarketSwing : public Skill
{
public:
    MarketSwing();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class BulkDeal : public Skill
{
public:
    BulkDeal();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Liquidate : public Skill
{
public:
    Liquidate();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class HostileTakeover : public Skill
{
public:
    HostileTakeover();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class InsurancePolicy : public Skill
{
public:
    InsurancePolicy();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class ShortSell : public Skill
{
public:
    ShortSell();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class PredatoryLending : public Skill
{
public:
    PredatoryLending();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class GoldReserves : public Skill
{
public:
    GoldReserves();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class MarginCall : public Skill
{
public:
    MarginCall();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Monopoly : public Skill
{
public:
    Monopoly();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class MarketCrash : public Skill
{
public:
    MarketCrash();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Taxation : public Skill
{
public:
    Taxation();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Plunder : public Skill
{
public:
    Plunder();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class CornerMarket : public Skill
{
public:
    CornerMarket();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class FortunesFavor : public Skill
{
public:
    FortunesFavor();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class HostileBid : public Skill
{
public:
    HostileBid();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class EconomicCollapse : public Skill
{
public:
    EconomicCollapse();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Monopolize : public Skill
{
public:
    Monopolize();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class HostileMerger : public Skill
{
public:
    HostileMerger();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class GoldenHandshake : public Skill
{
public:
    GoldenHandshake();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class LegendaryBargain : public Skill
{
public:
    LegendaryBargain();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

#endif
