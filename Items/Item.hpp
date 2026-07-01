#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <memory>

enum class ItemType
{
    Weapon,
    Armor,
    Accessory,
    Consumable,
    QuestItem
};

enum class ArmorType
{
    Cloth,
    Leather,
    Plate
};

enum class ArmorPiece
{
    Helmet,
    Chest,
    Gloves,
    Pants,
    Boots
};

class Item
{
public:
    Item(const std::string& name, ItemType type, int rarity = 1)
        : name(name), type(type), rarity(rarity), sellValue(10)
    {
    }

    virtual ~Item() = default;

    virtual std::string GetDescription() const { return name; }

    std::string name;
    ItemType type;
    int rarity;      // 1-5
    int sellValue;
};

class Weapon : public Item
{
public:
    Weapon(const std::string& name, int damage, int manaCost = 0, int rarity = 1)
        : Item(name, ItemType::Weapon, rarity), damage(damage), manaCost(manaCost)
    {
    }

    int damage;
    int manaCost;
};

class Armor : public Item
{
public:
    Armor(const std::string& name, ArmorType armorType, ArmorPiece piece, int defense, int rarity = 1)
        : Item(name, ItemType::Armor, rarity), armorType(armorType), piece(piece), defense(defense)
    {
    }

    ArmorType armorType;
    ArmorPiece piece;
    int defense;
};

class Accessory : public Item
{
public:
    Accessory(const std::string& name, int bonusHealth = 0, int bonusMana = 0, int rarity = 1)
        : Item(name, ItemType::Accessory, rarity), bonusHealth(bonusHealth), bonusMana(bonusMana)
    {
    }

    int bonusHealth;
    int bonusMana;
};

#endif
