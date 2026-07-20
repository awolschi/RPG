#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include <string>
#include <memory>
#include <vector>
#include "../Characters/Player.hpp"
#include "Religion.hpp"
#include "../World/Quests/Quest.hpp"
#include "../Achievements/Achievement.hpp"
#include "../Factions/FactionReputation.hpp"
#include "../Factions/Pet.hpp"
#include "Wiki.hpp"

struct SaveSlotInfo
{
    int slot;
    std::string playerName;
    CharacterClass characterClass;
    int level;
    bool occupied;
};

class SaveGameManager
{
public:
    static constexpr int SLOT_COUNT = 5;
    static constexpr int SAVE_VERSION = 10; // v10: persist wiki discovery state

    SaveGameManager();

    bool SaveGame(const std::shared_ptr<Player>& player, int slot, int areaIndex, const ReligionSystem& religion, const AchievementSystem& achievements, const ReputationSystem& reputation, const PetManager& pets, const Wiki& wiki);
    std::shared_ptr<Player> LoadGame(int slot, int& outAreaIndex, ReligionSystem& outReligion, AchievementSystem& outAchievements, ReputationSystem& outReputation, PetManager& outPets, Wiki& outWiki);

    bool IsSlotOccupied(int slot);
    SaveSlotInfo GetSlotInfo(int slot);
    std::vector<SaveSlotInfo> ListSlots();

    std::string GetSaveDirectory() const;

private:
    std::string saveDirectory;
    std::string SlotPath(int slot) const;
};

#endif
