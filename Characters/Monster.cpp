#include "Monster.hpp"
#include <cstdlib>

Monster::Monster(const std::string& name, const Stats& baseStats, int experienceReward, int goldReward)
    : Character(name, baseStats), experienceReward(experienceReward), goldReward(goldReward)
{
}

void Monster::DecideAction()
{
    // Simple AI: randomly choose between attack and do nothing
    // Can be expanded with more sophisticated behavior
    if (rand() % 2 == 0)
    {
        // Would perform attack
    }
    // else: do nothing this turn
}
