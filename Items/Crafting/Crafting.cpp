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

bool CraftingManager::CanCraft(size_t recipeIndex, const Inventory& inventory, int jobLevel) const
{
    if (recipeIndex >= recipes.size())
        return false;

    const auto& recipe = recipes[recipeIndex];
    if (recipe.requiredJobLevel > 0 && jobLevel < recipe.requiredJobLevel)
        return false;
    for (const auto& req : recipe.requirements)
    {
        if (CountResources(req.resourceName, inventory) < req.quantity)
            return false;
    }
    return true;
}

bool CraftingManager::IsLocked(size_t recipeIndex, int jobLevel) const
{
    if (recipeIndex >= recipes.size())
        return true;
    const auto& recipe = recipes[recipeIndex];
    return recipe.requiredJobLevel > 0 && jobLevel < recipe.requiredJobLevel;
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
        []() { return std::make_shared<Weapon>("Copper Dagger", 8, 0, 1, ElementType::Physical, 0, WeaponType::Dagger); },
        JobType::Smithing, 1
    });

    recipes.push_back({
        "Wooden Staff",
        "A simple staff for channeling magic.",
        {{"Wood", 2}, {"Simple Ingot", 1}},
        []() { return std::make_shared<Weapon>("Wooden Staff", 4, 12, 1, ElementType::Physical, 0, WeaponType::Staff); },
        JobType::Smithing, 1
    });

    recipes.push_back({
        "Short Bow",
        "A small hunting bow.",
        {{"Wood", 2}, {"Simple Ingot", 1}},
        []() { return std::make_shared<Weapon>("Short Bow", 10, 2, 1, ElementType::Physical, 0, WeaponType::Bow); },
        JobType::Smithing, 1
    });

    // === TIER 2 (job level 3-4) ===

    recipes.push_back({
        "Iron Sword",
        "A sturdy iron blade.",
        {{"Iron Ore", 2}, {"Wood", 2}},
        []() { return std::make_shared<Weapon>("Iron Sword", 14, 2, 1, ElementType::Physical, 0, WeaponType::Sword); },
        JobType::Smithing, 3
    });

    recipes.push_back({
        "Oak Staff",
        "A staff carved from oak, rich with natural energy.",
        {{"Oak Wood", 2}, {"Steel Ingot", 1}},
        []() { return std::make_shared<Weapon>("Oak Staff", 8, 20, 1, ElementType::Physical, 0, WeaponType::Staff); },
        JobType::Smithing, 3
    });

    recipes.push_back({
        "Reinforced Bow",
        "A bow reinforced with steel fittings.",
        {{"Oak Wood", 2}, {"Steel Ingot", 1}},
        []() { return std::make_shared<Weapon>("Reinforced Bow", 16, 4, 1, ElementType::Physical, 0, WeaponType::Bow); },
        JobType::Smithing, 3
    });

    recipes.push_back({
        "Cloth Armor",
        "Light cloth robes for magic users.",
        {{"Simple Ingot", 2}, {"Wood", 2}},
        []() { return std::make_shared<Armor>("Cloth Armor", ArmorType::Cloth, ArmorPiece::Chest, 4, 1); },
        JobType::Smithing, 3
    });

    // === TIER 3 (job level 5-6) ===

    recipes.push_back({
        "Steel Blade",
        "A finely crafted steel sword.",
        {{"Silver Ore", 2}, {"Oak Wood", 2}},
        []() { return std::make_shared<Weapon>("Steel Blade", 22, 5, 2, ElementType::Physical, 0, WeaponType::Sword); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Crystal Staff",
        "A staff tipped with a magical crystal.",
        {{"Maple Wood", 2}, {"Silver Ingot", 1}},
        []() { return std::make_shared<Weapon>("Crystal Staff", 12, 30, 2, ElementType::Physical, 0, WeaponType::Staff); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Longbow",
        "A long-range bow favored by archers.",
        {{"Maple Wood", 2}, {"Silver Ingot", 1}},
        []() { return std::make_shared<Weapon>("Longbow", 24, 6, 2, ElementType::Physical, 0, WeaponType::Bow); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Chainmail",
        "Flexible chain armor offering solid protection.",
        {{"Silver Ingot", 2}, {"Maple Wood", 2}},
        []() { return std::make_shared<Armor>("Chainmail", ArmorType::Leather, ArmorPiece::Chest, 14, 2); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Silver Ring",
        "A ring that grants vitality.",
        {{"Salmon", 2}, {"Silver Ore", 1}},
        []() { return std::make_shared<Accessory>("Silver Ring", 30, 15, 2); },
        JobType::Smithing, 5
    });

    // === TIER 4 (job level 7-8) ===

    recipes.push_back({
        "Gold Longsword",
        "A gleaming golden blade of great power.",
        {{"Gold Ore", 2}, {"Maple Wood", 2}},
        []() { return std::make_shared<Weapon>("Gold Longsword", 30, 8, 3, ElementType::Physical, 0, WeaponType::Sword); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Ebony Wand",
        "A dark wand crackling with arcane energy.",
        {{"Ebony Wood", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Weapon>("Ebony Wand", 16, 42, 3, ElementType::Physical, 0, WeaponType::Wand); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Ebony Bow",
        "A bow carved from rare ebony wood.",
        {{"Ebony Wood", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Weapon>("Ebony Bow", 32, 10, 3, ElementType::Physical, 0, WeaponType::Bow); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Plate Armor",
        "Heavy plate armor offering excellent defense.",
        {{"Gold Ingot", 2}, {"Ebony Wood", 2}},
        []() { return std::make_shared<Armor>("Plate Armor", ArmorType::Plate, ArmorPiece::Chest, 20, 3); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Gold Amulet",
        "A golden amulet pulsing with life energy.",
        {{"Tuna", 2}, {"Gold Ore", 1}},
        []() { return std::make_shared<Accessory>("Gold Amulet", 50, 30, 3); },
        JobType::Smithing, 7
    });

    // === TIER 5 (job level 9-10) ===

    recipes.push_back({
        "Mythril Greatsword",
        "The ultimate blade, forged from mythril.",
        {{"Mythril Ore", 2}, {"Ebony Wood", 2}},
        []() { return std::make_shared<Weapon>("Mythril Greatsword", 42, 12, 4, ElementType::Physical, 0, WeaponType::Sword); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Magical Scepter",
        "A legendary scepter of immense magical power.",
        {{"Magical Wood", 2}, {"Mythril Ingot", 1}},
        []() { return std::make_shared<Weapon>("Magical Scepter", 22, 55, 4, ElementType::Physical, 0, WeaponType::Scepter); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Mythril Bow",
        "A mythril bow of unparalleled accuracy.",
        {{"Magical Wood", 2}, {"Mythril Ingot", 1}},
        []() { return std::make_shared<Weapon>("Mythril Bow", 40, 14, 4, ElementType::Physical, 0, WeaponType::Bow); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Mythril Armor",
        "Legendary mythril armor, light yet impenetrable.",
        {{"Mythril Ingot", 2}, {"Magical Wood", 2}},
        []() { return std::make_shared<Armor>("Mythril Armor", ArmorType::Plate, ArmorPiece::Chest, 28, 4); },
        JobType::Smithing, 9
    });

    // === TIER 3 MAGE (job level 5-6) — Cloth Armor + Offhand ===

    recipes.push_back({
        "Reinforced Cloth Robe",
        "Sturdy cloth robes reinforced with silver thread.",
        {{"Silver Ingot", 2}, {"Maple Wood", 2}},
        []() { return std::make_shared<Armor>("Reinforced Cloth Robe", ArmorType::Cloth, ArmorPiece::Chest, 10, 2); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Arcane Circlet",
        "A circlet humming with faint arcane energy.",
        {{"Silver Ingot", 1}, {"Maple Wood", 1}, {"Salmon", 1}},
        []() { return std::make_shared<Armor>("Arcane Circlet", ArmorType::Cloth, ArmorPiece::Helmet, 7, 2); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Mana Focus Orb",
        "A crystal orb that amplifies mana flow.",
        {{"Silver Ingot", 1}, {"Maple Wood", 1}},
        []() { return std::make_shared<Offhand>("Mana Focus Orb", OffhandType::Orb, 0, 20, 5, 2); },
        JobType::Smithing, 5
    });

    // === TIER 4 MAGE (job level 7-8) — Enchanted Cloth + Staff + Tome ===

    recipes.push_back({
        "Enchanted Cloth Armor",
        "Cloth armor woven with enchanted fibers.",
        {{"Gold Ingot", 2}, {"Ebony Wood", 2}},
        []() { return std::make_shared<Armor>("Enchanted Cloth Armor", ArmorType::Cloth, ArmorPiece::Chest, 16, 3); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Ebony Staff",
        "A dark staff crackling with arcane potential.",
        {{"Ebony Wood", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Weapon>("Ebony Staff", 20, 35, 3, ElementType::Physical, 0, WeaponType::Staff); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Arcane Tome",
        "A tome inscribed with arcane runes of power.",
        {{"Ebony Wood", 1}, {"Gold Ingot", 1}, {"Arcane Crystal", 1}},
        []() { return std::make_shared<Offhand>("Arcane Tome", OffhandType::Book, 0, 40, 10, 3); },
        JobType::Smithing, 7
    });

    // === TIER 5 MAGE (job level 9-10) — Mythril Cloth + Wand + Orb ===

    recipes.push_back({
        "Mythril Woven Robes",
        "Legendary robes woven from mythril threads.",
        {{"Mythril Ingot", 2}, {"Magical Wood", 2}},
        []() { return std::make_shared<Armor>("Mythril Woven Robes", ArmorType::Cloth, ArmorPiece::Chest, 24, 4); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Mythril Focus",
        "A wand forged from mythril and magical wood.",
        {{"Mythril Ingot", 1}, {"Magical Wood", 2}},
        []() { return std::make_shared<Weapon>("Mythril Focus", 28, 55, 4, ElementType::Physical, 0, WeaponType::Wand); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Void Shard Focus",
        "An orb infused with void shards and arcane crystals.",
        {{"Mythril Ingot", 2}, {"Arcane Crystal", 2}},
        []() { return std::make_shared<Offhand>("Void Shard Focus", OffhandType::Orb, 0, 80, 18, 4); },
        JobType::Smithing, 9
    });

    // === OFFHAND RECIPES (all classes) ===

    recipes.push_back({
        "Wooden Buckler",
        "A simple shield carved from sturdy wood.",
        {{"Maple Wood", 3}},
        []() { return std::make_shared<Offhand>("Wooden Buckler", OffhandType::Shield, 8, 0, 0, 1); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Iron Buckler",
        "A shield reinforced with iron bands.",
        {{"Iron Ingot", 2}, {"Maple Wood", 1}},
        []() { return std::make_shared<Offhand>("Iron Buckler", OffhandType::Shield, 15, 0, 0, 2); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Reinforced Shield",
        "A heavy shield forged with gold and iron.",
        {{"Gold Ingot", 2}, {"Iron Ingot", 1}},
        []() { return std::make_shared<Offhand>("Reinforced Shield", OffhandType::Shield, 22, 0, 0, 3); },
        JobType::Smithing, 7
    });

    recipes.push_back({
        "Mythril Aegis",
        "An impenetrable shield forged from mythril.",
        {{"Mythril Ingot", 2}, {"Gold Ingot", 1}},
        []() { return std::make_shared<Offhand>("Mythril Aegis", OffhandType::Shield, 30, 0, 0, 4); },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Leather Satchel",
        "A simple leather bag for storing supplies.",
        {{"Leather", 3}},
        []() { return std::make_shared<Offhand>("Leather Satchel", OffhandType::Bag, 0, 0, 0, 1); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Reinforced Satchel",
        "A reinforced leather satchel with iron buckles.",
        {{"Leather", 2}, {"Iron Ingot", 1}},
        []() { return std::make_shared<Offhand>("Reinforced Satchel", OffhandType::Bag, 0, 0, 0, 2); },
        JobType::Smithing, 5
    });

    recipes.push_back({
        "Woven Tome",
        "A basic tome bound in woven wood fibers.",
        {{"Maple Wood", 2}, {"Magical Wood", 1}},
        []() { return std::make_shared<Offhand>("Woven Tome", OffhandType::Book, 0, 25, 3, 1); },
        JobType::Smithing, 5
    });

    // === SET ITEM CRAFTING (Archmage's Regalia — Epic Set) ===

    recipes.push_back({
        "Archmage's Circlet",
        "A set piece from the Archmage's Regalia. Arcane circlet of immense power.",
        {{"Arcane Crystal", 3}, {"Mythril Ingot", 2}, {"Silver Ingot", 1}},
        []() {
            auto item = std::make_shared<Armor>("Archmage's Circlet", ArmorType::Cloth, ArmorPiece::Helmet, 14, 4,
                std::map<ElementType, int>{{ElementType::Arcane, 5}}, ItemPassive::SpellDmgBoost15);
            item->setId = 1;
            return item;
        },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Archmage's Mantle",
        "A set piece from the Archmage's Regalia. Mantle of arcane mastery.",
        {{"Arcane Crystal", 3}, {"Mythril Ingot", 3}},
        []() {
            auto item = std::make_shared<Armor>("Archmage's Mantle", ArmorType::Cloth, ArmorPiece::Chest, 18, 4,
                std::map<ElementType, int>{{ElementType::Arcane, 8}}, ItemPassive::ManaCostReduce10);
            item->setId = 1;
            return item;
        },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Archmage's Handwraps",
        "A set piece from the Archmage's Regalia. Handwraps of spell weaving.",
        {{"Arcane Crystal", 2}, {"Mythril Ingot", 1}, {"Magical Wood", 1}},
        []() {
            auto item = std::make_shared<Armor>("Archmage's Handwraps", ArmorType::Cloth, ArmorPiece::Gloves, 11, 4,
                std::map<ElementType, int>{{ElementType::Arcane, 4}}, ItemPassive::ManaRegen5);
            item->setId = 1;
            return item;
        },
        JobType::Smithing, 9
    });

    recipes.push_back({
        "Archmage's Focus",
        "A set piece from the Archmage's Regalia. Orb of supreme arcane focus.",
        {{"Arcane Crystal", 4}, {"Mythril Ingot", 2}},
        []() {
            auto item = std::make_shared<Offhand>("Archmage's Focus", OffhandType::Orb, 0, 30, 5, 4,
                ItemPassive::ManaRegen5);
            item->setId = 1;
            return item;
        },
        JobType::Smithing, 9
    });
}
