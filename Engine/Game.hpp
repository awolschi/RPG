#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>
#include "Combat.hpp"
#include "../Characters/Player.hpp"
#include "../Characters/Monster.hpp"

enum class GameState
{
    MainMenu,
    CharacterCreation,
    Exploring,
    InCombat,
    Inventory,
    Stats,
    Jobs,
    EnemySelection,
    Exit
};

class Game
{
public:
    Game();

    void Run();

private:
    GameState currentState;
    std::shared_ptr<Player> player;
    std::shared_ptr<CombatSystem> combatSystem;

    // Main game states
    void MainMenu();
    void CharacterCreation();
    void ExploreMenu();
    void InventoryMenu();
    void StatsMenu();
    void JobsMenu();
    void EnemySelectionMenu();

    // Combat and encounters
    void StartRandomEncounter();
    void StartCombatWithEnemy(std::shared_ptr<Monster> enemy);
    void CreateTestMonster(int difficulty);
    void DisplayLootReward(std::shared_ptr<Monster> enemy);

    // Utility
    void ClearScreen();
    void DisplayWelcome();
};

#endif
