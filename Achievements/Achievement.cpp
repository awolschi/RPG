#include "Achievement.hpp"
#include <sstream>
#include <algorithm>

AchievementSystem::AchievementSystem()
{
    InitializeDefinitions();
}

void AchievementSystem::InitializeDefinitions()
{
    definitions.clear();

    // Helper to add achievements
    auto add = [&](const std::string& id, const std::string& name, const std::string& desc,
                   AchievementCategory cat, AchievementTier tier, int target,
                   int gold = 0, int hp = 0, int atk = 0, int defense = 0,
                   float jobXP = 0.0f, float questXP = 0.0f, const std::string& title = "",
                   const std::string& itemId = "")
    {
        AchievementDefinition def;
        def.id = id;
        def.name = name;
        def.description = desc;
        def.category = cat;
        def.tier = tier;
        def.targetValue = target;
        def.reward.goldBonus = gold;
        def.reward.statBonusHP = hp;
        def.reward.statBonusATK = atk;
        def.reward.statBonusDEF = defense;
        def.reward.jobXPBonus = jobXP;
        def.reward.questXPBonus = questXP;
        def.reward.title = title;
        def.reward.itemId = itemId;
        definitions.push_back(def);
    };

    // === JOB ACHIEVEMENTS ===
    add("job_ore_100", "Apprentice Miner", "Mine 100 ore", AchievementCategory::Job, AchievementTier::Bronze, 100, 100, 0, 1, 0, 0.02f);
    add("job_ore_500", "Journeyman Miner", "Mine 500 ore", AchievementCategory::Job, AchievementTier::Silver, 500, 500, 0, 3, 0, 0.05f);
    add("job_ore_1000", "Master Miner", "Mine 1000 ore", AchievementCategory::Job, AchievementTier::Gold, 1000, 2000, 0, 5, 0, 0.10f, 0.0f, "Master Miner", "ach_masters_pickaxe");

    add("job_wood_100", "Apprentice Lumberjack", "Chop 100 logs", AchievementCategory::Job, AchievementTier::Bronze, 100, 100, 0, 1, 0, 0.02f);
    add("job_wood_500", "Journeyman Lumberjack", "Chop 500 logs", AchievementCategory::Job, AchievementTier::Silver, 500, 500, 0, 3, 0, 0.05f);
    add("job_wood_1000", "Master Lumberjack", "Chop 1000 logs", AchievementCategory::Job, AchievementTier::Gold, 1000, 2000, 0, 5, 0, 0.10f, 0.0f, "Master Lumberjack", "ach_masters_axe");

    add("job_fish_100", "Apprentice Angler", "Catch 100 fish", AchievementCategory::Job, AchievementTier::Bronze, 100, 100, 0, 1, 0, 0.02f);
    add("job_fish_500", "Journeyman Angler", "Catch 500 fish", AchievementCategory::Job, AchievementTier::Silver, 500, 500, 0, 3, 0, 0.05f);
    add("job_fish_1000", "Master Angler", "Catch 1000 fish", AchievementCategory::Job, AchievementTier::Gold, 1000, 2000, 0, 5, 0, 0.10f, 0.0f, "Master Angler", "ach_masters_rod");

    add("job_craft_50", "Apprentice Artisan", "Craft 50 items", AchievementCategory::Job, AchievementTier::Bronze, 50, 100, 0, 1, 0, 0.02f);
    add("job_craft_200", "Journeyman Artisan", "Craft 200 items", AchievementCategory::Job, AchievementTier::Silver, 200, 500, 0, 3, 0, 0.05f);
    add("job_craft_500", "Master Artisan", "Craft 500 items", AchievementCategory::Job, AchievementTier::Gold, 500, 2000, 0, 5, 0, 0.10f, 0.0f, "Master Artisan", "ach_masters_hammer");

    add("job_masterwork_10", "Quality Seeker", "Find 10 Masterwork items", AchievementCategory::Job, AchievementTier::Bronze, 10, 150, 0, 2, 0, 0.03f);
    add("job_masterwork_50", "Quality Expert", "Find 50 Masterwork items", AchievementCategory::Job, AchievementTier::Silver, 50, 750, 0, 4, 0, 0.06f);
    add("job_masterwork_100", "Quality Master", "Find 100 Masterwork items", AchievementCategory::Job, AchievementTier::Gold, 100, 3000, 0, 6, 0, 0.12f, 0.0f, "Quality Master", "ach_masters_lens");

    add("job_hours_100", "Dedicated Worker", "Work 100 total hours", AchievementCategory::Job, AchievementTier::Bronze, 100, 200, 0, 2, 0, 0.03f);
    add("job_hours_500", "Tireless Worker", "Work 500 total hours", AchievementCategory::Job, AchievementTier::Silver, 500, 1000, 0, 4, 0, 0.06f);
    add("job_hours_1000", "Legendary Worker", "Work 1000 total hours", AchievementCategory::Job, AchievementTier::Gold, 1000, 5000, 0, 8, 0, 0.15f, 0.0f, "Legendary Worker", "ach_workers_medallion");

    add("job_rare_10", "Lucky Finder", "Get 10 rare finds", AchievementCategory::Job, AchievementTier::Bronze, 10, 150, 0, 2, 0, 0.03f);
    add("job_rare_50", "Lucky Expert", "Get 50 rare finds", AchievementCategory::Job, AchievementTier::Silver, 50, 750, 0, 4, 0, 0.06f);
    add("job_rare_100", "Lucky Master", "Get 100 rare finds", AchievementCategory::Job, AchievementTier::Gold, 100, 3000, 0, 6, 0, 0.12f, 0.0f, "Lucky Master", "ach_fortune_charm");

    // === COMBAT ACHIEVEMENTS ===
    add("combat_first_kill", "First Blood", "Defeat your first monster", AchievementCategory::Combat, AchievementTier::Bronze, 1, 50, 0, 1, 0, 0.01f);
    add("combat_kills_10", "Monster Slayer", "Defeat 10 monsters", AchievementCategory::Combat, AchievementTier::Bronze, 10, 100, 0, 2, 0, 0.02f);
    add("combat_kills_50", "Monster Hunter", "Defeat 50 monsters", AchievementCategory::Combat, AchievementTier::Silver, 50, 500, 0, 4, 0, 0.05f);
    add("combat_kills_100", "Monster Vanquisher", "Defeat 100 monsters", AchievementCategory::Combat, AchievementTier::Silver, 100, 1000, 0, 6, 0, 0.08f);
    add("combat_kills_500", "Monster Exterminator", "Defeat 500 monsters", AchievementCategory::Combat, AchievementTier::Gold, 500, 3000, 10, 10, 0, 0.12f, 0.0f, "Monster Exterminator", "ach_slayer_blade");
    add("combat_kills_1000", "Legendary Slayer", "Defeat 1000 monsters", AchievementCategory::Combat, AchievementTier::Gold, 1000, 10000, 20, 15, 0, 0.20f, 0.0f, "Legendary Slayer", "ach_legendary_blade");

    add("combat_boss_1", "Boss Hunter", "Defeat 1 boss", AchievementCategory::Combat, AchievementTier::Bronze, 1, 200, 0, 3, 0, 0.03f);
    add("combat_boss_5", "Boss Slayer", "Defeat 5 bosses", AchievementCategory::Combat, AchievementTier::Silver, 5, 1000, 5, 6, 0, 0.08f);
    add("combat_boss_10", "Boss Vanquisher", "Defeat 10 bosses", AchievementCategory::Combat, AchievementTier::Gold, 10, 5000, 10, 10, 0, 0.15f, 0.0f, "Boss Vanquisher", "ach_boss_amulet");

    add("combat_no_damage", "Untouchable", "Win a battle without taking damage", AchievementCategory::Combat, AchievementTier::Bronze, 1, 200, 0, 2, 0, 0.03f);
    add("combat_underdog", "Underdog", "Defeat an enemy 10+ levels above you", AchievementCategory::Combat, AchievementTier::Silver, 1, 500, 0, 4, 0, 0.06f);
    add("combat_speed_kill", "Speed Fighter", "Win a battle in 3 turns or less", AchievementCategory::Combat, AchievementTier::Bronze, 1, 150, 0, 2, 0, 0.02f);

    add("combat_critical_50", "Critical Beginner", "Land 50 critical hits", AchievementCategory::Combat, AchievementTier::Bronze, 50, 100, 0, 1, 0, 0.02f);
    add("combat_critical_200", "Critical Expert", "Land 200 critical hits", AchievementCategory::Combat, AchievementTier::Silver, 200, 500, 0, 3, 0, 0.05f);
    add("combat_critical_500", "Critical Champion", "Land 500 critical hits", AchievementCategory::Combat, AchievementTier::Gold, 500, 2000, 0, 6, 0, 0.10f, 0.0f, "Critical Champion", "ach_critical_ring");

    // === QUEST ACHIEVEMENTS ===
    add("quest_first", "Quest Starter", "Complete your first quest", AchievementCategory::Quest, AchievementTier::Bronze, 1, 50, 0, 0, 0, 0.00f, 0.05f);
    add("quest_5", "Adventurer", "Complete 5 quests", AchievementCategory::Quest, AchievementTier::Bronze, 5, 200, 0, 1, 0, 0.00f, 0.08f);
    add("quest_10", "Veteran Adventurer", "Complete 10 quests", AchievementCategory::Quest, AchievementTier::Silver, 10, 500, 0, 2, 0, 0.00f, 0.10f);
    add("quest_25", "Quest Explorer", "Complete 25 quests", AchievementCategory::Quest, AchievementTier::Silver, 25, 1000, 0, 3, 0, 0.00f, 0.12f);
    add("quest_50", "Quest Master", "Complete 50 quests", AchievementCategory::Quest, AchievementTier::Gold, 50, 3000, 5, 5, 0, 0.00f, 0.15f, "Quest Master", "ach_quest_talisman");
    add("quest_100", "Quest Legend", "Complete 100 quests", AchievementCategory::Quest, AchievementTier::Gold, 100, 10000, 10, 10, 0, 0.00f, 0.25f, "Quest Legend", "ach_quest_crown");

    add("quest_kill_10", "Monster Quester", "Complete 10 kill quests", AchievementCategory::Quest, AchievementTier::Bronze, 10, 300, 0, 2, 0, 0.00f, 0.08f);
    add("quest_kill_25", "Monster Slayer", "Complete 25 kill quests", AchievementCategory::Quest, AchievementTier::Silver, 25, 800, 0, 4, 0, 0.00f, 0.12f);
    add("quest_kill_50", "Monster Vanquisher", "Complete 50 kill quests", AchievementCategory::Quest, AchievementTier::Gold, 50, 2500, 5, 8, 0, 0.00f, 0.18f, "Monster Vanquisher", "ach_hunters_cloak");

    add("quest_gather_10", "Gatherer", "Complete 10 gather quests", AchievementCategory::Quest, AchievementTier::Bronze, 10, 300, 0, 2, 0, 0.00f, 0.08f);
    add("quest_gather_25", "Master Gatherer", "Complete 25 gather quests", AchievementCategory::Quest, AchievementTier::Silver, 25, 800, 0, 4, 0, 0.00f, 0.12f);
    add("quest_gather_50", "Gather Legend", "Complete 50 gather quests", AchievementCategory::Quest, AchievementTier::Gold, 50, 2500, 5, 8, 0, 0.00f, 0.18f, "Gather Legend", "ach_gatherers_pouch");

    add("quest_explore_5", "Pathfinder", "Complete 5 explore quests", AchievementCategory::Quest, AchievementTier::Bronze, 5, 200, 0, 1, 0, 0.00f, 0.06f);
    add("quest_explore_10", "Explorer", "Complete 10 explore quests", AchievementCategory::Quest, AchievementTier::Silver, 10, 600, 0, 3, 0, 0.00f, 0.10f);
    add("quest_explore_15", "World Walker", "Complete 15 explore quests", AchievementCategory::Quest, AchievementTier::Gold, 15, 2000, 5, 6, 0, 0.00f, 0.15f, "World Walker", "ach_explorers_compass");

    // === EXPLORATION ACHIEVEMENTS ===
    add("explore_3_areas", "Traveler", "Visit 3 areas", AchievementCategory::Exploration, AchievementTier::Bronze, 3, 150, 0, 1, 0);
    add("explore_6_areas", "Seasoned Traveler", "Visit 6 areas", AchievementCategory::Exploration, AchievementTier::Silver, 6, 600, 0, 3, 0);
    add("explore_all_areas", "World Explorer", "Visit all areas", AchievementCategory::Exploration, AchievementTier::Gold, 10, 3000, 10, 8, 0, 0.00f, 0.00f, "World Explorer", "ach_world_map");

    add("explore_secrets_3", "Secret Finder", "Discover 3 secret areas", AchievementCategory::Exploration, AchievementTier::Bronze, 3, 300, 0, 2, 0);
    add("explore_secrets_5", "Secret Hunter", "Discover 5 secret areas", AchievementCategory::Exploration, AchievementTier::Silver, 5, 1000, 0, 4, 0);
    add("explore_secrets_10", "Secret Master", "Discover 10 secret areas", AchievementCategory::Exploration, AchievementTier::Gold, 10, 5000, 5, 8, 0, 0.00f, 0.00f, "Secret Master", "ach_shadow_cloak");

    // === PROGRESSION ACHIEVEMENTS ===
    add("level_10", "Rising Star", "Reach character level 10", AchievementCategory::Progression, AchievementTier::Bronze, 10, 200, 5, 1, 1);
    add("level_25", "Seasoned Hero", "Reach character level 25", AchievementCategory::Progression, AchievementTier::Silver, 25, 1000, 10, 3, 3);
    add("level_50", "Legendary Hero", "Reach character level 50", AchievementCategory::Progression, AchievementTier::Gold, 50, 5000, 20, 8, 8, 0.00f, 0.00f, "Legendary Hero", "ach_heroes_crown");

    add("gold_1000", "Penny Pincher", "Accumulate 1000 gold", AchievementCategory::Progression, AchievementTier::Bronze, 1000, 200, 0, 0, 0);
    add("gold_10000", "Wealthy", "Accumulate 10000 gold", AchievementCategory::Progression, AchievementTier::Silver, 10000, 1000, 0, 2, 0);
    add("gold_100000", "Tycoon", "Accumulate 100000 gold", AchievementCategory::Progression, AchievementTier::Gold, 100000, 5000, 0, 5, 0, 0.00f, 0.00f, "Tycoon", "ach_tycoons_ring");

    add("items_25", "Collector", "Own 25 unique items", AchievementCategory::Progression, AchievementTier::Bronze, 25, 200, 0, 1, 0);
    add("items_50", "Hoarder", "Own 50 unique items", AchievementCategory::Progression, AchievementTier::Silver, 50, 800, 0, 3, 0);
    add("items_100", "Archivist", "Own 100 unique items", AchievementCategory::Progression, AchievementTier::Gold, 100, 3000, 5, 6, 0, 0.00f, 0.00f, "Archivist", "ach_archivists_satchel");

    // === SOCIAL ACHIEVEMENTS ===
    add("social_first_steps", "First Steps", "Complete the tutorial", AchievementCategory::Social, AchievementTier::Bronze, 1, 100, 0, 0, 0);
    add("social_shop_50", "Shopaholic", "Buy 50 items from shops", AchievementCategory::Social, AchievementTier::Bronze, 50, 150, 0, 1, 0);
    add("social_shop_200", "Regular Customer", "Buy 200 items from shops", AchievementCategory::Social, AchievementTier::Silver, 200, 600, 0, 3, 0);
    add("social_sell_100", "Merchant", "Sell 100 items", AchievementCategory::Social, AchievementTier::Bronze, 100, 200, 0, 1, 0);
    add("social_sell_500", "Trade Baron", "Sell 500 items", AchievementCategory::Social, AchievementTier::Silver, 500, 800, 0, 3, 0);
    add("social_sell_1000", "Trade Lord", "Sell 1000 items", AchievementCategory::Social, AchievementTier::Gold, 1000, 3000, 0, 6, 0, 0.00f, 0.00f, "Trade Lord", "ach_merchants_crown");
    add("social_inn_10", "Restful", "Rest at the Inn 10 times", AchievementCategory::Social, AchievementTier::Bronze, 10, 100, 0, 0, 0);
    add("social_inn_50", "Frequent Guest", "Rest at the Inn 50 times", AchievementCategory::Social, AchievementTier::Silver, 50, 500, 5, 0, 0);
    add("social_inn_100", "Inn Regular", "Rest at the Inn 100 times", AchievementCategory::Social, AchievementTier::Gold, 100, 2000, 10, 0, 0, 0.0f, 0.0f, "", "ach_innkeepers_key");
    add("social_survivor", "Survivor", "Survive a battle with 1 HP", AchievementCategory::Social, AchievementTier::Bronze, 1, 300, 0, 3, 0);
}

void AchievementSystem::UpdateProgress(const std::string& achievementId, int amount)
{
    AchievementProgress* p = FindProgress(achievementId);
    if (!p)
    {
        AchievementProgress newProgress;
        newProgress.achievementId = achievementId;
        newProgress.currentValue = 0;
        newProgress.unlocked = false;
        newProgress.notified = false;
        progress[achievementId] = newProgress;
        p = &progress[achievementId];
    }

    if (p->unlocked) return;

    p->currentValue += amount;

    const AchievementDefinition* def = GetDefinition(achievementId);
    if (def && p->currentValue >= def->targetValue)
    {
        p->currentValue = def->targetValue;
        if (!p->unlocked)
        {
            p->recentlyUnlocked = true;
            p->recentTimer = 5.0f;
        }
        p->unlocked = true;
        p->notified = false;
    }
}

void AchievementSystem::SetProgress(const std::string& achievementId, int value)
{
    AchievementProgress* p = FindProgress(achievementId);
    if (!p)
    {
        AchievementProgress newProgress;
        newProgress.achievementId = achievementId;
        newProgress.currentValue = value;
        newProgress.unlocked = false;
        newProgress.notified = false;
        progress[achievementId] = newProgress;
        p = &progress[achievementId];
    }

    if (p->unlocked) return;

    p->currentValue = value;

    const AchievementDefinition* def = GetDefinition(achievementId);
    if (def && p->currentValue >= def->targetValue)
    {
        p->currentValue = def->targetValue;
        if (!p->unlocked)
        {
            p->recentlyUnlocked = true;
            p->recentTimer = 5.0f;
        }
        p->unlocked = true;
        p->notified = false;
    }
}

bool AchievementSystem::IsUnlocked(const std::string& achievementId) const
{
    const AchievementProgress* p = FindProgress(achievementId);
    return p && p->unlocked;
}

int AchievementSystem::GetProgress(const std::string& achievementId) const
{
    const AchievementProgress* p = FindProgress(achievementId);
    return p ? p->currentValue : 0;
}

const AchievementDefinition* AchievementSystem::GetDefinition(const std::string& achievementId) const
{
    for (const auto& def : definitions)
    {
        if (def.id == achievementId)
            return &def;
    }
    return nullptr;
}

std::vector<AchievementDefinition> AchievementSystem::GetDefinitionsByCategory(AchievementCategory category) const
{
    std::vector<AchievementDefinition> result;
    for (const auto& def : definitions)
    {
        if (def.category == category)
            result.push_back(def);
    }
    return result;
}

std::vector<AchievementProgress> AchievementSystem::GetAllProgress() const
{
    std::vector<AchievementProgress> result;
    for (const auto& def : definitions)
    {
        auto it = progress.find(def.id);
        if (it != progress.end())
            result.push_back(it->second);
        else
        {
            AchievementProgress p;
            p.achievementId = def.id;
            p.currentValue = 0;
            p.unlocked = false;
            p.notified = false;
            result.push_back(p);
        }
    }
    return result;
}

std::vector<AchievementProgress> AchievementSystem::GetUnlockedAchievements() const
{
    std::vector<AchievementProgress> result;
    for (const auto& [id, p] : progress)
    {
        if (p.unlocked)
            result.push_back(p);
    }
    return result;
}

std::vector<AchievementProgress> AchievementSystem::GetLockedAchievements() const
{
    std::vector<AchievementProgress> result;
    for (const auto& def : definitions)
    {
        auto it = progress.find(def.id);
        if (it == progress.end() || !it->second.unlocked)
        {
            AchievementProgress p;
            p.achievementId = def.id;
            p.currentValue = (it != progress.end()) ? it->second.currentValue : 0;
            p.unlocked = false;
            p.notified = false;
            result.push_back(p);
        }
    }
    return result;
}

int AchievementSystem::GetTotalUnlocked() const
{
    int count = 0;
    for (const auto& [id, p] : progress)
    {
        if (p.unlocked) count++;
    }
    return count;
}

int AchievementSystem::GetTotalAchievements() const
{
    return static_cast<int>(definitions.size());
}

float AchievementSystem::GetCompletionPercentage() const
{
    int total = GetTotalAchievements();
    if (total == 0) return 0.0f;
    return static_cast<float>(GetTotalUnlocked()) / static_cast<float>(total) * 100.0f;
}

AchievementReward AchievementSystem::GetTotalRewards() const
{
    AchievementReward total;
    for (const auto& [id, p] : progress)
    {
        if (p.unlocked)
        {
            const AchievementDefinition* def = GetDefinition(id);
            if (def)
            {
                total.goldBonus += def->reward.goldBonus;
                total.statBonusHP += def->reward.statBonusHP;
                total.statBonusATK += def->reward.statBonusATK;
                total.statBonusDEF += def->reward.statBonusDEF;
                total.jobXPBonus += def->reward.jobXPBonus;
                total.questXPBonus += def->reward.questXPBonus;
            }
        }
    }
    return total;
}

AchievementReward AchievementSystem::GetCategoryRewards(AchievementCategory category) const
{
    AchievementReward total;
    for (const auto& [id, p] : progress)
    {
        if (p.unlocked)
        {
            const AchievementDefinition* def = GetDefinition(id);
            if (def && def->category == category)
            {
                total.goldBonus += def->reward.goldBonus;
                total.statBonusHP += def->reward.statBonusHP;
                total.statBonusATK += def->reward.statBonusATK;
                total.statBonusDEF += def->reward.statBonusDEF;
                total.jobXPBonus += def->reward.jobXPBonus;
                total.questXPBonus += def->reward.questXPBonus;
            }
        }
    }
    return total;
}

bool AchievementSystem::HasUnnotifiedAchievement() const
{
    for (const auto& [id, p] : progress)
    {
        if (p.unlocked && !p.notified)
            return true;
    }
    return false;
}

AchievementProgress AchievementSystem::GetNextUnnotified()
{
    for (auto& [id, p] : progress)
    {
        if (p.unlocked && !p.notified)
            return p;
    }
    return AchievementProgress();
}

void AchievementSystem::MarkNotified(const std::string& achievementId)
{
    AchievementProgress* p = FindProgress(achievementId);
    if (p)
        p->notified = true;
}

void AchievementSystem::Tick(float dt)
{
    for (auto& [id, p] : progress)
    {
        if (p.recentlyUnlocked && p.recentTimer > 0.0f)
        {
            p.recentTimer -= dt;
            if (p.recentTimer <= 0.0f)
            {
                p.recentlyUnlocked = false;
                p.recentTimer = 0.0f;
            }
        }
    }
}

bool AchievementSystem::HasRecentlyUnlocked() const
{
    for (const auto& [id, p] : progress)
    {
        if (p.recentlyUnlocked) return true;
    }
    return false;
}

std::string AchievementSystem::GetBestTitle() const
{
    // Priority order: Gold tier titles first, then Silver, then Bronze
    // Check all unlocked achievements for titles, return the most prestigious
    std::string bestTitle;
    AchievementTier bestTier = AchievementTier::Bronze;

    for (const auto& [id, p] : progress)
    {
        if (!p.unlocked) continue;
        const AchievementDefinition* def = GetDefinition(id);
        if (!def || def->reward.title.empty()) continue;

        if (def->tier > bestTier || (def->tier == bestTier && !bestTitle.empty()))
        {
            bestTitle = def->reward.title;
            bestTier = def->tier;
        }
    }
    return bestTitle;
}

float AchievementSystem::GetJobXPBonusByTier() const
{
    // Bronze Job: +2% job XP per achievement
    // Silver Job: +5% job XP per achievement
    // Gold Job: +10% job XP per achievement
    float total = 0.0f;
    for (const auto& [id, p] : progress)
    {
        if (!p.unlocked) continue;
        const AchievementDefinition* def = GetDefinition(id);
        if (!def || def->category != AchievementCategory::Job) continue;

        switch (def->tier)
        {
            case AchievementTier::Bronze: total += 0.02f; break;
            case AchievementTier::Silver: total += 0.05f; break;
            case AchievementTier::Gold:   total += 0.10f; break;
        }
    }
    return total;
}

float AchievementSystem::GetQuestXPBonusByTier() const
{
    // Bronze Quest: +5% quest XP per achievement
    // Silver Quest: +10% quest XP per achievement
    // Gold Quest: +15% quest XP per achievement
    float total = 0.0f;
    for (const auto& [id, p] : progress)
    {
        if (!p.unlocked) continue;
        const AchievementDefinition* def = GetDefinition(id);
        if (!def || def->category != AchievementCategory::Quest) continue;

        switch (def->tier)
        {
            case AchievementTier::Bronze: total += 0.05f; break;
            case AchievementTier::Silver: total += 0.10f; break;
            case AchievementTier::Gold:   total += 0.15f; break;
        }
    }
    return total;
}

int AchievementSystem::GetCombatATKBonusByTier() const
{
    // Bronze Combat: +1 ATK per achievement
    // Silver Combat: +3 ATK per achievement
    // Gold Combat: +5 ATK per achievement
    int total = 0;
    for (const auto& [id, p] : progress)
    {
        if (!p.unlocked) continue;
        const AchievementDefinition* def = GetDefinition(id);
        if (!def || def->category != AchievementCategory::Combat) continue;

        switch (def->tier)
        {
            case AchievementTier::Bronze: total += 1; break;
            case AchievementTier::Silver: total += 3; break;
            case AchievementTier::Gold:   total += 5; break;
        }
    }
    return total;
}

int AchievementSystem::GetCombatDEFBonusByTier() const
{
    // Same as ATK for symmetry
    return GetCombatATKBonusByTier();
}

bool AchievementSystem::IsAreaSecretUnlocked(int areaIndex) const
{
    // Unlock secret area at 100% area completion
    // This is checked externally by the game
    return false; // Placeholder - game checks completion externally
}

bool AchievementSystem::IsNewGamePlusUnlocked() const
{
    return newGamePlus;
}

bool AchievementSystem::IsBossRushUnlocked() const
{
    return bossRush;
}

bool AchievementSystem::IsMasterClassUnlocked() const
{
    return masterClass;
}

bool AchievementSystem::IsDevCommentaryUnlocked() const
{
    return devCommentary;
}

void AchievementSystem::CheckUnlockables()
{
    // All Quests Complete → New Game+
    int questCount = 0;
    int questUnlocked = 0;
    for (const auto& [id, p] : progress)
    {
        const AchievementDefinition* def = GetDefinition(id);
        if (def && def->category == AchievementCategory::Quest)
        {
            questCount++;
            if (p.unlocked) questUnlocked++;
        }
    }
    if (questCount > 0 && questUnlocked >= questCount)
        newGamePlus = true;

    // All Combat Complete → Boss Rush
    int combatCount = 0;
    int combatUnlocked = 0;
    for (const auto& [id, p] : progress)
    {
        const AchievementDefinition* def = GetDefinition(id);
        if (def && def->category == AchievementCategory::Combat)
        {
            combatCount++;
            if (p.unlocked) combatUnlocked++;
        }
    }
    if (combatCount > 0 && combatUnlocked >= combatCount)
        bossRush = true;

    // All Jobs Complete → Master class
    int jobCount = 0;
    int jobUnlocked = 0;
    for (const auto& [id, p] : progress)
    {
        const AchievementDefinition* def = GetDefinition(id);
        if (def && def->category == AchievementCategory::Job)
        {
            jobCount++;
            if (p.unlocked) jobUnlocked++;
        }
    }
    if (jobCount > 0 && jobUnlocked >= jobCount)
        masterClass = true;

    // All Achievements → Dev commentary
    if (GetTotalUnlocked() >= GetTotalAchievements())
        devCommentary = true;
}

std::vector<std::string> AchievementSystem::GetUnlockedItemIds() const
{
    std::vector<std::string> items;
    for (const auto& [id, p] : progress)
    {
        if (!p.unlocked) continue;
        const AchievementDefinition* def = GetDefinition(id);
        if (def && !def->reward.itemId.empty())
            items.push_back(def->reward.itemId);
    }
    return items;
}

std::string AchievementSystem::Serialize() const
{
    std::ostringstream ss;
    ss << progress.size();
    for (const auto& [id, p] : progress)
    {
        ss << "|" << id << "," << p.currentValue << "," << (p.unlocked ? "1" : "0") << "," << (p.notified ? "1" : "0");
    }
    ss << " " << (newGamePlus ? "1" : "0") << (bossRush ? "1" : "0") << (masterClass ? "1" : "0") << (devCommentary ? "1" : "0");
    return ss.str();
}

void AchievementSystem::Deserialize(const std::string& data)
{
    progress.clear();
    if (data.empty()) return;

    // Format: "N|id1,v1,u1,n1|id2,v2,u2,n2|... <flags>"
    // The entries are NOT whitespace-separated, so using operator>> would
    // swallow every entry as a single token. We split on '|'
    // explicitly: the first segment is N (count), subsequent segments are
    // id,v,u,n tuples, and the final segment after a space is the flags.

    size_t barPos = data.find('|');
    if (barPos == std::string::npos)
    {
        // Only a count (or empty) — nothing to restore
        return;
    }
    size_t count = 0;
    try { count = static_cast<size_t>(std::stoi(data.substr(0, barPos))); }
    catch (...) { return; }

    size_t pos = barPos;
    for (size_t i = 0; i < count && pos != std::string::npos; ++i)
    {
        size_t next = data.find('|', pos + 1);
        std::string entry = (next == std::string::npos)
            ? data.substr(pos + 1)
            : data.substr(pos + 1, next - pos - 1);

        // The final segment (after the space before flags) is "0000" —
        // detect via the leading space and bail.
        size_t comma1 = entry.find(',');
        if (comma1 == std::string::npos)
        {
            // This is the flags segment. Parse it.
            if (entry.size() >= 4)
            {
                newGamePlus = (entry[0] == '1');
                bossRush    = (entry[1] == '1');
                masterClass = (entry[2] == '1');
                devCommentary = (entry[3] == '1');
            }
            break;
        }

        std::string id = entry.substr(0, comma1);
        size_t comma2 = entry.find(',', comma1 + 1);
        size_t comma3 = entry.find(',', comma2 + 1);
        if (comma2 == std::string::npos) { pos = next; continue; }

        int value = 0;
        try { value = std::stoi(entry.substr(comma1 + 1, comma2 - comma1 - 1)); }
        catch (...) {}

        bool unlocked = false;
        if (comma3 != std::string::npos)
            unlocked = (entry.substr(comma2 + 1, comma3 - comma2 - 1) == "1");

        bool notified = false;
        if (comma3 != std::string::npos && comma3 + 1 < entry.size())
            notified = (entry.substr(comma3 + 1) == "1");
        else if (comma3 == std::string::npos)
            unlocked = (entry.substr(comma2 + 1) == "1");

        AchievementProgress p;
        p.achievementId = id;
        p.currentValue = value;
        p.unlocked = unlocked;
        p.notified = notified;
        progress[id] = p;

        pos = next;
    }

    // Flags may also appear after a space in the data (older save format had
    // flags trailing without their own '|'). Look for the trailing 4-char flag
    // pattern after the first space following the last parsed bar.
    size_t spacePos = data.find(' ', barPos);
    if (spacePos != std::string::npos && spacePos + 1 < data.size())
    {
        std::string flags = data.substr(spacePos + 1);
        if (flags.size() >= 4)
        {
            newGamePlus = (flags[0] == '1');
            bossRush    = (flags[1] == '1');
            masterClass = (flags[2] == '1');
            devCommentary = (flags[3] == '1');
        }
    }
}

AchievementProgress* AchievementSystem::FindProgress(const std::string& id)
{
    auto it = progress.find(id);
    return (it != progress.end()) ? &it->second : nullptr;
}

const AchievementProgress* AchievementSystem::FindProgress(const std::string& id) const
{
    auto it = progress.find(id);
    return (it != progress.end()) ? &it->second : nullptr;
}
