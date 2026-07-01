#ifndef LOOT_HPP
#define LOOT_HPP

#include <memory>
#include <vector>
#include "Item.hpp"

class LootTable
{
public:
    static std::vector<std::shared_ptr<Item>> GenerateLoot(int difficulty, int roll = -1);

private:
    static std::shared_ptr<Item> CreateCommonWeapon(int difficulty);
    static std::shared_ptr<Item> CreateCommonArmor(int difficulty);
    static std::shared_ptr<Item> CreateRareWeapon(int difficulty);
    static std::shared_ptr<Item> CreateRareArmor(int difficulty);
};

#endif
