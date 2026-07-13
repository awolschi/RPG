#ifndef NPC_HPP
#define NPC_HPP

#include <string>
#include <vector>

struct NPCDialogue
{
    std::string greeting;
    std::string questOffer;
    std::string questComplete;
    std::string farewell;
};

class NPC
{
public:
    NPC(const std::string& name, const NPCDialogue& dialogue, int linkedQuestIndex = -1);

    const std::string& GetName() const { return name; }
    const NPCDialogue& GetDialogue() const { return dialogue; }
    int GetLinkedQuestIndex() const { return linkedQuestIndex; }
    const std::vector<int>& GetLinkedQuestIndices() const { return linkedQuestIndices; }

    void AddLinkedQuest(int questIndex);

private:
    std::string name;
    NPCDialogue dialogue;
    int linkedQuestIndex;
    std::vector<int> linkedQuestIndices;
};

class NPCDatabase
{
public:
    static NPC CreateFelronaBlacksmith();
    static NPC CreateFelronaInnkeeper();
    static NPC CreateFelronaGuard();
    static NPC CreateDarkWoodsRanger();
    static NPC CreateDarkWoodsHerbalist();
    static NPC CreateCoastPirateHunter();
    static NPC CreateCoastFisher();
    static NPC CreateDragonPeakScholar();
    static NPC CreateDragonPeakElementalist();
    static NPC CreateHighlandsShieldbearer();
    static NPC CreateHighlandsSkywatcher();
    static NPC CreateShadowDungeonPriest();
    static NPC CreateShadowDungeonSlayer();
};
#endif
