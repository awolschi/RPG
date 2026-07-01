#include "Loot.hpp"
#include <cstdlib>
#include <ctime>

std::vector<std::shared_ptr<Item>> LootTable::GenerateLoot(int difficulty, int roll)
{
    std::vector<std::shared_ptr<Item>> loot;

    if (roll == -1)
    {
        roll = rand() % 100;
    }

    int baseDrop = 40 + (difficulty * 5);

    // 40-60% chance to drop common weapon
    if (roll < baseDrop)
    {
        loot.push_back(CreateCommonWeapon(difficulty));
        roll = rand() % 100;
    }

    // 30-40% chance to drop armor piece
    if (roll < 35 + (difficulty * 2))
    {
        loot.push_back(CreateCommonArmor(difficulty));
        roll = rand() % 100;
    }

    // 15-25% chance to drop rare item
    if (roll < 15 + (difficulty * 2))
    {
        if (rand() % 2 == 0)
        {
            loot.push_back(CreateRareWeapon(difficulty));
        }
        else
        {
            loot.push_back(CreateRareArmor(difficulty));
        }
    }

    return loot;
}

std::shared_ptr<Item> LootTable::CreateCommonWeapon(int difficulty)
{
    int damage = 10 + (difficulty * 5);
    int mana = rand() % (difficulty * 2);

    switch (rand() % 4)
    {
        case 0:
            return std::make_shared<Weapon>("Iron Sword", damage, mana, 1);
        case 1:
            return std::make_shared<Weapon>("Steel Axe", damage + 2, mana, 1);
        case 2:
            return std::make_shared<Weapon>("Wooden Staff", damage - 2, mana + 5, 1);
        case 3:
            return std::make_shared<Weapon>("Dagger", damage - 5, 0, 1);
        default:
            return std::make_shared<Weapon>("Sword", damage, mana, 1);
    }
}

std::shared_ptr<Item> LootTable::CreateCommonArmor(int difficulty)
{
    int defense = 2 + difficulty;
    ArmorType type = (rand() % 2 == 0) ? ArmorType::Leather : ArmorType::Cloth;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);

    std::string name;
    switch (piece)
    {
        case ArmorPiece::Helmet:
            name = (type == ArmorType::Leather) ? "Leather Helmet" : "Cloth Cap";
            break;
        case ArmorPiece::Chest:
            name = (type == ArmorType::Leather) ? "Leather Chest" : "Cloth Robe";
            break;
        case ArmorPiece::Gloves:
            name = (type == ArmorType::Leather) ? "Leather Gloves" : "Cloth Gloves";
            break;
        case ArmorPiece::Pants:
            name = (type == ArmorType::Leather) ? "Leather Pants" : "Cloth Pants";
            break;
        case ArmorPiece::Boots:
            name = (type == ArmorType::Leather) ? "Leather Boots" : "Cloth Boots";
            break;
    }

    return std::make_shared<Armor>(name, type, piece, defense, 1);
}

std::shared_ptr<Item> LootTable::CreateRareWeapon(int difficulty)
{
    int damage = 20 + (difficulty * 8);
    int mana = 3 + (difficulty * 2);

    switch (rand() % 3)
    {
        case 0:
            return std::make_shared<Weapon>("Enchanted Blade", damage, mana, 2);
        case 1:
            return std::make_shared<Weapon>("Mithril Mace", damage + 5, mana, 2);
        case 2:
            return std::make_shared<Weapon>("Crystal Staff", damage, mana + 10, 2);
        default:
            return std::make_shared<Weapon>("Legendary Sword", damage, mana, 2);
    }
}

std::shared_ptr<Item> LootTable::CreateRareArmor(int difficulty)
{
    int defense = 5 + (difficulty * 2);
    ArmorType type = (rand() % 2 == 0) ? ArmorType::Plate : ArmorType::Leather;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);

    std::string name;
    switch (piece)
    {
        case ArmorPiece::Helmet:
            name = (type == ArmorType::Plate) ? "Plate Helm" : "Reinforced Helm";
            break;
        case ArmorPiece::Chest:
            name = (type == ArmorType::Plate) ? "Plate Armor" : "Reinforced Chest";
            break;
        case ArmorPiece::Gloves:
            name = (type == ArmorType::Plate) ? "Plate Gauntlets" : "Reinforced Gloves";
            break;
        case ArmorPiece::Pants:
            name = (type == ArmorType::Plate) ? "Plate Leggings" : "Reinforced Pants";
            break;
        case ArmorPiece::Boots:
            name = (type == ArmorType::Plate) ? "Plate Boots" : "Reinforced Boots";
            break;
    }

    return std::make_shared<Armor>(name, type, piece, defense, 2);
}
