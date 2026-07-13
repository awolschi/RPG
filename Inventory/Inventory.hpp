#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <memory>
#include "../Items/Item.hpp"

class Inventory
{
public:
    Inventory();

    bool AddItem(std::shared_ptr<Item> item);
    bool RemoveItem(size_t index);
    bool RemoveOneItem(size_t index);
    std::shared_ptr<Item> GetItem(size_t index) const;
    int GetStackCount(size_t index) const;
    int GetItemIndex(std::shared_ptr<Item> item) const;
    size_t GetItemCount() const { return items.size(); }
    int GetGold() const { return gold; }
    void AddGold(int amount) { gold += amount; }
    bool RemoveGold(int amount);

private:
    std::vector<std::shared_ptr<Item>> items;
    int gold;
};

#endif
