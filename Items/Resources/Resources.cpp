#include "Resources.hpp"

Resource::Resource(ResourceType type, int quantity) : type(type), quantity(quantity)
{
}

std::string Resource::GetResourceName() const
{
    switch (type)
    {
        case ResourceType::Ore:
            return "Ore";
        case ResourceType::Wood:
            return "Wood";
        case ResourceType::Fish:
            return "Fish";
        case ResourceType::Herbs:
            return "Herbs";
        default:
            return "Unknown";
    }
}