#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <memory>
#include "../Item.hpp"

enum class ResourceQuality : int
{
    Normal    = 0,
    Fine      = 1,
    Masterwork = 2
};

inline const char* ResourceQualityName(ResourceQuality q)
{
    switch (q)
    {
        case ResourceQuality::Normal:    return "";
        case ResourceQuality::Fine:      return "Fine";
        case ResourceQuality::Masterwork: return "Masterwork";
    }
    return "";
}

inline float ResourceQualityMultiplier(ResourceQuality q)
{
    switch (q)
    {
        case ResourceQuality::Normal:    return 1.0f;
        case ResourceQuality::Fine:      return 1.5f;
        case ResourceQuality::Masterwork: return 3.0f;
    }
    return 1.0f;
}

class Resource : public Item
{
public:
    static constexpr ItemType ClassType = ItemType::Resource;
    Resource(const std::string& name, int tier, int sellValue = 5,
             int healAmount = 0, int manaAmount = 0,
             ResourceQuality quality = ResourceQuality::Normal);

    std::string GetDescription() const override;
    std::string GetFullName() const;

    int tier;
    int healAmount;
    int manaAmount;
    ResourceQuality quality;
};

// Factory functions for job-specific resources
namespace ResourceFactory
{
    std::shared_ptr<Resource> CreateMiningResource(int jobLevel);
    std::shared_ptr<Resource> CreateLumberjackingResource(int jobLevel);
    std::shared_ptr<Resource> CreateFishingResource(int jobLevel);
    std::shared_ptr<Resource> CreateCookedFish(int tier);
    std::shared_ptr<Resource> CreateSmithingResource(int jobLevel);
    std::shared_ptr<Resource> CreateSkinningResource(int jobLevel);
    std::shared_ptr<Resource> CreateArcaneResource(int jobLevel);

    // Quality rolling
    ResourceQuality RollQuality(int jobLevel);
}

#endif
