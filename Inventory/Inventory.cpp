#include "Inventory.hpp"

Inventory::Inventory(int maxSlots) : maxSlots(maxSlots), gold(0)
{
}

bool Inventory::AddItem(std::shared_ptr<Item> item)
{
    if (!item || items.size() >= static_cast<size_t>(maxSlots))
    {
        return false;
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

std::shared_ptr<Item> Inventory::GetItem(size_t index)
{
    if (index < items.size())
    {
        return items[index];
    }
    return nullptr;
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
