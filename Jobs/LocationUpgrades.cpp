#include "LocationUpgrades.hpp"
#include <algorithm>

LocationUpgrades::LocationUpgrades()
{
    Initialize();
}

void LocationUpgrades::Initialize()
{
    upgrades.clear();

    auto addUpgrade = [this](UpgradeType type, const char* name, const char* desc,
                              float speed, float qual, float fatigue, float rare, float xp, int storage, int baseCost)
    {
        LocationUpgrade u;
        u.type = type;
        u.name = name;
        u.description = desc;
        u.level = 0;
        u.maxLevel = 5;
        u.goldCostPerLevel = baseCost;
        u.speedBonusPerLevel = speed;
        u.qualityBonusPerLevel = qual;
        u.fatigueReductionPerLevel = fatigue;
        u.rareFindBonusPerLevel = rare;
        u.xpBonusPerLevel = xp;
        u.storageBonusPerLevel = storage;
        for (int i = 1; i <= u.maxLevel; ++i)
            u.costs.push_back(baseCost * i);
        upgrades.push_back(u);
    };

    addUpgrade(UpgradeType::ToolQuality,  "Tool Quality",  "Better tools improve speed and quality",
               0.05f, 0.04f, 0.0f, 0.0f, 0.0f, 0, 100);
    addUpgrade(UpgradeType::Workspace,    "Workspace",     "Larger workspace increases efficiency and XP",
               0.04f, 0.0f, 0.03f, 0.0f, 0.05f, 0, 200);
    addUpgrade(UpgradeType::Storage,      "Storage",       "More storage capacity for resources",
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 150);
    addUpgrade(UpgradeType::Lighting,     "Lighting",      "Better lighting improves quality and reduces fatigue",
               0.0f, 0.03f, 0.04f, 0.02f, 0.0f, 0, 120);
    addUpgrade(UpgradeType::Ventilation,  "Ventilation",   "Fresh air reduces fatigue buildup",
               0.0f, 0.0f, 0.05f, 0.0f, 0.0f, 0, 180);
    addUpgrade(UpgradeType::Automation,   "Automation",    "Semi-automated processes boost speed",
               0.06f, 0.0f, 0.02f, 0.0f, 0.0f, 0, 250);
    addUpgrade(UpgradeType::Enchantment,  "Enchantment",   "Magical enhancements boost rare finds and XP",
               0.0f, 0.0f, 0.0f, 0.04f, 0.03f, 0, 300);
}

bool LocationUpgrades::CanUpgrade(UpgradeType type, int gold) const
{
    for (const auto& u : upgrades)
    {
        if (u.type == type)
            return u.level < u.maxLevel && gold >= GetUpgradeCost(type);
    }
    return false;
}

bool LocationUpgrades::PerformUpgrade(UpgradeType type, int& gold)
{
    if (!CanUpgrade(type, gold)) return false;
    for (auto& u : upgrades)
    {
        if (u.type == type)
        {
            int cost = GetUpgradeCost(type);
            if (gold >= cost)
            {
                gold -= cost;
                u.level++;
                return true;
            }
            return false;
        }
    }
    return false;
}

int LocationUpgrades::GetLevel(UpgradeType type) const
{
    for (const auto& u : upgrades)
        if (u.type == type) return u.level;
    return 0;
}

int LocationUpgrades::GetUpgradeCost(UpgradeType type) const
{
    for (const auto& u : upgrades)
        if (u.type == type && u.level < u.maxLevel)
            return u.costs[u.level];
    return 0;
}

float LocationUpgrades::GetTotalSpeedBonus() const
{
    float total = 0;
    for (const auto& u : upgrades)
        total += u.speedBonusPerLevel * u.level;
    return total;
}

float LocationUpgrades::GetTotalQualityBonus() const
{
    float total = 0;
    for (const auto& u : upgrades)
        total += u.qualityBonusPerLevel * u.level;
    return total;
}

float LocationUpgrades::GetTotalFatigueReduction() const
{
    float total = 0;
    for (const auto& u : upgrades)
        total += u.fatigueReductionPerLevel * u.level;
    return std::min(total, 0.50f);
}

float LocationUpgrades::GetTotalRareFindBonus() const
{
    float total = 0;
    for (const auto& u : upgrades)
        total += u.rareFindBonusPerLevel * u.level;
    return total;
}

float LocationUpgrades::GetTotalXPBonus() const
{
    float total = 0;
    for (const auto& u : upgrades)
        total += u.xpBonusPerLevel * u.level;
    return total;
}

int LocationUpgrades::GetTotalStorageBonus() const
{
    int total = 0;
    for (const auto& u : upgrades)
        total += u.storageBonusPerLevel * u.level;
    return total;
}
