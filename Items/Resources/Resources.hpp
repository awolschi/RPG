#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <memory>
#include "../Item.hpp"

class Resource : public Item
{
public:
    Resource(const std::string& name, int tier, int sellValue = 5);

    std::string GetDescription() const override;

    int tier;
};

// Factory functions for job-specific resources
namespace ResourceFactory
{
    std::shared_ptr<Resource> CreateMiningResource(int jobLevel);
    std::shared_ptr<Resource> CreateLumberjackingResource(int jobLevel);
    std::shared_ptr<Resource> CreateFishingResource(int jobLevel);
    std::shared_ptr<Resource> CreateSmithingResource(int jobLevel);
}

#endif
