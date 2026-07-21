#ifndef RELIGION_HPP
#define RELIGION_HPP

#include <string>
#include <memory>

class Character;

enum class GodType
{
    None,
    Shaim,
    Karosh,
    Amala,
    Tordo
};

struct GodQuest
{
    std::string description;
    std::string targetEnemy;
    int targetCount;
    int currentCount;
    int rewardDevotion;
    bool completed;
};

struct GodAbility
{
    std::string name;
    std::string description;
    int requiredDevotion;
};

struct GodData
{
    std::string name;
    std::string description;
    int devotionLevel;
    int totalDonated;
};

class ReligionSystem
{
public:
    ReligionSystem();

    void SetGod(GodType god);
    GodType GetGod() const { return currentGod; }
    int GetDevotionLevel() const { return devotionLevel; }
    int GetTotalDonated() const { return totalDonated; }

    bool Donate(int amount);
    std::string GetGodName() const;
    std::string GetGodDescription() const;
    int GetNextDevotionCost() const;
    std::string ApplyDevotionBonus(std::shared_ptr<Character> player) const;
    void RestoreState(GodType god, int devotion, int donated);

    // Prayer system
    bool CanPray() const;
    std::string Pray(std::shared_ptr<Character> player);
    int GetPrayerCooldown() const;
    void ResetPrayers();

    // God ability cooldown (once per combat)
    bool CanUseGodAbility1() const;
    bool CanUseGodAbility2() const;
    void UseGodAbility1();
    void UseGodAbility2();
    void ResetGodAbilityCooldowns();
    std::string ExecuteGodAbility1(std::shared_ptr<Character> player);
    std::string ExecuteGodAbility2(std::shared_ptr<Character> player);

    // God-specific quests
    const GodQuest& GetActiveQuest() const { return activeQuest; }
    void GenerateQuest();
    void EnsureQuest();
    void RestoreQuest(const std::string& desc, const std::string& target, int targetCount,
                      int currentCount, int rewardDevotion, bool completed);
    bool ProgressQuest(const std::string& enemyName);
    bool IsQuestComplete() const { return activeQuest.completed; }
    void CompleteQuest();

    // God-specific abilities
    GodAbility GetGodAbility1() const;
    GodAbility GetGodAbility2() const;
    bool HasAbility1() const;
    bool HasAbility2() const;

    // Serialization
    std::string Serialize() const;
    void Deserialize(const std::string& data);

private:
    GodType currentGod;
    int devotionLevel;
    int totalDonated;
    int prayersToday;
    int lastPrayerDay;
    GodQuest activeQuest;
    bool godAbility1Used;
    bool godAbility2Used;
};

#endif
