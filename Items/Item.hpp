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
    Resource,
    Offhand,
    Summoning
};

enum class Rarity : int
{
    Common    = 1,
    Uncommon  = 2,
    Rare      = 3,
    Epic      = 4,
    Legendary = 5
};

inline const char* RarityName(Rarity r)
{
    switch (r)
    {
        case Rarity::Common:    return "Common";
        case Rarity::Uncommon:  return "Uncommon";
        case Rarity::Rare:      return "Rare";
        case Rarity::Epic:      return "Epic";
        case Rarity::Legendary: return "Legendary";
    }
    return "Unknown";
}

enum class ItemPassive : int
{
    None = 0,
    FireNoCd20,
    IceFreezePlus1,
    LightningStun15,
    ArcaneCrit30,
    PoisonDmgPlus25,
    PhysDmgBoost15,
    PhysDmgBoost20,
    AtkSpeed20,
    CritChance20,
    CritChance30,
    Lifesteal15,
    Lifesteal25,
    SpellDmgBoost15,
    SpellDmgBoost20,
    Dodge15,
    Dodge20,
    DamageReduce10,
    DamageReduce15,
    Thorns25,
    Thorns40,
    HealOnKill10,
    HealOnKill15,
    StunImmune,
    FreezeImmune,
    BurnImmune,
    PoisonImmune,
    TauntChance25,
    StatusExtend1,
    StatusExtend2,
    PhoenixRevive,
    Revive50,
    ManaRegen5,
    ManaRegen10,
    ManaCostReduce10,
    ManaCostReduce15,
    ManaShield30,
    ManaOnKill15,
    ManaOnKill25,
    ManaOnSkillUse10,
    HpOnSkillUse10,
    DoubleCast15,
    DoubleCast25,
    CooldownReduce1,
    CooldownReduce2,
    HolyHealOnKill,
    SpellDmgLowHp30,
    PhysDmgLowHp30,
    AllResist5,
    AllResist10,
    ExpBoost20,
    GoldFind30,
};

inline const char* PassiveName(ItemPassive p)
{
    switch (p)
    {
        case ItemPassive::None: return "None";
        case ItemPassive::FireNoCd20: return "Fire spells 20% chance to ignore cooldown";
        case ItemPassive::IceFreezePlus1: return "Ice spells freeze +1 turn";
        case ItemPassive::LightningStun15: return "Lightning spells 15% chance to stun 1 turn";
        case ItemPassive::ArcaneCrit30: return "Arcane spells 30% chance for double damage";
        case ItemPassive::PoisonDmgPlus25: return "Poison effects deal +25% damage";
        case ItemPassive::PhysDmgBoost15: return "+15% physical damage";
        case ItemPassive::PhysDmgBoost20: return "+20% physical damage";
        case ItemPassive::AtkSpeed20: return "+20% attack speed";
        case ItemPassive::CritChance20: return "20% chance for critical hit";
        case ItemPassive::CritChance30: return "30% chance for critical hit";
        case ItemPassive::Lifesteal15: return "Heal for 15% of damage dealt";
        case ItemPassive::Lifesteal25: return "Heal for 25% of damage dealt";
        case ItemPassive::SpellDmgBoost15: return "+15% spell damage";
        case ItemPassive::SpellDmgBoost20: return "+20% spell damage";
        case ItemPassive::Dodge15: return "15% chance to dodge attacks";
        case ItemPassive::Dodge20: return "20% chance to dodge attacks";
        case ItemPassive::DamageReduce10: return "Take 10% less damage";
        case ItemPassive::DamageReduce15: return "Take 15% less damage";
        case ItemPassive::Thorns25: return "Reflect 25% of damage taken";
        case ItemPassive::Thorns40: return "Reflect 40% of damage taken";
        case ItemPassive::HealOnKill10: return "Killing an enemy restores 10% max HP";
        case ItemPassive::HealOnKill15: return "Killing an enemy restores 15% max HP";
        case ItemPassive::StunImmune: return "Immune to stun";
        case ItemPassive::FreezeImmune: return "Immune to freeze";
        case ItemPassive::BurnImmune: return "Immune to burn";
        case ItemPassive::PoisonImmune: return "Immune to poison";
        case ItemPassive::TauntChance25: return "25% chance to force single-target attacks on self";
        case ItemPassive::StatusExtend1: return "Status effects you apply last +1 turn";
        case ItemPassive::StatusExtend2: return "Status effects you apply last +2 turns";
        case ItemPassive::PhoenixRevive: return "Revive once per battle with 30% HP";
        case ItemPassive::Revive50: return "Revive once per battle with 50% HP";
        case ItemPassive::ManaRegen5: return "+5 mana restored per turn";
        case ItemPassive::ManaRegen10: return "+10 mana restored per turn";
        case ItemPassive::ManaCostReduce10: return "All spells cost 10% less mana";
        case ItemPassive::ManaCostReduce15: return "All spells cost 15% less mana";
        case ItemPassive::ManaShield30: return "30% of damage taken absorbed by mana";
        case ItemPassive::ManaOnKill15: return "Restore 15 mana on kill";
        case ItemPassive::ManaOnKill25: return "Restore 25 mana on kill";
        case ItemPassive::ManaOnSkillUse10: return "Restore 10 mana each time you use a skill";
        case ItemPassive::HpOnSkillUse10: return "Heal 10 HP each time you use a skill";
        case ItemPassive::DoubleCast15: return "15% chance spells hit twice";
        case ItemPassive::DoubleCast25: return "25% chance spells hit twice";
        case ItemPassive::CooldownReduce1: return "All cooldowns reduced by 1 turn";
        case ItemPassive::CooldownReduce2: return "All cooldowns reduced by 2 turns";
        case ItemPassive::HolyHealOnKill: return "Killing an enemy heals 10% max HP (holy)";
        case ItemPassive::SpellDmgLowHp30: return "+30% spell damage when HP < 50%";
        case ItemPassive::PhysDmgLowHp30: return "+30% physical damage when HP < 50%";
        case ItemPassive::AllResist5: return "+5 resistance to all elements";
        case ItemPassive::AllResist10: return "+10 resistance to all elements";
        case ItemPassive::ExpBoost20: return "+20% experience from kills";
        case ItemPassive::GoldFind30: return "+30% gold from kills";
    }
    return "Unknown";
}

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
    Item(const std::string& name, ItemType type, int rarity = 1,
         ItemPassive passive1 = ItemPassive::None, ItemPassive passive2 = ItemPassive::None)
        : name(name), type(type), rarity(rarity), sellValue(10), count(1),
          passive1(passive1), passive2(passive2)
    {
    }

    virtual ~Item() = default;

    virtual std::shared_ptr<Item> Clone() const
    {
        auto c = std::make_shared<Item>(name, type, rarity, passive1, passive2);
        c->setId = setId;
        c->sellValue = sellValue;
        c->requiredLevel = requiredLevel;
        return c;
    }

    virtual std::string GetDescription() const { return name; }

    bool IsSameAs(const Item& other) const;
    bool HasPassive(ItemPassive p) const { return passive1 == p || passive2 == p; }
    bool HasAnyPassive() const { return passive1 != ItemPassive::None || passive2 != ItemPassive::None; }

    std::string name;
    ItemType type;
    int rarity;      // 1-5
    int sellValue;
    int count;
    int setId = -1;  // Set membership (-1 = not a set item)
    int requiredLevel = 1;
    ItemPassive passive1;
    ItemPassive passive2;
};

class Weapon : public Item
{
public:
    static constexpr ItemType ClassType = ItemType::Weapon;
    Weapon(const std::string& name, int damage, int manaCost = 0, int rarity = 1,
           ElementType element = ElementType::Physical, int elementDamage = 0,
           WeaponType weaponType = WeaponType::Sword,
           ItemPassive passive1 = ItemPassive::None, ItemPassive passive2 = ItemPassive::None)
        : Item(name, ItemType::Weapon, rarity, passive1, passive2), damage(damage), manaCost(manaCost),
          element(element), elementDamage(elementDamage), weaponType(weaponType)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Weapon>(name, damage, manaCost, rarity, element, elementDamage, weaponType, passive1, passive2);
        c->setId = setId;
        c->count = 1;
        c->requiredLevel = requiredLevel;
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
    static constexpr ItemType ClassType = ItemType::Armor;
    Armor(const std::string& name, ArmorType armorType, ArmorPiece piece, int defense, int rarity = 1,
          const std::map<ElementType, int>& elementalResist = {},
          ItemPassive passive1 = ItemPassive::None, ItemPassive passive2 = ItemPassive::None)
        : Item(name, ItemType::Armor, rarity, passive1, passive2), armorType(armorType), piece(piece), defense(defense),
          elementalResist(elementalResist)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Armor>(name, armorType, piece, defense, rarity, elementalResist, passive1, passive2);
        c->setId = setId;
        c->count = 1;
        c->requiredLevel = requiredLevel;
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
    static constexpr ItemType ClassType = ItemType::Accessory;
    Accessory(const std::string& name, int bonusHealth = 0, int bonusMana = 0, int rarity = 1,
              ElementType element = ElementType::Physical, int elementDamage = 0,
              ItemPassive passive1 = ItemPassive::None, ItemPassive passive2 = ItemPassive::None)
        : Item(name, ItemType::Accessory, rarity, passive1, passive2), bonusHealth(bonusHealth), bonusMana(bonusMana),
          element(element), elementDamage(elementDamage)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Accessory>(name, bonusHealth, bonusMana, rarity, element, elementDamage, passive1, passive2);
        c->setId = setId;
        c->count = 1;
        c->requiredLevel = requiredLevel;
        return c;
    }

    int bonusHealth;
    int bonusMana;
    ElementType element;
    int elementDamage;
};

enum class OffhandType
{
    Shield,
    Orb,
    Book,
    Bag,
    Quiver
};

inline const char* OffhandTypeName(OffhandType t)
{
    switch (t)
    {
        case OffhandType::Shield: return "Shield";
        case OffhandType::Orb:    return "Orb";
        case OffhandType::Book:   return "Book";
        case OffhandType::Bag:    return "Bag";
        case OffhandType::Quiver: return "Quiver";
    }
    return "Unknown";
}

class Offhand : public Item
{
public:
    static constexpr ItemType ClassType = ItemType::Offhand;
    Offhand(const std::string& name, OffhandType offhandType, int defense, int manaBonus,
            int arcaneDamage = 0, int rarity = 1,
            ItemPassive passive1 = ItemPassive::None, ItemPassive passive2 = ItemPassive::None,
            int damageBonus = 0)
        : Item(name, ItemType::Offhand, rarity, passive1, passive2), offhandType(offhandType),
          defense(defense), manaBonus(manaBonus), arcaneDamage(arcaneDamage), damageBonus(damageBonus)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Offhand>(name, offhandType, defense, manaBonus, arcaneDamage, rarity, passive1, passive2, damageBonus);
        c->setId = setId;
        c->count = 1;
        c->requiredLevel = requiredLevel;
        return c;
    }

    OffhandType offhandType;
    int defense;
    int manaBonus;
    int arcaneDamage;
    int damageBonus;
};

inline bool Item::IsSameAs(const Item& other) const
{
    if (name != other.name || type != other.type || rarity != other.rarity)
        return false;
    if (type == ItemType::Weapon)
    {
        auto w = static_cast<const Weapon*>(this);
        auto ow = static_cast<const Weapon*>(&other);
        return w->damage == ow->damage && w->manaCost == ow->manaCost;
    }
    if (type == ItemType::Armor)
    {
        auto a = static_cast<const Armor*>(this);
        auto oa = static_cast<const Armor*>(&other);
        return a->armorType == oa->armorType && a->piece == oa->piece && a->defense == oa->defense;
    }
    if (type == ItemType::Accessory)
    {
        auto ac = static_cast<const Accessory*>(this);
        auto oac = static_cast<const Accessory*>(&other);
        return ac->bonusHealth == oac->bonusHealth && ac->bonusMana == oac->bonusMana;
    }
    if (type == ItemType::Offhand)
    {
        auto oh = static_cast<const Offhand*>(this);
        auto ooh = static_cast<const Offhand*>(&other);
        return oh->offhandType == ooh->offhandType && oh->defense == ooh->defense && oh->manaBonus == ooh->manaBonus && oh->damageBonus == ooh->damageBonus;
    }
    return true;
}

template<typename T>
inline std::shared_ptr<T> item_cast(const std::shared_ptr<Item>& item)
{
    return (item && item->type == T::ClassType) ? std::static_pointer_cast<T>(item) : nullptr;
}

template<typename T>
inline std::shared_ptr<const T> item_cast(const std::shared_ptr<const Item>& item)
{
    return (item && item->type == T::ClassType) ? std::static_pointer_cast<const T>(item) : nullptr;
}

#endif
