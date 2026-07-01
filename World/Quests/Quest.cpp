#include "Quest.hpp"

Quest::Quest(const std::string& title, QuestType type, const std::string& description)
    : title(title), type(type), description(description), status(QuestStatus::NotStarted), rewardXP(50), rewardGold(100)
{
}

void Quest::Complete()
{
    status = QuestStatus::Completed;
}

void Quest::Fail()
{
    status = QuestStatus::Failed;
}

QuestManager::QuestManager()
{
}

void QuestManager::AddQuest(Quest quest)
{
    quests.push_back(quest);
}

void QuestManager::CompleteQuest(size_t index)
{
    if (index < quests.size())
    {
        quests[index].Complete();
    }
}

Quest QuestManager::GetQuest(size_t index)
{
    if (index < quests.size())
    {
        return quests[index];
    }
    return Quest("", QuestType::Kill, "");
}

size_t QuestManager::GetQuestCount() const
{
    return quests.size();
}
