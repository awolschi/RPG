#ifndef PRIEST_HPP
#define PRIEST_HPP

#include "../../Skill.hpp"

class HolySmite : public Skill
{
public:
    HolySmite();
    void Use(Character& caster, Character& target) override;
};

class Heal : public Skill
{
public:
    Heal();
    void Use(Character& caster, Character& target) override;
};

class MassHeal : public Skill
{
public:
    MassHeal();
    void Use(Character& caster, Character& target) override;
};

#endif
