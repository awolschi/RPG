#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include <string>
#include <memory>
#include <vector>
#include "../Characters/Player.hpp"
#include "Religion.hpp"
#include "../World/Quests/Quest.hpp"

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

    SaveGameManager();

    bool SaveGame(const std::shared_ptr<Player>& player, int slot, int areaIndex, const ReligionSystem& religion);
    std::shared_ptr<Player> LoadGame(int slot, int& outAreaIndex, ReligionSystem& outReligion);

    bool IsSlotOccupied(int slot);
    SaveSlotInfo GetSlotInfo(int slot);
    std::vector<SaveSlotInfo> ListSlots();

    std::string GetSaveDirectory() const;

private:
    std::string saveDirectory;
    std::string SlotPath(int slot) const;
};

#endif
