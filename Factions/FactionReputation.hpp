#ifndef FACTION_REPUTATION_HPP
#define FACTION_REPUTATION_HPP

#include <string>
#include <vector>
#include <functional>

enum class FactionID
{
    GuardiansOfFelrona,   // Area 0 - Town guard, militia
    ShadowWardens,        // Area 1 - Rangers, herbalists of Dark Woods
    TideCallers,          // Area 2 - Coastal folk, pirate hunters
    DragonScholars,       // Area 3 - Scholars studying dragons
    HighlandClans,        // Area 4 - Shieldbearers, skywatchers
    VoidSeekers,          // Area 5 - Priests and slayers fighting the void
    CelestialOrder,       // Area 6 - Heavenly guardians
    VoidExarchs,          // Area 7 - Warriors against the primordial
    ArcaneConclave,       // Area 8 - Mage scholars
    ChronosWardens,       // Area 9 - Time keepers
    FactionCount
};

enum class RepRank
{
    Stranger,      // 0
    Acquaintance,  // 100
    Friend,        // 300
    Ally,          // 600
    Champion,      // 1000
    Legend,         // 1500
    RankCount
};

struct FactionReward
{
    RepRank requiredRank;
    std::string name;
    std::string description;
    int goldReward = 0;
    float shopDiscount = 0.0f;   // e.g. 0.10 = 10% off
    float xpBonus = 0.0f;        // e.g. 0.05 = 5% XP bonus
    float combatStatBonus = 0.0f; // e.g. 0.03 = 3% ATK/DEF
    bool unlocked = false;
};

struct RepeatableQuest
{
    std::string title;
    std::string description;
    FactionID faction;
    int requiredRank;        // Minimum rep rank to accept
    int targetCount;         // Kill count / gather count needed
    int currentCount = 0;
    int reputationReward;    // Rep points earned on completion
    int goldReward;
    int xpReward;
    bool active = false;
    bool completed = false;
    // Precise kill-matching targets (exact monster names as spawned by EnemyDatabase)
    std::string enemyName;        // Regular enemy this quest counts kills of (when !bossKillQuest)
    std::string bossName;         // Boss this quest counts kills of (when bossKillQuest)
    bool bossKillQuest = false;   // true: count kills of bossName; false: count kills of enemyName
};

struct FactionData
{
    FactionID id;
    std::string name;
    std::string description;
    std::string areaName;     // Associated area
    int areaIndex;
    int reputation = 0;
    RepRank rank = RepRank::Stranger;
    std::vector<FactionReward> rewards;
    std::vector<RepeatableQuest> repeatableQuests;
    int totalReputationEarned = 0;  // Lifetime tracking (never resets)
};

class ReputationSystem
{
public:
    ReputationSystem();

    void Initialize();

    // Reputation gains
    void AddReputation(FactionID faction, int amount, const std::string& source = "");
    void AddKillReputation(FactionID faction, bool isBoss = false);
    void AddDungeonReputation(FactionID faction, int dungeonDifficulty);

    // Quest integration
    void OnQuestCompleted(FactionID faction, int reputationReward);
    void RefreshRepeatableQuests(FactionID faction);

    // Queries
    int GetReputation(FactionID faction) const;
    RepRank GetRank(FactionID faction) const;
    float GetShopDiscount(FactionID faction) const;
    float GetXPBonus(FactionID faction) const;
    float GetCombatStatBonus(FactionID faction) const;
    std::string GetRankName(RepRank rank) const;
    std::string GetRankTitle(FactionID faction) const;
    int GetRepToNextRank(FactionID faction) const;
    int GetRankThreshold(RepRank rank) const;
    const FactionData& GetFactionData(FactionID faction) const;
    FactionData& GetFactionData(FactionID faction);
    int GetFactionCount() const { return static_cast<int>(FactionID::FactionCount); }
    FactionID GetFactionByArea(int areaIndex) const;

    // Repeatable quests
    const std::vector<RepeatableQuest>& GetRepeatableQuests(FactionID faction) const;
    bool AcceptRepeatableQuest(FactionID faction, int questIndex);
    void UpdateRepeatableQuestProgress(const std::string& enemyName);
    bool IsRepeatableQuestComplete(FactionID faction, int questIndex) const;
    void ClaimRepeatableQuestReward(FactionID faction, int questIndex);

    // Total tracking
    int GetTotalReputationEarned() const;

    // Serialization
    std::string Serialize() const;
    void Deserialize(const std::string& data);

    // Recent rep gains for notifications
    struct RepGainNotification
    {
        FactionID faction;
        int amount;
        std::string source;
        float life;
        float maxLife;
    };
    std::vector<RepGainNotification> notifications;
    void UpdateNotifications(float dt);
    void AddNotification(FactionID faction, int amount, const std::string& source);

private:
    std::vector<FactionData> factions;

    void InitializeFaction(FactionID id, const std::string& name, const std::string& desc,
                           const std::string& areaName, int areaIndex);
    void InitializeRewards(FactionID id);
    void InitializeRepeatableQuests(FactionID id);
    void UpdateRank(FactionID id);
    FactionData& GetFaction(FactionID id);
    const FactionData& GetFaction(FactionID id) const;
};

#endif
