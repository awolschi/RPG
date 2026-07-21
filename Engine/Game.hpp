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
#include "../Factions/FactionReputation.hpp"
#include "../Factions/Pet.hpp"

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

struct QuestRewardNotification
{
    std::string questTitle;
    int xpEarned;
    int goldEarned;
    float life;
    float maxLife;

    QuestRewardNotification(const std::string& title, int xp, int gold, float dur = 3.0f)
        : questTitle(title), xpEarned(xp), goldEarned(gold), life(dur), maxLife(dur) {}
};

struct TutorialHint
{
    std::string title;
    std::string message;
    float life;
    float maxLife;

    TutorialHint(const std::string& t, const std::string& m, float dur = 6.0f)
        : title(t), message(m), life(dur), maxLife(dur) {}
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
    Reputation,
    Pets,
    JobSkillTree,
    JobUpgrades,
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
    ReputationSystem reputationSystem;
    PetManager petManager;
    JobQuestSystem jobQuestSystem;
    std::vector<std::shared_ptr<Item>> shopItems;

    // Combat log
    std::vector<std::string> combatLog;
    std::vector<FloatingText> floatingTexts;
    std::vector<Particle> particles;
    std::vector<AchievementNotification> achNotifications;
    std::vector<QuestRewardNotification> questRewardNotifications;
    std::vector<PetNotification> petNotifications;
    std::vector<TutorialHint> tutorialHints;
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
    int selectedSkillIdx = -1;

    // Skill loadout editing state (persist across frames)
    std::vector<int> loadoutEditCopy;
    int loadoutAttackSkillEdit = 0;
    bool loadoutEditActive = false;

    // Job perks state
    int selectedJobIdx;
    int jobPerkPage = 0;
    static constexpr int PERKS_PER_PAGE = 8;

    // Job skill tree state
    int skillTreeJobIdx = -1;
    int skillTreePage = 0;
    static constexpr int SKILL_TREE_PER_PAGE = 10;

    // Job upgrades state
    int upgradeJobIdx = -1;

    // Reputation UI state
    int selectedFactionIdx = -1;
    int repQuestTab = 0;  // 0=Overview, 1=Repeatable Quests
    int repQuestPage = 0;
    static constexpr int REPEATABLE_QUESTS_PER_PAGE = 5;

    // Inventory UI state
    int inventoryTab;
    int inventoryPage = 0;
    static constexpr int INVENTORY_ITEMS_PER_PAGE = 8;

    // Stats UI state (skill list pagination for high-level mages)
    int skillStatsPage = 0;
    static constexpr int SKILLS_PER_PAGE = 16;

    // Shop UI state (sell-side pagination)
    int shopSellPage = 0;
    static constexpr int SHOP_SELL_PER_PAGE = 18;

    // Achievements UI state
    int achievementsPage = 0;
    int selectedAchievementCategory = -1;
    static constexpr int ACHIEVEMENTS_PER_PAGE = 12;

    // Pet UI state
    int petListPage = 0;
    static constexpr int PETS_PER_PAGE = 9;

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
    void StateJobSkillTree();
    void StateJobUpgrades();
    void StateAreaSelect();
    void StateQuestLog();
    void StateWiki();
    void StateAchievements();
    void StateReputation();
    void StatePets();
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
    void ApplyPetPassivesToPlayer();
    int saveConfirmSlot = -1;

    // Drawing helpers
    void DrawTopBar();
    void DrawFloatingTexts();
    void AddFloatingText(const std::string& text, float x, float y, Color color, int fontSize = 18);
    void DrawParticles();
    void AddParticleBurst(float x, float y, Color color, int count = 12);
    void AddHealParticles(float x, float y);
    void AddCriticalParticles(float x, float y);
    void DrawAchievementNotifications();
    void DrawQuestRewardNotifications();
    void DrawReputationNotifications();
    void DrawPetNotifications();
    void DrawTutorialHints();
    void AddTutorialHint(const std::string& title, const std::string& message);
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

    // Death penalty tracking
    bool deathPenaltyApplied = false;
    int deathGoldLost = 0;
    int deathXpLost = 0;

    // Tutorial system (session-scoped)
    bool tutorialEnabled = true;
    bool tutorialExplored = false;
    bool tutorialCombatEntered = false;
    bool tutorialInventoryOpened = false;
    bool tutorialTraveled = false;
    bool tutorialQuestsChecked = false;
    bool tutorialCraftingOpened = false;
    bool tutorialSkillsOpened = false;
    bool tutorialRestedAtInn = false;
};

#endif
