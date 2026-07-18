#ifndef RESOURCE_CHAIN_HPP
#define RESOURCE_CHAIN_HPP

#include <string>
#include <vector>
#include <memory>

class Inventory;
class Item;

struct ChainRecipe
{
    std::string name;
    std::string description;
    std::string ingredient1;
    int ingredient1Count;
    std::string ingredient2;
    int ingredient2Count;
    std::string resultName;
    int resultTier;
    int resultValue;
    int requiredJobLevel;
    int bonusXP;
};

class ResourceChainSystem
{
public:
    ResourceChainSystem();

    void InitializeRecipes();
    const std::vector<ChainRecipe>& GetRecipes() const { return recipes; }
    std::vector<ChainRecipe> GetAvailableRecipes(int jobLevel) const;

    bool CanCraft(const ChainRecipe& recipe, const Inventory& inventory, int jobLevel) const;
    std::shared_ptr<Item> Craft(const ChainRecipe& recipe, Inventory& inventory, int jobLevel);

private:
    std::vector<ChainRecipe> recipes;
};

#endif
