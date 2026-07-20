#ifndef JOBSYSTEM_HPP
#define JOBSYSTEM_HPP

#include <vector>
#include <memory>
#include <string>
#include "Job.hpp"
#include "JobSkillTree.hpp"
#include "JobEnvironment.hpp"
#include "LocationUpgrades.hpp"
#include "ResourceChain.hpp"

class Inventory;
class AchievementSystem;

struct JobSynergy
{
    std::string name;
    std::string description;
    JobType job1;
    JobType job2;
    int requiredLevel1;
    int requiredLevel2;
    float xpBonus1;      // XP bonus for job1 when using job2 resources
    float xpBonus2;      // XP bonus for job2 when using job1 resources
    float rareFindBonus; // Bonus to rare finds in both jobs
    float craftingSpeed; // Crafting speed bonus
    float qualityBonus;  // Quality chance bonus
};

class JobSystem
{
public:
    JobSystem();

    void AddJob(JobType type);
    Job& GetJob(JobType type);
    void DisplayAllJobs() const;
    std::string WorkJob(JobType type, int hours, Inventory& inventory, AchievementSystem* achSystem = nullptr);
    std::vector<Job>& GetJobs() { return jobs; }

    // Combat synergy - total bonuses from all jobs
    int GetTotalCombatDefense() const;
    int GetTotalCombatDamage() const;
    int GetTotalCombatHealth() const;
    std::string GetCombatSynergyDescription() const;

    // Resource chain system
    ResourceChainSystem& GetResourceChain() { return resourceChain; }
    const ResourceChainSystem& GetResourceChain() const { return resourceChain; }
    std::string CraftChain(const ChainRecipe& recipe, Inventory& inventory, JobType jobType);

    // Job synergies
    void InitializeSynergies();
    std::vector<JobSynergy> GetActiveSynergies() const;
    std::vector<JobSynergy> GetAllSynergies() const { return synergies; }
    float GetSynergyXPBonus(JobType jobType) const;
    float GetSynergyRareFindBonus(JobType jobType) const;
    float GetSynergyCraftingSpeed() const;
    float GetSynergyQualityBonus() const;
    std::string GetSynergyDescription() const;

    // Total hours tracking
    int GetTotalHoursWorked() const { return totalHoursWorked; }

    // Weather / Environment
    JobEnvironment& GetEnvironment() { return environment; }
    const JobEnvironment& GetEnvironment() const { return environment; }

    // Location upgrades
    LocationUpgrades& GetLocationUpgrades() { return locationUpgrades; }
    const LocationUpgrades& GetLocationUpgrades() const { return locationUpgrades; }

    // Save/load perk data
    std::string SerializePerks() const;
    void DeserializePerks(const std::string& data);

    // Skill tree
    JobSkillTree& GetSkillTree() { return skillTree; }
    const JobSkillTree& GetSkillTree() const { return skillTree; }
    std::string SerializeSkillTree() const;
    void DeserializeSkillTree(const std::string& data);

private:
    std::vector<Job> jobs;
    ResourceChainSystem resourceChain;
    JobSkillTree skillTree;
    JobEnvironment environment;
    LocationUpgrades locationUpgrades;
    std::vector<JobSynergy> synergies;
    int totalHoursWorked = 0;
    Job* FindJob(JobType type);
};

#endif
