#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include "../../Skill.hpp"

class PowerStrike : public Skill
{
public:
    PowerStrike();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Whirlwind : public Skill
{
public:
    Whirlwind();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class DefensiveStance : public Skill
{
public:
    DefensiveStance();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class WarCry : public Skill
{
public:
    WarCry();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class ShieldBash : public Skill
{
public:
    ShieldBash();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class BattleCry : public Skill
{
public:
    BattleCry();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class Execute : public Skill
{
public:
    Execute();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class WarStomp : public Skill
{
public:
    WarStomp();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

#endif
