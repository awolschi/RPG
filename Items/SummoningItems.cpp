#include "SummoningItem.hpp"
#include "../World/Enemies/Enemies.hpp"

static std::map<std::string, std::function<std::shared_ptr<Monster>()>> g_summonFactories;

void SummoningRegistry::Initialize()
{
    g_summonFactories["Sentinel's Essence"]   = [] { return EnemyDatabase::CreateAbyssalSentinel(); };
    g_summonFactories["Empress's Essence"]    = [] { return EnemyDatabase::CreateVoidEmpress(); };
    g_summonFactories["Colossus Essence"]     = [] { return EnemyDatabase::CreateInfernalColossus(); };
    g_summonFactories["Wraith's Essence"]     = [] { return EnemyDatabase::CreateGlacialWraith(); };
    g_summonFactories["Arbiter's Essence"]    = [] { return EnemyDatabase::CreateStormArbiter(); };
    g_summonFactories["Sovereign Essence"]    = [] { return EnemyDatabase::CreatePlagueSovereign(); };
    g_summonFactories["Holy Essence"]         = [] { return EnemyDatabase::CreateCitadelHolyArbiter(); };
    g_summonFactories["Overlord's Essence"]   = [] { return EnemyDatabase::CreateChronoOverlord(); };
    g_summonFactories["Monarch's Essence"]    = [] { return EnemyDatabase::CreateVoidMonarch(); };
    g_summonFactories["Unbroken Essence"]     = [] { return EnemyDatabase::CreateTheUnbroken(); };
}

std::shared_ptr<SummoningItem> SummoningRegistry::Create(const std::string& itemName)
{
    auto it = g_summonFactories.find(itemName);
    if (it == g_summonFactories.end()) return nullptr;

    std::string bossName;
    if (itemName == "Sentinel's Essence")   bossName = "Abyssal Sentinel";
    else if (itemName == "Empress's Essence")    bossName = "Void Empress";
    else if (itemName == "Colossus Essence")     bossName = "Infernal Colossus";
    else if (itemName == "Wraith's Essence")     bossName = "Glacial Wraith";
    else if (itemName == "Arbiter's Essence")    bossName = "Storm Arbiter";
    else if (itemName == "Sovereign Essence")    bossName = "Plague Sovereign";
    else if (itemName == "Holy Essence")         bossName = "Holy Arbiter";
    else if (itemName == "Overlord's Essence")   bossName = "Chrono Overlord";
    else if (itemName == "Monarch's Essence")    bossName = "Void Monarch";
    else if (itemName == "Unbroken Essence")     bossName = "The Unbroken";
    else bossName = itemName;

    return std::make_shared<SummoningItem>(itemName, bossName, it->second);
}

const std::map<std::string, std::function<std::shared_ptr<Monster>()>>& SummoningRegistry::GetFactories()
{
    return g_summonFactories;
}
