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

class Cleave : public Skill
{
public:
    Cleave();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class ShieldWall : public Skill
{
public:
    ShieldWall();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class RallyingCry : public Skill
{
public:
    RallyingCry();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class HammerSlam : public Skill
{
public:
    HammerSlam();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class VictoryRush : public Skill
{
public:
    VictoryRush();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Charge : public Skill
{
public:
    Charge();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class CleaveStrike : public Skill
{
public:
    CleaveStrike();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Intimidate : public Skill
{
public:
    Intimidate();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class BerserkerRage : public Skill
{
public:
    BerserkerRage();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Intercept : public Skill
{
public:
    Intercept();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Fortress : public Skill
{
public:
    Fortress();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class ShieldCharge : public Skill
{
public:
    ShieldCharge();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Devastate : public Skill
{
public:
    Devastate();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Warbanner : public Skill
{
public:
    Warbanner();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
};

class ColossusStrike : public Skill
{
public:
    ColossusStrike();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class Rampage : public Skill
{
public:
    Rampage();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

class TitansGrip : public Skill
{
public:
    TitansGrip();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

#endif
