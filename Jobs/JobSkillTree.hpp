#ifndef JOB_SKILL_TREE_HPP
#define JOB_SKILL_TREE_HPP

#include <string>
#include <vector>
#include "Job.hpp"

enum class SkillBranch
{
    Efficiency,
    Quality,
    Luck
};

struct JobSkill
{
    std::string name;
    std::string description;
    SkillBranch branch;
    int tier;           // 1-5 within branch
    int requiredLevel;  // Job level required to unlock
    bool unlocked = false;

    // Bonuses
    float speedBonus = 0.0f;        // % faster collection
    float fatigueReduction = 0.0f;   // % less fatigue
    float qualityBonus = 0.0f;       // % better quality chance
    float rareFindBonus = 0.0f;      // % more rare finds
    float doubleChanceBonus = 0.0f;  // % chance for double resources
    float xpBonus = 0.0f;           // % XP bonus
    float sellValueBonus = 0.0f;    // % sell value bonus
    bool autoPickup = false;         // No clicking needed (Efficiency branch)
    bool perfectQuality = false;     // Guaranteed quality (Quality branch)
    bool legendaryQuality = false;   // Unique effect (Quality branch)
    bool ultraRare = false;          // Ultra-rare items (Luck branch)
    bool mythicalItems = false;      // Mythical items (Luck branch)
};

class JobSkillTree
{
public:
    JobSkillTree();

    void InitializeSkills(JobType jobType);

    // Skill queries
    const JobSkill& GetSkill(JobType jobType, SkillBranch branch, int tier) const;
    bool CanUnlockSkill(JobType jobType, SkillBranch branch, int tier, int jobLevel, int skillPoints) const;
    bool UnlockSkill(JobType jobType, SkillBranch branch, int tier, int& skillPoints);

    // Bonus calculations
    float GetSpeedBonus(JobType jobType) const;
    float GetFatigueReduction(JobType jobType) const;
    float GetQualityBonus(JobType jobType) const;
    float GetRareFindBonus(JobType jobType) const;
    float GetDoubleChanceBonus(JobType jobType) const;
    float GetXPBonus(JobType jobType) const;
    float GetSellValueBonus(JobType jobType) const;
    bool HasAutoPickup(JobType jobType) const;
    bool HasPerfectQuality(JobType jobType) const;
    bool HasLegendaryQuality(JobType jobType) const;
    bool HasUltraRare(JobType jobType) const;
    bool HasMythicalItems(JobType jobType) const;

    // Get all skills for a job
    const std::vector<JobSkill>& GetSkills(JobType jobType) const;

    // Serialization
    std::string Serialize() const;
    void Deserialize(const std::string& data);

private:
    std::vector<JobSkill> miningSkills;
    std::vector<JobSkill> lumberjackingSkills;
    std::vector<JobSkill> fishingSkills;
    std::vector<JobSkill> smithingSkills;

    std::vector<JobSkill>& GetSkillsRef(JobType jobType);
    const std::vector<JobSkill>& GetSkillsRef(JobType jobType) const;

    void InitializeMiningSkills();
    void InitializeLumberjackingSkills();
    void InitializeFishingSkills();
    void InitializeSmithingSkills();
};

#endif
