#ifndef QUEST_HPP
#define QUEST_HPP

#include <string>
#include <vector>

enum class QuestStatus
{
    NotStarted,
    InProgress,
    Completed,
    Failed
};

enum class QuestType
{
    Kill,
    Collect,
    Explore,
    Escort
};

class Inventory;

class Quest
{
public:
    Quest(const std::string& title, QuestType type, const std::string& description,
          const std::string& targetName = "", int targetCount = 0, int rewardXP = 50, int rewardGold = 100,
          const std::string& gatherItemName = "");

    std::string title;
    QuestType type;
    std::string description;
    QuestStatus status;
    std::string targetName;
    int targetCount;
    int currentCount;
    int rewardXP;
    int rewardGold;
    bool rewarded;
    std::string gatherItemName;

    void UpdateProgress(int amount = 1);
    void Complete();
    void Fail();
    std::string GetProgressString() const;
};

class QuestManager
{
public:
    QuestManager();

    void AddQuest(const Quest& quest);
    void CompleteQuest(size_t index);
    Quest* GetQuest(size_t index);
    size_t GetQuestCount() const;
    void ClearQuests() { quests.clear(); }
    void InitializeDefaultQuests();
    void UpdateKillQuests(const std::string& enemyName);
    void UpdateGatherQuests(const Inventory& inventory);
    void CompleteExploreQuest(const std::string& locationName);

private:
    std::vector<Quest> quests;
};

#endif
