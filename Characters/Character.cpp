#include "Character.hpp"
#include "../Items/Passives.hpp"
#include <algorithm>

Character::Character(const std::string& name, const Stats& baseStats)
    : name(name), stats(baseStats), currentHealth(baseStats.health), currentMana(baseStats.mana),
      tempDefenseBonus(0), attackBonus(0), level(1), experience(0)
{
}

int Character::CalculateRequiredXP(int level)
{
    return 100 + (level - 1) * 100 + (level - 1) * (level - 1) * 15;
}

void Character::TakeDamage(int damage, ElementType element)
{
    float resist = GetResistance(element);
    int totalDefense = stats.defense + equipment.GetTotalDefense() + tempDefenseBonus + masteryBonusDEF;

    // All resist bonus from passives (flat reduction)
    int allResist = Passives::GetAllResistBonus(equipment);

    // Pet defense bonus reduces incoming damage by a fractional amount
    if (petBonusDefense > 0.0f)
        damage = static_cast<int>(damage * (1.0f - std::min(petBonusDefense, 0.9f)));

    // Diminishing returns on defense: 200 defense = ~50% reduction, 400 = ~66%, 800 = ~80%
    float defFactor = 1.0f - std::min(0.85f, totalDefense / (totalDefense + 200.0f));
    int mitigatedDamage = std::max(1, static_cast<int>(damage * defFactor) - allResist);

    // Damage reduction from passives
    int dmgReducePct = Passives::GetDamageReductionPercent(equipment);
    if (dmgReducePct > 0)
        mitigatedDamage = mitigatedDamage * (100 - dmgReducePct) / 100;

    // Character mastery damage reduction
    if (masteryDamageReduction > 0.0f)
        mitigatedDamage = static_cast<int>(mitigatedDamage * (1.0f - masteryDamageReduction));

    // Master class damage reduction (+10%)
    if (masterClassDamageReduction > 0.0f)
        mitigatedDamage = static_cast<int>(mitigatedDamage * (1.0f - masterClassDamageReduction));

    mitigatedDamage = static_cast<int>(mitigatedDamage * resist);

    // Mana shield: absorb damage with mana
    int manaShieldPct = Passives::GetManaShieldPercent(equipment);
    if (manaShieldPct > 0 && currentMana > 0)
    {
        int absorbed = mitigatedDamage * manaShieldPct / 100;
        if (absorbed > currentMana) absorbed = currentMana;
        mitigatedDamage -= absorbed;
        currentMana -= absorbed;
    }

    mitigatedDamage = std::max(1, mitigatedDamage);
    currentHealth -= mitigatedDamage;

    if (currentHealth < 0)
        currentHealth = 0;
}

void Character::TakeDamageRaw(int damage)
{
    if (damage <= 0) return;
    currentHealth -= damage;
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
    if (masteryHealingBonus > 0.0f)
        amount = static_cast<int>(amount * (1.0f + masteryHealingBonus));
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

    // Status immunity from passives
    if (type == EffectType::Stun && Passives::IsStunImmune(equipment)) return;
    if (type == EffectType::Freeze && Passives::IsFreezeImmune(equipment)) return;
    if (type == EffectType::Burn && Passives::IsBurnImmune(equipment)) return;
    if (type == EffectType::Poison && Passives::IsPoisonImmune(equipment)) return;

    // Status extend from passives
    int extendTurns = Passives::GetStatusExtendTurns(equipment);
    duration += extendTurns;

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
    for (auto& e : effects)
    {
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
        }
    }
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const ActiveEffect& e) { return e.duration <= 0; }),
        effects.end());
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
    if (level >= MAX_LEVEL)
    {
        OnOverflowXP(xp);
        return;
    }
    experience += xp;
    int required = CalculateRequiredXP(level);
    while (experience >= required && level < MAX_LEVEL)
    {
        experience -= required;
        LevelUp();
        required = CalculateRequiredXP(level);
    }
    if (level >= MAX_LEVEL)
    {
        int overflow = experience;
        experience = 0;
        OnOverflowXP(overflow);
    }
}

void Character::LevelUp()
{
    if (level >= MAX_LEVEL) return;
    level++;
    // Stats increase is handled by subclasses
}

