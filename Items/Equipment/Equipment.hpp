#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

#include <memory>
#include "../Item.hpp"

class Equipment
{
public:
    Equipment() = default;

    // Weapons
    std::shared_ptr<Weapon> weapon;
    std::shared_ptr<Weapon> offhand;

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

    // Calculate total bonus stats from accessories
    int GetHealthBonus() const;
    int GetManaBonus() const;
};

#endif
