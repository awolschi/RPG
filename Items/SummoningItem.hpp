#ifndef SUMMONING_ITEM_HPP
#define SUMMONING_ITEM_HPP

#include "Item.hpp"
#include "../Characters/Monster.hpp"
#include <functional>
#include <string>
#include <memory>
#include <map>

class SummoningItem : public Item
{
public:
    SummoningItem(const std::string& name, const std::string& bossName,
                  std::function<std::shared_ptr<Monster>()> factory, int rarity = 5)
        : Item(name, ItemType::Summoning, rarity), bossName(bossName),
          bossFactory(std::move(factory))
    {
        sellValue = 500;
    }

    std::shared_ptr<Item> Clone() const override
    {
        auto c = std::make_shared<SummoningItem>(name, bossName, bossFactory, rarity);
        c->setId = setId;
        c->count = 1;
        c->requiredLevel = requiredLevel;
        return c;
    }

    std::string GetDescription() const override
    {
        return "Summon " + bossName + " for battle";
    }

    std::string bossName;
    std::function<std::shared_ptr<Monster>()> bossFactory;
};

namespace SummoningRegistry
{
    void Initialize();
    std::shared_ptr<SummoningItem> Create(const std::string& itemName);
    const std::map<std::string, std::function<std::shared_ptr<Monster>()>>& GetFactories();
}

#endif
