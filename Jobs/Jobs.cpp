#include "Job.hpp"
#include <cstdlib>

Job::Job(JobType type) : type(type), level(1), experience(0), jobPoints(0)
{
    InitializePerks();
}

int Job::RequiredXP(int level)
{
    if (level <= 0) return 100;
    if (level < 10)  return 80 + level * 20;
    if (level < 25)  return 250 + (level - 10) * 40;
    if (level < 50)  return 850 + (level - 25) * 60;
    if (level < 75)  return 2350 + (level - 50) * 100;
    return 4850 + (level - 75) * 150;
}

void Job::GainXP(int xp)
{
    experience += xp;
    int required = RequiredXP(level);
    while (experience >= required && level < MAX_JOB_LEVEL)
    {
        experience -= required;
        level++;
        jobPoints++;
        required = RequiredXP(level);
    }
    if (level >= MAX_JOB_LEVEL)
    {
        experience = 0;
    }
}

std::shared_ptr<Resource> Job::CollectResource()
{
    std::shared_ptr<Resource> resource;

    // Check fatigue
    float fatigueMult = GetFatigueMultiplier();

    // Roll for random event
    JobEvent event = RollEvent();

    // Apply fatigue gain from event
    if (event.fatigueGain > 0)
        AddFatigue(event.fatigueGain);

    // Base fatigue gain per collection
    AddFatigue(2);

    switch (type)
    {
        case JobType::Mining:
            resource = ResourceFactory::CreateMiningResource(level);
            break;
        case JobType::Lumberjacking:
            resource = ResourceFactory::CreateLumberjackingResource(level);
            break;
        case JobType::Fishing:
            resource = ResourceFactory::CreateFishingResource(level);
            break;
        case JobType::Smithing:
            resource = ResourceFactory::CreateSmithingResource(level);
            break;
    }

    // Apply fatigue multiplier to resource value
    if (resource && fatigueMult < 1.0f)
    {
        resource->sellValue = static_cast<int>(resource->sellValue * fatigueMult);
    }

    // Apply specialization sell value bonus
    if (resource && specialization != SpecializationType::None)
    {
        int specSellBonus = GetSpecializationSellValueBonus();
        if (specSellBonus > 0)
            resource->sellValue = static_cast<int>(resource->sellValue * (100 + specSellBonus) / 100);
    }

    // Apply event bonuses
    int xpGain = 10 + level * 2;
    if (event.xpBonus > 0)
        xpGain += event.xpBonus;

    // Apply specialization XP bonus
    if (specialization != SpecializationType::None)
    {
        int specXPBonus = GetSpecializationXPBonus();
        if (specXPBonus > 0)
            xpGain = xpGain * (100 + specXPBonus) / 100;
    }

    // Apply fatigue to XP
    xpGain = static_cast<int>(xpGain * fatigueMult);

    GainXP(xpGain);

    // Apply resource bonus from event
    if (event.resourceBonus > 0 && resource)
    {
        resource->sellValue *= (1 + event.resourceBonus);
    }

    return resource;
}

std::string Job::GetJobName() const
{
    switch (type)
    {
        case JobType::Mining:
            return "Mining";
        case JobType::Lumberjacking:
            return "Lumberjacking";
        case JobType::Fishing:
            return "Fishing";
        case JobType::Smithing:
            return "Smithing";
        default:
            return "Unknown";
    }
}

void Job::InitializePerks()
{
    perks.clear();

    switch (type)
    {
        case JobType::Mining:
            // Branch 0: Ore Bonus
            perks.push_back(JobPerk("Efficient Mining", "+25% ore sell value", JobPerkType::SellValueBonus, 25, 2, 0, 1));
            perks.push_back(JobPerk("Rich Veins", "30% chance for double ore", JobPerkType::DoubleChance, 30, 5, 0, 2));
            perks.push_back(JobPerk("Mythril Touch", "15% chance for rare gem", JobPerkType::RareFindChance, 15, 8, 0, 3));
            perks.push_back(JobPerk("Ore Mastery", "+50% ore sell value", JobPerkType::SellValueBonus, 50, 15, 0, 4));
            perks.push_back(JobPerk("Deep Veins", "40% chance for double ore", JobPerkType::DoubleChance, 40, 25, 0, 5));
            perks.push_back(JobPerk("Gemfinder", "25% chance for rare gem", JobPerkType::RareFindChance, 25, 35, 0, 6));
            perks.push_back(JobPerk("Mythril Soul", "+75% ore sell value", JobPerkType::SellValueBonus, 75, 50, 0, 7));
            perks.push_back(JobPerk("Motherlode", "50% chance for double ore", JobPerkType::DoubleChance, 50, 65, 0, 8));
            perks.push_back(JobPerk("Legendary Veins", "35% chance for rare gem", JobPerkType::RareFindChance, 35, 80, 0, 9));
            perks.push_back(JobPerk("Earth's Bounty", "60% chance for double ore + 40% rare", JobPerkType::DoubleChance, 60, 95, 0, 10));
            // Branch 1: Combat
            perks.push_back(JobPerk("Stone Skin", "+5 DEF after mining", JobPerkType::CombatDefense, 5, 3, 1, 1));
            perks.push_back(JobPerk("Mining Strike", "+8% physical damage", JobPerkType::CombatDamage, 8, 6, 1, 2));
            perks.push_back(JobPerk("Mountain's Might", "+50 max HP (mastery)", JobPerkType::CombatHealth, 50, 10, 1, 3));
            perks.push_back(JobPerk("Iron Body", "+10 DEF", JobPerkType::CombatDefense, 10, 20, 1, 4));
            perks.push_back(JobPerk("Seismic Blow", "+15% physical damage", JobPerkType::CombatDamage, 15, 30, 1, 5));
            perks.push_back(JobPerk("Titan's Vitality", "+100 max HP", JobPerkType::CombatHealth, 100, 40, 1, 6));
            perks.push_back(JobPerk("Bedrock Defense", "+18 DEF", JobPerkType::CombatDefense, 18, 55, 1, 7));
            perks.push_back(JobPerk("Avalanche Force", "+22% physical damage", JobPerkType::CombatDamage, 22, 70, 1, 8));
            perks.push_back(JobPerk("Mountain Lord", "+200 max HP", JobPerkType::CombatHealth, 200, 85, 1, 9));
            perks.push_back(JobPerk("Earthen Godhood", "+30 DEF, +30% DMG, +300 HP", JobPerkType::CombatHealth, 300, 100, 1, 10));
            break;

        case JobType::Lumberjacking:
            // Branch 0: Wood Bonus
            perks.push_back(JobPerk("Keen Eye", "+25% wood sell value", JobPerkType::SellValueBonus, 25, 2, 0, 1));
            perks.push_back(JobPerk("Double Cut", "30% chance for double wood", JobPerkType::DoubleChance, 30, 5, 0, 2));
            perks.push_back(JobPerk("Ancient Harvest", "15% chance for rare wood", JobPerkType::RareFindChance, 15, 8, 0, 3));
            perks.push_back(JobPerk("Lumber Baron", "+50% wood sell value", JobPerkType::SellValueBonus, 50, 15, 0, 4));
            perks.push_back(JobPerk("Precision Cut", "40% chance for double wood", JobPerkType::DoubleChance, 40, 25, 0, 5));
            perks.push_back(JobPerk("Wild Gather", "25% chance for rare wood", JobPerkType::RareFindChance, 25, 35, 0, 6));
            perks.push_back(JobPerk("Ancient Woodcraft", "+75% wood sell value", JobPerkType::SellValueBonus, 75, 50, 0, 7));
            perks.push_back(JobPerk("Forest's Gift", "50% chance for double wood", JobPerkType::DoubleChance, 50, 65, 0, 8));
            perks.push_back(JobPerk("World Tree Seed", "35% chance for rare wood", JobPerkType::RareFindChance, 35, 80, 0, 9));
            perks.push_back(JobPerk("Verdant Mastery", "60% double + 40% rare wood", JobPerkType::DoubleChance, 60, 95, 0, 10));
            // Branch 1: Combat
            perks.push_back(JobPerk("Forest's Grace", "+5 DEX after lumberjacking", JobPerkType::CombatDefense, 5, 3, 1, 1));
            perks.push_back(JobPerk("Quick Hands", "+8% dodge chance", JobPerkType::CombatDamage, 8, 6, 1, 2));
            perks.push_back(JobPerk("Wild Instincts", "+50 max HP (mastery)", JobPerkType::CombatHealth, 50, 10, 1, 3));
            perks.push_back(JobPerk("Woodland Swiftness", "+10 DEF", JobPerkType::CombatDefense, 10, 20, 1, 4));
            perks.push_back(JobPerk("Timber Fury", "+15% physical damage", JobPerkType::CombatDamage, 15, 30, 1, 5));
            perks.push_back(JobPerk("Ent's Blessing", "+100 max HP", JobPerkType::CombatHealth, 100, 40, 1, 6));
            perks.push_back(JobPerk("Canopy Guard", "+18 DEF", JobPerkType::CombatDefense, 18, 55, 1, 7));
            perks.push_back(JobPerk("Raging Storm", "+22% physical damage", JobPerkType::CombatDamage, 22, 70, 1, 8));
            perks.push_back(JobPerk("Forest Guardian", "+200 max HP", JobPerkType::CombatHealth, 200, 85, 1, 9));
            perks.push_back(JobPerk("Nature's Wrath", "+30 DEF, +30% DMG, +300 HP", JobPerkType::CombatHealth, 300, 100, 1, 10));
            break;

        case JobType::Fishing:
            // Branch 0: Fish Bonus
            perks.push_back(JobPerk("Lucky Hook", "+25% fish sell value", JobPerkType::SellValueBonus, 25, 2, 0, 1));
            perks.push_back(JobPerk("Big Catch", "30% chance for double fish", JobPerkType::DoubleChance, 30, 5, 0, 2));
            perks.push_back(JobPerk("Deep Sea Bounty", "15% chance for rare fish", JobPerkType::RareFindChance, 15, 8, 0, 3));
            perks.push_back(JobPerk("Tidal Fortune", "+50% fish sell value", JobPerkType::SellValueBonus, 50, 15, 0, 4));
            perks.push_back(JobPerk("Angler's Net", "40% chance for double fish", JobPerkType::DoubleChance, 40, 25, 0, 5));
            perks.push_back(JobPerk("Siren's Call", "25% chance for rare fish", JobPerkType::RareFindChance, 25, 35, 0, 6));
            perks.push_back(JobPerk("Ocean's Treasury", "+75% fish sell value", JobPerkType::SellValueBonus, 75, 50, 0, 7));
            perks.push_back(JobPerk("Leviathan's Prize", "50% chance for double fish", JobPerkType::DoubleChance, 50, 65, 0, 8));
            perks.push_back(JobPerk("Abyssal Angler", "35% chance for rare fish", JobPerkType::RareFindChance, 35, 80, 0, 9));
            perks.push_back(JobPerk("Poseidon's Chosen", "60% double + 40% rare", JobPerkType::DoubleChance, 60, 95, 0, 10));
            // Branch 1: Combat
            perks.push_back(JobPerk("Sea Legs", "+5 VIT after fishing", JobPerkType::CombatDefense, 5, 3, 1, 1));
            perks.push_back(JobPerk("Refreshed", "+8% max HP", JobPerkType::CombatDamage, 8, 6, 1, 2));
            perks.push_back(JobPerk("Ocean's Blessing", "+50 max HP (mastery)", JobPerkType::CombatHealth, 50, 10, 1, 3));
            perks.push_back(JobPerk("Tidal Force", "+10 DEF", JobPerkType::CombatDefense, 10, 20, 1, 4));
            perks.push_back(JobPerk("Reef Breaker", "+15% physical damage", JobPerkType::CombatDamage, 15, 30, 1, 5));
            perks.push_back(JobPerk("Kraken's Gift", "+100 max HP", JobPerkType::CombatHealth, 100, 40, 1, 6));
            perks.push_back(JobPerk("Depth Pressure", "+18 DEF", JobPerkType::CombatDefense, 18, 55, 1, 7));
            perks.push_back(JobPerk("Maelstrom", "+22% physical damage", JobPerkType::CombatDamage, 22, 70, 1, 8));
            perks.push_back(JobPerk("Deep One", "+200 max HP", JobPerkType::CombatHealth, 200, 85, 1, 9));
            perks.push_back(JobPerk("Ocean Godhood", "+30 DEF, +30% DMG, +300 HP", JobPerkType::CombatHealth, 300, 100, 1, 10));
            break;

        case JobType::Smithing:
            // Branch 0: Ingot Bonus
            perks.push_back(JobPerk("Efficient Forging", "+25% ingot sell value", JobPerkType::SellValueBonus, 25, 2, 0, 1));
            perks.push_back(JobPerk("Double Strike", "30% chance for double ingots", JobPerkType::DoubleChance, 30, 5, 0, 2));
            perks.push_back(JobPerk("Master Smith", "15% chance for rare ingot", JobPerkType::RareFindChance, 15, 8, 0, 3));
            perks.push_back(JobPerk("Artisan's Pride", "+50% ingot sell value", JobPerkType::SellValueBonus, 50, 15, 0, 4));
            perks.push_back(JobPerk("Twin Hammer", "40% chance for double ingots", JobPerkType::DoubleChance, 40, 25, 0, 5));
            perks.push_back(JobPerk("Rare Alloy", "25% chance for rare ingot", JobPerkType::RareFindChance, 25, 35, 0, 6));
            perks.push_back(JobPerk("Grand Smithy", "+75% ingot sell value", JobPerkType::SellValueBonus, 75, 50, 0, 7));
            perks.push_back(JobPerk("Mythril Forge", "50% chance for double ingots", JobPerkType::DoubleChance, 50, 65, 0, 8));
            perks.push_back(JobPerk("Legendary Smith", "35% chance for rare ingot", JobPerkType::RareFindChance, 35, 80, 0, 9));
            perks.push_back(JobPerk("Celestial Anvil", "60% double + 40% rare", JobPerkType::DoubleChance, 60, 95, 0, 10));
            // Branch 1: Combat
            perks.push_back(JobPerk("Forge Master", "+5 STR after smithing", JobPerkType::CombatDefense, 5, 3, 1, 1));
            perks.push_back(JobPerk("Sharpened Edge", "+8% weapon damage", JobPerkType::CombatDamage, 8, 6, 1, 2));
            perks.push_back(JobPerk("Legendary Smith", "+50 max HP (mastery)", JobPerkType::CombatHealth, 50, 10, 1, 3));
            perks.push_back(JobPerk("Anvil's Strength", "+10 DEF", JobPerkType::CombatDefense, 10, 20, 1, 4));
            perks.push_back(JobPerk("Molten Edge", "+15% physical damage", JobPerkType::CombatDamage, 15, 30, 1, 5));
            perks.push_back(JobPerk("Living Steel", "+100 max HP", JobPerkType::CombatHealth, 100, 40, 1, 6));
            perks.push_back(JobPerk("Adamantine Skin", "+18 DEF", JobPerkType::CombatDefense, 18, 55, 1, 7));
            perks.push_back(JobPerk("Dragonforge", "+22% physical damage", JobPerkType::CombatDamage, 22, 70, 1, 8));
            perks.push_back(JobPerk("Divine Smith", "+200 max HP", JobPerkType::CombatHealth, 200, 85, 1, 9));
            perks.push_back(JobPerk("Forge Godhood", "+30 DEF, +30% DMG, +300 HP", JobPerkType::CombatHealth, 300, 100, 1, 10));
            break;
    }
}

bool Job::CanUnlockPerk(int index) const
{
    if (index < 0 || index >= (int)perks.size()) return false;
    if (perks[index].unlocked) return false;
    if (jobPoints <= 0) return false;
    if (level < perks[index].requiredLevel) return false;

    // Tier 2 requires tier 1 in same branch, tier 3 requires tier 2
    int branch = perks[index].branch;
    int tier = perks[index].tier;
    if (tier > 1)
    {
        for (const auto& p : perks)
        {
            if (p.branch == branch && p.tier == tier - 1 && p.unlocked)
                return true;
        }
        return false;
    }
    return true;
}

bool Job::UnlockPerk(int index)
{
    if (!CanUnlockPerk(index)) return false;
    perks[index].unlocked = true;
    jobPoints--;
    return true;
}

int Job::GetSellValueBonus() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::SellValueBonus)
            total += p.value;
    total += GetSpecializationSellValueBonus();
    return total;
}

int Job::GetDoubleChance() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::DoubleChance)
            total += p.value;
    total += GetSpecializationDoubleChanceBonus();
    return total;
}

int Job::GetRareFindChance() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::RareFindChance)
            total += p.value;
    total += GetSpecializationRareFindBonus();
    return total;
}

int Job::GetBonusResourceChance() const
{
    // Level-based scaling: higher levels give a chance for an extra resource per hour
    // Formula: level * 1%, capped at 50%
    // Level 10 = 10%, Level 25 = 25%, Level 50 = 50%, Level 75+ = 50%
    int chance = level;
    if (chance > 50) chance = 50;
    return chance;
}

int Job::GetCombatDefenseBonus() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::CombatDefense)
            total += p.value;
    return total;
}

int Job::GetCombatDamageBonus() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::CombatDamage)
            total += p.value;
    return total;
}

int Job::GetCombatHealthBonus() const
{
    int total = 0;
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::CombatHealth)
            total += p.value;
    return total;
}

bool Job::HasMasteryPerk() const
{
    for (const auto& p : perks)
        if (p.unlocked && p.type == JobPerkType::MasteryBonus)
            return true;
    return false;
}

int Job::GetMasteryRank() const
{
    if (level >= 100) return 5;
    if (level >= 75)  return 4;
    if (level >= 50)  return 3;
    if (level >= 25)  return 2;
    if (level >= 10)  return 1;
    return 0;
}

std::string Job::GetMasteryTitle() const
{
    switch (GetMasteryRank())
    {
        case 5: return "Grandmaster";
        case 4: return "Master";
        case 3: return "Expert";
        case 2: return "Journeyman";
        case 1: return "Apprentice";
        default: return "";
    }
}

void Job::AddFatigue(int amount)
{
    fatigue += amount;
    if (fatigue > MAX_FATIGUE) fatigue = MAX_FATIGUE;
}

void Job::ReduceFatigue(int amount)
{
    fatigue -= amount;
    if (fatigue < 0) fatigue = 0;
}

float Job::GetFatigueMultiplier() const
{
    if (fatigue < FATIGUE_THRESHOLD) return 1.0f;
    float over = static_cast<float>(fatigue - FATIGUE_THRESHOLD);
    float maxOver = static_cast<float>(MAX_FATIGUE - FATIGUE_THRESHOLD);
    return 1.0f - (over / maxOver) * 0.5f;
}

std::string Job::GetFatigueStatus() const
{
    if (fatigue < 20)  return "Fresh";
    if (fatigue < 40)  return "Rested";
    if (fatigue < 60)  return "Tired";
    if (fatigue < 80)  return "Weary";
    if (fatigue < 100) return "Exhausted";
    return "Burned Out";
}

JobEvent Job::CreateEvent(JobEventType type)
{
    JobEvent event;
    event.type = type;

    switch (type)
    {
        case JobEventType::LuckyFind:
            event.message = "You discovered a hidden cache!";
            event.resourceBonus = 2;
            event.xpBonus = 15;
            break;
        case JobEventType::ResourceSpill:
            event.message = "You dropped some resources...";
            event.resourceLoss = 1;
            break;
        case JobEventType::ToolWear:
            event.message = "Your tool took extra damage!";
            event.durabilityLoss = 10;
            break;
        case JobEventType::Discovery:
            event.message = "You found something special!";
            event.goldBonus = 25;
            event.xpBonus = 20;
            break;
        case JobEventType::Fatigued:
            event.message = "You're too tired to work efficiently.";
            event.fatigueGain = 10;
            break;
        case JobEventType::Renowned:
            event.message = "Word of your skill spreads! XP bonus!";
            event.xpBonus = 30;
            break;
        case JobEventType::Merchant:
            event.message = "A merchant arrives with a special offer!";
            event.goldBonus = 40;
            break;
        case JobEventType::None:
            break;
    }
    return event;
}

JobEvent Job::RollEvent() const
{
    int roll = rand() % 100;

    if (roll < 3)  return CreateEvent(JobEventType::LuckyFind);
    if (roll < 6)  return CreateEvent(JobEventType::ResourceSpill);
    if (roll < 9)  return CreateEvent(JobEventType::ToolWear);
    if (roll < 11) return CreateEvent(JobEventType::Discovery);
    if (roll < 13 && fatigue > 50) return CreateEvent(JobEventType::Fatigued);
    if (roll < 15) return CreateEvent(JobEventType::Renowned);
    if (roll < 17) return CreateEvent(JobEventType::Merchant);

    return CreateEvent(JobEventType::None);
}

bool Job::ChooseSpecialization(SpecializationType spec)
{
    if (!CanChooseSpecialization()) return false;

    // Verify the spec belongs to this job
    if (SpecializationManager::GetSpecA(type) != spec && SpecializationManager::GetSpecB(type) != spec)
        return false;

    specialization = spec;
    return true;
}

const JobSpecialization& Job::GetSpecializationData() const
{
    return SpecializationManager::GetSpecialization(specialization);
}

int Job::GetSpecializationRareFindBonus() const
{
    if (specialization == SpecializationType::None) return 0;
    return GetSpecializationData().rareFindBonus;
}

int Job::GetSpecializationSellValueBonus() const
{
    if (specialization == SpecializationType::None) return 0;
    return GetSpecializationData().sellValueBonus;
}

int Job::GetSpecializationDoubleChanceBonus() const
{
    if (specialization == SpecializationType::None) return 0;
    return GetSpecializationData().doubleChanceBonus;
}

int Job::GetSpecializationQualityBonus() const
{
    if (specialization == SpecializationType::None) return 0;
    return GetSpecializationData().qualityBonus;
}

int Job::GetSpecializationXPBonus() const
{
    if (specialization == SpecializationType::None) return 0;
    return GetSpecializationData().xpBonus;
}
