#ifndef JOB_QUEST_HPP
#define JOB_QUEST_HPP

#include <string>
#include <vector>
#include <map>
#include "Job.hpp"

enum class JobQuestType
{
    Collect,        // Collect X resources
    Craft,          // Craft X items
    WorkHours,      // Work X hours
    FindRare,       // Find X rare resources
    QualityFind,    // Find X quality items
    ChainCraft,     // Chain craft X times
    Mastery         // Special mastery challenge
};

enum class JobQuestStatus
{
    Available,
    Active,
    Completed,
    Failed
};

struct JobQuest
{
    std::string id;
    std::string title;
    std::string description;
    JobType jobType;
    JobQuestType questType;
    JobQuestStatus status;
    std::string targetName;
    int targetCount;
    int currentCount;
    int rewardXP;
    int rewardGold;
    int rewardJobPoints;
    bool rewarded;
    bool isDaily;
    bool isMastery;
    int requiredJobLevel;
};

class JobQuestSystem
{
public:
    JobQuestSystem();

    // Daily quests
    void GenerateDailyQuests(const std::vector<Job>& jobs);
    std::vector<JobQuest> GetDailyQuests() const;
    bool HasDailyQuests() const;

    // Mastery challenges
    void CheckMasteryChallenges(const std::vector<Job>& jobs);
    std::vector<JobQuest> GetMasteryChallenges() const;

    // Quest management
    void AcceptQuest(const std::string& questId);
    void UpdateProgress(JobQuestType type, const std::string& targetName, int amount = 1);
    void CompleteQuest(const std::string& questId);
    bool IsQuestComplete(const std::string& questId) const;

    // Query methods
    std::vector<JobQuest> GetActiveQuests() const;
    std::vector<JobQuest> GetAvailableQuests() const;
    std::vector<JobQuest> GetCompletedQuests() const;
    JobQuest* GetQuest(const std::string& questId);
    int GetCompletedQuestCount() const;
    int GetTotalQuestsCompleted() const { return totalCompleted; }

    // Serialization
    std::string Serialize() const;
    void Deserialize(const std::string& data);

private:
    std::vector<JobQuest> quests;
    int totalCompleted = 0;
    int lastDailyGenerationDay = -1;

    void AddQuest(const JobQuest& quest);
    JobQuest* FindQuest(const std::string& id);
    std::string GenerateQuestId();
    int GetRandomTarget(JobQuestType type, int jobLevel) const;
    int GetQuestReward(JobQuestType type, int targetCount) const;
};

#endif
