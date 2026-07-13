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

class DivineShield : public Skill
{
public:
    DivineShield();
    void Use(Character& caster, Character& target) override;
};

class Smite : public Skill
{
public:
    Smite();
    void Use(Character& caster, Character& target) override;
};

class Renew : public Skill
{
public:
    Renew();
    void Use(Character& caster, Character& target) override;
};

class HolyNova : public Skill
{
public:
    HolyNova();
    void Use(Character& caster, Character& target) override;
};

class DivineWrath : public Skill
{
public:
    DivineWrath();
    void Use(Character& caster, Character& target) override;
};

#endif
