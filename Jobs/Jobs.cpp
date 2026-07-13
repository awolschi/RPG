#include "Job.hpp"

Job::Job(JobType type) : type(type), level(1), experience(0)
{
}

int Job::RequiredXP(int level)
{
    return 100 + (level - 1) * 50;
}

void Job::GainXP(int xp)
{
    experience += xp;
    int required = RequiredXP(level);
    while (experience >= required && level < 10)
    {
        experience -= required;
        level++;
        required = RequiredXP(level);
    }
}

std::shared_ptr<Resource> Job::CollectResource()
{
    std::shared_ptr<Resource> resource;

    switch (type)
    {
        case JobType::Mining:
            resource = ResourceFactory::CreateMiningResource(level);
            break;
        case JobType::Lumberjacking:
            resource = ResourceFactory::CreateLumberjackingResource(level);
            break;
        case JobType::Fishing:
            resource = ResourceFactory::CreateFishingResource(level);
            break;
        case JobType::Smithing:
            resource = ResourceFactory::CreateSmithingResource(level);
            break;
    }

    int xpGain = 10 + level * 2;
    GainXP(xpGain);

    return resource;
}

std::string Job::GetJobName() const
{
    switch (type)
    {
        case JobType::Mining:
            return "Mining";
        case JobType::Lumberjacking:
            return "Lumberjacking";
        case JobType::Fishing:
            return "Fishing";
        case JobType::Smithing:
            return "Smithing";
        default:
            return "Unknown";
    }
}
