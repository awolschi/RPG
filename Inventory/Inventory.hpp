#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <memory>
#include "../Items/Item.hpp"

class Inventory
{
public:
    Inventory(int maxSlots = 20);

    bool AddItem(std::shared_ptr<Item> item);
    bool RemoveItem(size_t index);
    std::shared_ptr<Item> GetItem(size_t index);
    size_t GetItemCount() const { return items.size(); }
    int GetMaxSlots() const { return maxSlots; }
    int GetGold() const { return gold; }
    void AddGold(int amount) { gold += amount; }
    bool RemoveGold(int amount);

private:
    std::vector<std::shared_ptr<Item>> items;
    int maxSlots;
    int gold;
};

#endif
