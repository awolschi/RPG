#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Character.hpp"

class Monster : public Character
{
public:
    Monster(const std::string& name, const Stats& baseStats, int experienceReward, int goldReward);

    int GetExperienceReward() const { return experienceReward; }
    int GetGoldReward() const { return goldReward; }

    // AI behavior
    void DecideAction();

protected:
    int experienceReward;
    int goldReward;
};

#endif
