#ifndef CRAFTING_HPP
#define CRAFTING_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Item.hpp"
#include "../Resources/Resources.hpp"
#include "../../Inventory/Inventory.hpp"
#include "../../Jobs/Job.hpp"

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
    JobType requiredJob = JobType::Mining; // default
    int requiredJobLevel = 0; // 0 = no requirement
};

class CraftingManager
{
public:
    CraftingManager();

    const std::vector<CraftingRecipe>& GetRecipes() const { return recipes; }
    int CountResources(const std::string& resourceName, const Inventory& inventory) const;
    bool CanCraft(size_t recipeIndex, const Inventory& inventory, int jobLevel = 0) const;
    bool IsLocked(size_t recipeIndex, int jobLevel) const;
    std::shared_ptr<Item> Craft(size_t recipeIndex, Inventory& inventory);

private:
    std::vector<CraftingRecipe> recipes;
    void InitializeRecipes();
};

#endif
