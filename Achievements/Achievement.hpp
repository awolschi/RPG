#ifndef ACHIEVEMENT_HPP
#define ACHIEVEMENT_HPP

#include <string>
#include <vector>
#include <map>

enum class AchievementCategory
{
    Job,
    Combat,
    Quest,
    Exploration,
    Progression,
    Social
};

enum class AchievementTier
{
    Bronze = 0,
    Silver = 1,
    Gold = 2
};

struct AchievementReward
{
    int goldBonus = 0;
    int statBonusHP = 0;
    int statBonusATK = 0;
    int statBonusDEF = 0;
    float jobXPBonus = 0.0f;
    float questXPBonus = 0.0f;
    std::string title;
    std::string itemId; // Unique item reward ID
};

struct AchievementDefinition
{
    std::string id;
    std::string name;
    std::string description;
    AchievementCategory category;
    AchievementTier tier;
    int targetValue;
    AchievementReward reward;
};

struct AchievementProgress
{
    std::string achievementId;
    int currentValue = 0;
    bool unlocked = false;
    bool notified = false;
    bool recentlyUnlocked = false;
    float recentTimer = 0.0f;
};

class AchievementSystem
{
public:
    AchievementSystem();

    void InitializeDefinitions();

    // Progress tracking
    void UpdateProgress(const std::string& achievementId, int amount = 1);
    void SetProgress(const std::string& achievementId, int value);
    bool IsUnlocked(const std::string& achievementId) const;
    int GetProgress(const std::string& achievementId) const;

    // Query methods
    const AchievementDefinition* GetDefinition(const std::string& achievementId) const;
    std::vector<AchievementDefinition> GetDefinitionsByCategory(AchievementCategory category) const;
    std::vector<AchievementProgress> GetAllProgress() const;
    std::vector<AchievementProgress> GetUnlockedAchievements() const;
    std::vector<AchievementProgress> GetLockedAchievements() const;
    int GetTotalUnlocked() const;
    int GetTotalAchievements() const;
    float GetCompletionPercentage() const;

    // Reward calculation
    AchievementReward GetTotalRewards() const;
    AchievementReward GetCategoryRewards(AchievementCategory category) const;

    // Notification system
    bool HasUnnotifiedAchievement() const;
    AchievementProgress GetNextUnnotified();
    void MarkNotified(const std::string& achievementId);

    // Recently unlocked tracking
    void Tick(float dt);
    bool HasRecentlyUnlocked() const;

    // Title system
    std::string GetBestTitle() const;

    // Tier-based bonus system (Section 15.4)
    float GetJobXPBonusByTier() const;       // % bonus from job achievements by tier
    float GetQuestXPBonusByTier() const;     // % bonus from quest achievements by tier
    int GetCombatATKBonusByTier() const;     // flat ATK from combat achievements by tier
    int GetCombatDEFBonusByTier() const;     // flat DEF from combat achievements by tier

    // Unlockable system
    bool IsAreaSecretUnlocked(int areaIndex) const;
    bool IsNewGamePlusUnlocked() const;
    bool IsBossRushUnlocked() const;
    bool IsMasterClassUnlocked() const;
    bool IsDevCommentaryUnlocked() const;
    void CheckUnlockables();

    // Item rewards
    std::vector<std::string> GetUnlockedItemIds() const;

    // Serialization
    std::string Serialize() const;
    void Deserialize(const std::string& data);

private:
    std::vector<AchievementDefinition> definitions;
    std::map<std::string, AchievementProgress> progress;
    std::vector<std::string> unlockedItems;
    bool newGamePlus = false;
    bool bossRush = false;
    bool masterClass = false;
    bool devCommentary = false;

    AchievementProgress* FindProgress(const std::string& id);
    const AchievementProgress* FindProgress(const std::string& id) const;
};

#endif
