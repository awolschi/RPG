#ifndef JOB_HPP
#define JOB_HPP

#include <string>
#include <memory>
#include "../Items/Resources/Resources.hpp"

class Inventory;

enum class JobType
{
    Mining,
    Lumberjacking,
    Fishing,
    Smithing
};

class Job
{
public:
    Job(JobType type);

    JobType type;
    int level;
    int experience;

    void GainXP(int xp);
    std::shared_ptr<Resource> CollectResource();
    static int RequiredXP(int level);
    std::string GetJobName() const;
};

#endif
