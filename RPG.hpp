#ifndef RPG_HPP
#define RPG_HPP

// Main header file that includes all essential RPG components

// Character system
#include "Characters/Character.hpp"
#include "Characters/Player.hpp"
#include "Characters/Monster.hpp"
#include "Characters/Stats/Stats.hpp"
#include "Characters/Classes/Classes.hpp"
#include "Characters/Races/Races.hpp"

// Inventory and Equipment
#include "Inventory/Inventory.hpp"
#include "Items/Item.hpp"
#include "Items/Equipment/Equipment.hpp"

// Skills
#include "Skills/Skill.hpp"
#include "Skills/Skills.hpp"
#include "Skills/CommonSkills/CommonAttack.hpp"
#include "Skills/ClassSkills/Warrior/Warrior.hpp"
#include "Skills/ClassSkills/Mage/Mage.hpp"
#include "Skills/ClassSkills/Priest/Priest.hpp"
#include "Skills/ClassSkills/Archer/Archer.hpp"
#include "Skills/ClassSkills/Merchant/Merchant.hpp"

// Game Engine
#include "Engine/Game.hpp"
#include "Engine/Combat.hpp"
#include "Engine/SaveGame.hpp"
#include "Engine/Religion.hpp"

#endif
