#include "Equipment.hpp"

int Equipment::GetTotalDefense() const
{
    int totalDefense = 0;

    if (helmet) totalDefense += helmet->defense;
    if (chest) totalDefense += chest->defense;
    if (gloves) totalDefense += gloves->defense;
    if (pants) totalDefense += pants->defense;
    if (boots) totalDefense += boots->defense;

    if (offhand)
    {
        if (auto oh = item_cast<Offhand>(offhand))
            totalDefense += oh->defense;
    }

    return totalDefense;
}

int Equipment::GetWeaponDamage() const
{
    int dmg = 0;
    if (weapon) dmg += weapon->damage;
    if (offhand)
    {
        if (auto oh = item_cast<Offhand>(offhand))
            dmg += oh->damageBonus;
    }
    return dmg;
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

    if (offhand)
    {
        if (auto oh = item_cast<Offhand>(offhand))
            bonus += oh->manaBonus;
    }

    return bonus;
}

int Equipment::GetOffhandDefense() const
{
    if (offhand)
    {
        if (auto oh = item_cast<Offhand>(offhand))
            return oh->defense;
    }
    return 0;
}

ElementType Equipment::GetWeaponElement() const
{
    if (weapon && weapon->element != ElementType::Physical)
        return weapon->element;
    if (offhand)
    {
        if (auto w = item_cast<Weapon>(offhand))
            if (w->element != ElementType::Physical)
                return w->element;
    }
    return ElementType::Physical;
}

int Equipment::GetElementalDamage(ElementType element) const
{
    int total = 0;
    if (weapon && weapon->element == element)
        total += weapon->elementDamage;
    if (offhand)
    {
        if (auto w = item_cast<Weapon>(offhand))
            if (w->element == element)
                total += w->elementDamage;
        if (auto oh = item_cast<Offhand>(offhand))
            if (oh->arcaneDamage > 0 && element == ElementType::Arcane)
                total += oh->arcaneDamage;
    }
    if (ring1 && ring1->element == element)
        total += ring1->elementDamage;
    if (ring2 && ring2->element == element)
        total += ring2->elementDamage;
    if (amulet && amulet->element == element)
        total += amulet->elementDamage;
    return total;
}

int Equipment::GetElementalReduction(ElementType element) const
{
    int total = 0;
    if (helmet) total += helmet->GetElementalReduction(element);
    if (chest)  total += chest->GetElementalReduction(element);
    if (gloves) total += gloves->GetElementalReduction(element);
    if (pants)  total += pants->GetElementalReduction(element);
    if (boots)  total += boots->GetElementalReduction(element);
    return total;
}
