#include "FactionReputation.hpp"
#include "../Items/Loot.hpp"
#include "../Items/Consumable.hpp"
#include "../Items/SummoningItem.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

// Rank thresholds: Stranger=0, Acquaintance=100, Friend=300, Ally=600, Champion=1000, Legend=1500
static const int RANK_THRESHOLDS[] = { 0, 100, 300, 600, 1000, 1500 };

// Naive English pluraliser used for quest description text. Matching against
// real monster names is done with the exact (singular) `enemyName`/`bossName`
// fields stored on each RepeatableQuest, so any grammar quirks here are
// display-only.
static std::string Plural(const std::string& noun)
{
    if (noun.empty()) return noun;
    if (noun == "Wolf")     return "Wolves";
    if (noun == "Seraphim") return "Seraphim";   // already plural
    char last  = noun.back();
    char prev  = (noun.size() >= 2) ? noun[noun.size() - 2] : '\0';
    if (last == 's' || last == 'x' || last == 'z' ||
        (last == 'h' && (prev == 's' || prev == 'c')))
        return noun + "es";
    if (last == 'y' && !(prev == 'a' || prev == 'e' || prev == 'i' || prev == 'o' || prev == 'u'))
        return noun.substr(0, noun.size() - 1) + "ies";
    return noun + "s";
}

// Build a "Defeat <boss> N times..." phrase. Bosses whose name already starts
// with "The" (e.g. "The Primordial One") don't get an extra "the" prepended.
static std::string BossPhrase(const std::string& bossName)
{
    if (bossName.rfind("The ", 0) == 0) return bossName;
    return "the " + bossName;
}

ReputationSystem::ReputationSystem()
{
    factions.resize(static_cast<int>(FactionID::FactionCount));
    Initialize();
}

void ReputationSystem::Initialize()
{
    InitializeFaction(FactionID::GuardiansOfFelrona, "Guardians of Felrona",
        "The stalwart defenders of Felrona village. They protect the innocent and maintain order in the frontier lands.",
        "Felrona", 0);
    InitializeFaction(FactionID::ShadowWardens, "Shadow Wardens",
        "Rangers and herbalists who patrol the Dark Woods, keeping the darkness at bay and protecting travelers.",
        "Dark Woods", 1);
    InitializeFaction(FactionID::TideCallers, "Tide Callers",
        "Coastal folk who command the seas. Pirate hunters, fishers, and lighthouse keepers of the Salty Coast.",
        "Salty Siren Coast", 2);
    InitializeFaction(FactionID::DragonScholars, "Dragon Scholars",
        "Brave scholars who study the ancient dragons of Dragon's Peak, seeking to understand their power.",
        "Dragon's Peak", 3);
    InitializeFaction(FactionID::HighlandClans, "Highland Clans",
        "Fierce warriors and mystics of the Aran Highlands. They guard the mountain passes and ancient shrines.",
        "Aran Highlands", 4);
    InitializeFaction(FactionID::VoidSeekers, "Void Seekers",
        "Holy warriors who descend into the Shadow Dungeon to combat the encroaching void corruption.",
        "Shadow Dungeon", 5);
    InitializeFaction(FactionID::CelestialOrder, "Celestial Order",
        "Celestial guardians who maintain the balance between worlds from the Celestial Spire.",
        "Celestial Spire", 6);
    InitializeFaction(FactionID::VoidExarchs, "Void Exarchs",
        "Warriors who venture beyond the veil to fight the primordial darkness threatening all existence.",
        "The Void Beyond", 7);
    InitializeFaction(FactionID::ArcaneConclave, "Arcane Conclave",
        "Powerful mages who guard the secrets of the Arcane Sanctum and study forbidden knowledge safely.",
        "Arcane Sanctum", 8);
    InitializeFaction(FactionID::ChronosWardens, "Chronos Wardens",
        "Time keepers who protect the flow of causality in the Chronos Depths from those who would break reality.",
        "Chronos Depths", 9);

    // Forbidden Citadel boss factions
    InitializeFaction(FactionID::AbyssalSentinel, "Abyssal Sentinel",
        "The first guardian of the Forbidden Citadel. Its armor of abyssal stone is said to be impervious.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::VoidEmpress, "Void Empress",
        "She commands the void itself, summoning darkness from between the stars.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::InfernalColossus, "Infernal Colossus",
        "A towering giant forged from living flame and volcanic stone.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::GlacialWraith, "Glacial Wraith",
        "A spirit of eternal winter that freezes the souls of the living.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::StormArbiter, "Storm Arbiter",
        "Judge and executioner of the Citadel, wielding the fury of the tempest.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::PlagueSovereign, "Plague Sovereign",
        "Lord of pestilence. Its mere presence withers the land and sickens the strong.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::HolyArbiter, "Holy Arbiter",
        "A fallen angel of devastating power, wielding sacred fire against the unworthy.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::ChronoOverlord, "Chrono Overlord",
        "Master of time within the Citadel. It bends causality to strike twice before you can react.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::VoidMonarch, "Void Monarch",
        "The supreme ruler of the void dimension, surpassing even the Primordial One.",
        "Forbidden Citadel", 10);
    InitializeFaction(FactionID::TheUnbroken, "The Unbroken",
        "An ancient warrior who has never been defeated. It grows stronger with every passing moment.",
        "Forbidden Citadel", 10);

    for (int i = 0; i < static_cast<int>(FactionID::FactionCount); ++i)
    {
        InitializeRewards(static_cast<FactionID>(i));
        InitializeRepeatableQuests(static_cast<FactionID>(i));
        InitializeVendorStock(static_cast<FactionID>(i));
    }
}

void ReputationSystem::InitializeFaction(FactionID id, const std::string& name,
    const std::string& desc, const std::string& areaName, int areaIndex)
{
    auto& f = GetFaction(id);
    f.id = id;
    f.name = name;
    f.description = desc;
    f.areaName = areaName;
    f.areaIndex = areaIndex;
    f.reputation = 0;
    f.rank = RepRank::Stranger;
}

// ---- Rewards per faction ----

void ReputationSystem::InitializeRewards(FactionID id)
{
    auto& f = GetFaction(id);

    f.rewards.clear();
    f.rewards.push_back({RepRank::Stranger, "Novice Permit", "Basic access to faction supplies", 100, 0.0f, 0.0f, 0.0f});
    f.rewards.push_back({RepRank::Acquaintance, "Trusted Outsider", "5% shop discount, 100 gold bonus", 250, 0.05f, 0.0f, 0.0f});
    f.rewards.push_back({RepRank::Friend, "Friend of the Faction", "10% shop discount, 5% XP bonus, 500 gold", 500, 0.10f, 0.05f, 0.0f});
    f.rewards.push_back({RepRank::Ally, "Sworn Ally", "15% shop discount, 10% XP bonus, 3% combat stats, 1000 gold", 1000, 0.15f, 0.10f, 0.03f});
    f.rewards.push_back({RepRank::Champion, "Faction Champion", "20% shop discount, 15% XP bonus, 5% combat stats, 2500 gold", 2500, 0.20f, 0.15f, 0.05f});
    f.rewards.push_back({RepRank::Legend, "Living Legend", "25% shop discount, 20% XP bonus, 8% combat stats, 5000 gold + Faction Pet", 5000, 0.25f, 0.20f, 0.08f});
}

// ---- Repeatable Quests per faction ----

void ReputationSystem::InitializeRepeatableQuests(FactionID id)
{
    auto& f = GetFaction(id);
    f.repeatableQuests.clear();

    // Exact enemy & boss names as spawned by EnemyDatabase (see Enemies.cpp).
    // These must match Monster::GetName() exactly so that
    // UpdateRepeatableQuestProgress can do precise string comparison.
    std::string enemy;
    std::string boss;
    switch (id)
    {
        case FactionID::GuardiansOfFelrona: enemy = "Bandit";            boss = "Farm Overseer";          break;
        case FactionID::ShadowWardens:      enemy = "Wolf";             boss = "Treant King";            break;
        case FactionID::TideCallers:        enemy = "Siren";            boss = "Kraken";                 break;
        case FactionID::DragonScholars:     enemy = "Drake";            boss = "Elder Dragon";           break;
        case FactionID::HighlandClans:      enemy = "Gryphon";          boss = "Aran Warlord";           break;
        case FactionID::VoidSeekers:        enemy = "Shadow";           boss = "Void Lord";              break;
        case FactionID::CelestialOrder:     enemy = "Seraphim";         boss = "Seraphim Council";       break;
        case FactionID::VoidExarchs:        enemy = "Void Walker";      boss = "The Primordial One";     break;
        case FactionID::ArcaneConclave:     enemy = "Arcane Guardian";  boss = "Arcane Construct";       break;
        case FactionID::ChronosWardens:     enemy = "Time Weaver";      boss = "Chronos, the Time Ender"; break;
        // Forbidden Citadel factions: boss-only area, all quests target the boss
    case FactionID::AbyssalSentinel:  enemy = ""; boss = "Abyssal Sentinel";  break;
    case FactionID::VoidEmpress:      enemy = ""; boss = "Void Empress";      break;
    case FactionID::InfernalColossus: enemy = ""; boss = "Infernal Colossus"; break;
    case FactionID::GlacialWraith:    enemy = ""; boss = "Glacial Wraith";    break;
    case FactionID::StormArbiter:     enemy = ""; boss = "Storm Arbiter";     break;
    case FactionID::PlagueSovereign:  enemy = ""; boss = "Plague Sovereign";  break;
    case FactionID::HolyArbiter:      enemy = ""; boss = "Holy Arbiter";      break;
    case FactionID::ChronoOverlord:   enemy = ""; boss = "Chrono Overlord";   break;
    case FactionID::VoidMonarch:      enemy = ""; boss = "Void Monarch";      break;
    case FactionID::TheUnbroken:      enemy = ""; boss = "The Unbroken";      break;
    default: enemy = "Enemy"; boss = "Boss"; break;
    }

    std::string enemyPl = enemy.empty() ? "" : Plural(enemy);
    std::string bossPh  = BossPhrase(boss);

    bool citadel = enemy.empty(); // Citadel factions have no regular enemies

    if (!citadel)
    {
        // Normal factions: mix of enemy-kill and boss-kill quests
        // Rank 0 (Stranger) — small enemy kill quests
        f.repeatableQuests.push_back({
            "Patrol " + f.areaName,
            "Defeat 5 " + enemyPl + " to prove your worth.",
            id, 0, 5, 0, 15, 50, 30, false, false, enemy, boss, false
        });
        f.repeatableQuests.push_back({
            "Gather Intelligence",
            "Defeat 8 " + enemyPl + " and report back.",
            id, 0, 8, 0, 25, 75, 50, false, false, enemy, boss, false
        });

        // Rank 1 (Acquaintance) — larger enemy kill quests
        f.repeatableQuests.push_back({
            "Clear the Perimeter",
            "Defeat 12 " + enemyPl + " threatening the area.",
            id, 1, 12, 0, 35, 100, 75, false, false, enemy, boss, false
        });
        f.repeatableQuests.push_back({
            "Strike at the Heart",
            "Defeat 15 " + enemyPl + " to cripple the enemy's forces.",
            id, 1, 15, 0, 50, 150, 100, false, false, enemy, boss, false
        });

        // Rank 2 (Friend) — one enemy hunt and one boss hunt
        f.repeatableQuests.push_back({
            "Heavy Assault",
            "Defeat 20 " + enemyPl + " in a coordinated strike.",
            id, 2, 20, 0, 60, 200, 125, false, false, enemy, boss, false
        });
        f.repeatableQuests.push_back({
            "Boss Bounty",
            "Defeat " + bossPh + " 3 times to earn the faction's respect.",
            id, 2, 3, 0, 80, 300, 150, false, false, enemy, boss, true
        });

        // Rank 3 (Ally)
        f.repeatableQuests.push_back({
            "War Effort",
            "Defeat 30 " + enemyPl + ". The faction needs its allies now more than ever.",
            id, 3, 30, 0, 100, 400, 200, false, false, enemy, boss, false
        });
        f.repeatableQuests.push_back({
            "Elite Hunting",
            "Defeat " + bossPh + " 5 times. Prove you are a true warrior.",
            id, 3, 5, 0, 120, 500, 250, false, false, enemy, boss, true
        });

        // Rank 4 (Champion)
        f.repeatableQuests.push_back({
            "The Great Campaign",
            "Defeat 50 " + enemyPl + ". Lead the charge for your faction.",
            id, 4, 50, 0, 150, 600, 300, false, false, enemy, boss, false
        });
        f.repeatableQuests.push_back({
            "Legendary Hunt",
            "Defeat " + bossPh + " 8 times. Become a legend among legends.",
            id, 4, 8, 0, 200, 800, 400, false, false, enemy, boss, true
        });
    }
    else
    {
        // Citadel factions: boss-only quests, escalating counts
        f.repeatableQuests.push_back({
            "Prove Your Strength",
            "Defeat " + bossPh + " once to prove you belong here.",
            id, 0, 1, 0, 30, 100, 60, false, false, enemy, boss, true
        });
        f.repeatableQuests.push_back({
            "Trial of the Citadel",
            "Defeat " + bossPh + " 2 times. The Citadel demands respect.",
            id, 1, 2, 0, 50, 200, 100, false, false, enemy, boss, true
        });
        f.repeatableQuests.push_back({
            "Champion's Challenge",
            "Defeat " + bossPh + " 3 times to earn the faction's trust.",
            id, 2, 3, 0, 80, 350, 175, false, false, enemy, boss, true
        });
        f.repeatableQuests.push_back({
            "Elite Hunt",
            "Defeat " + bossPh + " 5 times. Only the strongest survive.",
            id, 3, 5, 0, 120, 500, 250, false, false, enemy, boss, true
        });
        f.repeatableQuests.push_back({
            "The Great Campaign",
            "Defeat " + bossPh + " 8 times. Lead the charge into legend.",
            id, 4, 8, 0, 200, 800, 400, false, false, enemy, boss, true
        });
    }
}

void ReputationSystem::InitializeVendorStock(FactionID id)
{
    auto& f = GetFaction(id);
    int diff = f.areaIndex + 1;
    f.vendorStock.clear();

    // Citadel factions sell endgame-tier gear (diff 11)
    if (id >= FactionID::AbyssalSentinel && id <= FactionID::TheUnbroken)
    {
        f.vendorStock.push_back({"Health Potion x5", "Restores 50 HP each", 100, RepRank::Stranger,
            [diff]() { return LootTable::CreatePotion(diff); }});
        f.vendorStock.push_back({"Mana Potion x5", "Restores 30 MP each", 100, RepRank::Stranger,
            [diff]() { return LootTable::CreatePotion(diff); }});

        f.vendorStock.push_back({"Epic Weapon", "A weapon of immense power", 3000, RepRank::Friend,
            [diff]() { return LootTable::CreateEpicWeapon(diff); }});
        f.vendorStock.push_back({"Epic Armor", "Armor forged in citadel fire", 2500, RepRank::Friend,
            [diff]() { return LootTable::CreateEpicArmor(diff); }});
        f.vendorStock.push_back({"Epic Accessory", "Channels arcane citadel energy", 2000, RepRank::Friend,
            [diff]() { return LootTable::CreateAccessory(diff); }});

        f.vendorStock.push_back({"Legendary Weapon", "A weapon of unrivaled power", 8000, RepRank::Champion,
            [diff]() { return LootTable::CreateLegendaryWeapon(diff); }});
        f.vendorStock.push_back({"Legendary Armor", "Armor blessed by citadel forces", 6000, RepRank::Champion,
            [diff]() { return LootTable::CreateLegendaryArmor(diff); }});

        f.vendorStock.push_back({"Citadel Elixir", "A potent elixir from the Citadel depths", 500, RepRank::Ally,
            [diff]() { return LootTable::CreatePotion(diff); }});
        f.vendorStock.push_back({"Boss Trophy", "Proof of vanquishing a Citadel guardian", 1500, RepRank::Ally,
            [diff]() { return LootTable::CreateAccessory(diff); }});

        // Summoning essences at Ally rank
        std::string essenceName;
        std::string bossTarget;
        if (id == FactionID::AbyssalSentinel) { essenceName = "Sentinel's Essence"; bossTarget = "Abyssal Sentinel"; }
        else if (id == FactionID::VoidEmpress) { essenceName = "Empress's Essence"; bossTarget = "Void Empress"; }
        else if (id == FactionID::InfernalColossus) { essenceName = "Colossus Essence"; bossTarget = "Infernal Colossus"; }
        else if (id == FactionID::GlacialWraith) { essenceName = "Wraith's Essence"; bossTarget = "Glacial Wraith"; }
        else if (id == FactionID::StormArbiter) { essenceName = "Arbiter's Essence"; bossTarget = "Storm Arbiter"; }
        else if (id == FactionID::PlagueSovereign) { essenceName = "Sovereign Essence"; bossTarget = "Plague Sovereign"; }
        else if (id == FactionID::HolyArbiter) { essenceName = "Holy Essence"; bossTarget = "Holy Arbiter"; }
        else if (id == FactionID::ChronoOverlord) { essenceName = "Overlord's Essence"; bossTarget = "Chrono Overlord"; }
        else if (id == FactionID::VoidMonarch) { essenceName = "Monarch's Essence"; bossTarget = "Void Monarch"; }
        else if (id == FactionID::TheUnbroken) { essenceName = "Unbroken Essence"; bossTarget = "The Unbroken"; }
        if (!essenceName.empty())
        {
            f.vendorStock.push_back({essenceName, "Summon " + bossTarget, 5000, RepRank::Ally,
                [essenceName]() { return SummoningRegistry::Create(essenceName); }});
        }
        return;
    }

    f.vendorStock.push_back({"Health Potion x5", "Restores 50 HP each", 50, RepRank::Stranger,
        [diff]() { return LootTable::CreatePotion(diff); }});
    f.vendorStock.push_back({"Mana Potion x5", "Restores 30 MP each", 50, RepRank::Stranger,
        [diff]() { return LootTable::CreatePotion(diff); }});

    f.vendorStock.push_back({"Reinforced Shield", "A sturdy wooden shield", 200, RepRank::Acquaintance,
        [diff]() { return LootTable::CreateCommonArmor(diff); }});
    f.vendorStock.push_back({"Leather Armor", "Light protective gear", 150, RepRank::Acquaintance,
        [diff]() { return LootTable::CreateCommonArmor(diff); }});
    f.vendorStock.push_back({"Silver Ring", "A modest magical ring", 150, RepRank::Acquaintance,
        [diff]() { return LootTable::CreateAccessory(diff); }});

    f.vendorStock.push_back({"Steel Blade", "A well-forged sword", 500, RepRank::Friend,
        [diff]() { return LootTable::CreateRareWeapon(diff); }});
    f.vendorStock.push_back({"Chainmail", "Interlocking steel rings", 400, RepRank::Friend,
        [diff]() { return LootTable::CreateRareArmor(diff); }});
    f.vendorStock.push_back({"Arcane Circlet", "Enhances magical power", 400, RepRank::Friend,
        [diff]() { return LootTable::CreateAccessory(diff); }});

    f.vendorStock.push_back({"Gold Longsword", "A gleaming blade of gold", 1200, RepRank::Ally,
        [diff]() { return LootTable::CreateBossWeapon(diff); }});
    f.vendorStock.push_back({"Plate Armor", "Heavy full-body protection", 1000, RepRank::Ally,
        [diff]() { return LootTable::CreateBossArmor(diff); }});
    f.vendorStock.push_back({"Gold Amulet", "Radiates divine warmth", 800, RepRank::Ally,
        [diff]() { return LootTable::CreateAccessory(diff); }});

    f.vendorStock.push_back({"Mythril Greatsword", "Forged from rare mythril", 3000, RepRank::Champion,
        [diff]() { return LootTable::CreateEpicWeapon(diff); }});
    f.vendorStock.push_back({"Mythril Armor", "Light yet incredibly strong", 2500, RepRank::Champion,
        [diff]() { return LootTable::CreateEpicArmor(diff); }});
    f.vendorStock.push_back({"Mythril Focus", "Channels immense arcane energy", 2000, RepRank::Champion,
        [diff]() { return LootTable::CreateAccessory(diff); }});

    f.vendorStock.push_back({"Legendary Weapon", "A weapon of unrivaled power", 8000, RepRank::Legend,
        [diff]() { return LootTable::CreateLegendaryWeapon(diff); }});
    f.vendorStock.push_back({"Legendary Armor", "Armor blessed by the gods", 6000, RepRank::Legend,
        [diff]() { return LootTable::CreateLegendaryArmor(diff); }});
}

// ---- Reputation gains ----

void ReputationSystem::AddReputation(FactionID faction, int amount, const std::string& source)
{
    auto& f = GetFaction(faction);
    f.reputation += amount;
    f.totalReputationEarned += amount;
    UpdateRank(faction);
    AddNotification(faction, amount, source);
}

void ReputationSystem::AddKillReputation(FactionID faction, bool isBoss)
{
    int base = isBoss ? 25 : 3;
    AddReputation(faction, base, isBoss ? "Boss Defeated" : "Enemy Slain");
}

void ReputationSystem::AddDungeonReputation(FactionID faction, int dungeonDifficulty)
{
    int rep = 10 + dungeonDifficulty * 5;
    AddReputation(faction, rep, "Dungeon Cleared");
}

void ReputationSystem::OnQuestCompleted(FactionID faction, int reputationReward)
{
    AddReputation(faction, reputationReward, "Quest Complete");
}

void ReputationSystem::RefreshRepeatableQuests(FactionID faction)
{
    auto& f = GetFaction(faction);
    for (auto& q : f.repeatableQuests)
    {
        q.currentCount = 0;
        q.completed = false;
        q.active = false;
    }
}

// ---- Queries ----

int ReputationSystem::GetReputation(FactionID faction) const
{
    return GetFaction(faction).reputation;
}

RepRank ReputationSystem::GetRank(FactionID faction) const
{
    return GetFaction(faction).rank;
}

float ReputationSystem::GetShopDiscount(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    float discount = 0.0f;
    for (const auto& r : f.rewards)
    {
        if (static_cast<int>(r.requiredRank) <= static_cast<int>(f.rank))
            discount = std::max(discount, r.shopDiscount);
    }
    return discount;
}

float ReputationSystem::GetXPBonus(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    float bonus = 0.0f;
    for (const auto& r : f.rewards)
    {
        if (static_cast<int>(r.requiredRank) <= static_cast<int>(f.rank))
            bonus = std::max(bonus, r.xpBonus);
    }
    return bonus;
}

float ReputationSystem::GetCombatStatBonus(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    float bonus = 0.0f;
    for (const auto& r : f.rewards)
    {
        if (static_cast<int>(r.requiredRank) <= static_cast<int>(f.rank))
            bonus = std::max(bonus, r.combatStatBonus);
    }
    return bonus;
}

std::string ReputationSystem::GetRankName(RepRank rank) const
{
    switch (rank)
    {
        case RepRank::Stranger:     return "Stranger";
        case RepRank::Acquaintance: return "Acquaintance";
        case RepRank::Friend:       return "Friend";
        case RepRank::Ally:         return "Ally";
        case RepRank::Champion:     return "Champion";
        case RepRank::Legend:       return "Legend";
        default:                    return "Unknown";
    }
}

std::string ReputationSystem::GetRankTitle(FactionID faction) const
{
    return GetRankName(GetFaction(faction).rank);
}

int ReputationSystem::GetRepToNextRank(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    if (f.rank == RepRank::Legend) return 0;
    int nextRank = static_cast<int>(f.rank) + 1;
    return RANK_THRESHOLDS[nextRank] - f.reputation;
}

int ReputationSystem::GetRankThreshold(RepRank rank) const
{
    return RANK_THRESHOLDS[static_cast<int>(rank)];
}

int ReputationSystem::GetRepIntoCurrentRank(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    int curThreshold = RANK_THRESHOLDS[static_cast<int>(f.rank)];
    return f.reputation - curThreshold;
}

int ReputationSystem::GetRepForCurrentRankBracket(FactionID faction) const
{
    const auto& f = GetFaction(faction);
    if (f.rank == RepRank::Legend) return 0;
    int curThreshold = RANK_THRESHOLDS[static_cast<int>(f.rank)];
    int nextThreshold = RANK_THRESHOLDS[static_cast<int>(f.rank) + 1];
    return nextThreshold - curThreshold;
}

const FactionData& ReputationSystem::GetFactionData(FactionID faction) const
{
    return GetFaction(faction);
}

FactionData& ReputationSystem::GetFactionData(FactionID faction)
{
    return GetFaction(faction);
}

FactionID ReputationSystem::GetFactionByArea(int areaIndex) const
{
    for (const auto& f : factions)
    {
        if (f.areaIndex == areaIndex)
            return f.id;
    }
    return FactionID::GuardiansOfFelrona;
}

// ---- Repeatable quest management ----

const std::vector<RepeatableQuest>& ReputationSystem::GetRepeatableQuests(FactionID faction) const
{
    return GetFaction(faction).repeatableQuests;
}

bool ReputationSystem::AcceptRepeatableQuest(FactionID faction, int questIndex)
{
    auto& f = GetFaction(faction);
    if (questIndex < 0 || questIndex >= static_cast<int>(f.repeatableQuests.size()))
        return false;

    auto& q = f.repeatableQuests[questIndex];
    if (q.active || q.completed) return false;
    if (static_cast<int>(f.rank) < q.requiredRank) return false;

    q.active = true;
    q.currentCount = 0;
    return true;
}

void ReputationSystem::UpdateRepeatableQuestProgress(const std::string& enemyName)
{
    // Precise, faction-scoped matching: a kill only counts toward a quest if
    // the killed monster's name exactly equals that quest's stored target
    // (enemyName for regular quests, bossName for boss-kill quests). No more
    // substring scanning of the quest description and no cross-faction boss
    // bleed-through.
    for (auto& f : factions)
    {
        for (auto& q : f.repeatableQuests)
        {
            if (!q.active || q.completed) continue;

            bool matches = q.bossKillQuest
                ? (!q.bossName.empty() && enemyName == q.bossName)
                : (!q.enemyName.empty() && enemyName == q.enemyName);

            if (matches)
            {
                q.currentCount++;
                if (q.currentCount >= q.targetCount)
                {
                    q.completed = true;
                    q.currentCount = q.targetCount;
                }
            }
        }
    }
}

bool ReputationSystem::IsRepeatableQuestComplete(FactionID faction, int questIndex) const
{
    const auto& f = GetFaction(faction);
    if (questIndex < 0 || questIndex >= static_cast<int>(f.repeatableQuests.size()))
        return false;
    return f.repeatableQuests[questIndex].completed;
}

void ReputationSystem::ClaimRepeatableQuestReward(FactionID faction, int questIndex)
{
    auto& f = GetFaction(faction);
    if (questIndex < 0 || questIndex >= static_cast<int>(f.repeatableQuests.size()))
        return;

    auto& q = f.repeatableQuests[questIndex];
    if (!q.completed) return;

    AddReputation(faction, q.reputationReward, "Repeatable Quest");
    q.active = false;
    q.completed = false;
    q.currentCount = 0;
}

// ---- Vendor ----

const std::vector<VendorItem>& ReputationSystem::GetVendorStock(FactionID faction) const
{
    return GetFaction(faction).vendorStock;
}

bool ReputationSystem::CanBuyVendorItem(FactionID faction, int itemIndex, int playerGold) const
{
    const auto& stock = GetVendorStock(faction);
    if (itemIndex < 0 || itemIndex >= static_cast<int>(stock.size()))
        return false;
    const auto& item = stock[itemIndex];
    if (playerGold < item.goldCost)
        return false;
    if (static_cast<int>(GetFaction(faction).rank) < static_cast<int>(item.requiredRank))
        return false;
    return true;
}

bool ReputationSystem::BuyVendorItem(FactionID faction, int itemIndex, int& playerGold)
{
    if (!CanBuyVendorItem(faction, itemIndex, playerGold))
        return false;
    auto& stock = GetFaction(faction).vendorStock;
    playerGold -= stock[itemIndex].goldCost;
    return true;
}

// ---- Rank update ----

void ReputationSystem::UpdateRank(FactionID id)
{
    auto& f = GetFaction(id);
    for (int r = static_cast<int>(RepRank::Legend); r >= 0; --r)
    {
        if (f.reputation >= RANK_THRESHOLDS[r])
        {
            f.rank = static_cast<RepRank>(r);
            break;
        }
    }
}

int ReputationSystem::GetTotalReputationEarned() const
{
    int total = 0;
    for (const auto& f : factions)
        total += f.totalReputationEarned;
    return total;
}

// ---- Notifications ----

void ReputationSystem::AddNotification(FactionID faction, int amount, const std::string& source)
{
    RepGainNotification n;
    n.faction = faction;
    n.amount = amount;
    n.source = source;
    n.life = 3.0f;
    n.maxLife = 3.0f;
    notifications.push_back(n);
}

void ReputationSystem::UpdateNotifications(float dt)
{
    for (int i = static_cast<int>(notifications.size()) - 1; i >= 0; --i)
    {
        notifications[i].life -= dt;
        if (notifications[i].life <= 0.0f)
            notifications.erase(notifications.begin() + i);
    }
}

// ---- Faction accessors ----

FactionData& ReputationSystem::GetFaction(FactionID id)
{
    return factions[static_cast<int>(id)];
}

const FactionData& ReputationSystem::GetFaction(FactionID id) const
{
    return factions[static_cast<int>(id)];
}

// ---- Serialization ----

std::string ReputationSystem::Serialize() const
{
    std::ostringstream ss;
    for (const auto& f : factions)
    {
        ss << static_cast<int>(f.id) << ":"
           << f.reputation << ":"
           << static_cast<int>(f.rank) << ":"
           << f.totalReputationEarned << ";";
    }
    return ss.str();
}

void ReputationSystem::Deserialize(const std::string& data)
{
    if (data.empty()) return;

    std::istringstream ss(data);
    std::string entry;
    while (std::getline(ss, entry, ';'))
    {
        if (entry.empty()) continue;

        size_t c1 = entry.find(':');
        if (c1 == std::string::npos) continue;
        int id = std::stoi(entry.substr(0, c1));

        size_t c2 = entry.find(':', c1 + 1);
        if (c2 == std::string::npos) continue;
        int rep = std::stoi(entry.substr(c1 + 1, c2 - c1 - 1));

        size_t c3 = entry.find(':', c2 + 1);
        if (c3 == std::string::npos) continue;
        int rank = std::stoi(entry.substr(c2 + 1, c3 - c2 - 1));

        int totalRep = 0;
        if (c3 + 1 < entry.size())
            totalRep = std::stoi(entry.substr(c3 + 1));

        if (id >= 0 && id < static_cast<int>(FactionID::FactionCount))
        {
            auto& f = factions[id];
            f.reputation = rep;
            f.rank = static_cast<RepRank>(rank);
            f.totalReputationEarned = totalRep;
        }
    }
}
