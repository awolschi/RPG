#ifndef COMMON_ATTACK_HPP
#define COMMON_ATTACK_HPP

#include "../Skill.hpp"

class CommonAttack : public Skill
{
public:
    CommonAttack();
    void Use(Character& caster, Character& target) override;
};

#endif
