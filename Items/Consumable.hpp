#ifndef CONSUMABLE_HPP
#define CONSUMABLE_HPP

#include "Item.hpp"
#include "../Characters/Character.hpp"

class Consumable : public Item
{
public:
    Consumable(const std::string& name, int healAmount, int manaAmount = 0, int rarity = 1)
        : Item(name, ItemType::Consumable, rarity), healAmount(healAmount), manaAmount(manaAmount)
    {
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<Consumable>(name, healAmount, manaAmount, rarity);
        c->count = 1;
        return c;
    }

    std::string GetDescription() const override
    {
        std::string desc;
        if (healAmount > 0) desc += "Restores " + std::to_string(healAmount) + " HP";
        if (manaAmount > 0)
        {
            if (!desc.empty()) desc += ", ";
            desc += std::to_string(manaAmount) + " MP";
        }
        return desc;
    }

    void Use(Character& user) const
    {
        if (healAmount > 0) user.RestoreHealth(healAmount);
        if (manaAmount > 0) user.RestoreMana(manaAmount);
    }

    int healAmount;
    int manaAmount;
};

#endif
