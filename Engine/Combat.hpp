#ifndef COMBAT_HPP
#define COMBAT_HPP

#include <memory>
#include <iostream>
#include "../Characters/Character.hpp"

enum class CombatAction
{
    Attack,
    UseSkill,
    Defend,
    Flee,
    UseItem
};

class CombatSystem
{
public:
    CombatSystem() = default;

    // Combat flow
    bool StartCombat(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy);
    bool ExecuteTurn(std::shared_ptr<Character> attacker, std::shared_ptr<Character> defender, CombatAction action, int skillIndex = 0);

    // Utility
    void DisplayCombatStatus(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy);
    void DisplaySkillOptions(std::shared_ptr<Character> character);

private:
    void DisplayCombatLog(const std::string& message);
};

#endif
