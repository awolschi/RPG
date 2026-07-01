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

#endif
