#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "Stats/Stats.hpp"
#include "../Items/Equipment/Equipment.hpp"
#include "../Skills/Skills.hpp"

enum class EffectType
{
    None,
    Poison,
    Burn,
    Stun,
    Freeze
};

struct ActiveEffect
{
    EffectType type;
    int duration;
    int potency;
    std::string sourceName;
};

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
    int GetMaxHealth() const { return stats.health + equipment.GetHealthBonus() + petBonusHP; }
    int GetMaxMana() const { return stats.mana + equipment.GetManaBonus() + petBonusMP; }
    int GetLevel() const { return level; }
    int GetExperience() const { return experience; }
    bool IsAlive() const { return currentHealth > 0; }
    int GetWeaponDamage() const { return equipment.GetWeaponDamage() + attackBonus; }
    int GetAttackBonus() const { return attackBonus; }
    void SetAttackBonus(int bonus) { attackBonus = bonus; }
    void ResetAttackBonus() { attackBonus = 0; }
    ElementType GetWeaponElement() const { return equipment.GetWeaponElement(); }
    ElementType GetEffectiveElement(ElementType skillElement) const;
    int GetElementalBonus(ElementType skillElement) const;
    int GetElementalReduction(ElementType element) const { return equipment.GetElementalReduction(element); }
    SkillSet& GetSkills() { return skills; }
    Equipment& GetEquipment() { return equipment; }

    // Health/Mana management
    void TakeDamage(int damage, ElementType element = ElementType::Physical);
    void RestoreHealth(int amount);
    void RestoreMana(int amount);
    void ReduceMana(int amount);
    void IncreaseTempDefense(int amount);
    void ResetTempDefense();
    void SetStats(const Stats& newStats);
    void SetCurrentHealth(int hp);
    void SetCurrentMana(int mana);
    void SetLevel(int lvl);
    void SetExperience(int xp);

    // Pet passive bonuses (set by the Game when pets are equipped / leveled)
    void SetPetBonuses(int hp, int mp, float dmg, float def, float crit, float critDmg)
    {
        petBonusHP = hp;
        petBonusMP = mp;
        petBonusDamage = dmg;
        petBonusDefense = def;
        petBonusCritChance = crit;
        petBonusCritDamage = critDmg;
    }
    int  GetPetBonusHP() const { return petBonusHP; }
    int  GetPetBonusMP() const { return petBonusMP; }
    float GetPetBonusDamage() const { return petBonusDamage; }
    float GetPetBonusDefense() const { return petBonusDefense; }
    float GetPetBonusCritChance() const { return petBonusCritChance; }
    float GetPetBonusCritDamage() const { return petBonusCritDamage; }

    // Status effects
    void ApplyEffect(EffectType type, int duration, int potency = 0, const std::string& source = "");
    std::string ProcessEffects();
    bool HasEffect(EffectType type) const;
    void RemoveEffect(EffectType type);
    bool IsStunned() const;
    void ClearAllEffects();
    const std::vector<ActiveEffect>& GetEffects() const { return effects; }

    // Elemental resistances
    void SetResistance(ElementType element, float value);
    float GetResistance(ElementType element) const;

    static constexpr int MAX_LEVEL = 50;

    // Experience and leveling
    void GainXP(int xp);
    virtual void LevelUp();
    static int CalculateRequiredXP(int level);

    // Display
    void DisplayStats() const;

protected:
    std::string name;
    Stats stats;
    int currentHealth;
    int currentMana;
    int tempDefenseBonus;
    int attackBonus;
    int level;
    int experience;

    // Pet passive bonuses (applied externally by the Game)
    int petBonusHP = 0;
    int petBonusMP = 0;
    float petBonusDamage = 0.0f;
    float petBonusDefense = 0.0f;
    float petBonusCritChance = 0.0f;
    float petBonusCritDamage = 0.0f;

    SkillSet skills;
    Equipment equipment;
    std::vector<ActiveEffect> effects;
    std::map<ElementType, float> resistances;
};

#endif
