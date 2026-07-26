#include "Quest.hpp"
#include "../../Inventory/Inventory.hpp"
#include <iostream>
#include <algorithm>

Quest::Quest(const std::string& title, QuestType type, const std::string& description,
             const std::string& targetName, int targetCount, int rewardXP, int rewardGold,
             const std::string& gatherItemName)
    : title(title), type(type), description(description), status(QuestStatus::NotStarted),
      targetName(targetName), targetCount(targetCount), currentCount(0),
      rewardXP(rewardXP), rewardGold(rewardGold), rewarded(false),
      gatherItemName(gatherItemName)
{
}

void Quest::UpdateProgress(int amount)
{
    if (status != QuestStatus::InProgress)
        return;

    currentCount += amount;
    if (currentCount >= targetCount)
    {
        currentCount = targetCount;
        Complete();
    }
}

void Quest::Complete()
{
    status = QuestStatus::Completed;
}

void Quest::Fail()
{
    status = QuestStatus::Failed;
}

std::string Quest::GetProgressString() const
{
    std::string statusStr;
    switch (status)
    {
        case QuestStatus::NotStarted: statusStr = "Not Started"; break;
        case QuestStatus::InProgress: statusStr = "In Progress"; break;
        case QuestStatus::Completed:  statusStr = "Completed!"; break;
        case QuestStatus::Failed:     statusStr = "Failed"; break;
    }

    std::string progress = title + " [" + statusStr + "]";
    if (targetCount > 0 && status == QuestStatus::InProgress)
    {
        progress += " (" + std::to_string(currentCount) + "/" + std::to_string(targetCount) + " " + targetName + ")";
    }
    return progress;
}

QuestManager::QuestManager()
{
}

void QuestManager::AddQuest(const Quest& quest)
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

Quest* QuestManager::GetQuest(size_t index)
{
    if (index < quests.size())
    {
        return &quests[index];
    }
    return nullptr;
}

size_t QuestManager::GetQuestCount() const
{
    return quests.size();
}

void QuestManager::InitializeDefaultQuests()
{
    quests.clear();

    // === Felrona kill quests ===
    quests.emplace_back("Slime Exterminator",  QuestType::Kill, "The slimes of Felrona have grown aggressive. Thin their numbers.",            "Slime",   5, 100,  50);
    quests.emplace_back("Rat Catcher",          QuestType::Kill, "Rats are infesting Felrona's buildings. Put a stop to it.",             "Rat",     5, 100,  50);
    quests.emplace_back("Bandit Troubles",      QuestType::Kill, "Bandits prey on travelers near Felrona. Teach them a lesson.",         "Bandit",  5, 150,  75);

    // === Felrona gather quests ===
    quests.emplace_back("Gel Collection",       QuestType::Collect, "Collect Slime Gel for the blacksmith's experiments.",              "Slime Gel", 5, 80,  40, "Slime Gel");
    quests.emplace_back("Rat Tails",            QuestType::Collect, "Gather Rat Tails to prove your hunting prowess.",                 "Rat Tail",  5, 80,  40, "Rat Tail");

    // === Dark Woods kill quests ===
    quests.emplace_back("Wolf Hunter",          QuestType::Kill, "Wolves stalk the Dark Woods paths. Reduce their pack.",         "Wolf",    5, 200, 100);
    quests.emplace_back("Spider Menace",        QuestType::Kill, "Giant Spiders spin their webs across Dark Woods. Clear them out.",  "Giant Spider", 5, 200, 100);

    // === Dark Woods gather quests ===
    quests.emplace_back("Wolf Pelts",           QuestType::Collect, "Collect Wolf Pelts for warm winter clothing.",                   "Wolf Pelt",   3, 150, 75, "Wolf Pelt");
    quests.emplace_back("Silk Harvest",         QuestType::Collect, "Gather Spider Silk for the weaver's workshop.",                  "Spider Silk", 3, 160, 80, "Spider Silk");

    // === Coast kill quests ===
    quests.emplace_back("Pirate Scourge",       QuestType::Kill, "Pirates raid coastal settlements. Sink their operation.",       "Pirate",  5, 300, 150);

    // === Coast gather quests ===
    quests.emplace_back("Pirate Bounty",        QuestType::Collect, "Collect Doubloons from defeated pirates.",                      "Doubloon",     5, 250, 125, "Doubloon");
    quests.emplace_back("Scale Collection",     QuestType::Collect, "Gather Serpent Scales for alchemical research.",                "Serpent Scale", 3, 280, 140, "Serpent Scale");

    // === Dragon's Peak kill quests ===
    quests.emplace_back("Orc Bane",             QuestType::Kill, "Orc warbands terrorize Dragon's Peak. Break their ranks.",        "Orc",     5, 400, 200);
    quests.emplace_back("Dragon Slayer",        QuestType::Kill, "The Elder Dragon threatens the realm. Only the bravest dare challenge it.",               "Elder Dragon", 1, 1000, 500);

    // === Dragon's Peak gather quests ===
    quests.emplace_back("Orc Tusks",            QuestType::Collect, "Collect Orc Tusks as proof of your victories.",                  "Orc Tusk",     5, 350, 175, "Orc Tusk");
    quests.emplace_back("Essence Gathering",    QuestType::Collect, "Gather Fire Essence for the Elementalist.",                     "Fire Essence", 3, 400, 200, "Fire Essence");

    // === Aran Highlands kill quests ===
    quests.emplace_back("Highland Challenge",   QuestType::Kill, "Highland Warriors seek worthy opponents. Show them one.",        "Highland Warrior", 5, 500, 250);
    quests.emplace_back("Gryphon Hunt",         QuestType::Kill, "Gryphons soar above the Highlands. Bring one down.",            "Gryphon", 3, 600, 300);

    // === Shadow Dungeon kill quests ===
    quests.emplace_back("Shadow Purge",         QuestType::Kill, "Shadows lurk in the Dungeon's depths. Purge the darkness.", "Shadow",  5, 600, 300);

    // === Shadow Dungeon gather quests ===
    quests.emplace_back("Shadow Research",      QuestType::Collect, "Collect Shadow Essence for arcane research.",                   "Shadow Essence", 5, 550, 275, "Shadow Essence");
    quests.emplace_back("Demon Parts",          QuestType::Collect, "Gather Demon Horns for protective ward crafting.",              "Demon Horn",     3, 700, 350, "Demon Horn");

    // === Escort quests ===
    quests.emplace_back("Merchant Escort",      QuestType::Escort, "A merchant needs protection traveling through Dark Woods. Defeat 5 wolves on the road.",  "Wolf",     5, 300, 150);
    quests.emplace_back("Pilgrim's Passage",    QuestType::Escort, "A pilgrim seeks safe passage through Dragon's Peak. Defeat 3 Orcs along the way.",       "Orc",      3, 500, 250);
    quests.emplace_back("Royal Courier",        QuestType::Escort, "A courier must deliver urgent messages through the Highlands. Defeat 4 Highland Warriors.", "Highland Warrior", 4, 700, 350);

    // === Celestial Spire kill quests ===
    quests.emplace_back("Knight Fall",          QuestType::Kill, "Corrupted Celestial Knights guard forbidden halls. Defeat them.",          "Celestial Knight", 5, 1200, 600);

    // === Void Beyond kill quests ===
    quests.emplace_back("Void Purge",           QuestType::Kill, "Void Walkers tear at reality itself. Seal them away.",              "Void Walker",     5, 2000, 1000);

    // === Dungeon completion quests ===
    quests.emplace_back("Farmland Liberation", QuestType::Explore, "The Abandoned Farm is overrun. Drive out the bandits and reclaim it.",           "Abandoned Farm",    1, 300,   150);
    quests.emplace_back("Twilight Purification", QuestType::Explore, "Twilight Hollow teems with undead. Cleanse the catacombs.",           "Twilight Hollow",   1, 400,   200);
    quests.emplace_back("Sunken Treasure",    QuestType::Explore, "The Sunken Grotto holds ancient secrets — and ancient dangers.",           "Sunken Grotto",     1, 500,   250);
    quests.emplace_back("Summit Conquest",    QuestType::Explore, "The Summit is a proving ground for warriors. Claim victory.",                  "The Summit",        1, 800,   400);
    quests.emplace_back("Citadel Siege",      QuestType::Explore, "Storm The Citadel and face the corrupted knights within.",                   "The Citadel",       1, 1000,  500);
    quests.emplace_back("Abyssal Victory",    QuestType::Explore, "The Abyssal Depths test even the strongest. Survive and emerge triumphant.",          "Abyssal Depths",    1, 1500,  750);
    quests.emplace_back("Heavenly Ascent",    QuestType::Explore, "The Heavenly Ascent challenges body and spirit. Ascend to glory.",          "The Heavenly Ascent", 1, 3000, 1500);
    quests.emplace_back("Void Conqueror",     QuestType::Explore, "The Endless Void consumes all who enter. Be the one who conquers it.",            "The Endless Void",  1, 5000, 2500);

    // Quest giver: quests start as NotStarted — accept them from NPCs
    for (auto& q : quests)
        q.status = QuestStatus::NotStarted;
}

void QuestManager::UpdateKillQuests(const std::string& enemyName)
{
    for (auto& quest : quests)
    {
        if ((quest.type == QuestType::Kill || quest.type == QuestType::Escort) &&
            quest.status == QuestStatus::InProgress &&
            quest.targetName == enemyName)
        {
            quest.UpdateProgress(1);
        }
    }
}

void QuestManager::UpdateGatherQuests(const Inventory& inventory)
{
    for (auto& quest : quests)
    {
        if (quest.type == QuestType::Collect &&
            quest.status == QuestStatus::InProgress &&
            !quest.gatherItemName.empty())
        {
            int count = 0;
            for (size_t i = 0; i < inventory.GetItemCount(); ++i)
            {
                auto item = inventory.GetItem(i);
                if (item && item->name == quest.gatherItemName)
                    count += item->count;
            }
            quest.currentCount = std::min(count, quest.targetCount);
            if (quest.currentCount >= quest.targetCount)
                quest.Complete();
        }
    }
}

void QuestManager::CompleteExploreQuest(const std::string& locationName)
{
    for (auto& quest : quests)
    {
        if (quest.type == QuestType::Explore &&
            quest.status == QuestStatus::InProgress &&
            quest.targetName == locationName)
        {
            quest.Complete();
        }
    }
}
