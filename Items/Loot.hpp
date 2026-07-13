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
    static std::vector<std::shared_ptr<Item>> GenerateLoot(int difficulty, int dropTier = 10, int roll = -1);

    // Boss loot — dropTier caps the maximum rarity
    static std::vector<std::shared_ptr<Item>> GenerateBossLoot(int difficulty, int dropTier = 10, int roll = -1);

    // Legendary class-specific gear (from Primordial One)
    static std::vector<std::shared_ptr<Item>> GenerateLegendaryClassLoot(CharacterClass charClass);

    // Individual item generators (public for shop)
    static std::shared_ptr<Item> CreateCommonWeapon(int difficulty);
    static std::shared_ptr<Item> CreateCommonArmor(int difficulty);
    static std::shared_ptr<Item> CreateRareWeapon(int difficulty);
    static std::shared_ptr<Item> CreateRareArmor(int difficulty);
    static std::shared_ptr<Item> CreateBossWeapon(int difficulty);
    static std::shared_ptr<Item> CreateBossArmor(int difficulty);
    static std::shared_ptr<Item> CreateAccessory(int difficulty);
    static std::shared_ptr<Item> CreateLegendaryWeapon(CharacterClass charClass);
    static std::shared_ptr<Item> CreateLegendaryChest(CharacterClass charClass);
    static std::shared_ptr<Item> CreateLegendaryAccessory(CharacterClass charClass);
    static std::shared_ptr<Item> CreatePotion(int difficulty);
};

#endif
