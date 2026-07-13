#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include "../../Skill.hpp"

class PowerStrike : public Skill
{
public:
    PowerStrike();
    void Use(Character& caster, Character& target) override;
};

class Whirlwind : public Skill
{
public:
    Whirlwind();
    void Use(Character& caster, Character& target) override;
};

class DefensiveStance : public Skill
{
public:
    DefensiveStance();
    void Use(Character& caster, Character& target) override;
};

class WarCry : public Skill
{
public:
    WarCry();
    void Use(Character& caster, Character& target) override;
};

class ShieldBash : public Skill
{
public:
    ShieldBash();
    void Use(Character& caster, Character& target) override;
};

class BattleCry : public Skill
{
public:
    BattleCry();
    void Use(Character& caster, Character& target) override;
};

class Execute : public Skill
{
public:
    Execute();
    void Use(Character& caster, Character& target) override;
};

class WarStomp : public Skill
{
public:
    WarStomp();
    void Use(Character& caster, Character& target) override;
};

#endif
