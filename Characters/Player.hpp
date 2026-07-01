#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "../Characters/Classes/Classes.hpp"
#include "../Inventory/Inventory.hpp"
#include "../Jobs/JobSystem.hpp"

class Player : public Character
{
public:
    Player(const std::string& name, CharacterClass characterClass);

    CharacterClass GetCharacterClass() const { return characterClass; }
    Inventory& GetInventory() { return inventory; }
    const Inventory& GetInventory() const { return inventory; }
    JobSystem& GetJobSystem() { return jobSystem; }
    const JobSystem& GetJobSystem() const { return jobSystem; }

    void LevelUp() override;
    void InitializeClassSkills();

private:
    CharacterClass characterClass;
    Inventory inventory;
    JobSystem jobSystem;
};

#endif
