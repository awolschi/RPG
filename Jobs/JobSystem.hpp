#ifndef JOBSYSTEM_HPP
#define JOBSYSTEM_HPP

#include <vector>
#include <memory>
#include "Job.hpp"

class JobSystem
{
public:
    JobSystem();

    void AddJob(JobType type);
    Job& GetJob(JobType type);
    void DisplayAllJobs() const;
    void WorkJob(JobType type, int hours = 1);
    std::vector<Job>& GetJobs() { return jobs; }

private:
    std::vector<Job> jobs;
    Job* FindJob(JobType type);
};

#endif
