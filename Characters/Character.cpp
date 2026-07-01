#include "Character.hpp"
#include <iostream>
#include <algorithm>

const int Character::XP_TO_LEVEL = 100;

Character::Character(const std::string& name, const Stats& baseStats)
    : name(name), stats(baseStats), currentHealth(baseStats.health), currentMana(baseStats.mana),
      tempDefenseBonus(0), level(1), experience(0)
{
}

void Character::TakeDamage(int damage)
{
    int totalDefense = stats.defense + equipment.GetTotalDefense() + tempDefenseBonus;
    int mitigatedDamage = std::max(1, damage - totalDefense / 2);
    currentHealth -= mitigatedDamage;

    if (currentHealth < 0)
        currentHealth = 0;
}

void Character::RestoreHealth(int amount)
{
    currentHealth += amount;
    if (currentHealth > stats.health)
        currentHealth = stats.health;
}

void Character::RestoreMana(int amount)
{
    currentMana += amount;
    if (currentMana > stats.mana)
        currentMana = stats.mana;
}

void Character::ReduceMana(int amount)
{
    currentMana -= amount;
    if (currentMana < 0)
        currentMana = 0;
}

void Character::IncreaseTempDefense(int amount)
{
    tempDefenseBonus += amount;
}

void Character::GainXP(int xp)
{
    experience += xp;
    if (experience >= XP_TO_LEVEL)
    {
        LevelUp();
    }
}

void Character::LevelUp()
{
    level++;
    experience = 0;
    // Stats increase is handled by subclasses
}

void Character::DisplayStats() const
{
    std::cout << "\n=== " << name << " (Level " << level << ") ===" << std::endl;
    std::cout << "Health: " << currentHealth << "/" << stats.health << std::endl;
    std::cout << "Mana: " << currentMana << "/" << stats.mana << std::endl;
    std::cout << "Strength: " << stats.strength << std::endl;
    std::cout << "Vitality: " << stats.vitality << std::endl;
    std::cout << "Intelligence: " << stats.intelligence << std::endl;
    std::cout << "Wisdom: " << stats.wisdom << std::endl;
    std::cout << "Dexterity: " << stats.dexterity << std::endl;
    std::cout << "Defense: " << stats.defense << std::endl;
    std::cout << "Experience: " << experience << "/" << XP_TO_LEVEL << std::endl;
    std::cout << "Skills: " << skills.GetSkillCount() << std::endl;
}