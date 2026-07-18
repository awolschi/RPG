#include "Resources.hpp"
#include <cstdlib>

Resource::Resource(const std::string& name, int tier, int sellValue,
                   int healAmount, int manaAmount, ResourceQuality quality)
    : Item(name, ItemType::Resource, tier), tier(tier),
      healAmount(healAmount), manaAmount(manaAmount), quality(quality)
{
    float mult = ResourceQualityMultiplier(quality);
    this->sellValue = static_cast<int>(sellValue * mult);
    if (healAmount > 0)
        this->healAmount = static_cast<int>(healAmount * mult);
    if (manaAmount > 0)
        this->manaAmount = static_cast<int>(manaAmount * mult);
}

std::string Resource::GetDescription() const
{
    std::string desc = GetFullName() + " (Tier " + std::to_string(tier) + ")";
    if (healAmount > 0 || manaAmount > 0)
    {
        desc += " - Restores ";
        if (healAmount > 0) desc += std::to_string(healAmount) + " HP";
        if (manaAmount > 0)
        {
            if (healAmount > 0) desc += ", ";
            desc += std::to_string(manaAmount) + " MP";
        }
    }
    return desc;
}

std::string Resource::GetFullName() const
{
    std::string qname = ResourceQualityName(quality);
    if (!qname.empty())
        return qname + " " + name;
    return name;
}

ResourceQuality ResourceFactory::RollQuality(int jobLevel)
{
    int roll = rand() % 100;
    int masterworkChance = 0;
    int fineChance = 0;

    if (jobLevel >= 100)      { fineChance = 35; masterworkChance = 12; }
    else if (jobLevel >= 75)  { fineChance = 30; masterworkChance = 9; }
    else if (jobLevel >= 50)  { fineChance = 25; masterworkChance = 7; }
    else if (jobLevel >= 35)  { fineChance = 20; masterworkChance = 5; }
    else if (jobLevel >= 20)  { fineChance = 15; masterworkChance = 3; }
    else if (jobLevel >= 10)  { fineChance = 10; masterworkChance = 2; }
    else if (jobLevel >= 5)   { fineChance = 5;  masterworkChance = 1; }

    if (roll < masterworkChance) return ResourceQuality::Masterwork;
    if (roll < masterworkChance + fineChance) return ResourceQuality::Fine;
    return ResourceQuality::Normal;
}

std::shared_ptr<Resource> ResourceFactory::CreateMiningResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 90) return std::make_shared<Resource>("Aetherium Ore", 10, 180, 0, 0, q);
    if (jobLevel >= 80) return std::make_shared<Resource>("Starstone Ore", 9, 150, 0, 0, q);
    if (jobLevel >= 70) return std::make_shared<Resource>("Void Ore", 8, 120, 0, 0, q);
    if (jobLevel >= 60) return std::make_shared<Resource>("Adamantite Ore", 7, 95, 0, 0, q);
    if (jobLevel >= 50) return std::make_shared<Resource>("Titanium Ore", 6, 75, 0, 0, q);
    if (jobLevel >= 40) return std::make_shared<Resource>("Platinum Ore", 5, 55, 0, 0, q);
    if (jobLevel >= 30) return std::make_shared<Resource>("Mythril Ore", 4, 40, 0, 0, q);
    if (jobLevel >= 20) return std::make_shared<Resource>("Gold Ore", 3, 28, 0, 0, q);
    if (jobLevel >= 10) return std::make_shared<Resource>("Silver Ore", 2, 18, 0, 0, q);
    if (jobLevel >= 5)  return std::make_shared<Resource>("Iron Ore", 1, 10, 0, 0, q);
    return std::make_shared<Resource>("Copper Ore", 1, 5, 0, 0, q);
}

std::shared_ptr<Resource> ResourceFactory::CreateLumberjackingResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 90) return std::make_shared<Resource>("Celestial Wood", 10, 170, 0, 0, q);
    if (jobLevel >= 80) return std::make_shared<Resource>("Void Timber", 9, 140, 0, 0, q);
    if (jobLevel >= 70) return std::make_shared<Resource>("Ancient Heartwood", 8, 110, 0, 0, q);
    if (jobLevel >= 60) return std::make_shared<Resource>("Ironbark", 7, 88, 0, 0, q);
    if (jobLevel >= 50) return std::make_shared<Resource>("Starwood", 6, 68, 0, 0, q);
    if (jobLevel >= 40) return std::make_shared<Resource>("Enchanted Oak", 5, 50, 0, 0, q);
    if (jobLevel >= 30) return std::make_shared<Resource>("Magical Wood", 4, 36, 0, 0, q);
    if (jobLevel >= 20) return std::make_shared<Resource>("Ebony Wood", 3, 25, 0, 0, q);
    if (jobLevel >= 10) return std::make_shared<Resource>("Maple Wood", 2, 16, 0, 0, q);
    if (jobLevel >= 5)  return std::make_shared<Resource>("Oak Wood", 1, 9, 0, 0, q);
    return std::make_shared<Resource>("Wood", 1, 4, 0, 0, q);
}

std::shared_ptr<Resource> ResourceFactory::CreateFishingResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 90) return std::make_shared<Resource>("Leviathan", 10, 200, 500, 120, q);
    if (jobLevel >= 80) return std::make_shared<Resource>("Void Angler", 9, 165, 400, 100, q);
    if (jobLevel >= 70) return std::make_shared<Resource>("Dragon Eel", 8, 130, 320, 80, q);
    if (jobLevel >= 60) return std::make_shared<Resource>("Thunderfish", 7, 105, 260, 65, q);
    if (jobLevel >= 50) return std::make_shared<Resource>("Crystal Koi", 6, 82, 210, 52, q);
    if (jobLevel >= 40) return std::make_shared<Resource>("Phoenix Salmon", 5, 60, 160, 40, q);
    if (jobLevel >= 30) return std::make_shared<Resource>("Legendary Fish", 4, 44, 120, 30, q);
    if (jobLevel >= 20) return std::make_shared<Resource>("Exotic Fish", 3, 30, 80, 20, q);
    if (jobLevel >= 10) return std::make_shared<Resource>("Tuna", 2, 20, 50, 12, q);
    if (jobLevel >= 5)  return std::make_shared<Resource>("Salmon", 1, 12, 30, 6, q);
    return std::make_shared<Resource>("Raw Fish", 1, 6, 20, 0, q);
}

std::shared_ptr<Resource> ResourceFactory::CreateCookedFish(int tier)
{
    switch (tier)
    {
        case 10: return std::make_shared<Resource>("Cooked Leviathan", 10, 280, 750, 180);
        case 9:  return std::make_shared<Resource>("Cooked Void Angler", 9, 230, 600, 150);
        case 8:  return std::make_shared<Resource>("Cooked Dragon Eel", 8, 180, 480, 120);
        case 7:  return std::make_shared<Resource>("Cooked Thunderfish", 7, 145, 390, 98);
        case 6:  return std::make_shared<Resource>("Cooked Crystal Koi", 6, 112, 315, 78);
        case 5:  return std::make_shared<Resource>("Cooked Phoenix Salmon", 5, 82, 240, 60);
        case 4:  return std::make_shared<Resource>("Cooked Legendary Fish", 4, 60, 180, 45);
        case 3:  return std::make_shared<Resource>("Cooked Exotic Fish", 3, 42, 120, 30);
        case 2:  return std::make_shared<Resource>("Cooked Tuna", 2, 28, 75, 18);
        case 1:  return std::make_shared<Resource>("Cooked Salmon", 1, 17, 45, 9);
        default: return std::make_shared<Resource>("Cooked Fish", 1, 10, 30, 0);
    }
}

std::shared_ptr<Resource> ResourceFactory::CreateSmithingResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 90) return std::make_shared<Resource>("Aetherium Ingot", 10, 220, 0, 0, q);
    if (jobLevel >= 80) return std::make_shared<Resource>("Starsteel Ingot", 9, 180, 0, 0, q);
    if (jobLevel >= 70) return std::make_shared<Resource>("Voidsteel Ingot", 8, 145, 0, 0, q);
    if (jobLevel >= 60) return std::make_shared<Resource>("Adamantite Ingot", 7, 115, 0, 0, q);
    if (jobLevel >= 50) return std::make_shared<Resource>("Titanium Ingot", 6, 90, 0, 0, q);
    if (jobLevel >= 40) return std::make_shared<Resource>("Platinum Ingot", 5, 68, 0, 0, q);
    if (jobLevel >= 30) return std::make_shared<Resource>("Mythril Ingot", 4, 50, 0, 0, q);
    if (jobLevel >= 20) return std::make_shared<Resource>("Gold Ingot", 3, 35, 0, 0, q);
    if (jobLevel >= 10) return std::make_shared<Resource>("Silver Ingot", 2, 22, 0, 0, q);
    if (jobLevel >= 5)  return std::make_shared<Resource>("Steel Ingot", 1, 12, 0, 0, q);
    return std::make_shared<Resource>("Simple Ingot", 1, 8, 0, 0, q);
}

std::shared_ptr<Resource> ResourceFactory::CreateSkinningResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 70) return std::make_shared<Resource>("Primordial Hide", 8, 100, 0, 0, q);
    if (jobLevel >= 50) return std::make_shared<Resource>("Dragon Hide", 6, 65, 0, 0, q);
    if (jobLevel >= 30) return std::make_shared<Resource>("Enchanted Hide", 4, 35, 0, 0, q);
    if (jobLevel >= 10) return std::make_shared<Resource>("Leather", 2, 12, 0, 0, q);
    return std::make_shared<Resource>("Hide", 1, 6, 0, 0, q);
}

std::shared_ptr<Resource> ResourceFactory::CreateArcaneResource(int jobLevel)
{
    ResourceQuality q = RollQuality(jobLevel);
    if (jobLevel >= 70) return std::make_shared<Resource>("Void Essence", 8, 110, 0, 0, q);
    if (jobLevel >= 40) return std::make_shared<Resource>("Arcane Crystal", 5, 42, 0, 0, q);
    if (jobLevel >= 20) return std::make_shared<Resource>("Mana Shard", 3, 24, 0, 0, q);
    return std::make_shared<Resource>("Arcane Dust", 1, 8, 0, 0, q);
}
