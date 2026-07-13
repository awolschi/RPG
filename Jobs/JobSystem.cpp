#include "JobSystem.hpp"
#include "../Inventory/Inventory.hpp"
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
        int required = Job::RequiredXP(jobs[i].level);
        std::cout << i + 1 << ". ";
        std::cout << jobs[i].GetJobName() << " - Level " << jobs[i].level
                  << ", XP: " << jobs[i].experience << "/" << required << "\n";
    }
}

std::string JobSystem::WorkJob(JobType type, int hours, Inventory& inventory)
{
    Job* job = FindJob(type);
    if (!job)
        return "Job not found!";

    std::string result = "You worked as a " + job->GetJobName() + " for " + std::to_string(hours) + " hours.\n";

    int collected = 0;
    int levelsGained = 0;
    int startLevel = job->level;

    for (int i = 0; i < hours; ++i)
    {
        auto resource = job->CollectResource();
        if (resource && inventory.AddItem(resource))
        {
            collected++;
        }
    }

    result += "Resources collected: " + std::to_string(collected) + "\n";

    if (job->level > startLevel)
    {
        levelsGained = job->level - startLevel;
        result += "Job leveled up " + std::to_string(levelsGained) + " time(s)! Now level " + std::to_string(job->level) + ".\n";
    }

    return result;
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
