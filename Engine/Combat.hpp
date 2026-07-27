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
    bool StartCombat(const std::shared_ptr<Character>& player, const std::shared_ptr<Character>& enemy);

    // Execute a single turn; returns description of what happened
    std::string ExecuteTurn(const std::shared_ptr<Character>& attacker,
                            const std::shared_ptr<Character>& defender,
                            CombatAction action,
                            int skillIndex = 0);

};

#endif
