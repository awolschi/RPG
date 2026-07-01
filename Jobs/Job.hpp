#ifndef JOB_HPP
#define JOB_HPP

#include <string>

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
    int resourcesCollected;

    void GainXP(int xp);
    void CollectResource();
    std::string GetJobName() const;

    static const int XP_TO_LEVEL;
};

#endif
