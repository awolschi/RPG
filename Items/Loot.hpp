#ifndef LOOT_HPP
#define LOOT_HPP

#include <memory>
#include <vector>
#include "Item.hpp"
#include "Consumable.hpp"
#include "../Characters/Classes/Classes.hpp"

class LootTable
{
public:
    // Normal enemy loot — dropTier caps the maximum rarity
    static std::vector<std::shared_ptr<Item>> GenerateLoot(int difficulty, int dropTier = 10, int roll = -1, CharacterClass cc = CharacterClass::Warrior);

    // Boss loot — dropTier caps the maximum rarity
    static std::vector<std::shared_ptr<Item>> GenerateBossLoot(int difficulty, int dropTier = 10, int roll = -1, CharacterClass cc = CharacterClass::Warrior);

    // Legendary class-specific gear (from Primordial One)
    static std::vector<std::shared_ptr<Item>> GenerateLegendaryClassLoot(CharacterClass charClass);

    // Roll for a unique (Epic/Legendary) drop from the UniqueItemRegistry
    static std::shared_ptr<Item> GenerateUniqueDrop(
        const std::string& enemyName, int difficulty, bool isBoss, CharacterClass cc);

    // Individual item generators (public for shop)
    static std::shared_ptr<Item> CreateCommonWeapon(int difficulty);
    static std::shared_ptr<Item> CreateCommonArmor(int difficulty);
    static std::shared_ptr<Item> CreateRareWeapon(int difficulty);
    static std::shared_ptr<Item> CreateRareArmor(int difficulty);
    static std::shared_ptr<Item> CreateBossWeapon(int difficulty);
    static std::shared_ptr<Item> CreateBossArmor(int difficulty);
    static std::shared_ptr<Item> CreateEpicWeapon(int difficulty);
    static std::shared_ptr<Item> CreateEpicArmor(int difficulty);
    static std::shared_ptr<Item> CreateLegendaryWeapon(int difficulty);
    static std::shared_ptr<Item> CreateLegendaryArmor(int difficulty);
    static std::shared_ptr<Item> CreateAccessory(int difficulty);
    static std::shared_ptr<Item> CreateLegendaryWeapon(CharacterClass charClass);
    static std::shared_ptr<Item> CreateLegendaryChest(CharacterClass charClass);
    static std::shared_ptr<Item> CreateLegendaryAccessory(CharacterClass charClass);
    static std::shared_ptr<Item> CreatePotion(int difficulty);
};

#endif
