#include "Equipment.hpp"

int Equipment::GetTotalDefense() const
{
    int totalDefense = 0;

    if (helmet) totalDefense += helmet->defense;
    if (chest) totalDefense += chest->defense;
    if (gloves) totalDefense += gloves->defense;
    if (pants) totalDefense += pants->defense;
    if (boots) totalDefense += boots->defense;

    return totalDefense;
}

int Equipment::GetHealthBonus() const
{
    int bonus = 0;

    if (ring1) bonus += ring1->bonusHealth;
    if (ring2) bonus += ring2->bonusHealth;
    if (amulet) bonus += amulet->bonusHealth;

    return bonus;
}

int Equipment::GetManaBonus() const
{
    int bonus = 0;

    if (ring1) bonus += ring1->bonusMana;
    if (ring2) bonus += ring2->bonusMana;
    if (amulet) bonus += amulet->bonusMana;

    return bonus;
}