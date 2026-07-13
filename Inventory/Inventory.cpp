#include "Inventory.hpp"

Inventory::Inventory() : gold(0)
{
}

bool Inventory::AddItem(std::shared_ptr<Item> item)
{
    if (!item) return false;

    for (auto& entry : items)
    {
        if (entry->IsSameAs(*item))
        {
            entry->count += item->count;
            return true;
        }
    }

    items.push_back(item);
    return true;
}

bool Inventory::RemoveItem(size_t index)
{
    if (index < items.size())
    {
        items.erase(items.begin() + index);
        return true;
    }
    return false;
}

bool Inventory::RemoveOneItem(size_t index)
{
    if (index < items.size())
    {
        auto& entry = items[index];
        if (entry->count > 1)
        {
            entry->count--;
            return true;
        }
        items.erase(items.begin() + index);
        return true;
    }
    return false;
}

std::shared_ptr<Item> Inventory::GetItem(size_t index) const
{
    if (index < items.size())
    {
        return items[index];
    }
    return nullptr;
}

int Inventory::GetStackCount(size_t index) const
{
    if (index < items.size())
        return items[index]->count;
    return 0;
}

int Inventory::GetItemIndex(std::shared_ptr<Item> item) const
{
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (items[i] == item)
            return static_cast<int>(i);
    }
    return -1;
}

bool Inventory::RemoveGold(int amount)
{
    if (amount <= gold)
    {
        gold -= amount;
        return true;
    }
    return false;
}
