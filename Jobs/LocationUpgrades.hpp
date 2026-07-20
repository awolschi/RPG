#ifndef LOCATION_UPGRADES_HPP
#define LOCATION_UPGRADES_HPP

#include <string>
#include <vector>
#include <map>
#include "Job.hpp"

enum class UpgradeType
{
    ToolQuality,
    Workspace,
    Storage,
    Lighting,
    Ventilation,
    Automation,
    Enchantment
};

inline const char* UpgradeTypeName(UpgradeType t)
{
    switch (t)
    {
        case UpgradeType::ToolQuality:  return "Tool Quality";
        case UpgradeType::Workspace:    return "Workspace";
        case UpgradeType::Storage:      return "Storage";
        case UpgradeType::Lighting:     return "Lighting";
        case UpgradeType::Ventilation:  return "Ventilation";
        case UpgradeType::Automation:   return "Automation";
        case UpgradeType::Enchantment:  return "Enchantment";
        default: return "Unknown";
    }
}

struct LocationUpgrade
{
    UpgradeType type;
    std::string name;
    std::string description;
    int level = 0;
    int maxLevel = 5;
    int goldCostPerLevel = 100;
    std::vector<int> costs; // gold costs per level

    float speedBonusPerLevel = 0.0f;
    float qualityBonusPerLevel = 0.0f;
    float fatigueReductionPerLevel = 0.0f;
    float rareFindBonusPerLevel = 0.0f;
    float xpBonusPerLevel = 0.0f;
    int storageBonusPerLevel = 0;
};

class LocationUpgrades
{
public:
    LocationUpgrades();

    void Initialize();
    bool CanUpgrade(UpgradeType type, int gold) const;
    bool PerformUpgrade(UpgradeType type, int& gold);
    int GetLevel(UpgradeType type) const;
    int GetUpgradeCost(UpgradeType type) const;

    float GetTotalSpeedBonus() const;
    float GetTotalQualityBonus() const;
    float GetTotalFatigueReduction() const;
    float GetTotalRareFindBonus() const;
    float GetTotalXPBonus() const;
    int GetTotalStorageBonus() const;

    std::vector<LocationUpgrade> upgrades;
};

#endif
