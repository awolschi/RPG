#ifndef COMBAT_HPP
#define COMBAT_HPP

#include <memory>
#include <string>
#include <vector>
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

    // Initialize combat (returns false if invalid)
    bool StartCombat(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy);

    // Execute a single turn; returns description of what happened
    std::string ExecuteTurn(std::shared_ptr<Character> attacker,
                            std::shared_ptr<Character> defender,
                            CombatAction action,
                            int skillIndex = 0);

    // Static helpers
    static std::string GetStatusLine(std::shared_ptr<Character> c);
    static std::vector<std::string> GetSkillOptions(std::shared_ptr<Character> c);
};

#endif
