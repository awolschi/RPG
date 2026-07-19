#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <memory>
#include <vector>
#include "../Items/Resources/Resources.hpp"
#include "JobPerk.hpp"
#include "JobSpecialization.hpp"

class Inventory;

enum class JobType
{
    Mining,
    Lumberjacking,
    Fishing,
    Smithing
};

enum class JobEventType
{
    LuckyFind,       // Bonus resources
    ResourceSpill,   // Lost some resources
    ToolWear,        // Extra tool durability loss
    Discovery,       // Found something special
    Fatigued,        // Reduced yield
    Renowned,        // XP bonus
    Merchant,        // Can buy/sell at bonus
    None
};

struct JobEvent
{
    JobEventType type = JobEventType::None;
    std::string message;
    int xpBonus = 0;
    int resourceBonus = 0;
    int resourceLoss = 0;
    int goldBonus = 0;
    int durabilityLoss = 0;
    int fatigueGain = 0;
};

class Job
{
public:
    static constexpr int MAX_JOB_LEVEL = 100;
    static constexpr int MAX_FATIGUE = 100;
    static constexpr int FATIGUE_THRESHOLD = 80;

    Job(JobType type);

    JobType type;
    int level;
    int experience;
    int jobPoints = 0;
    int skillPoints = 0;
    int fatigue = 0;
    std::vector<JobPerk> perks;
    SpecializationType specialization = SpecializationType::None;

    void GainXP(int xp);
    std::shared_ptr<Resource> CollectResource();
    static int RequiredXP(int level);
    std::string GetJobName() const;

    // Specialization
    bool HasSpecialization() const { return specialization != SpecializationType::None; }
    bool CanChooseSpecialization() const { return level >= 5 && specialization == SpecializationType::None; }
    bool ChooseSpecialization(SpecializationType spec);
    const JobSpecialization& GetSpecializationData() const;
    int GetSpecializationRareFindBonus() const;
    int GetSpecializationSellValueBonus() const;
    int GetSpecializationDoubleChanceBonus() const;
    int GetSpecializationQualityBonus() const;
    int GetSpecializationXPBonus() const;

    // Fatigue system
    void AddFatigue(int amount);
    void ReduceFatigue(int amount);
    bool IsFatigued() const { return fatigue >= FATIGUE_THRESHOLD; }
    float GetFatigueMultiplier() const;
    std::string GetFatigueStatus() const;

    // Random events
    JobEvent RollEvent() const;
    static JobEvent CreateEvent(JobEventType type);

    // Perk system
    void InitializePerks();
    bool CanUnlockPerk(int index) const;
    bool UnlockPerk(int index);

    // Perk effect queries
    int GetSellValueBonus() const;
    int GetDoubleChance() const;
    int GetRareFindChance() const;
    int GetBonusResourceChance() const; // Level-based extra resource/hour chance
    int GetCombatDefenseBonus() const;
    int GetCombatDamageBonus() const;
    int GetCombatHealthBonus() const;
    bool HasMasteryPerk() const;

    // Mastery system (post-level 50)
    bool IsMastered() const { return level >= 50; }
    bool IsGrandmaster() const { return level >= 100; }
    int GetMasteryRank() const; // 0=none, 1=Apprentice(10), 2=Journeyman(25), 3=Expert(50), 4=Master(75), 5=Grandmaster(100)
    std::string GetMasteryTitle() const;
};

#endif
