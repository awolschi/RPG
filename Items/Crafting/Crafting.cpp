#include "Crafting.hpp"
#include <algorithm>
#include <iostream>

CraftingManager::CraftingManager()
{
    InitializeRecipes();
}

int CraftingManager::CountResources(const std::string& resourceName, const Inventory& inventory) const
{
    int count = 0;
    for (size_t i = 0; i < inventory.GetItemCount(); ++i)
    {
        auto item = inventory.GetItem(i);
        if (item && item->type == ItemType::Resource && item->name == resourceName)
        {
            count += item->count;
        }
    }
    return count;
}

bool CraftingManager::CanCraft(size_t recipeIndex, const Inventory& inventory) const
{
    if (recipeIndex >= recipes.size())
        return false;

    const auto& recipe = recipes[recipeIndex];
    for (const auto& req : recipe.requirements)
    {
        if (CountResources(req.resourceName, inventory) < req.quantity)
            return false;
    }
    return true;
}

std::shared_ptr<Item> CraftingManager::Craft(size_t recipeIndex, Inventory& inventory)
{
    if (recipeIndex >= recipes.size())
        return nullptr;

    const auto& recipe = recipes[recipeIndex];

    for (const auto& req : recipe.requirements)
    {
        int remaining = req.quantity;
        for (size_t i = 0; i < inventory.GetItemCount() && remaining > 0; )
        {
            auto item = inventory.GetItem(i);
            if (item && item->type == ItemType::Resource && item->name == req.resourceName)
            {
                int take = std::min(remaining, item->count);
                remaining -= take;
                item->count -= take;
                if (item->count <= 0)
                    inventory.RemoveItem(i);
                else
                    i++;
            }
            else
            {
                i++;
            }
        }
    }

    auto result = recipe.createItem();
    if (result)
    {
        inventory.AddItem(result);
    }
    return result;
}

void CraftingManager::InitializeRecipes()
{
    // === TIER 1 (job level 1-2) ===

    recipes.push_back({
        "Copper Dagger",
        "A basic dagger forged from copper.",
        {{"Copper Ore", 2}},
        []() { return std::make_shared<Weapon>("Copper Dagger", 8, 0, 1, ElementType::Physical, 0, WeaponType::Dagger); }
    });

    recipes.push_back({
        "Wooden Staff",
        "A simple staff for channeling magic.",
        {{"Wood", 2}, {"Simple Ingot", 1}},
        []() { return std::make_shared<Weapon>("Wooden Staff", 4, 12, 1, ElementType::Physical, 0, WeaponType::Staff); }
    });

    recipes.push_back({
        "Short Bow",
        "A small hunting bow.",
        {{"Wood", 2}, {"Simple Ingot", 1}},
        []() { return std::make_shared<Weapon>("Short Bow", 10, 2, 1, ElementType::Physical, 0, WeaponType::Bow); }
    });

    // === TIER 2 (job level 3-4) ===

    recipes.push_back({
        "Iron Sword",
        "A sturdy iron blade.",
        {{"Iron Ore", 2}, {"Wood", 2}},
        []() { return std::make_shared<Weapon>("Iron Sword", 14, 2, 1, ElementType::Physical, 0, WeaponType::Sword); }
    });

    recipes.push_back({
        "Oak Staff",
        "A staff carved from oak, rich with natural energy.",
        {{"Oak Wood", 2}, {"Steel Ingot", 1}},
        []() { return std::make_shared<Weapon>("Oak Staff", 8, 20, 1, ElementType::Physical, 0, WeaponType::Staff); }
    });

    recipes.push_back({
        "Reinforced Bow",
        "A bow reinforced with steel fittings.",
        {{"Oak Wood", 2}, {"Steel Ingot", 1}},
        []() { return std::make_shared<Weapon>("Reinforced Bow", 16, 4, 1, ElementType::Physical, 0, WeaponType::Bow); }
    });

    recipes.push_back({
        "Cloth Armor",
        "Light cloth robes for magic users.",
        {{"Simple Ingot", 2}, {"Wood", 2}},
        []() { return std::make_shared<Armor>("Cloth Armor", ArmorType::Cloth, ArmorPiece::Chest, 4, 1); }
    });

    // === TIER 3 (job level 5-6) ===

    recipes.push_back({
        "Steel Blade",
        "A finely crafted steel sword.",
        {{"Silver Ore", 2}, {"Oak Wood", 2}},
        []() { return std::make_shared<Weapon>("Steel Blade", 22, 5, 2, ElementType::Physical, 0, WeaponType::Sword); }
    });

    recipes.push_back({
        "Crystal Staff",
        "A staff tipped with a magical crystal.",
        {{"Maple Wood", 2}, {"Silver Ingot", 1}},
        []() { return std::make_shared<Weapon>("Crystal Staff", 12, 30, 2, ElementType::Physical, 0, WeaponType::Staff); }
    });

    recipes.push_back({
        "Longbow",
        "A long-range bow favored by archers.",
        {{"Maple Wood", 2}, {"Silver Ingot", 1}},
        []() { return std::make_shared<Weapon>("Longbow", 24, 6, 2, ElementType::Physical, 0, WeaponType::Bow); }
    });

    recipes.push_back({
        "Chainmail",
        "Flexible chain armor offering solid protection.",
        {{"Silver Ingot", 2}, {"Maple Wood", 2}},
        []() { return std::make_shared<Armor>("Chainmail", ArmorType::Leather, ArmorPiece::Chest, 14, 2); }
    });

    recipes.push_back({
        "Silver Ring",
        "A ring that grants vitality.",
        {{"Salmon", 2}, {"Silver Ore", 1}},
        []() { return std::make_shared<Accessory>("Silver Ring", 30, 15, 2); }
    });

    // === TIER 4 (job level 7-8) ===

    recipes.push_back({
        "Gold Longsword",
        "A gleaming golden blade of great power.",
        {{"Gold Ore", 2}, {"Maple Wood", 2}},
        []() { return std::make_shared<Weapon>("Gold Longsword", 30, 8, 3, ElementType::Physical, 0, WeaponType::Sword); }
    });

    recipes.push_back({
        "Ebony Wand",
        "A dark wand crackling with arcane energy.",
        {{"Ebony Wood", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Weapon>("Ebony Wand", 16, 42, 3, ElementType::Physical, 0, WeaponType::Wand); }
    });

    recipes.push_back({
        "Ebony Bow",
        "A bow carved from rare ebony wood.",
        {{"Ebony Wood", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Weapon>("Ebony Bow", 32, 10, 3, ElementType::Physical, 0, WeaponType::Bow); }
    });

    recipes.push_back({
        "Plate Armor",
        "Heavy plate armor offering excellent defense.",
        {{"Gold Ingot", 2}, {"Ebony Wood", 2}},
        []() { return std::make_shared<Armor>("Plate Armor", ArmorType::Plate, ArmorPiece::Chest, 20, 3); }
    });

    recipes.push_back({
        "Gold Amulet",
        "A golden amulet pulsing with life energy.",
        {{"Tuna", 2}, {"Gold Ore", 1}},
        []() { return std::make_shared<Accessory>("Gold Amulet", 50, 30, 3); }
    });

    // === TIER 5 (job level 9-10) ===

    recipes.push_back({
        "Mythril Greatsword",
        "The ultimate blade, forged from mythril.",
        {{"Mythril Ore", 2}, {"Ebony Wood", 2}},
        []() { return std::make_shared<Weapon>("Mythril Greatsword", 42, 12, 4, ElementType::Physical, 0, WeaponType::Sword); }
    });

    recipes.push_back({
        "Magical Scepter",
        "A legendary scepter of immense magical power.",
        {{"Magical Wood", 2}, {"Mythril Ingot", 1}},
        []() { return std::make_shared<Weapon>("Magical Scepter", 22, 55, 4, ElementType::Physical, 0, WeaponType::Scepter); }
    });

    recipes.push_back({
        "Mythril Bow",
        "A mythril bow of unparalleled accuracy.",
        {{"Magical Wood", 2}, {"Mythril Ingot", 1}},
        []() { return std::make_shared<Weapon>("Mythril Bow", 40, 14, 4, ElementType::Physical, 0, WeaponType::Bow); }
    });

    recipes.push_back({
        "Mythril Armor",
        "Legendary mythril armor, light yet impenetrable.",
        {{"Mythril Ingot", 2}, {"Magical Wood", 2}},
        []() { return std::make_shared<Armor>("Mythril Armor", ArmorType::Plate, ArmorPiece::Chest, 28, 4); }
    });
}
