#include "Job.hpp"

const int Job::XP_TO_LEVEL = 100;

Job::Job(JobType type) : type(type), level(1), experience(0), resourcesCollected(0)
{
}

void Job::GainXP(int xp)
{
    experience += xp;
    if (experience >= XP_TO_LEVEL)
    {
        level++;
        experience = 0;
    }
}

void Job::CollectResource()
{
    resourcesCollected++;
    GainXP(10);
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