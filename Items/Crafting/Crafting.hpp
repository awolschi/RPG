#ifndef CRAFTING_HPP
#define CRAFTING_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Item.hpp"
#include "../Resources/Resources.hpp"
#include "../../Inventory/Inventory.hpp"

struct CraftingRequirement
{
    std::string resourceName;
    int quantity;
};

struct CraftingRecipe
{
    std::string name;
    std::string description;
    std::vector<CraftingRequirement> requirements;
    std::function<std::shared_ptr<Item>()> createItem;
};

class CraftingManager
{
public:
    CraftingManager();

    const std::vector<CraftingRecipe>& GetRecipes() const { return recipes; }
    int CountResources(const std::string& resourceName, const Inventory& inventory) const;
    bool CanCraft(size_t recipeIndex, const Inventory& inventory) const;
    std::shared_ptr<Item> Craft(size_t recipeIndex, Inventory& inventory);

private:
    std::vector<CraftingRecipe> recipes;
    void InitializeRecipes();
};

#endif
