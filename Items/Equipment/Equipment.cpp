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

int Equipment::GetWeaponDamage() const
{
    int dmg = 0;
    if (weapon) dmg += weapon->damage;
    if (offhand) dmg += offhand->damage;
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

    return bonus;
}

void Equipment::ListEquipment(std::vector<std::string>& out) const
{
    auto add = [&](const std::string& slot, const std::string& name) {
        out.push_back(slot + ": " + name);
    };
    if (weapon)  add("Weapon", weapon->name);  else out.push_back("Weapon: (none)");
    if (offhand) add("Offhand", offhand->name);
    if (helmet)  add("Helmet", helmet->name);   else out.push_back("Helmet: (none)");
    if (chest)   add("Chest", chest->name);     else out.push_back("Chest: (none)");
    if (gloves)  add("Gloves", gloves->name);   else out.push_back("Gloves: (none)");
    if (pants)   add("Pants", pants->name);     else out.push_back("Pants: (none)");
    if (boots)   add("Boots", boots->name);     else out.push_back("Boots: (none)");
    if (ring1)   add("Ring 1", ring1->name);    else out.push_back("Ring 1: (none)");
    if (ring2)   add("Ring 2", ring2->name);    else out.push_back("Ring 2: (none)");
    if (amulet)  add("Amulet", amulet->name);   else out.push_back("Amulet: (none)");
}

ElementType Equipment::GetWeaponElement() const
{
    if (weapon && weapon->element != ElementType::Physical)
        return weapon->element;
    if (offhand && offhand->element != ElementType::Physical)
        return offhand->element;
    return ElementType::Physical;
}

int Equipment::GetElementalDamage(ElementType element) const
{
    int total = 0;
    if (weapon && weapon->element == element)
        total += weapon->elementDamage;
    if (offhand && offhand->element == element)
        total += offhand->elementDamage;
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