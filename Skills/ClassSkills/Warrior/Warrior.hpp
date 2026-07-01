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

#endif
