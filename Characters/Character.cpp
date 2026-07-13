#include "Character.hpp"
#include <iostream>
#include <algorithm>

Character::Character(const std::string& name, const Stats& baseStats)
    : name(name), stats(baseStats), currentHealth(baseStats.health), currentMana(baseStats.mana),
      tempDefenseBonus(0), attackBonus(0), level(1), experience(0)
{
}

int Character::CalculateRequiredXP(int level)
{
    return 100 + (level - 1) * 50;
}

void Character::TakeDamage(int damage, ElementType element)
{
    float resist = GetResistance(element);
    int elemReduction = GetElementalReduction(element);
    int totalDefense = stats.defense + equipment.GetTotalDefense() + tempDefenseBonus;
    int mitigatedDamage = std::max(1, damage - totalDefense / 2 - elemReduction);
    mitigatedDamage = static_cast<int>(mitigatedDamage * resist);
    mitigatedDamage = std::max(1, mitigatedDamage);
    currentHealth -= mitigatedDamage;

    if (currentHealth < 0)
        currentHealth = 0;
}

float Character::GetResistance(ElementType element) const
{
    auto it = resistances.find(element);
    if (it != resistances.end())
        return it->second;
    return 1.0f;
}

void Character::RestoreHealth(int amount)
{
    currentHealth += amount;
    if (currentHealth > GetMaxHealth())
        currentHealth = GetMaxHealth();
}

void Character::RestoreMana(int amount)
{
    currentMana += amount;
    if (currentMana > GetMaxMana())
        currentMana = GetMaxMana();
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

void Character::SetStats(const Stats& newStats)
{
    stats = newStats;
}

void Character::SetCurrentHealth(int hp)
{
    currentHealth = hp;
}

void Character::SetCurrentMana(int mana)
{
    currentMana = mana;
}

void Character::ResetTempDefense()
{
    tempDefenseBonus = 0;
    attackBonus = 0;
}

void Character::SetResistance(ElementType element, float value)
{
    resistances[element] = value;
}

ElementType Character::GetEffectiveElement(ElementType skillElement) const
{
    if (skillElement == ElementType::Physical)
        return equipment.GetWeaponElement();
    return skillElement;
}

int Character::GetElementalBonus(ElementType skillElement) const
{
    ElementType effective = GetEffectiveElement(skillElement);
    return equipment.GetElementalDamage(effective);
}

void Character::ApplyEffect(EffectType type, int duration, int potency, const std::string& source)
{
    if (type == EffectType::None) return;
    for (auto& e : effects)
    {
        if (e.type == type)
        {
            e.duration = std::max(e.duration, duration);
            e.potency = std::max(e.potency, potency);
            return;
        }
    }
    effects.push_back({type, duration, potency, source});
}

std::string Character::ProcessEffects()
{
    std::string msg;
    std::vector<size_t> toRemove;
    for (size_t i = 0; i < effects.size(); ++i)
    {
        auto& e = effects[i];
        if (e.type == EffectType::Poison)
        {
            TakeDamage(e.potency);
            msg += name + " takes " + std::to_string(e.potency) + " poison damage! ";
        }
        else if (e.type == EffectType::Burn)
        {
            TakeDamage(e.potency);
            msg += name + " takes " + std::to_string(e.potency) + " burn damage! ";
        }
        e.duration--;
        if (e.duration <= 0)
        {
            if (e.type == EffectType::Poison) msg += "Poison fades from " + name + ". ";
            else if (e.type == EffectType::Burn) msg += "Burn fades from " + name + ". ";
            else if (e.type == EffectType::Stun) msg += name + " recovers from stun. ";
            else if (e.type == EffectType::Freeze) msg += name + " thaws out. ";
            toRemove.push_back(i);
        }
    }
    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it)
        effects.erase(effects.begin() + *it);
    return msg;
}

bool Character::HasEffect(EffectType type) const
{
    for (const auto& e : effects)
        if (e.type == type) return true;
    return false;
}

void Character::RemoveEffect(EffectType type)
{
    for (auto it = effects.begin(); it != effects.end(); ++it)
    {
        if (it->type == type) { effects.erase(it); return; }
    }
}

bool Character::IsStunned() const
{
    return HasEffect(EffectType::Stun) || HasEffect(EffectType::Freeze);
}

void Character::ClearAllEffects()
{
    effects.clear();
}

void Character::SetLevel(int lvl)
{
    level = lvl;
}

void Character::SetExperience(int xp)
{
    experience = xp;
}

void Character::GainXP(int xp)
{
    if (level >= MAX_LEVEL) return;
    experience += xp;
    int required = CalculateRequiredXP(level);
    while (experience >= required && level < MAX_LEVEL)
    {
        experience -= required;
        LevelUp();
        required = CalculateRequiredXP(level);
    }
    if (level >= MAX_LEVEL)
        experience = 0;
}

void Character::LevelUp()
{
    if (level >= MAX_LEVEL) return;
    level++;
    // Stats increase is handled by subclasses
}

void Character::DisplayStats() const
{
    std::cout << "\n=== " << name << " (Level " << level << ") ===" << std::endl;
    std::cout << "Health: " << currentHealth << "/" << GetMaxHealth() << std::endl;
    std::cout << "Mana: " << currentMana << "/" << GetMaxMana() << std::endl;
    std::cout << "Strength: " << stats.strength << std::endl;
    std::cout << "Vitality: " << stats.vitality << std::endl;
    std::cout << "Intelligence: " << stats.intelligence << std::endl;
    std::cout << "Wisdom: " << stats.wisdom << std::endl;
    std::cout << "Dexterity: " << stats.dexterity << std::endl;
    std::cout << "Defense: " << stats.defense << std::endl;
    std::cout << "Experience: " << experience << "/" << CalculateRequiredXP(level) << std::endl;
    std::cout << "Skills: " << skills.GetSkillCount() << std::endl;
}