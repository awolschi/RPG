#ifndef UNIQUE_ITEMS_HPP
#define UNIQUE_ITEMS_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "../Item.hpp"

struct UniqueWeaponDef
{
    std::string name;
    Rarity rarity = Rarity::Epic;
    WeaponType weaponType = WeaponType::Sword;
    int baseDamage = 0;
    int damagePerDiff = 0;
    int manaCost = 0;
    ElementType element = ElementType::Physical;
    int baseElementDamage = 0;
    int elementDamagePerDiff = 0;
    std::string dropSource;
    int setId = -1;
    ItemPassive passive1 = ItemPassive::None;
    ItemPassive passive2 = ItemPassive::None;
};

struct UniqueArmorDef
{
    std::string name;
    Rarity rarity = Rarity::Epic;
    ArmorType armorType = ArmorType::Cloth;
    ArmorPiece piece = ArmorPiece::Helmet;
    int baseDefense = 0;
    int defensePerDiff = 0;
    std::map<ElementType, int> baseResist;
    int resistPerDiff = 0;
    std::string dropSource;
    int setId = -1;
    ItemPassive passive1 = ItemPassive::None;
    ItemPassive passive2 = ItemPassive::None;
};

struct UniqueAccessoryDef
{
    std::string name;
    Rarity rarity = Rarity::Epic;
    int baseHealth = 0;
    int healthPerDiff = 0;
    int baseMana = 0;
    int manaPerDiff = 0;
    ElementType element = ElementType::Physical;
    int baseElementDamage = 0;
    int elementDamagePerDiff = 0;
    std::string dropSource;
    int setId = -1;
    ItemPassive passive1 = ItemPassive::None;
    ItemPassive passive2 = ItemPassive::None;
};

class UniqueItemRegistry
{
public:
    static void Initialize();

    static const UniqueWeaponDef* FindWeapon(const std::string& name);
    static const UniqueArmorDef* FindArmor(const std::string& name);
    static const UniqueAccessoryDef* FindAccessory(const std::string& name);

    static std::shared_ptr<Item> Create(const std::string& name, int difficulty);

    static std::vector<const UniqueWeaponDef*> GetWeaponsByDropSource(const std::string& enemyName);
    static std::vector<const UniqueArmorDef*> GetArmorByDropSource(const std::string& enemyName);
    static std::vector<const UniqueAccessoryDef*> GetAccessoriesByDropSource(const std::string& enemyName);

    static std::vector<const UniqueWeaponDef*> GetWeaponsByRarity(Rarity r);
    static std::vector<const UniqueArmorDef*> GetArmorByRarity(Rarity r);
    static std::vector<const UniqueAccessoryDef*> GetAccessoriesByRarity(Rarity r);

    static const std::vector<UniqueWeaponDef>& GetAllWeapons() { return s_weapons; }
    static const std::vector<UniqueArmorDef>& GetAllArmor() { return s_armor; }
    static const std::vector<UniqueAccessoryDef>& GetAllAccessories() { return s_accessories; }

private:
    static std::vector<UniqueWeaponDef> s_weapons;
    static std::vector<UniqueArmorDef> s_armor;
    static std::vector<UniqueAccessoryDef> s_accessories;
    static bool s_initialized;

    static void RegisterMageEpics();
    static void RegisterBossLegendaries();
    static void RegisterSetItems();
    static void RegisterMerchantUniques();
    static void RegisterMerchantEpics();
    static void RegisterWarriorLegendaries();
    static void RegisterWarriorEpics();
    static void RegisterArcherLegendaries();
    static void RegisterArcherEpics();
    static void RegisterPriestLegendaries();
    static void RegisterPriestEpics();
    static void RegisterAchievementItems();
};

#endif
