#include "Skill.hpp"
#include "../Characters/Character.hpp"
#include <algorithm>

void Skill::Use(Character& caster, Character& target)
{
}

void Skill::ReduceCooldown()
{
    if (currentCooldown > 0)
        currentCooldown--;
}

bool Skill::IsReady() const
{
    return currentCooldown == 0;
}

void Skill::ResetCooldown()
{
    currentCooldown = GetEffectiveCooldown();
}

int Skill::GetXPToLevel(int currentLevel)
{
    return currentLevel * 80;
}

void Skill::GainXP(int xp)
{
    if (level >= 50)
    {
        int bonus = GetMasteryXPGainBonus();
        if (bonus > 0) xp = xp * (100 + bonus) / 100;
        GainMasteryXP(xp);
        return;
    }
    experience += xp;
    int needed = GetXPToLevel(level);
    while (experience >= needed)
    {
        experience -= needed;
        LevelUp();
        needed = GetXPToLevel(level);
        if (level >= 50)
        {
            GainMasteryXP(experience);
            experience = 0;
            break;
        }
    }
}

void Skill::LevelUp()
{
    level++;
    baseDamage = (int)(baseDamage * 1.12f) + 1;
    if (level % 5 == 0 && cooldown > 1)
        cooldown--;
    skillPoints++;
}

void Skill::InitializeUpgrades()
{
    upgrades.clear();

    if (baseDamage > 0)
    {
        upgrades.push_back({"Empower I", "+10% skill damage", 1, 0, false, 10});
        upgrades.push_back({"Empower II", "+20% skill damage", 2, 0, false, 20});
        upgrades.push_back({"Elemental Focus I", "+3 elemental effect damage", 1, 1, false, 0, 0, 0, 0, 3});
        upgrades.push_back({"Elemental Focus II", "+1 turn effect duration", 2, 1, false, 0, 0, 0, 1});
        upgrades.push_back({"Efficiency I", "-2 mana cost", 1, 2, false, 0, 2});
        upgrades.push_back({"Efficiency II", "-1 cooldown turn", 2, 2, false, 0, 0, 1});
    }
    else
    {
        upgrades.push_back({"Fortify I", "+5 DEF", 1, 0, false, 0, 0, 0, 0, 0, 0, 5});
        upgrades.push_back({"Fortify II", "+10 DEF", 2, 0, false, 0, 0, 0, 0, 0, 0, 10});
        upgrades.push_back({"Rejuvenate I", "+15 heal amount", 1, 1, false, 0, 0, 0, 0, 0, 15});
        upgrades.push_back({"Rejuvenate II", "+30 heal amount", 2, 1, false, 0, 0, 0, 0, 0, 30});
        upgrades.push_back({"Efficiency I", "-2 mana cost", 1, 2, false, 0, 2});
        upgrades.push_back({"Efficiency II", "-1 cooldown turn", 2, 2, false, 0, 0, 1});
    }
}

bool Skill::CanUnlockUpgrade(int index) const
{
    if (index < 0 || index >= (int)upgrades.size()) return false;
    if (upgrades[index].unlocked) return false;
    if (skillPoints <= 0) return false;

    int branch = upgrades[index].branch;
    int tier = upgrades[index].tier;

    if (tier == 1) return true;

    for (const auto& up : upgrades)
    {
        if (up.branch == branch && up.tier == tier - 1 && up.unlocked)
            return true;
    }
    return false;
}

bool Skill::UnlockUpgrade(int index)
{
    if (!CanUnlockUpgrade(index)) return false;
    upgrades[index].unlocked = true;
    skillPoints--;
    return true;
}

int Skill::GetTotalDamagePercentBonus() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.damagePercentBonus;
    return total;
}

int Skill::GetTotalManaCostReduction() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.manaCostReduction;
    return total;
}

int Skill::GetTotalCooldownReduction() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.cooldownReduction;
    return total;
}

int Skill::GetTotalEffectDurationBonus() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.effectDurationBonus;
    return total;
}

int Skill::GetTotalEffectDamageBonus() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.effectDamageBonus;
    return total;
}

int Skill::GetTotalHealBonus() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.healBonus;
    return total;
}

int Skill::GetTotalDefenseBonus() const
{
    int total = 0;
    for (const auto& up : upgrades)
        if (up.unlocked) total += up.defenseBonus;
    return total;
}

int Skill::ApplyDamageBonus(int damage) const
{
    int bonus = GetTotalDamagePercentBonus() + GetMasteryDamageBonus();
    if (bonus == 0) return damage;
    return damage * (100 + bonus) / 100;
}

int Skill::ApplyCharacterMasteryBonus(int damage, const Character& caster) const
{
    float charMasteryBonus = caster.GetMasteryDamageBonus();
    if (charMasteryBonus <= 0.0f) return damage;
    return damage + static_cast<int>(damage * charMasteryBonus);
}

int Skill::GetEffectiveManaCost() const
{
    return std::max(0, manaCost - GetTotalManaCostReduction() - GetMasteryManaCostReduction());
}

int Skill::GetEffectiveCooldown() const
{
    return std::max(0, cooldown - GetTotalCooldownReduction() - GetMasteryCooldownReduction());
}

std::string Skill::GetDamageFormula() const
{
    return description;
}

int Skill::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    if (baseDamage <= 0) return 0;
    int damage = (baseDamage / 4) + weaponDamage + elementalBonus;
    damage = ApplyDamageBonus(damage);
    return damage;
}

// ---- Mastery System ----

int Skill::GetMasteryXPToLevel() const
{
    return std::max(200, masteryLevel * 200 + masteryLevel * masteryLevel * 5);
}

void Skill::GainMasteryXP(int xp)
{
    masteryXP += xp;
    while (masteryXP >= GetMasteryXPToLevel())
    {
        masteryXP -= GetMasteryXPToLevel();
        MasteryLevelUp();
    }
}

void Skill::MasteryLevelUp()
{
    masteryLevel++;
    masteryPoints++;
}

bool Skill::CanUnlockMasteryNode(int branch, int node) const
{
    if (branch < 0 || branch >= MASTERY_TREE_BRANCHES) return false;
    if (node < 0 || node >= MASTERY_NODES_PER_BRANCH) return false;
    if (masteryNodes[branch][node]) return false;
    if (masteryPoints <= 0) return false;
    if (masteryLevel < (node + 1) * 2) return false;
    if (node > 0 && !masteryNodes[branch][node - 1]) return false;
    return true;
}

bool Skill::UnlockMasteryNode(int branch, int node)
{
    if (!CanUnlockMasteryNode(branch, node)) return false;
    masteryNodes[branch][node] = true;
    masteryPoints--;
    return true;
}

static const char* MASTERY_BRANCH_NAMES[] = { "Damage", "Utility", "Special" };

int Skill::GetMasteryDamageBonus() const
{
    int total = 0;
    for (int n = 0; n < MASTERY_NODES_PER_BRANCH; ++n)
        if (masteryNodes[0][n]) total += 5;
    return total;
}

int Skill::GetMasteryCooldownReduction() const
{
    int total = 0;
    for (int n = 0; n < MASTERY_NODES_PER_BRANCH; ++n)
        if (masteryNodes[1][n]) total += 1;
    return total;
}

int Skill::GetMasteryManaCostReduction() const
{
    int total = 0;
    if (masteryNodes[1][2]) total += 2;
    if (masteryNodes[1][4]) total += 3;
    return total;
}

int Skill::GetMasteryXPGainBonus() const
{
    int total = 0;
    for (int n = 0; n < MASTERY_NODES_PER_BRANCH; ++n)
        if (masteryNodes[2][n]) total += 10;
    return total;
}
