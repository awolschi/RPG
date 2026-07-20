#include "JobSystem.hpp"
#include "../Inventory/Inventory.hpp"
#include "../Achievements/Achievement.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

JobSystem::JobSystem()
{
    InitializeSynergies();
}

void JobSystem::AddJob(JobType type)
{
    if (!FindJob(type))
    {
        jobs.emplace_back(type);
    }
}

Job& JobSystem::GetJob(JobType type)
{
    Job* job = FindJob(type);
    if (job)
    {
        return *job;
    }
    throw std::runtime_error("Job not found");
}

void JobSystem::DisplayAllJobs() const
{
    std::cout << "\n=== YOUR JOBS ===" << std::endl;
    for (size_t i = 0; i < jobs.size(); ++i)
    {
        int required = Job::RequiredXP(jobs[i].level);
        std::cout << i + 1 << ". ";
        std::cout << jobs[i].GetJobName() << " - Level " << jobs[i].level
                  << ", XP: " << jobs[i].experience << "/" << required
                  << ", Points: " << jobs[i].jobPoints << "\n";
    }
}

std::string JobSystem::WorkJob(JobType type, int hours, Inventory& inventory, AchievementSystem* achSystem)
{
    Job* job = FindJob(type);
    if (!job)
        return "Job not found!";

    const WeatherEffect& weather = environment.GetEffect();
    LocationUpgrades& locUpgrades = locationUpgrades;

    float speedBonus = skillTree.GetSpeedBonus(type) + locUpgrades.GetTotalSpeedBonus();
    float fatigueReduction = skillTree.GetFatigueReduction(type) + locUpgrades.GetTotalFatigueReduction();
    float qualityBonus = skillTree.GetQualityBonus(type) + locUpgrades.GetTotalQualityBonus();
    float rareFindBonus = skillTree.GetRareFindBonus(type) + locUpgrades.GetTotalRareFindBonus();
    float doubleChanceBonus = skillTree.GetDoubleChanceBonus(type);
    float xpBonusMult = skillTree.GetXPBonus(type) + locUpgrades.GetTotalXPBonus();
    float sellValueBonus = skillTree.GetSellValueBonus(type);
    bool hasAutoPickup = skillTree.HasAutoPickup(type);
    bool hasPerfectQuality = skillTree.HasPerfectQuality(type);
    bool hasLegendaryQuality = skillTree.HasLegendaryQuality(type);
    bool hasUltraRare = skillTree.HasUltraRare(type);
    bool hasMythical = skillTree.HasMythicalItems(type);

    // Apply weather modifiers
    speedBonus += (weather.speedMod - 1.0f);
    fatigueReduction += (weather.fatigueMod - 1.0f);
    qualityBonus += (weather.qualityMod - 1.0f);
    rareFindBonus += (weather.rareFindMod - 1.0f);
    float weatherXpMult = weather.xpMod;

    // Speed bonus: reduce effective hours
    int effectiveHours = hours;
    if (speedBonus > 0.0f)
    {
        int saved = static_cast<int>(hours * speedBonus);
        if (saved > 0) effectiveHours = std::max(1, hours - saved);
    }

    std::string result = "You worked as a " + job->GetJobName() + " for " + std::to_string(hours) + " hours.\n";

    int collected = 0;
    int doubles = 0;
    int rares = 0;
    int startLevel = job->level;

    // Get achievement job XP bonus
    float achJobXPBonus = 0.0f;
    if (achSystem)
        achJobXPBonus = achSystem->GetJobXPBonusByTier();

    for (int i = 0; i < effectiveHours; ++i)
    {
        // Apply fatigue reduction
        if (fatigueReduction > 0.0f)
        {
            int reduced = static_cast<int>(2 * fatigueReduction);
            if (reduced > 0) job->ReduceFatigue(reduced);
        }

        auto resource = job->CollectResource();
        if (resource && inventory.AddItem(resource))
        {
            collected++;

            // Apply quality bonus from skill tree
            if (qualityBonus > 0.0f && (rand() % 100) < static_cast<int>(qualityBonus * 100.0f))
            {
                resource->quality = static_cast<ResourceQuality>(
                    std::min(static_cast<int>(resource->quality) + 1, static_cast<int>(ResourceQuality::Masterwork)));
            }

            // Apply sell value bonus from skill tree
            if (sellValueBonus > 0.0f)
            {
                resource->sellValue = static_cast<int>(resource->sellValue * (1.0f + sellValueBonus));
            }

            // Double chance roll (from perks + skill tree)
            int doubleChance = job->GetDoubleChance() + static_cast<int>(doubleChanceBonus * 100.0f);
            if (doubleChance > 0 && (rand() % 100) < doubleChance)
            {
                auto bonus = job->CollectResource();
                if (bonus && inventory.AddItem(bonus))
                {
                    doubles++;
                    collected++;
                }
            }

            // Level-based bonus resource roll
            int levelBonus = job->GetBonusResourceChance();
            if (levelBonus > 0 && (rand() % 100) < levelBonus)
            {
                auto bonus = job->CollectResource();
                if (bonus && inventory.AddItem(bonus))
                {
                    collected++;
                }
            }

            // Rare find roll (from perks + skill tree)
            int rareChance = job->GetRareFindChance() + static_cast<int>(rareFindBonus * 100.0f);
            if (rareChance > 0 && (rand() % 100) < rareChance)
            {
                auto rare = std::make_shared<Resource>(
                    "Rare " + resource->name, resource->tier + 1,
                    resource->sellValue * 3);
                if (inventory.AddItem(rare))
                {
                    rares++;
                }
            }

            // Legendary quality from skill tree
            if (hasLegendaryQuality && (rand() % 100) < 5)
            {
                auto legendary = std::make_shared<Resource>(
                    "Legendary " + resource->name, resource->tier + 2,
                    resource->sellValue * 5);
                if (inventory.AddItem(legendary))
                {
                    result += "Legendary " + resource->name + " discovered!\n";
                }
            }

            // Ultra-rare from skill tree
            if (hasUltraRare && (rand() % 100) < 3)
            {
                auto ultra = std::make_shared<Resource>(
                    "Ultra-Rare " + resource->name, resource->tier + 3,
                    resource->sellValue * 8);
                if (inventory.AddItem(ultra))
                {
                    result += "Ultra-Rare " + resource->name + " discovered!\n";
                }
            }

            // Mythical from skill tree
            if (hasMythical && (rand() % 100) < 1)
            {
                auto myth = std::make_shared<Resource>(
                    "Mythical " + resource->name, resource->tier + 4,
                    resource->sellValue * 15);
                if (inventory.AddItem(myth))
                {
                    result += "Mythical " + resource->name + " discovered!\n";
                }
            }
        }
    }

    result += "Resources collected: " + std::to_string(collected) + "\n";
    if (doubles > 0)
        result += "Double bonuses: " + std::to_string(doubles) + " extra!\n";
    if (rares > 0)
        result += "Rare finds: " + std::to_string(rares) + "!\n";

    // Apply XP bonus from skill tree + weather
    float totalXpMult = xpBonusMult + (weatherXpMult - 1.0f);
    if (totalXpMult > 0.0f && collected > 0)
    {
        int baseXPPerResource = 10 + job->level * 2;
        int totalBaseXP = baseXPPerResource * collected;
        int xpBonus = static_cast<int>(totalBaseXP * totalXpMult);
        if (xpBonus > 0)
        {
            job->GainXP(xpBonus);
            result += "[Bonus] XP Bonus: +" + std::to_string(xpBonus) + " XP!\n";
        }
    }

    // Apply achievement job XP bonus
    if (achJobXPBonus > 0.0f && collected > 0)
    {
        int baseXPPerResource = 10 + job->level * 2;
        int totalBaseXP = baseXPPerResource * collected;
        int achBonusXP = static_cast<int>(totalBaseXP * achJobXPBonus);
        if (achBonusXP > 0)
        {
            job->GainXP(achBonusXP);
            result += "[Achievement] Job XP Bonus: +" + std::to_string(achBonusXP) + " XP!\n";
        }
    }

    if (job->level > startLevel)
    {
        int levelsGained = job->level - startLevel;
        result += "Job leveled up " + std::to_string(levelsGained) + " time(s)! Now level " + std::to_string(job->level) + ".\n";
        result += "Gained " + std::to_string(levelsGained) + " job point(s)!\n";
    }

    totalHoursWorked += hours;

    return result;
}

int JobSystem::GetTotalCombatDefense() const
{
    int total = 0;
    for (const auto& job : jobs)
        total += job.GetCombatDefenseBonus();
    return total;
}

int JobSystem::GetTotalCombatDamage() const
{
    int total = 0;
    for (const auto& job : jobs)
        total += job.GetCombatDamageBonus();
    return total;
}

int JobSystem::GetTotalCombatHealth() const
{
    int total = 0;
    for (const auto& job : jobs)
        total += job.GetCombatHealthBonus();
    return total;
}

std::string JobSystem::GetCombatSynergyDescription() const
{
    std::string desc;
    int def = GetTotalCombatDefense();
    int dmg = GetTotalCombatDamage();
    int hp = GetTotalCombatHealth();

    if (def > 0) desc += "Job DEF: +" + std::to_string(def) + "  ";
    if (dmg > 0) desc += "Job DMG: +" + std::to_string(dmg) + "%  ";
    if (hp > 0) desc += "Job HP: +" + std::to_string(hp) + "  ";

    if (desc.empty()) desc = "No job combat bonuses active";
    return desc;
}

std::string JobSystem::SerializePerks() const
{
    std::ostringstream ss;
    for (const auto& job : jobs)
    {
        ss << static_cast<int>(job.type) << ":" << job.jobPoints << ":" << job.skillPoints << ":";
        for (size_t i = 0; i < job.perks.size(); ++i)
        {
            if (i > 0) ss << ",";
            ss << (job.perks[i].unlocked ? "1" : "0");
        }
        ss << ";";
    }
    return ss.str();
}

void JobSystem::DeserializePerks(const std::string& data)
{
    std::istringstream ss(data);
    std::string jobStr;
    while (std::getline(ss, jobStr, ';'))
    {
        if (jobStr.empty()) continue;

        size_t colon1 = jobStr.find(':');
        if (colon1 == std::string::npos) continue;
        int type = std::stoi(jobStr.substr(0, colon1));

        size_t colon2 = jobStr.find(':', colon1 + 1);
        if (colon2 == std::string::npos) continue;
        int points = std::stoi(jobStr.substr(colon1 + 1, colon2 - colon1 - 1));

        size_t colon3 = jobStr.find(':', colon2 + 1);
        int skillPts = 0;
        std::string perksStr;
        if (colon3 != std::string::npos)
        {
            skillPts = std::stoi(jobStr.substr(colon2 + 1, colon3 - colon2 - 1));
            perksStr = jobStr.substr(colon3 + 1);
        }
        else
        {
            perksStr = jobStr.substr(colon2 + 1);
        }

        Job* job = FindJob(static_cast<JobType>(type));
        if (job)
        {
            job->jobPoints = points;
            job->skillPoints = skillPts;
            std::istringstream ps(perksStr);
            std::string perkStr;
            int idx = 0;
            while (std::getline(ps, perkStr, ',') && idx < (int)job->perks.size())
            {
                job->perks[idx].unlocked = (perkStr == "1");
                idx++;
            }
        }
    }
}

std::string JobSystem::SerializeSkillTree() const
{
    return skillTree.Serialize();
}

void JobSystem::DeserializeSkillTree(const std::string& data)
{
    if (!data.empty())
        skillTree.Deserialize(data);
}

Job* JobSystem::FindJob(JobType type)
{
    for (auto& job : jobs)
    {
        if (job.type == type)
        {
            return &job;
        }
    }
    return nullptr;
}

std::string JobSystem::CraftChain(const ChainRecipe& recipe, Inventory& inventory, JobType jobType)
{
    Job* job = FindJob(jobType);
    if (!job)
        return "Job not found!";

    if (!resourceChain.CanCraft(recipe, inventory, job->level))
        return "Cannot craft this recipe!";

    auto result = resourceChain.Craft(recipe, inventory, job->level);
    if (!result)
        return "Crafting failed!";

    // Give bonus XP for chain crafting
    job->GainXP(recipe.bonusXP);

    return "Crafted " + recipe.name + "! (+" + std::to_string(recipe.bonusXP) + " " + job->GetJobName() + " XP)";
}

void JobSystem::InitializeSynergies()
{
    synergies.clear();

    // Mining + Smithing: "Ore to Ingot" bonus
    synergies.push_back({
        "Ore to Ingot",
        "+25% Smithing XP when using mined ores",
        JobType::Mining, JobType::Smithing,
        5, 5,    // Required levels
        0.0f,    // XP bonus for Mining
        0.25f,   // XP bonus for Smithing
        0.0f,    // Rare find bonus
        0.0f,    // Crafting speed
        0.05f    // Quality bonus
    });

    // Lumberjacking + Smithing: "Wood & Steel" bonus
    synergies.push_back({
        "Wood & Steel",
        "+20% crafting speed when using wood",
        JobType::Lumberjacking, JobType::Smithing,
        5, 5,
        0.0f,
        0.15f,
        0.0f,
        0.20f,
        0.0f
    });

    // Fishing + Cooking: "Fresh Catch" bonus
    synergies.push_back({
        "Fresh Catch",
        "Better fish quality from cooking",
        JobType::Fishing, JobType::Smithing,
        3, 3,
        0.10f,
        0.0f,
        0.10f,
        0.0f,
        0.15f
    });

    // Mining + Lumberjacking: "Explorer" bonus
    synergies.push_back({
        "Explorer",
        "+15% rare finds in both jobs",
        JobType::Mining, JobType::Lumberjacking,
        10, 10,
        0.0f,
        0.0f,
        0.15f,
        0.0f,
        0.0f
    });

    // Mining + Fishing: "Deep Prospector" bonus
    synergies.push_back({
        "Deep Prospector",
        "+10% XP in both jobs when working together",
        JobType::Mining, JobType::Fishing,
        15, 15,
        0.10f,
        0.10f,
        0.05f,
        0.0f,
        0.0f
    });

    // Lumberjacking + Fishing: "Nature's Bounty" bonus
    synergies.push_back({
        "Nature's Bounty",
        "+20% resource value in both jobs",
        JobType::Lumberjacking, JobType::Fishing,
        10, 10,
        0.05f,
        0.05f,
        0.05f,
        0.0f,
        0.0f
    });
}

std::vector<JobSynergy> JobSystem::GetActiveSynergies() const
{
    std::vector<JobSynergy> active;
    for (const auto& syn : synergies)
    {
        const Job* job1 = nullptr;
        const Job* job2 = nullptr;
        for (const auto& job : jobs)
        {
            if (job.type == syn.job1) job1 = &job;
            if (job.type == syn.job2) job2 = &job;
        }

        if (job1 && job2 &&
            job1->level >= syn.requiredLevel1 &&
            job2->level >= syn.requiredLevel2)
        {
            active.push_back(syn);
        }
    }
    return active;
}

float JobSystem::GetSynergyXPBonus(JobType jobType) const
{
    float totalBonus = 0.0f;
    for (const auto& syn : synergies)
    {
        const Job* job1 = nullptr;
        const Job* job2 = nullptr;
        for (const auto& job : jobs)
        {
            if (job.type == syn.job1) job1 = &job;
            if (job.type == syn.job2) job2 = &job;
        }

        if (job1 && job2 &&
            job1->level >= syn.requiredLevel1 &&
            job2->level >= syn.requiredLevel2)
        {
            if (jobType == syn.job1)
                totalBonus += syn.xpBonus1;
            else if (jobType == syn.job2)
                totalBonus += syn.xpBonus2;
        }
    }
    return totalBonus;
}

float JobSystem::GetSynergyRareFindBonus(JobType jobType) const
{
    float totalBonus = 0.0f;
    for (const auto& syn : synergies)
    {
        const Job* job1 = nullptr;
        const Job* job2 = nullptr;
        for (const auto& job : jobs)
        {
            if (job.type == syn.job1) job1 = &job;
            if (job.type == syn.job2) job2 = &job;
        }

        if (job1 && job2 &&
            job1->level >= syn.requiredLevel1 &&
            job2->level >= syn.requiredLevel2)
        {
            if (jobType == syn.job1 || jobType == syn.job2)
                totalBonus += syn.rareFindBonus;
        }
    }
    return totalBonus;
}

float JobSystem::GetSynergyCraftingSpeed() const
{
    float totalBonus = 0.0f;
    for (const auto& syn : synergies)
    {
        const Job* job1 = nullptr;
        const Job* job2 = nullptr;
        for (const auto& job : jobs)
        {
            if (job.type == syn.job1) job1 = &job;
            if (job.type == syn.job2) job2 = &job;
        }

        if (job1 && job2 &&
            job1->level >= syn.requiredLevel1 &&
            job2->level >= syn.requiredLevel2)
        {
            totalBonus += syn.craftingSpeed;
        }
    }
    return totalBonus;
}

float JobSystem::GetSynergyQualityBonus() const
{
    float totalBonus = 0.0f;
    for (const auto& syn : synergies)
    {
        const Job* job1 = nullptr;
        const Job* job2 = nullptr;
        for (const auto& job : jobs)
        {
            if (job.type == syn.job1) job1 = &job;
            if (job.type == syn.job2) job2 = &job;
        }

        if (job1 && job2 &&
            job1->level >= syn.requiredLevel1 &&
            job2->level >= syn.requiredLevel2)
        {
            totalBonus += syn.qualityBonus;
        }
    }
    return totalBonus;
}

std::string JobSystem::GetSynergyDescription() const
{
    std::vector<JobSynergy> active = GetActiveSynergies();
    if (active.empty())
        return "No active synergies";

    std::string desc;
    for (const auto& syn : active)
    {
        if (!desc.empty()) desc += "\n";
        desc += syn.name + ": " + syn.description;
    }
    return desc;
}
