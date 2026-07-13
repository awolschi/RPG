#ifndef ARCHER_HPP
#define ARCHER_HPP

#include "../../Skill.hpp"

class PiercingShot : public Skill
{
public:
    PiercingShot();
    void Use(Character& caster, Character& target) override;
};

class MultiShot : public Skill
{
public:
    MultiShot();
    void Use(Character& caster, Character& target) override;
};

class RainOfArrows : public Skill
{
public:
    RainOfArrows();
    void Use(Character& caster, Character& target) override;
};

class QuickShot : public Skill
{
public:
    QuickShot();
    void Use(Character& caster, Character& target) override;
};

class Snipe : public Skill
{
public:
    Snipe();
    void Use(Character& caster, Character& target) override;
};

class ArrowStorm : public Skill
{
public:
    ArrowStorm();
    void Use(Character& caster, Character& target) override;
};

class DeathMark : public Skill
{
public:
    DeathMark();
    void Use(Character& caster, Character& target) override;
};

#endif
