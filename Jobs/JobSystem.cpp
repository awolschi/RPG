#include "JobSystem.hpp"
#include <iostream>

JobSystem::JobSystem()
{
}

void JobSystem::AddJob(JobType type)
{
    if (!FindJob(type))
    {
        jobs.emplace_back(type);
    }
}

Job& JobSystem::GetJob(JobType type)
{
    Job* job = FindJob(type);
    if (job)
    {
        return *job;
    }
    throw std::runtime_error("Job not found");
}

void JobSystem::DisplayAllJobs() const
{
    std::cout << "\n=== YOUR JOBS ===" << std::endl;
    for (size_t i = 0; i < jobs.size(); ++i)
    {
        std::cout << i + 1 << ". ";
        std::cout << jobs[i].GetJobName() << " - Level " << jobs[i].level << ", XP: " << jobs[i].experience << "/100\n";
        std::cout << "   Resources Collected: " << jobs[i].resourcesCollected << std::endl;
    }
}

void JobSystem::WorkJob(JobType type, int hours)
{
    Job* job = FindJob(type);
    if (job)
    {
        for (int i = 0; i < hours; ++i)
        {
            job->CollectResource();
        }
        std::cout << "You worked for " << hours << " hours and collected resources!\n";
    }
}

Job* JobSystem::FindJob(JobType type)
{
    for (auto& job : jobs)
    {
        if (job.type == type)
        {
            return &job;
        }
    }
    return nullptr;
}
