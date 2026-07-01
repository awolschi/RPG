#ifndef MAGE_HPP
#define MAGE_HPP

#include "../../Skill.hpp"

class Fireball : public Skill
{
public:
    Fireball();
    void Use(Character& caster, Character& target) override;
};

class IceBolt : public Skill
{
public:
    IceBolt();
    void Use(Character& caster, Character& target) override;
};

class Meteor : public Skill
{
public:
    Meteor();
    void Use(Character& caster, Character& target) override;
};

#endif
