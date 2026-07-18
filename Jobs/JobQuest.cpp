#include "JobQuest.hpp"
#include <cstdlib>
#include <sstream>
#include <algorithm>

JobQuestSystem::JobQuestSystem()
{
}

void JobQuestSystem::GenerateDailyQuests(const std::vector<Job>& jobs)
{
    // Clear old daily quests
    quests.erase(std::remove_if(quests.begin(), quests.end(),
        [](const JobQuest& q) { return q.isDaily && q.status != JobQuestStatus::Active; }),
        quests.end());

    // Generate 2-3 daily quests per job
    for (const auto& job : jobs)
    {
        int questCount = 2 + (rand() % 2); // 2-3 quests per job

        for (int i = 0; i < questCount; ++i)
        {
            JobQuest quest;
            quest.id = GenerateQuestId();
            quest.isDaily = true;
            quest.isMastery = false;
            quest.jobType = job.type;
            quest.status = JobQuestStatus::Available;
            quest.currentCount = 0;
            quest.rewarded = false;
            quest.requiredJobLevel = 1;

            // Random quest type
            int typeRoll = rand() % 100;
            if (typeRoll < 40)
            {
                quest.questType = JobQuestType::Collect;
                quest.targetName = job.GetJobName() + " resources";
                quest.targetCount = GetRandomTarget(JobQuestType::Collect, job.level);
                quest.title = "Collect " + std::to_string(quest.targetCount) + " " + quest.targetName;
                quest.description = "Gather resources through " + job.GetJobName() + ".";
            }
            else if (typeRoll < 60)
            {
                quest.questType = JobQuestType::WorkHours;
                quest.targetName = job.GetJobName() + " hours";
                quest.targetCount = 1 + (rand() % 3); // 1-3 hours
                quest.title = "Work " + std::to_string(quest.targetCount) + " " + job.GetJobName() + " hours";
                quest.description = "Spend time working as a " + job.GetJobName() + ".";
            }
            else if (typeRoll < 80)
            {
                quest.questType = JobQuestType::FindRare;
                quest.targetName = "Rare " + job.GetJobName() + " finds";
                quest.targetCount = 1 + (rand() % 2); // 1-2 rare finds
                quest.title = "Find " + std::to_string(quest.targetCount) + " rare resources";
                quest.description = "Discover rare materials while " + job.GetJobName() + ".";
            }
            else
            {
                quest.questType = JobQuestType::QualityFind;
                quest.targetName = "Quality " + job.GetJobName() + " items";
                quest.targetCount = 1 + (rand() % 3); // 1-3 quality items
                quest.title = "Find " + std::to_string(quest.targetCount) + " quality items";
                quest.description = "Find Fine or Masterwork quality items.";
            }

            quest.rewardXP = GetQuestReward(quest.questType, quest.targetCount);
            quest.rewardGold = quest.rewardXP * 2;
            quest.rewardJobPoints = 0;

            AddQuest(quest);
        }
    }
}

void JobQuestSystem::CheckMasteryChallenges(const std::vector<Job>& jobs)
{
    for (const auto& job : jobs)
    {
        int rank = job.GetMasteryRank();

        // Check for mastery challenges at each rank
        std::string challengeId = "mastery_" + std::to_string(static_cast<int>(job.type)) + "_" + std::to_string(rank);

        // Skip if already exists
        bool exists = false;
        for (const auto& q : quests)
        {
            if (q.id == challengeId)
            {
                exists = true;
                break;
            }
        }
        if (exists) continue;

        // Create mastery challenge
        JobQuest quest;
        quest.id = challengeId;
        quest.isDaily = false;
        quest.isMastery = true;
        quest.jobType = job.type;
        quest.status = JobQuestStatus::Available;
        quest.currentCount = 0;
        quest.rewarded = false;
        quest.requiredJobLevel = 1;

        switch (rank)
        {
            case 1: // Apprentice (Level 10)
                quest.questType = JobQuestType::Collect;
                quest.targetName = job.GetJobName() + " resources";
                quest.targetCount = 50;
                quest.title = "Apprentice Challenge: Collect 50 resources";
                quest.description = "Prove your dedication by collecting 50 resources.";
                quest.rewardXP = 200;
                quest.rewardGold = 400;
                quest.rewardJobPoints = 1;
                break;

            case 2: // Journeyman (Level 25)
                quest.questType = JobQuestType::FindRare;
                quest.targetName = "Rare resources";
                quest.targetCount = 5;
                quest.title = "Journeyman Challenge: Find 5 rare resources";
                quest.description = "Show your expertise by finding rare materials.";
                quest.rewardXP = 500;
                quest.rewardGold = 1000;
                quest.rewardJobPoints = 2;
                break;

            case 3: // Expert (Level 50)
                quest.questType = JobQuestType::QualityFind;
                quest.targetName = "Masterwork items";
                quest.targetCount = 3;
                quest.title = "Expert Challenge: Find 3 Masterwork items";
                quest.description = "Demonstrate mastery by finding Masterwork quality items.";
                quest.rewardXP = 1000;
                quest.rewardGold = 2000;
                quest.rewardJobPoints = 3;
                break;

            case 4: // Master (Level 75)
                quest.questType = JobQuestType::WorkHours;
                quest.targetName = "Work hours";
                quest.targetCount = 10;
                quest.title = "Master Challenge: Work 10 hours";
                quest.description = "Commit to your craft with 10 hours of work.";
                quest.rewardXP = 2000;
                quest.rewardGold = 4000;
                quest.rewardJobPoints = 5;
                break;

            case 5: // Grandmaster (Level 100)
                quest.questType = JobQuestType::Collect;
                quest.targetName = job.GetJobName() + " resources";
                quest.targetCount = 200;
                quest.title = "Grandmaster Challenge: Collect 200 resources";
                quest.description = "Achieve the ultimate mastery by collecting 200 resources.";
                quest.rewardXP = 5000;
                quest.rewardGold = 10000;
                quest.rewardJobPoints = 10;
                break;

            default:
                continue;
        }

        AddQuest(quest);
    }
}

std::vector<JobQuest> JobQuestSystem::GetDailyQuests() const
{
    std::vector<JobQuest> result;
    for (const auto& q : quests)
    {
        if (q.isDaily)
            result.push_back(q);
    }
    return result;
}

bool JobQuestSystem::HasDailyQuests() const
{
    for (const auto& q : quests)
    {
        if (q.isDaily && q.status == JobQuestStatus::Available)
            return true;
    }
    return false;
}

std::vector<JobQuest> JobQuestSystem::GetMasteryChallenges() const
{
    std::vector<JobQuest> result;
    for (const auto& q : quests)
    {
        if (q.isMastery)
            result.push_back(q);
    }
    return result;
}

void JobQuestSystem::AcceptQuest(const std::string& questId)
{
    JobQuest* quest = FindQuest(questId);
    if (quest && quest->status == JobQuestStatus::Available)
    {
        quest->status = JobQuestStatus::Active;
    }
}

void JobQuestSystem::UpdateProgress(JobQuestType type, const std::string& targetName, int amount)
{
    for (auto& quest : quests)
    {
        if (quest.status != JobQuestStatus::Active) continue;
        if (quest.questType != type) continue;

        // Check if target matches
        bool targetMatch = false;
        switch (type)
        {
            case JobQuestType::Collect:
            case JobQuestType::FindRare:
            case JobQuestType::QualityFind:
                targetMatch = true; // Any resource matches
                break;
            case JobQuestType::WorkHours:
                targetMatch = true; // Any job hours match
                break;
            case JobQuestType::Craft:
            case JobQuestType::ChainCraft:
                targetMatch = true;
                break;
            case JobQuestType::Mastery:
                targetMatch = true;
                break;
        }

        if (targetMatch)
        {
            quest.currentCount += amount;
            if (quest.currentCount >= quest.targetCount)
            {
                quest.currentCount = quest.targetCount;
                quest.status = JobQuestStatus::Completed;
            }
        }
    }
}

void JobQuestSystem::CompleteQuest(const std::string& questId)
{
    JobQuest* quest = FindQuest(questId);
    if (quest && quest->status == JobQuestStatus::Completed && !quest->rewarded)
    {
        quest->rewarded = true;
        totalCompleted++;
    }
}

bool JobQuestSystem::IsQuestComplete(const std::string& questId) const
{
    const JobQuest* quest = nullptr;
    for (const auto& q : quests)
    {
        if (q.id == questId)
        {
            quest = &q;
            break;
        }
    }
    return quest && quest->status == JobQuestStatus::Completed;
}

std::vector<JobQuest> JobQuestSystem::GetActiveQuests() const
{
    std::vector<JobQuest> result;
    for (const auto& q : quests)
    {
        if (q.status == JobQuestStatus::Active)
            result.push_back(q);
    }
    return result;
}

std::vector<JobQuest> JobQuestSystem::GetAvailableQuests() const
{
    std::vector<JobQuest> result;
    for (const auto& q : quests)
    {
        if (q.status == JobQuestStatus::Available)
            result.push_back(q);
    }
    return result;
}

std::vector<JobQuest> JobQuestSystem::GetCompletedQuests() const
{
    std::vector<JobQuest> result;
    for (const auto& q : quests)
    {
        if (q.status == JobQuestStatus::Completed)
            result.push_back(q);
    }
    return result;
}

JobQuest* JobQuestSystem::GetQuest(const std::string& questId)
{
    return FindQuest(questId);
}

int JobQuestSystem::GetCompletedQuestCount() const
{
    int count = 0;
    for (const auto& q : quests)
    {
        if (q.status == JobQuestStatus::Completed)
            count++;
    }
    return count;
}

std::string JobQuestSystem::Serialize() const
{
    std::ostringstream ss;
    ss << quests.size() << " " << totalCompleted;
    for (const auto& q : quests)
    {
        ss << "|" << q.id << "," << q.title << "," << static_cast<int>(q.jobType)
           << "," << static_cast<int>(q.questType) << "," << static_cast<int>(q.status)
           << "," << q.currentCount << "," << q.targetCount
           << "," << q.rewardXP << "," << q.rewardGold << "," << q.rewardJobPoints
           << "," << (q.rewarded ? "1" : "0") << "," << (q.isDaily ? "1" : "0")
           << "," << (q.isMastery ? "1" : "0");
    }
    return ss.str();
}

void JobQuestSystem::Deserialize(const std::string& data)
{
    quests.clear();
    if (data.empty()) return;

    std::istringstream ss(data);
    size_t count;
    int total;
    ss >> count >> total;
    totalCompleted = total;

    for (size_t i = 0; i < count; ++i)
    {
        std::string token;
        ss >> token;
        if (token.empty() || token[0] != '|') continue;
        token = token.substr(1);

        JobQuest quest;
        std::istringstream qs(token);
        std::string segment;

        std::getline(qs, quest.id, ',');
        std::getline(qs, quest.title, ',');

        std::getline(qs, segment, ',');
        quest.jobType = static_cast<JobType>(std::stoi(segment));

        std::getline(qs, segment, ',');
        quest.questType = static_cast<JobQuestType>(std::stoi(segment));

        std::getline(qs, segment, ',');
        quest.status = static_cast<JobQuestStatus>(std::stoi(segment));

        std::getline(qs, segment, ',');
        quest.currentCount = std::stoi(segment);

        std::getline(qs, segment, ',');
        quest.targetCount = std::stoi(segment);

        std::getline(qs, segment, ',');
        quest.rewardXP = std::stoi(segment);

        std::getline(qs, segment, ',');
        quest.rewardGold = std::stoi(segment);

        std::getline(qs, segment, ',');
        quest.rewardJobPoints = std::stoi(segment);

        std::getline(qs, segment, ',');
        quest.rewarded = (segment == "1");

        std::getline(qs, segment, ',');
        quest.isDaily = (segment == "1");

        std::getline(qs, segment, ',');
        quest.isMastery = (segment == "1");

        quest.requiredJobLevel = 1;
        quest.targetName = "";
        quest.description = "";

        quests.push_back(quest);
    }
}

void JobQuestSystem::AddQuest(const JobQuest& quest)
{
    quests.push_back(quest);
}

JobQuest* JobQuestSystem::FindQuest(const std::string& id)
{
    for (auto& q : quests)
    {
        if (q.id == id)
            return &q;
    }
    return nullptr;
}

std::string JobQuestSystem::GenerateQuestId()
{
    static int counter = 0;
    return "jq_" + std::to_string(counter++);
}

int JobQuestSystem::GetRandomTarget(JobQuestType type, int jobLevel) const
{
    int base = 0;
    int range = 0;

    switch (type)
    {
        case JobQuestType::Collect:
            base = 5 + jobLevel / 5;
            range = 5 + jobLevel / 10;
            break;
        case JobQuestType::WorkHours:
            base = 1;
            range = 2 + jobLevel / 25;
            break;
        case JobQuestType::FindRare:
            base = 1;
            range = 1 + jobLevel / 30;
            break;
        case JobQuestType::QualityFind:
            base = 1;
            range = 1 + jobLevel / 20;
            break;
        default:
            base = 5;
            range = 5;
            break;
    }

    return base + (rand() % range);
}

int JobQuestSystem::GetQuestReward(JobQuestType type, int targetCount) const
{
    int base = 0;

    switch (type)
    {
        case JobQuestType::Collect:
            base = 10 + targetCount * 2;
            break;
        case JobQuestType::WorkHours:
            base = 15 + targetCount * 5;
            break;
        case JobQuestType::FindRare:
            base = 25 + targetCount * 10;
            break;
        case JobQuestType::QualityFind:
            base = 20 + targetCount * 8;
            break;
        default:
            base = 15;
            break;
    }

    return base;
}
