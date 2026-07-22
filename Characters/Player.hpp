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

    // Skill loadout (max 4 skills carried into combat, 6 when evolved)
    static constexpr int MAX_LOADOUT_SKILLS = 4;
    static constexpr int EVOLVED_LOADOUT_SKILLS = 6;
    int GetMaxLoadoutSkills() const { return evolved ? EVOLVED_LOADOUT_SKILLS : MAX_LOADOUT_SKILLS; }
    const std::vector<int>& GetSkillLoadout() const { return skillLoadout; }
    void SetSkillLoadout(const std::vector<int>& loadout);
    void RebuildLoadout();
    void AutoFillLoadout();
    bool IsInLoadout(int skillIndex) const;

    // Custom attack skill (replaces standard attack)
    int GetAttackSkillIndex() const { return attackSkillIndex; }
    void SetAttackSkillIndex(int idx) { attackSkillIndex = idx; }

    // Class Evolution
    bool HasClassEvolved() const { return evolved; }
    void EvolveClass();
    std::string GetEvolvedClassName() const;
    int GetEvolvedLoadoutSize() const { return evolved ? 6 : 4; }

    // Evolution passives (class-specific)
    float GetEvolvedDamageReduction() const;  // Warrior Hero: 10% DR
    float GetEvolvedHealingBonus() const;     // Priest Sage: +15% healing
    float GetEvolvedManaCostReduction() const; // Mage Archmage: -15% mana cost
    float GetEvolvedCritBonus() const;        // Archer Ranger: +10% crit
    float GetEvolvedGoldFind() const;         // Merchant Tycoon: +25% gold find

private:
    CharacterClass characterClass;
    CharacterRace race;
    Inventory inventory;
    JobSystem jobSystem;
    QuestManager questManager;
    std::vector<int> skillLoadout;
    int attackSkillIndex = 0;
    bool evolved = false;
};

#endif
