#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>
#include <vector>
#include "raylib.h"
#include "Combat.hpp"
#include "SaveGame.hpp"
#include "Religion.hpp"
#include "Wiki.hpp"
#include "../Graphics/GRenderer.hpp"
#include "KeyboardNav.hpp"
#include "../Characters/Player.hpp"
#include "../Characters/Monster.hpp"
#include "../Characters/Races/Races.hpp"
#include "../World/Areas/Area.hpp"
#include "../World/Quests/Quest.hpp"
#include "../World/NPCs/NPC.hpp"
#include "../Items/Crafting/Crafting.hpp"
#include "../Jobs/JobQuest.hpp"
#include "../Achievements/Achievement.hpp"

struct FloatingText
{
    std::string text;
    float x, y;
    float vy;
    float life;
    float maxLife;
    Color color;
    int fontSize;

    FloatingText(const std::string& t, float px, float py, Color c, int fs = 18, float dur = 1.2f)
        : text(t), x(px), y(py), vy(-40.0f), life(dur), maxLife(dur), color(c), fontSize(fs) {}
};

struct Particle
{
    float x, y;
    float vx, vy;
    float life;
    float maxLife;
    Color color;
    float size;

    Particle(float px, float py, float pvx, float pvy, Color c, float sz = 3.0f, float dur = 0.8f)
        : x(px), y(py), vx(pvx), vy(pvy), life(dur), maxLife(dur), color(c), size(sz) {}
};

struct AchievementNotification
{
    std::string name;
    std::string description;
    float life;
    float maxLife;

    AchievementNotification(const std::string& n, const std::string& d, float dur = 4.0f)
        : name(n), description(d), life(dur), maxLife(dur) {}
};

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
    SkillUpgrade,
    JobPerks,
    SavePrompt,
    LoadPrompt,
    Wiki,
    NPCDialogue,
    Achievements,
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
    KeyboardNav keyboardNav;
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
    AchievementSystem achievementSystem;
    JobQuestSystem jobQuestSystem;
    std::vector<std::shared_ptr<Item>> shopItems;

    // Combat log
    std::vector<std::string> combatLog;
    std::vector<FloatingText> floatingTexts;
    std::vector<Particle> particles;
    std::vector<AchievementNotification> achNotifications;
    double enemyActionTime;

    // Combat animation state
    float enemyFlashTimer;

    // State tracking for slide-in
    GameState previousState;

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

    // Skill overview pagination
    int skillOverviewPage;
    int skillLoadoutPage;

    // Job perks state
    int selectedJobIdx;

    // Inventory UI state
    int inventoryTab;

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
    void StateSkillUpgrade();
    void StateJobPerks();
    void StateAreaSelect();
    void StateQuestLog();
    void StateWiki();
    void StateAchievements();
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
    void DoPlayerGodAbility(int abilityIndex);
    void DoEnemyTurn();
    void AddCombatLog(const std::string& msg);
    void ProcessVictory(std::shared_ptr<Monster> enemy);
    void TrackEnemyDefeat(const std::string& enemyName);

    // NPC helpers
    void StartNPCDialogue(const NPC& npc);

    // Save / Load
    void SaveGamePrompt();
    void LoadGamePrompt();
    void SaveToSlot(int slot);
    void LoadFromSlot(int slot);

    // Drawing helpers
    void DrawTopBar();
    void DrawFloatingTexts();
    void AddFloatingText(const std::string& text, float x, float y, Color color, int fontSize = 18);
    void DrawParticles();
    void AddParticleBurst(float x, float y, Color color, int count = 12);
    void AddHealParticles(float x, float y);
    void AddCriticalParticles(float x, float y);
    void DrawAchievementNotifications();
    void CheckAchievementNotifications();
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
