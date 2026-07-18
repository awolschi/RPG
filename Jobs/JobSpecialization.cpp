#include "JobSpecialization.hpp"
#include "Job.hpp"
#include <algorithm>

std::vector<JobSpecialization> SpecializationManager::specs;
bool SpecializationManager::initialized = false;

JobSpecialization::JobSpecialization()
    : type(SpecializationType::None), parentJob(JobType::Mining),
      rareFindBonus(0), sellValueBonus(0), doubleChanceBonus(0),
      qualityBonus(0), xpBonus(0)
{
}

SpecializationManager::SpecializationManager()
{
    InitializeSpecs();
}

void SpecializationManager::InitializeSpecs()
{
    if (initialized) return;
    specs.clear();

    // Mining specializations
    {
        JobSpecialization s;
        s.type = SpecializationType::Gemcutter;
        s.name = "Gemcutter";
        s.description = "Master of precious stones. Finds rare gems and precious metals.";
        s.bonusDescription = "+15% rare finds, +10% quality chance, +5% XP";
        s.parentJob = JobType::Mining;
        s.rareFindBonus = 15;
        s.qualityBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Rare Gem";
        specs.push_back(s);
    }
    {
        JobSpecialization s;
        s.type = SpecializationType::Blacksmith;
        s.name = "Blacksmith";
        s.description = "Expert at smelting ore into ingots. Better metal yields.";
        s.bonusDescription = "+20% sell value, +10% double chance, +5% XP";
        s.parentJob = JobType::Mining;
        s.sellValueBonus = 20;
        s.doubleChanceBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Refined Ore";
        specs.push_back(s);
    }

    // Lumberjacking specializations
    {
        JobSpecialization s;
        s.type = SpecializationType::Carpenter;
        s.name = "Carpenter";
        s.description = "Skilled woodworker. Produces superior lumber.";
        s.bonusDescription = "+20% sell value, +10% double chance, +5% XP";
        s.parentJob = JobType::Lumberjacking;
        s.sellValueBonus = 20;
        s.doubleChanceBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Sturdy Plank";
        specs.push_back(s);
    }
    {
        JobSpecialization s;
        s.type = SpecializationType::Herbalist;
        s.name = "Herbalist";
        s.description = "Finds rare herbs and medicinal plants among the trees.";
        s.bonusDescription = "+15% rare finds, +10% quality chance, +5% XP";
        s.parentJob = JobType::Lumberjacking;
        s.rareFindBonus = 15;
        s.qualityBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Rare Herb";
        specs.push_back(s);
    }

    // Fishing specializations
    {
        JobSpecialization s;
        s.type = SpecializationType::Angler;
        s.name = "Angler";
        s.description = "Expert fisherman. Catches bigger and rarer fish.";
        s.bonusDescription = "+15% rare finds, +10% quality chance, +5% XP";
        s.parentJob = JobType::Fishing;
        s.rareFindBonus = 15;
        s.qualityBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Rare Fish";
        specs.push_back(s);
    }
    {
        JobSpecialization s;
        s.type = SpecializationType::Oceanographer;
        s.name = "Oceanographer";
        s.description = "Explores the deep. Finds sunken treasures and exotic fish.";
        s.bonusDescription = "+20% sell value, +10% double chance, +5% XP";
        s.parentJob = JobType::Fishing;
        s.sellValueBonus = 20;
        s.doubleChanceBonus = 10;
        s.xpBonus = 5;
        s.resourceBonusName = "Sunken Treasure";
        specs.push_back(s);
    }

    // Smithing specializations
    {
        JobSpecialization s;
        s.type = SpecializationType::Weaponsmith;
        s.name = "Weaponsmith";
        s.description = "Forges powerful weapons. Better combat bonuses from smithing.";
        s.bonusDescription = "+15% combat damage, +10% quality chance, +5% XP";
        s.parentJob = JobType::Smithing;
        s.qualityBonus = 10;
        s.xpBonus = 5;
        s.sellValueBonus = 15;
        s.resourceBonusName = "Masterwork Weapon";
        specs.push_back(s);
    }
    {
        JobSpecialization s;
        s.type = SpecializationType::Armorsmith;
        s.name = "Armorsmith";
        s.description = "Crafts sturdy armor. Better defensive bonuses from smithing.";
        s.bonusDescription = "+15% combat defense, +10% quality chance, +5% XP";
        s.parentJob = JobType::Smithing;
        s.qualityBonus = 10;
        s.xpBonus = 5;
        s.sellValueBonus = 15;
        s.resourceBonusName = "Masterwork Armor";
        specs.push_back(s);
    }

    initialized = true;
}

const JobSpecialization& SpecializationManager::GetSpecialization(SpecializationType type)
{
    InitializeSpecs();
    for (const auto& s : specs)
        if (s.type == type) return s;
    static JobSpecialization empty;
    return empty;
}

SpecializationType SpecializationManager::GetAlternative(SpecializationType type)
{
    switch (type)
    {
        case SpecializationType::Gemcutter: return SpecializationType::Blacksmith;
        case SpecializationType::Blacksmith: return SpecializationType::Gemcutter;
        case SpecializationType::Carpenter: return SpecializationType::Herbalist;
        case SpecializationType::Herbalist: return SpecializationType::Carpenter;
        case SpecializationType::Angler: return SpecializationType::Oceanographer;
        case SpecializationType::Oceanographer: return SpecializationType::Angler;
        case SpecializationType::Weaponsmith: return SpecializationType::Armorsmith;
        case SpecializationType::Armorsmith: return SpecializationType::Weaponsmith;
        default: return SpecializationType::None;
    }
}

std::string SpecializationManager::GetSpecializationName(SpecializationType type)
{
    return GetSpecialization(type).name;
}

std::string SpecializationManager::GetSpecializationDescription(SpecializationType type)
{
    return GetSpecialization(type).description;
}

SpecializationType SpecializationManager::GetSpecA(JobType job)
{
    switch (job)
    {
        case JobType::Mining: return SpecializationType::Gemcutter;
        case JobType::Lumberjacking: return SpecializationType::Carpenter;
        case JobType::Fishing: return SpecializationType::Angler;
        case JobType::Smithing: return SpecializationType::Weaponsmith;
        default: return SpecializationType::None;
    }
}

SpecializationType SpecializationManager::GetSpecB(JobType job)
{
    switch (job)
    {
        case JobType::Mining: return SpecializationType::Blacksmith;
        case JobType::Lumberjacking: return SpecializationType::Herbalist;
        case JobType::Fishing: return SpecializationType::Oceanographer;
        case JobType::Smithing: return SpecializationType::Armorsmith;
        default: return SpecializationType::None;
    }
}
