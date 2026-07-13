#ifndef JOBSYSTEM_HPP
#define JOBSYSTEM_HPP

#include <vector>
#include <memory>
#include <string>
#include "Job.hpp"

class Inventory;

class JobSystem
{
public:
    JobSystem();

    void AddJob(JobType type);
    Job& GetJob(JobType type);
    void DisplayAllJobs() const;
    std::string WorkJob(JobType type, int hours, Inventory& inventory);
    std::vector<Job>& GetJobs() { return jobs; }

private:
    std::vector<Job> jobs;
    Job* FindJob(JobType type);
};

#endif
