#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>
#include "Stats/Stats.hpp"
#include "../Items/Equipment/Equipment.hpp"
#include "../Skills/Skills.hpp"

class Character
{
public:
    Character(const std::string& name, const Stats& baseStats);
    virtual ~Character() = default;

    // Getters
    const std::string& GetName() const { return name; }
    const Stats& GetStats() const { return stats; }
    int GetCurrentHealth() const { return currentHealth; }
    int GetCurrentMana() const { return currentMana; }
    int GetLevel() const { return level; }
    int GetExperience() const { return experience; }
    bool IsAlive() const { return currentHealth > 0; }
    SkillSet& GetSkills() { return skills; }
    Equipment& GetEquipment() { return equipment; }

    // Health/Mana management
    void TakeDamage(int damage);
    void RestoreHealth(int amount);
    void RestoreMana(int amount);
    void ReduceMana(int amount);
    void IncreaseTempDefense(int amount);

    // Experience and leveling
    void GainXP(int xp);
    virtual void LevelUp();

    // Display
    void DisplayStats() const;

protected:
    std::string name;
    Stats stats;
    int currentHealth;
    int currentMana;
    int tempDefenseBonus;
    int level;
    int experience;
    static const int XP_TO_LEVEL;

    SkillSet skills;
    Equipment equipment;
};

#endif
