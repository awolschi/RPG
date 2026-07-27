#include "CommonAttack.hpp"
#include "../../Characters/Character.hpp"
#include "../../Characters/Player.hpp"
#include "../../Characters/Classes/Classes.hpp"
#include "../../Engine/RNG.hpp"
#include "../../Items/Passives.hpp"
#include <algorithm>

CommonAttack::CommonAttack() : Skill("Attack", 0, 0, 25)
{
        description = "Basic attack dealing 8 base + STR/2 + weapon damage";
    InitializeUpgrades();
}

void CommonAttack::InitializeUpgrades()
{
    upgrades.clear();

    // Common upgrades for all classes
    upgrades.push_back({"Empower I", "+10% attack damage", 1, 0, false, 10});
    upgrades.push_back({"Empower II", "+20% attack damage", 2, 0, false, 20});

    // Class-specific upgrades
    switch (characterClass)
    {
        case CharacterClass::Warrior:
            upgrades.push_back({"Stun Chance I", "15% chance to stun for 1 turn", 1, 1, false});
            upgrades.push_back({"Stun Chance II", "25% chance to stun for 1 turn", 2, 1, false});
            break;
        case CharacterClass::Archer:
            upgrades.push_back({"Poison I", "Apply poison (3 turns, 5 dmg)", 1, 1, false});
            upgrades.push_back({"Poison II", "Apply poison (4 turns, 8 dmg)", 2, 1, false});
            break;
        case CharacterClass::Mage:
            upgrades.push_back({"Elemental Strike I", "+10 elemental damage on hit", 1, 1, false});
            upgrades.push_back({"Elemental Strike II", "+20 elemental damage on hit", 2, 1, false});
            break;
        case CharacterClass::Priest:
            upgrades.push_back({"Holy Strike I", "+15% damage vs undead", 1, 1, false});
            upgrades.push_back({"Holy Strike II", "+30% damage vs undead", 2, 1, false});
            break;
        case CharacterClass::Merchant:
            upgrades.push_back({"Golden Strike I", "Bonus damage based on gold (1% of gold, max 50)", 1, 1, false});
            upgrades.push_back({"Golden Strike II", "Bonus damage based on gold (2% of gold, max 100)", 2, 1, false});
            break;
    }

    // Utility upgrades (common)
    upgrades.push_back({"Efficiency I", "-1 mana cost (if applicable)", 1, 2, false});
    upgrades.push_back({"Efficiency II", "-1 cooldown (if applicable)", 2, 2, false});
}

void CommonAttack::Use(Character& caster, Character& target)
{
    // Basic attack is always available
    int damage = (baseDamage / 3) + (caster.GetStats().strength / 2) + caster.GetWeaponDamage();
    damage += caster.GetElementalBonus(element);
    damage = ApplyDamageBonus(damage);
    damage = ApplyCharacterMasteryBonus(damage, caster);
    target.TakeDamage(damage, caster.GetEffectiveElement(element));

    // Apply class-specific effects
    if (characterClass == CharacterClass::Warrior)
    {
        // Stun chance
        int stunChance = 0;
        if (upgrades.size() > 2 && upgrades[2].unlocked) stunChance += 15;
        if (upgrades.size() > 3 && upgrades[3].unlocked) stunChance += 10;
        if (stunChance > 0 && RNG::Next(100) < stunChance)
        {
            target.ApplyEffect(EffectType::Stun, 1, 0, "Attack");
        }
    }
    else if (characterClass == CharacterClass::Archer)
    {
        // Poison
        bool hasPoison1 = upgrades.size() > 2 && upgrades[2].unlocked;
        bool hasPoison2 = upgrades.size() > 3 && upgrades[3].unlocked;
        if (hasPoison1 || hasPoison2)
        {
            int duration = hasPoison2 ? 4 : 3;
            int potency = hasPoison2 ? 8 : 5;
            target.ApplyEffect(EffectType::Poison, duration, potency, "Attack");
        }
    }
    else if (characterClass == CharacterClass::Mage)
    {
        // Elemental strike - add bonus elemental damage based on intelligence
        int elementalBonus = 0;
        if (upgrades.size() > 2 && upgrades[2].unlocked) elementalBonus += 10;
        if (upgrades.size() > 3 && upgrades[3].unlocked) elementalBonus += 10;
        if (elementalBonus > 0)
        {
            // Scale with intelligence (INT/10 bonus damage)
            int intBonus = caster.GetStats().intelligence / 10;
            int totalBonus = elementalBonus + intBonus;
            target.TakeDamage(totalBonus, ElementType::Arcane);
        }
    }
    else if (characterClass == CharacterClass::Merchant)
    {
        // Golden strike - bonus damage based on gold carried
        bool hasGolden1 = upgrades.size() > 2 && upgrades[2].unlocked;
        bool hasGolden2 = upgrades.size() > 3 && upgrades[3].unlocked;
        if (hasGolden1 || hasGolden2)
        {
            // Get gold from player (need to cast to Player to access inventory)
            if (auto player = dynamic_cast<Player*>(&caster))
            {
                int gold = player->GetInventory().GetGold();
                int goldBonus = 0;
                if (hasGolden2)
                {
                    goldBonus = std::min(100, gold * 2 / 100);
                }
                else if (hasGolden1)
                {
                    goldBonus = std::min(50, gold / 100);
                }
                if (goldBonus > 0)
                {
                    target.TakeDamage(goldBonus, ElementType::Physical);
                }
            }
        }
    }

    GainXP(1);
}

std::string CommonAttack::GetDamageFormula() const
{
    return "base/3 + STR/2 + Weapon";
}

int CommonAttack::EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const
{
    int damage = (baseDamage / 3) + (stats.strength / 2) + weaponDamage + elementalBonus;
    return ApplyDamageBonus(damage);
}
