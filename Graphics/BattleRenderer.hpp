#ifndef BATTLE_RENDERER_HPP
#define BATTLE_RENDERER_HPP

#include <string>
#include <vector>
#include "raylib.h"
#include "GRenderer.hpp"
#include "BattleLayout.hpp"

class Character;
class Monster;
class PetManager;
enum class CombatPhase;

namespace BattleRenderer
{
    void DrawBattleScreen(GRenderer& renderer,
                          Character& enemy,
                          Character& player,
                          const std::vector<std::string>& combatLog,
                          CombatPhase phase,
                          float enemyFlashTimer,
                          bool isBoss,
                          const std::string& battleMessage = "",
                          const PetManager* petManager = nullptr,
                          int playerLevel = 1,
                          int playerXP = 0,
                          int playerMaxXP = 0,
                          int reputationValue = 0,
                          int reputationMax = 0,
                          const std::string& repLabel = "");

    void DrawAbilityButton(GRenderer& renderer,
                           const std::string& label,
                           int x, int y, int size,
                           bool hovered, bool pressed,
                           Color iconColor);

    void DrawPlayerBar(GRenderer& renderer,
                       const std::string& name,
                       int currentHP, int maxHP,
                       int currentMP, int maxMP,
                       int y,
                       int currentXP = 0,
                       int maxXP = 0);
}

#endif
