#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

enum class ResourceType
{
    Ore,
    Wood,
    Fish,
    Herbs
};

class Resource
{
public:
    Resource(ResourceType type, int quantity = 1);

    ResourceType type;
    int quantity;
    std::string GetResourceName() const;
};

#endif
