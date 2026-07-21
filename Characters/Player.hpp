#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "../Characters/Classes/Classes.hpp"
#include "../Characters/Races/Races.hpp"
#include "../Inventory/Inventory.hpp"
#include "../Items/Item.hpp"
#include "../Jobs/JobSystem.hpp"
#include "../World/Quests/Quest.hpp"

class Player : public Character
{
public:
    Player(const std::string& name, CharacterClass characterClass, CharacterRace race = CharacterRace::Aran);

    CharacterClass GetCharacterClass() const { return characterClass; }
    CharacterRace GetRace() const { return race; }
    Inventory& GetInventory() { return inventory; }
    const Inventory& GetInventory() const { return inventory; }
    int GetInventoryCap() const { return inventory.GetCapacity() + GetBagBonus(); }
    int GetBagBonus() const;
    JobSystem& GetJobSystem() { return jobSystem; }
    QuestManager& GetQuestManager() { return questManager; }
    const QuestManager& GetQuestManager() const { return questManager; }

    void LevelUp() override;
    void InitializeClassSkills();
    void CheckNewSkills();

    // Equipment management
    bool EquipItem(std::shared_ptr<Item> item);
    bool UnequipItem(const std::string& slotName);
    void ListEquipment() const;
    bool CanEquip(const std::shared_ptr<Item>& item) const;

    // Skill loadout (max 4 skills carried into combat)
    static constexpr int MAX_LOADOUT_SKILLS = 4;
    const std::vector<int>& GetSkillLoadout() const { return skillLoadout; }
    void SetSkillLoadout(const std::vector<int>& loadout);
    void RebuildLoadout();
    void AutoFillLoadout();
    bool IsInLoadout(int skillIndex) const;

    // Custom attack skill (replaces standard attack)
    int GetAttackSkillIndex() const { return attackSkillIndex; }
    void SetAttackSkillIndex(int idx) { attackSkillIndex = idx; }

private:
    CharacterClass characterClass;
    CharacterRace race;
    Inventory inventory;
    JobSystem jobSystem;
    QuestManager questManager;
    std::vector<int> skillLoadout;
    int attackSkillIndex = 0;
};

#endif
