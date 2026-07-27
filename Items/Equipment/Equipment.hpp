#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

#include <memory>
#include <string>
#include <vector>
#include "../Item.hpp"

class Equipment
{
public:
    Equipment() = default;

    // Weapons
    std::shared_ptr<Weapon> weapon;
    std::shared_ptr<Item> offhand;

    // Armor
    std::shared_ptr<Armor> helmet;
    std::shared_ptr<Armor> chest;
    std::shared_ptr<Armor> gloves;
    std::shared_ptr<Armor> pants;
    std::shared_ptr<Armor> boots;

    // Accessories
    std::shared_ptr<Accessory> ring1;
    std::shared_ptr<Accessory> ring2;
    std::shared_ptr<Accessory> amulet;

    // Calculate total defense from equipped armor
    int GetTotalDefense() const;

    // Calculate total weapon damage from equipped weapons
    int GetWeaponDamage() const;

    // Calculate total bonus stats from accessories and offhand
    int GetHealthBonus() const;
    int GetManaBonus() const;

    // Offhand defense (from shields)
    int GetOffhandDefense() const;

    // Elemental
    ElementType GetWeaponElement() const;
    int GetElementalDamage(ElementType element) const;
    int GetElementalReduction(ElementType element) const;

};

#endif
