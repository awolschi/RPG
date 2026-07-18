#include "ResourceChain.hpp"
#include "../Inventory/Inventory.hpp"
#include "../Items/Resources/Resources.hpp"

ResourceChainSystem::ResourceChainSystem()
{
    InitializeRecipes();
}

void ResourceChainSystem::InitializeRecipes()
{
    recipes.clear();

    // Tier 1 chains (Level 1+)
    recipes.push_back({"Copper Ingot", "Smelt copper ore into an ingot",
                       "Copper Ore", 2, "", 0, "Copper Ingot", 1, 15, 1, 10});
    recipes.push_back({"Sturdy Plank", "Process wood into a sturdy plank",
                       "Wood", 2, "", 0, "Sturdy Plank", 1, 12, 1, 8});
    recipes.push_back({"Cooked Fish", "Cook a raw fish over fire",
                       "Raw Fish", 1, "", 0, "Cooked Fish", 1, 10, 1, 5});

    // Tier 2 chains (Level 5+)
    recipes.push_back({"Iron Ingot", "Smelt iron ore into a sturdy ingot",
                       "Iron Ore", 2, "", 0, "Iron Ingot", 2, 25, 5, 15});
    recipes.push_back({"Oak Plank", "Process oak wood into a fine plank",
                       "Oak Wood", 2, "", 0, "Oak Plank", 2, 20, 5, 12});
    recipes.push_back({"Cooked Salmon", "Cook a salmon to perfection",
                       "Salmon", 1, "", 0, "Cooked Salmon", 2, 18, 5, 8});

    // Tier 3 chains (Level 10+)
    recipes.push_back({"Silver Ingot", "Smelt silver ore into a gleaming ingot",
                       "Silver Ore", 2, "", 0, "Silver Ingot", 3, 40, 10, 20});
    recipes.push_back({"Maple Plank", "Process maple into a refined plank",
                       "Maple Wood", 2, "", 0, "Maple Plank", 3, 32, 10, 16});
    recipes.push_back({"Cooked Tuna", "Cook a tuna for a hearty meal",
                       "Tuna", 1, "", 0, "Cooked Tuna", 3, 28, 10, 12});

    // Tier 4 chains (Level 20+)
    recipes.push_back({"Gold Ingot", "Smelt gold ore into a precious ingot",
                       "Gold Ore", 2, "", 0, "Gold Ingot", 4, 60, 20, 25});
    recipes.push_back({"Ebony Plank", "Process ebony into a dark plank",
                       "Ebony Wood", 2, "", 0, "Ebony Plank", 4, 48, 20, 20});
    recipes.push_back({"Cooked Exotic Fish", "Cook an exotic fish with care",
                       "Exotic Fish", 1, "", 0, "Cooked Exotic Fish", 4, 42, 20, 15});

    // Tier 5 chains (Level 30+)
    recipes.push_back({"Mythril Ingot", "Smelt mythril ore into a legendary ingot",
                       "Mythril Ore", 2, "", 0, "Mythril Ingot", 5, 85, 30, 30});
    recipes.push_back({"Magical Plank", "Process magical wood into an enchanted plank",
                       "Magical Wood", 2, "", 0, "Magical Plank", 5, 68, 30, 25});
    recipes.push_back({"Cooked Legendary Fish", "Cook a legendary fish",
                       "Legendary Fish", 1, "", 0, "Cooked Legendary Fish", 5, 60, 30, 20});

    // Tier 6 chains (Level 40+)
    recipes.push_back({"Platinum Ingot", "Smelt platinum into a refined ingot",
                       "Platinum Ore", 2, "", 0, "Platinum Ingot", 6, 110, 40, 35});
    recipes.push_back({"Enchanted Plank", "Process enchanted oak into a magical plank",
                       "Enchanted Oak", 2, "", 0, "Enchanted Plank", 6, 88, 40, 30});
    recipes.push_back({"Cooked Phoenix Salmon", "Cook a phoenix salmon",
                       "Phoenix Salmon", 1, "", 0, "Cooked Phoenix Salmon", 6, 78, 40, 25});

    // Tier 7 chains (Level 50+)
    recipes.push_back({"Titanium Ingot", "Smelt titanium into a powerful ingot",
                       "Titanium Ore", 2, "", 0, "Titanium Ingot", 7, 140, 50, 40});
    recipes.push_back({"Starwood Plank", "Process starwood into a radiant plank",
                       "Starwood", 2, "", 0, "Starwood Plank", 7, 112, 50, 35});
    recipes.push_back({"Cooked Crystal Koi", "Cook a crystal koi",
                       "Crystal Koi", 1, "", 0, "Cooked Crystal Koi", 7, 100, 50, 30});

    // Tier 8 chains (Level 60+)
    recipes.push_back({"Adamantite Ingot", "Smelt adamantite into a mighty ingot",
                       "Adamantite Ore", 2, "", 0, "Adamantite Ingot", 8, 175, 60, 45});
    recipes.push_back({"Ironbark Plank", "Process ironbark into an unbreakable plank",
                       "Ironbark", 2, "", 0, "Ironbark Plank", 8, 140, 60, 40});
    recipes.push_back({"Cooked Thunderfish", "Cook a thunderfish",
                       "Thunderfish", 1, "", 0, "Cooked Thunderfish", 8, 125, 60, 35});

    // Tier 9 chains (Level 75+)
    recipes.push_back({"Voidsteel Ingot", "Smelt voidsteel into a dark ingot",
                       "Void Ore", 2, "", 0, "Voidsteel Ingot", 9, 215, 75, 50});
    recipes.push_back({"Ancient Plank", "Process ancient heartwood into an elder plank",
                       "Ancient Heartwood", 2, "", 0, "Ancient Plank", 9, 172, 75, 45});
    recipes.push_back({"Cooked Dragon Eel", "Cook a dragon eel",
                       "Dragon Eel", 1, "", 0, "Cooked Dragon Eel", 9, 155, 75, 40});

    // Tier 10 chains (Level 90+)
    recipes.push_back({"Aetherium Ingot", "Smelt aetherium into a divine ingot",
                       "Aetherium Ore", 2, "", 0, "Aetherium Ingot", 10, 260, 90, 60});
    recipes.push_back({"Celestial Plank", "Process celestial wood into a divine plank",
                       "Celestial Wood", 2, "", 0, "Celestial Plank", 10, 208, 90, 55});
    recipes.push_back({"Cooked Leviathan", "Cook a leviathan",
                       "Leviathan", 1, "", 0, "Cooked Leviathan", 10, 190, 90, 50});

    // Special combination chains (Level 15+)
    recipes.push_back({"Steel Ingot", "Combine copper and iron for steel",
                       "Copper Ore", 1, "Iron Ore", 1, "Steel Ingot", 2, 30, 15, 18});
    recipes.push_back({"Reinforced Plank", "Combine wood and oak for a reinforced plank",
                       "Wood", 1, "Oak Wood", 1, "Reinforced Plank", 2, 24, 15, 14});

    // High-tier combination chains (Level 35+)
    recipes.push_back({"Alloy Ingot", "Combine silver and gold for a precious alloy",
                       "Silver Ore", 1, "Gold Ore", 1, "Alloy Ingot", 4, 55, 35, 28});
    recipes.push_back({"Enchanted Plank", "Combine maple and ebony for an enchanted plank",
                       "Maple Wood", 1, "Ebony Wood", 1, "Enchanted Plank", 4, 44, 35, 22});
}

std::vector<ChainRecipe> ResourceChainSystem::GetAvailableRecipes(int jobLevel) const
{
    std::vector<ChainRecipe> available;
    for (const auto& recipe : recipes)
    {
        if (jobLevel >= recipe.requiredJobLevel)
            available.push_back(recipe);
    }
    return available;
}

bool ResourceChainSystem::CanCraft(const ChainRecipe& recipe, const Inventory& inventory, int jobLevel) const
{
    if (jobLevel < recipe.requiredJobLevel) return false;

    // Check ingredient 1
    if (!recipe.ingredient1.empty())
    {
        bool found = false;
        for (size_t i = 0; i < inventory.GetItemCount(); ++i)
        {
            auto item = inventory.GetItem(i);
            if (item && item->name == recipe.ingredient1 && inventory.GetStackCount(i) >= recipe.ingredient1Count)
            {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    // Check ingredient 2
    if (!recipe.ingredient2.empty())
    {
        bool found = false;
        for (size_t i = 0; i < inventory.GetItemCount(); ++i)
        {
            auto item = inventory.GetItem(i);
            if (item && item->name == recipe.ingredient2 && inventory.GetStackCount(i) >= recipe.ingredient2Count)
            {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}

std::shared_ptr<Item> ResourceChainSystem::Craft(const ChainRecipe& recipe, Inventory& inventory, int jobLevel)
{
    if (!CanCraft(recipe, inventory, jobLevel))
        return nullptr;

    // Remove ingredients
    if (!recipe.ingredient1.empty())
    {
        for (size_t i = 0; i < inventory.GetItemCount(); ++i)
        {
            auto item = inventory.GetItem(i);
            if (item && item->name == recipe.ingredient1)
            {
                for (int c = 0; c < recipe.ingredient1Count; ++c)
                    inventory.RemoveOneItem(i);
                break;
            }
        }
    }

    if (!recipe.ingredient2.empty())
    {
        for (size_t i = 0; i < inventory.GetItemCount(); ++i)
        {
            auto item = inventory.GetItem(i);
            if (item && item->name == recipe.ingredient2)
            {
                for (int c = 0; c < recipe.ingredient2Count; ++c)
                    inventory.RemoveOneItem(i);
                break;
            }
        }
    }

    // Create result
    auto result = std::make_shared<Resource>(recipe.resultName, recipe.resultTier, recipe.resultValue);
    inventory.AddItem(result);

    return result;
}
