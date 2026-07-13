#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>
#include <vector>
#include "Combat.hpp"
#include "SaveGame.hpp"
#include "Religion.hpp"
#include "Wiki.hpp"
#include "../Graphics/GRenderer.hpp"
#include "../Characters/Player.hpp"
#include "../Characters/Monster.hpp"
#include "../Characters/Races/Races.hpp"
#include "../World/Areas/Area.hpp"
#include "../World/Quests/Quest.hpp"
#include "../World/NPCs/NPC.hpp"
#include "../Items/Crafting/Crafting.hpp"

enum class GameState
{
    MainMenu,
    CharacterCreation,
    Exploring,
    InCombat,
    Inventory,
    Stats,
    Jobs,
    Crafting,
    Religion,
    Shop,
    EnemySelection,
    AreaSelect,
    QuestLog,
    DungeonSelect,
    DungeonExplore,
    DungeonComplete,
    SkillLoadout,
    SavePrompt,
    LoadPrompt,
    Wiki,
    NPCDialogue,
    Exit
};

enum class CombatPhase
{
    PlayerTurn,
    SkillSelect,
    ItemSelect,
    EnemyTurn,
    Victory,
    Defeat
};

class Game
{
public:
    Game();
    void Run();

private:
    GRenderer renderer;
    GameState currentState;
    CombatPhase combatPhase;
    std::shared_ptr<Player> player;
    std::shared_ptr<Character> currentEnemy;
    std::shared_ptr<CombatSystem> combatSystem;
    SaveGameManager saveManager;
    std::vector<Area> areas;
    int currentAreaIndex;
    CraftingManager crafting;
    ReligionSystem religion;
    Wiki wiki;
    std::vector<std::shared_ptr<Item>> shopItems;

    // Combat log
    std::vector<std::string> combatLog;
    double enemyActionTime;

    // Dungeon state
    int currentDungeonIndex;
    int currentDungeonRoom;
    int currentRoomEnemyIndex;
    int dungeonRoomEnemyCount;
    std::vector<std::shared_ptr<Monster>> roomQueue;
    bool isDungeonBossFight;
    bool inDungeonEncounter;
    bool leveledUpThisCombat;
    bool dungeonCompleted;

    // NPC state
    NPC currentNPC;
    int npcDialoguePhase;

    // Quest UI state
    int questTab;
    bool questsChecked;
    int questPage;
    int questMaxPage;

    // Crafting UI state
    int craftPage;
    int craftMaxPage;

    // Main game states
    void StateMainMenu();
    void StateCharacterCreation();
    void StateExplore();
    void StateInventory();
    void StateStats();
    void StateJobs();
    void StateCraft();
    void StateReligion();
    void StateShop();
    void StateSkillLoadout();
    void StateAreaSelect();
    void StateQuestLog();
    void StateWiki();
    void StateCombat();
    void StateDungeonSelect();
    void StateDungeonExplore();
    void StateDungeonComplete();
    void StateNPCDialogue();

    // Combat helpers
    void StartAreaEncounter();
    void StartDungeonEncounter();
    void StartCombatWithEnemy(std::shared_ptr<Monster> enemy,bool isBoss=false);
    void DoPlayerAttack(CombatAction action,int skillIdx=0);
    void DoPlayerUseItem(int inventoryIndex);
    void DoEnemyTurn();
    void AddCombatLog(const std::string& msg);
    void ProcessVictory(std::shared_ptr<Monster> enemy);

    // NPC helpers
    void StartNPCDialogue(const NPC& npc);

    // Save / Load
    void SaveGamePrompt();
    void LoadGamePrompt();
    void SaveToSlot(int slot);
    void LoadFromSlot(int slot);

    // Drawing helpers
    void DrawTopBar();
    void DrawMessagePage(const std::string& title,
                         const std::vector<std::string>& lines,
                         GameState returnState);
    void DrawCharacterCard(int x,int y,int w,int h,
                           const std::string& name,const std::string& subtitle,
                           const std::string& desc,bool selected,bool hover);

    // Utility
    void InitializeAreas();
    void ClearLog();
};

#endif
