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

class Quest
{
public:
    Quest(const std::string& title, QuestType type, const std::string& description);

    std::string title;
    QuestType type;
    std::string description;
    QuestStatus status;
    int rewardXP;
    int rewardGold;

    void Complete();
    void Fail();
};

class QuestManager
{
public:
    QuestManager();

    void AddQuest(Quest quest);
    void CompleteQuest(size_t index);
    Quest GetQuest(size_t index);
    size_t GetQuestCount() const;

private:
    std::vector<Quest> quests;
};

#endif
