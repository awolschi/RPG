#ifndef COMMON_ATTACK_HPP
#define COMMON_ATTACK_HPP

#include "../Skill.hpp"

class CommonAttack : public Skill
{
public:
    CommonAttack();
    void Use(Character& caster, Character& target) override;
    std::string GetDamageFormula() const override;
    int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const override;
};

#endif
