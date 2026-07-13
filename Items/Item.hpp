#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <memory>
#include <map>
#include "../Skills/Skill.hpp"

enum class ItemType
{
    Weapon,
    Armor,
    Accessory,
    Consumable,
    QuestItem,
    Resource
};

enum class WeaponType
{
    Sword,
    Axe,
    Bow,
    Staff,
    Dagger,
    Mace,
    Wand,
    Hammer,
    Scepter
};

inline const char* WeaponTypeName(WeaponType t)
{
    switch (t)
    {
        case WeaponType::Sword:    return "Sword";
        case WeaponType::Axe:      return "Axe";
        case WeaponType::Bow:      return "Bow";
        case WeaponType::Staff:    return "Staff";
        case WeaponType::Dagger:   return "Dagger";
        case WeaponType::Mace:     return "Mace";
        case WeaponType::Wand:     return "Wand";
        case WeaponType::Hammer:   return "Hammer";
        case WeaponType::Scepter:  return "Scepter";
    }
    return "Unknown";
}

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
        : name(name), type(type), rarity(rarity), sellValue(10), count(1)
    {
    }

    virtual ~Item() = default;

    virtual std::shared_ptr<Item> Clone() const
    {
        return std::make_shared<Item>(name, type, rarity);
    }

    virtual std::string GetDescription() const { return name; }

    bool IsSameAs(const Item& other) const;

    std::string name;
    ItemType type;
    int rarity;      // 1-5
    int sellValue;
    int count;
};

class Weapon : public Item
{
public:
    Weapon(const std::string& name, int damage, int manaCost = 0, int rarity = 1,
           ElementType element = ElementType::Physical, int elementDamage = 0,
           WeaponType weaponType = WeaponType::Sword)
        : Item(name, ItemType::Weapon, rarity), damage(damage), manaCost(manaCost),
          element(element), elementDamage(elementDamage), weaponType(weaponType)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Weapon>(name, damage, manaCost, rarity, element, elementDamage, weaponType);
        c->count = 1;
        return c;
    }

    int damage;
    int manaCost;
    ElementType element;
    int elementDamage;
    WeaponType weaponType;
};

class Armor : public Item
{
public:
    Armor(const std::string& name, ArmorType armorType, ArmorPiece piece, int defense, int rarity = 1,
          const std::map<ElementType, int>& elementalResist = {})
        : Item(name, ItemType::Armor, rarity), armorType(armorType), piece(piece), defense(defense),
          elementalResist(elementalResist)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Armor>(name, armorType, piece, defense, rarity, elementalResist);
        c->count = 1;
        return c;
    }

    int GetElementalReduction(ElementType element) const
    {
        auto it = elementalResist.find(element);
        if (it != elementalResist.end()) return it->second;
        return 0;
    }

    ArmorType armorType;
    ArmorPiece piece;
    int defense;
    std::map<ElementType, int> elementalResist;
};

class Accessory : public Item
{
public:
    Accessory(const std::string& name, int bonusHealth = 0, int bonusMana = 0, int rarity = 1,
              ElementType element = ElementType::Physical, int elementDamage = 0)
        : Item(name, ItemType::Accessory, rarity), bonusHealth(bonusHealth), bonusMana(bonusMana),
          element(element), elementDamage(elementDamage)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Accessory>(name, bonusHealth, bonusMana, rarity, element, elementDamage);
        c->count = 1;
        return c;
    }

    int bonusHealth;
    int bonusMana;
    ElementType element;
    int elementDamage;
};

inline bool Item::IsSameAs(const Item& other) const
{
    if (name != other.name || type != other.type || rarity != other.rarity)
        return false;
    if (auto w = dynamic_cast<const Weapon*>(this))
        if (auto ow = dynamic_cast<const Weapon*>(&other))
            return w->damage == ow->damage && w->manaCost == ow->manaCost;
    if (auto a = dynamic_cast<const Armor*>(this))
        if (auto oa = dynamic_cast<const Armor*>(&other))
            return a->armorType == oa->armorType && a->piece == oa->piece && a->defense == oa->defense;
    if (auto ac = dynamic_cast<const Accessory*>(this))
        if (auto oac = dynamic_cast<const Accessory*>(&other))
            return ac->bonusHealth == oac->bonusHealth && ac->bonusMana == oac->bonusMana;
    return true;
}

#endif
