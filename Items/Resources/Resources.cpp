#include "Resources.hpp"

Resource::Resource(const std::string& name, int tier, int sellValue)
    : Item(name, ItemType::Resource, tier), tier(tier)
{
    this->sellValue = sellValue;
}

std::string Resource::GetDescription() const
{
    return name + " (Tier " + std::to_string(tier) + ")";
}

std::shared_ptr<Resource> ResourceFactory::CreateMiningResource(int jobLevel)
{
    if (jobLevel >= 9) return std::make_shared<Resource>("Mythril Ore", 5, 50);
    if (jobLevel >= 7) return std::make_shared<Resource>("Gold Ore", 4, 30);
    if (jobLevel >= 5) return std::make_shared<Resource>("Silver Ore", 3, 20);
    if (jobLevel >= 3) return std::make_shared<Resource>("Iron Ore", 2, 12);
    return std::make_shared<Resource>("Copper Ore", 1, 5);
}

std::shared_ptr<Resource> ResourceFactory::CreateLumberjackingResource(int jobLevel)
{
    if (jobLevel >= 9) return std::make_shared<Resource>("Magical Wood", 5, 45);
    if (jobLevel >= 7) return std::make_shared<Resource>("Ebony Wood", 4, 28);
    if (jobLevel >= 5) return std::make_shared<Resource>("Maple Wood", 3, 18);
    if (jobLevel >= 3) return std::make_shared<Resource>("Oak Wood", 2, 10);
    return std::make_shared<Resource>("Wood", 1, 4);
}

std::shared_ptr<Resource> ResourceFactory::CreateFishingResource(int jobLevel)
{
    if (jobLevel >= 9) return std::make_shared<Resource>("Legendary Fish", 5, 55);
    if (jobLevel >= 7) return std::make_shared<Resource>("Exotic Fish", 4, 35);
    if (jobLevel >= 5) return std::make_shared<Resource>("Tuna", 3, 22);
    if (jobLevel >= 3) return std::make_shared<Resource>("Salmon", 2, 14);
    return std::make_shared<Resource>("Raw Fish", 1, 6);
}

std::shared_ptr<Resource> ResourceFactory::CreateSmithingResource(int jobLevel)
{
    if (jobLevel >= 9) return std::make_shared<Resource>("Mythril Ingot", 5, 60);
    if (jobLevel >= 7) return std::make_shared<Resource>("Gold Ingot", 4, 40);
    if (jobLevel >= 5) return std::make_shared<Resource>("Silver Ingot", 3, 25);
    if (jobLevel >= 3) return std::make_shared<Resource>("Steel Ingot", 2, 15);
    return std::make_shared<Resource>("Simple Ingot", 1, 8);
}

std::shared_ptr<Resource> ResourceFactory::CreateSkinningResource(int jobLevel)
{
    if (jobLevel >= 7) return std::make_shared<Resource>("Enchanted Hide", 4, 35);
    if (jobLevel >= 5) return std::make_shared<Resource>("Hardened Leather", 3, 22);
    if (jobLevel >= 3) return std::make_shared<Resource>("Leather", 2, 12);
    return std::make_shared<Resource>("Hide", 1, 6);
}

std::shared_ptr<Resource> ResourceFactory::CreateArcaneResource(int jobLevel)
{
    if (jobLevel >= 9) return std::make_shared<Resource>("Void Essence", 5, 65);
    if (jobLevel >= 7) return std::make_shared<Resource>("Arcane Crystal", 4, 42);
    if (jobLevel >= 5) return std::make_shared<Resource>("Mana Shard", 3, 24);
    return std::make_shared<Resource>("Arcane Dust", 1, 8);
}
