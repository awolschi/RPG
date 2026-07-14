#include "UniqueItems.hpp"
#include <algorithm>

// Static member definitions
std::vector<UniqueWeaponDef> UniqueItemRegistry::s_weapons;
std::vector<UniqueArmorDef> UniqueItemRegistry::s_armor;
std::vector<UniqueAccessoryDef> UniqueItemRegistry::s_accessories;
bool UniqueItemRegistry::s_initialized = false;

// Helper: apply difficulty multiplier to base + perDiff pattern
static int ScaledValue(int base, int perDiff, int diff)
{
    return base + perDiff * diff;
}

// ---- Initialize ----

void UniqueItemRegistry::Initialize()
{
    if (s_initialized) return;
    s_weapons.clear();
    s_armor.clear();
    s_accessories.clear();
    RegisterMageEpics();
    RegisterBossLegendaries();
    RegisterSetItems();
    s_initialized = true;
}

// ---- Mage Unique Epics (Section 4) ----

void UniqueItemRegistry::RegisterMageEpics()
{
    // === EPIC WEAPONS (5) ===

    s_weapons.push_back({
        "Staff of Frozen Memories", Rarity::Epic, WeaponType::Staff,
        45, 3, 40, ElementType::Ice, 15, 2,
        "Giant Spider", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_weapons.push_back({
        "Wand of the Blazing Sun", Rarity::Epic, WeaponType::Wand,
        35, 3, 50, ElementType::Fire, 20, 2,
        "Fire Elemental", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_weapons.push_back({
        "Tome of Whispered Truths", Rarity::Epic, WeaponType::Wand,
        40, 3, 55, ElementType::Arcane, 18, 2,
        "Lich", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_weapons.push_back({
        "Stormweaver's Focus", Rarity::Epic, WeaponType::Staff,
        50, 2, 35, ElementType::Lightning, 22, 2,
        "Sea Serpent", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_weapons.push_back({
        "Void-Touched Scepter", Rarity::Epic, WeaponType::Staff,
        55, 3, 45, ElementType::Arcane, 25, 2,
        "Void Knight", -1,
        ItemPassive::None, ItemPassive::None
    });

    // === EPIC ARMOR (5) — All Cloth ===

    s_armor.push_back({
        "Hood of the Frozen Eye", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Helmet,
        12, 2, {{ElementType::Ice, 5}}, 1,
        "Wolf", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_armor.push_back({
        "Mantle of the Flamecaller", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Chest,
        15, 2, {{ElementType::Fire, 6}}, 1,
        "Drake", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_armor.push_back({
        "Gloves of Arcane Weaving", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Gloves,
        10, 2, {{ElementType::Arcane, 4}}, 1,
        "Dark Elf Scout", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_armor.push_back({
        "Leggings of the Storm", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Pants,
        11, 2, {{ElementType::Lightning, 5}}, 1,
        "Highland Warrior", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_armor.push_back({
        "Sandals of the Void", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Boots,
        9, 2, {{ElementType::Arcane, 6}}, 1,
        "Shadow", -1,
        ItemPassive::None, ItemPassive::None
    });

    // === EPIC ACCESSORIES (3) ===

    s_accessories.push_back({
        "Circlet of Withered Dreams", Rarity::Epic,
        30, 5, 50, 8, ElementType::Arcane, 10, 2,
        "Wraith", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_accessories.push_back({
        "Pendant of the Tidal Sage", Rarity::Epic,
        40, 5, 60, 8, ElementType::Ice, 12, 2,
        "Siren", -1,
        ItemPassive::None, ItemPassive::None
    });

    s_accessories.push_back({
        "Ring of the Ember Court", Rarity::Epic,
        25, 5, 45, 8, ElementType::Fire, 15, 2,
        "Orc", -1,
        ItemPassive::None, ItemPassive::None
    });
}

// ---- Boss Legendaries (Section 5) ----

void UniqueItemRegistry::RegisterBossLegendaries()
{
    // ================================================================
    // BOSS 1: Farm Overseer — 9 items
    // ================================================================

    // #1 Overseer's Cleaver — Warrior Axe
    s_weapons.push_back({
        "Overseer's Cleaver", Rarity::Legendary, WeaponType::Axe,
        55, 0, 5, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // #2 Pitchfork of the Fields — Warrior Spear (closest: Mace)
    s_weapons.push_back({
        "Pitchfork of the Fields", Rarity::Legendary, WeaponType::Mace,
        50, 0, 0, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::Thorns25, ItemPassive::None
    });

    // #3 Rusty Spellgrimoire — Mage Wand
    s_weapons.push_back({
        "Rusty Spellgrimoire", Rarity::Legendary, WeaponType::Wand,
        35, 0, 30, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::ManaRegen5, ItemPassive::None
    });

    // #4 Scarecrow's Hood — Helmet
    s_armor.push_back({
        "Scarecrow's Hood", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        12, 0, {{ElementType::Fire, 5}}, 0,
        "Farm Overseer", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #5 Straw-Filled Vest — Chest
    s_armor.push_back({
        "Straw-Filled Vest", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        15, 0, {{ElementType::Ice, 3}}, 0,
        "Farm Overseer", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // #6 Work-Worn Gloves — Gloves
    s_armor.push_back({
        "Work-Worn Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        8, 0, {}, 0,
        "Farm Overseer", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #7 Fieldwalker Boots — Boots
    s_armor.push_back({
        "Fieldwalker Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        7, 0, {}, 0,
        "Farm Overseer", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // #8 Ring of the Harvest — Ring
    s_accessories.push_back({
        "Ring of the Harvest", Rarity::Legendary,
        40, 0, 20, 0, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::GoldFind30, ItemPassive::None
    });

    // #9 Pendant of the Landlord — Amulet
    s_accessories.push_back({
        "Pendant of the Landlord", Rarity::Legendary,
        30, 0, 15, 0, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::ExpBoost20, ItemPassive::None
    });

    // ================================================================
    // BOSS 2: Treant King — 11 items (Set: Verdant)
    // ================================================================

    // #10 Briarwood Staff — Mage Staff (Verdant set)
    s_weapons.push_back({
        "Briarwood Staff", Rarity::Legendary, WeaponType::Staff,
        80, 0, 60, ElementType::Fire, 30, 0,
        "Treant King", 1,
        ItemPassive::FireNoCd20, ItemPassive::None
    });

    // #11 Thornwhip — Archer Dagger
    s_weapons.push_back({
        "Thornwhip", Rarity::Legendary, WeaponType::Dagger,
        70, 0, 10, ElementType::Poison, 25, 0,
        "Treant King", -1,
        ItemPassive::PoisonDmgPlus25, ItemPassive::None
    });

    // #12 Living Root Bow — Archer Bow
    s_weapons.push_back({
        "Living Root Bow", Rarity::Legendary, WeaponType::Bow,
        75, 0, 15, ElementType::Poison, 20, 0,
        "Treant King", -1,
        ItemPassive::StatusExtend1, ItemPassive::None
    });

    // #13 Crown of Thorns — Helmet (Verdant set)
    s_armor.push_back({
        "Crown of Thorns", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        18, 0, {{ElementType::Fire, 8}}, 0,
        "Treant King", 1,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #14 Barkheart Chestguard — Warrior Chest (Verdant set)
    s_armor.push_back({
        "Barkheart Chestguard", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        25, 0, {{ElementType::Ice, 6}}, 0,
        "Treant King", 1,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #15 Mantle of the Wild — Mage/Priest Chest (Verdant set)
    s_armor.push_back({
        "Mantle of the Wild", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        16, 0, {{ElementType::Fire, 10}}, 0,
        "Treant King", 1,
        ItemPassive::SpellDmgBoost15, ItemPassive::None
    });

    // #16 Sapweave Gloves — Gloves
    s_armor.push_back({
        "Sapweave Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        10, 0, {{ElementType::Poison, 8}}, 0,
        "Treant King", -1,
        ItemPassive::Lifesteal15, ItemPassive::None
    });

    // #17 Rootwalker Leggings — Pants
    s_armor.push_back({
        "Rootwalker Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        12, 0, {{ElementType::Ice, 5}}, 0,
        "Treant King", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #18 Seedkeeper Boots — Boots (Verdant set)
    s_armor.push_back({
        "Seedkeeper Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        9, 0, {{ElementType::Poison, 6}}, 0,
        "Treant King", 1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // #19 Ring of Living Wood — Ring
    s_accessories.push_back({
        "Ring of Living Wood", Rarity::Legendary,
        50, 0, 25, 0, ElementType::Physical, 0, 0,
        "Treant King", -1,
        ItemPassive::AllResist5, ItemPassive::None
    });

    // #20 Amulet of the Forest Heart — Amulet
    s_accessories.push_back({
        "Amulet of the Forest Heart", Rarity::Legendary,
        35, 0, 40, 0, ElementType::Physical, 0, 0,
        "Treant King", -1,
        ItemPassive::ManaRegen5, ItemPassive::None
    });

    // ================================================================
    // BOSS 3: Kraken — 11 items (Set: Abyssal)
    // ================================================================

    // #21 Tidecaller's Staff — Mage Staff (Abyssal set)
    s_weapons.push_back({
        "Tidecaller's Staff", Rarity::Legendary, WeaponType::Staff,
        100, 0, 70, ElementType::Ice, 35, 0,
        "Kraken", 2,
        ItemPassive::IceFreezePlus1, ItemPassive::None
    });

    // #22 Kraken's Grip — Priest Mace (Abyssal set)
    s_weapons.push_back({
        "Kraken's Grip", Rarity::Legendary, WeaponType::Mace,
        85, 0, 40, ElementType::Ice, 30, 0,
        "Kraken", 2,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #23 Inkshot Longbow — Archer Bow
    s_weapons.push_back({
        "Inkshot Longbow", Rarity::Legendary, WeaponType::Bow,
        90, 0, 20, ElementType::Physical, 0, 0,
        "Kraken", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #24 Abyssal Helm — Warrior Helmet (Abyssal set)
    s_armor.push_back({
        "Abyssal Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        22, 0, {{ElementType::Ice, 12}}, 0,
        "Kraken", 2,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // #25 Tide-Sworn Robes — Mage Chest (Abyssal set)
    s_armor.push_back({
        "Tide-Sworn Robes", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        18, 0, {{ElementType::Ice, 14}}, 0,
        "Kraken", 2,
        ItemPassive::ManaCostReduce10, ItemPassive::None
    });

    // #26 Mantle of the Deep — Priest Chest
    s_armor.push_back({
        "Mantle of the Deep", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        17, 0, {{ElementType::Ice, 10}}, 0,
        "Kraken", -1,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #27 Tentacle-Woven Bracers — Gloves
    s_armor.push_back({
        "Tentacle-Woven Bracers", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        11, 0, {{ElementType::Lightning, 8}}, 0,
        "Kraken", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #28 Pressure-Tossed Leggings — Pants
    s_armor.push_back({
        "Pressure-Tossed Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        14, 0, {{ElementType::Ice, 6}}, 0,
        "Kraken", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #29 Coral Stalkers — Boots (Abyssal set)
    s_armor.push_back({
        "Coral Stalkers", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        10, 0, {{ElementType::Ice, 8}}, 0,
        "Kraken", 2,
        ItemPassive::FreezeImmune, ItemPassive::None
    });

    // #30 Ring of the Drowned King — Ring
    s_accessories.push_back({
        "Ring of the Drowned King", Rarity::Legendary,
        60, 0, 30, 0, ElementType::Ice, 15, 0,
        "Kraken", -1,
        ItemPassive::AllResist5, ItemPassive::None
    });

    // #31 Abyssal Pearl Pendant — Amulet
    s_accessories.push_back({
        "Abyssal Pearl Pendant", Rarity::Legendary,
        45, 0, 50, 0, ElementType::Ice, 20, 0,
        "Kraken", -1,
        ItemPassive::ManaOnKill15, ItemPassive::None
    });

    // ================================================================
    // BOSS 4: Elder Dragon — 12 items (Set: Dragonfire)
    // ================================================================

    // #32 Staff of the Eternal Flame — Mage Staff (Dragonfire set)
    s_weapons.push_back({
        "Staff of the Eternal Flame", Rarity::Legendary, WeaponType::Staff,
        140, 0, 90, ElementType::Fire, 50, 0,
        "Elder Dragon", 3,
        ItemPassive::FireNoCd20, ItemPassive::None
    });

    // #33 Dragonbone Wand — Mage Wand (Dragonfire set)
    s_weapons.push_back({
        "Dragonbone Wand", Rarity::Legendary, WeaponType::Wand,
        120, 0, 100, ElementType::Fire, 45, 0,
        "Elder Dragon", 3,
        ItemPassive::DoubleCast15, ItemPassive::None
    });

    // #34 Fang of the Elder — Warrior Sword (Dragonfire set)
    s_weapons.push_back({
        "Fang of the Elder", Rarity::Legendary, WeaponType::Sword,
        150, 0, 10, ElementType::Fire, 30, 0,
        "Elder Dragon", 3,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #35 Drake's Eye Scepter — Priest Scepter
    s_weapons.push_back({
        "Drake's Eye Scepter", Rarity::Legendary, WeaponType::Scepter,
        110, 0, 80, ElementType::Holy, 40, 0,
        "Elder Dragon", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #36 Dragonhide Helm — Warrior Helmet (Dragonfire set)
    s_armor.push_back({
        "Dragonhide Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        28, 0, {{ElementType::Fire, 15}}, 0,
        "Elder Dragon", 3,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #37 Drakefire Cowl — Mage Helmet (Dragonfire set)
    s_armor.push_back({
        "Drakefire Cowl", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        18, 0, {{ElementType::Fire, 12}}, 0,
        "Elder Dragon", 3,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #38 Scales of the Eternal — Warrior Chest (Dragonfire set)
    s_armor.push_back({
        "Scales of the Eternal", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        35, 0, {{ElementType::Fire, 10}, {ElementType::Ice, 5}}, 0,
        "Elder Dragon", 3,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #39 Mantle of Living Flame — Mage/Priest Chest
    s_armor.push_back({
        "Mantle of Living Flame", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        20, 0, {{ElementType::Fire, 14}}, 0,
        "Elder Dragon", -1,
        ItemPassive::SpellDmgLowHp30, ItemPassive::None
    });

    // #40 Gauntlets of the Brood — Warrior Gloves
    s_armor.push_back({
        "Gauntlets of the Brood", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        14, 0, {{ElementType::Fire, 8}}, 0,
        "Elder Dragon", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::None
    });

    // #41 Dragonclaw Boots — Boots
    s_armor.push_back({
        "Dragonclaw Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        12, 0, {{ElementType::Fire, 6}}, 0,
        "Elder Dragon", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #42 Ring of the Broodmother — Ring
    s_accessories.push_back({
        "Ring of the Broodmother", Rarity::Legendary,
        70, 0, 35, 0, ElementType::Fire, 20, 0,
        "Elder Dragon", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #43 Heart of the Dragon — Amulet
    s_accessories.push_back({
        "Heart of the Dragon", Rarity::Legendary,
        50, 0, 60, 0, ElementType::Fire, 35, 0,
        "Elder Dragon", -1,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // ================================================================
    // BOSS 5: Aran Warlord — 12 items (Set: Warlord's)
    // ================================================================

    // #44 Warlord's Decree — Mage Staff
    s_weapons.push_back({
        "Warlord's Decree", Rarity::Legendary, WeaponType::Staff,
        150, 0, 80, ElementType::Lightning, 40, 0,
        "Aran Warlord", -1,
        ItemPassive::LightningStun15, ItemPassive::None
    });

    // #45 Blade of the Aran — Warrior Sword (Warlord's set)
    s_weapons.push_back({
        "Blade of the Aran", Rarity::Legendary, WeaponType::Sword,
        170, 0, 5, ElementType::Physical, 0, 0,
        "Aran Warlord", 4,
        ItemPassive::PhysDmgBoost20, ItemPassive::None
    });

    // #46 Warbringer's Mace — Priest Mace (Warlord's set)
    s_weapons.push_back({
        "Warbringer's Mace", Rarity::Legendary, WeaponType::Mace,
        130, 0, 60, ElementType::Holy, 35, 0,
        "Aran Warlord", 4,
        ItemPassive::StunImmune, ItemPassive::None
    });

    // #47 Siegebow of Aran — Archer Bow
    s_weapons.push_back({
        "Siegebow of Aran", Rarity::Legendary, WeaponType::Bow,
        155, 0, 20, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::AtkSpeed20, ItemPassive::None
    });

    // #48 Warlord's Helm — Warrior Helmet (Warlord's set)
    s_armor.push_back({
        "Warlord's Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        30, 0, {{ElementType::Lightning, 10}}, 0,
        "Aran Warlord", 4,
        ItemPassive::TauntChance25, ItemPassive::None
    });

    // #49 Mantle of the Battle-Mage — Mage Chest
    s_armor.push_back({
        "Mantle of the Battle-Mage", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        22, 0, {{ElementType::Lightning, 12}}, 0,
        "Aran Warlord", -1,
        ItemPassive::ManaCostReduce15, ItemPassive::None
    });

    // #50 Battle-Hardened Plate — Warrior Chest (Warlord's set)
    s_armor.push_back({
        "Battle-Hardened Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        40, 0, {{ElementType::Lightning, 8}}, 0,
        "Aran Warlord", 4,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #51 Bracers of Command — Gloves
    s_armor.push_back({
        "Bracers of Command", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        13, 0, {{ElementType::Lightning, 6}}, 0,
        "Aran Warlord", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #52 Greaves of Conquest — Pants (Warlord's set)
    s_armor.push_back({
        "Greaves of Conquest", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Pants,
        15, 0, {}, 0,
        "Aran Warlord", 4,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // #53 Warboots of the Vanguard — Boots (Warlord's set)
    s_armor.push_back({
        "Warboots of the Vanguard", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Boots,
        11, 0, {}, 0,
        "Aran Warlord", 4,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #54 Signet of the Warlord — Ring
    s_accessories.push_back({
        "Signet of the Warlord", Rarity::Legendary,
        80, 0, 30, 0, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #55 Amulet of Broken Chains — Amulet
    s_accessories.push_back({
        "Amulet of Broken Chains", Rarity::Legendary,
        60, 0, 40, 0, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::StunImmune, ItemPassive::None
    });

    // ================================================================
    // BOSS 6: Void Lord — 13 items (Set: Voidborn)
    // ================================================================

    // #56 Wand of the Void — Mage Wand (Voidborn set)
    s_weapons.push_back({
        "Wand of the Void", Rarity::Legendary, WeaponType::Wand,
        160, 0, 120, ElementType::Arcane, 60, 0,
        "Void Lord", 5,
        ItemPassive::ManaCostReduce10, ItemPassive::ManaRegen5
    });

    // #57 Null-Staff — Mage Staff (Voidborn set)
    s_weapons.push_back({
        "Null-Staff", Rarity::Legendary, WeaponType::Staff,
        145, 0, 100, ElementType::Arcane, 55, 0,
        "Void Lord", 5,
        ItemPassive::ArcaneCrit30, ItemPassive::None
    });

    // #58 Soulreaper's Scythe — Warrior Axe (Voidborn set)
    s_weapons.push_back({
        "Soulreaper's Scythe", Rarity::Legendary, WeaponType::Axe,
        180, 0, 15, ElementType::Arcane, 30, 0,
        "Void Lord", 5,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // #59 Voidchaser Bow — Archer Bow
    s_weapons.push_back({
        "Voidchaser Bow", Rarity::Legendary, WeaponType::Bow,
        165, 0, 25, ElementType::Arcane, 25, 0,
        "Void Lord", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #60 Void-Touched Hood — Mage Helmet (Voidborn set)
    s_armor.push_back({
        "Void-Touched Hood", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 0, {{ElementType::Arcane, 18}}, 0,
        "Void Lord", 5,
        ItemPassive::ManaShield30, ItemPassive::None
    });

    // #61 Helm of the Null Knight — Warrior Helmet (Voidborn set)
    s_armor.push_back({
        "Helm of the Null Knight", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        32, 0, {{ElementType::Arcane, 14}}, 0,
        "Void Lord", 5,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #62 Void Shroud — Mage/Priest Chest (Voidborn set)
    s_armor.push_back({
        "Void Shroud", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        24, 0, {{ElementType::Arcane, 16}}, 0,
        "Void Lord", 5,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #63 Voidforged Plate — Warrior Chest
    s_armor.push_back({
        "Voidforged Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        42, 0, {{ElementType::Arcane, 10}}, 0,
        "Void Lord", -1,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #64 Null-Grip Gauntlets — Gloves (Voidborn set)
    s_armor.push_back({
        "Null-Grip Gauntlets", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        14, 0, {{ElementType::Arcane, 8}}, 0,
        "Void Lord", 5,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #65 Shadowthread Leggings — Pants
    s_armor.push_back({
        "Shadowthread Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        16, 0, {{ElementType::Arcane, 6}}, 0,
        "Void Lord", -1,
        ItemPassive::CooldownReduce1, ItemPassive::None
    });

    // #66 Voidstep Treads — Boots (Voidborn set)
    s_armor.push_back({
        "Voidstep Treads", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        13, 0, {{ElementType::Arcane, 10}}, 0,
        "Void Lord", 5,
        ItemPassive::FreezeImmune, ItemPassive::None
    });

    // #67 Band of the Void Lord — Ring
    s_accessories.push_back({
        "Band of the Void Lord", Rarity::Legendary,
        90, 0, 50, 0, ElementType::Arcane, 25, 0,
        "Void Lord", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #68 Pendant of Entropy — Amulet
    s_accessories.push_back({
        "Pendant of Entropy", Rarity::Legendary,
        70, 0, 70, 0, ElementType::Arcane, 30, 0,
        "Void Lord", -1,
        ItemPassive::CooldownReduce2, ItemPassive::None
    });

    // ================================================================
    // BOSS 7: Seraphim Council — 12 items (Set: Seraphic)
    // ================================================================

    // #69 Staff of Radiant Truth — Mage Staff (Seraphic set)
    s_weapons.push_back({
        "Staff of Radiant Truth", Rarity::Legendary, WeaponType::Staff,
        175, 0, 110, ElementType::Holy, 55, 0,
        "Seraphim Council", 6,
        ItemPassive::DoubleCast15, ItemPassive::HolyHealOnKill
    });

    // #70 Scepter of Judgment — Priest Scepter (Seraphic set)
    s_weapons.push_back({
        "Scepter of Judgment", Rarity::Legendary, WeaponType::Scepter,
        150, 0, 100, ElementType::Holy, 60, 0,
        "Seraphim Council", 6,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #71 Blade of Dawn — Warrior Sword (Seraphic set)
    s_weapons.push_back({
        "Blade of Dawn", Rarity::Legendary, WeaponType::Sword,
        190, 0, 15, ElementType::Holy, 35, 0,
        "Seraphim Council", 6,
        ItemPassive::PhysDmgBoost20, ItemPassive::HealOnKill10
    });

    // #72 Celestial Longbow — Archer Bow
    s_weapons.push_back({
        "Celestial Longbow", Rarity::Legendary, WeaponType::Bow,
        170, 0, 30, ElementType::Holy, 30, 0,
        "Seraphim Council", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #73 Halo of Ascension — Mage/Priest Helmet (Seraphic set)
    s_armor.push_back({
        "Halo of Ascension", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        22, 0, {{ElementType::Holy, 16}}, 0,
        "Seraphim Council", 6,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #74 Helm of the Fallen Seraph — Warrior Helmet (Seraphic set)
    s_armor.push_back({
        "Helm of the Fallen Seraph", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        34, 0, {{ElementType::Holy, 12}}, 0,
        "Seraphim Council", 6,
        ItemPassive::DamageReduce15, ItemPassive::TauntChance25
    });

    // #75 Raiment of the Celestial Choir — Priest Chest (Seraphic set)
    s_armor.push_back({
        "Raiment of the Celestial Choir", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        26, 0, {{ElementType::Holy, 18}}, 0,
        "Seraphim Council", 6,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #76 Mantle of Divine Light — Mage Chest
    s_armor.push_back({
        "Mantle of Divine Light", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        24, 0, {{ElementType::Holy, 14}}, 0,
        "Seraphim Council", -1,
        ItemPassive::ManaCostReduce15, ItemPassive::None
    });

    // #77 Seraphim's Grace Gloves — Gloves (Seraphic set)
    s_armor.push_back({
        "Seraphim's Grace Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        15, 0, {{ElementType::Holy, 10}}, 0,
        "Seraphim Council", 6,
        ItemPassive::Lifesteal15, ItemPassive::None
    });

    // #78 Sandals of the Ascended — Boots (Seraphic set)
    s_armor.push_back({
        "Sandals of the Ascended", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        14, 0, {{ElementType::Holy, 8}}, 0,
        "Seraphim Council", 6,
        ItemPassive::Revive50, ItemPassive::None
    });

    // #79 Radiant Band — Ring
    s_accessories.push_back({
        "Radiant Band", Rarity::Legendary,
        80, 0, 60, 0, ElementType::Holy, 20, 0,
        "Seraphim Council", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #80 Amulet of the Divine Eye — Amulet
    s_accessories.push_back({
        "Amulet of the Divine Eye", Rarity::Legendary,
        65, 0, 80, 0, ElementType::Holy, 30, 0,
        "Seraphim Council", -1,
        ItemPassive::ManaRegen10, ItemPassive::None
    });

    // ================================================================
    // BOSS 8: The Primordial One — 13 items (Set: Primordial)
    // ================================================================

    // #81 Archmage Staff *(existing)* — Mage Staff (Primordial set)
    s_weapons.push_back({
        "Archmage Staff", Rarity::Legendary, WeaponType::Staff,
        200, 0, 100, ElementType::Arcane, 50, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #82 Titan's Greatsword *(existing)* — Warrior Sword (Primordial set)
    s_weapons.push_back({
        "Titan's Greatsword", Rarity::Legendary, WeaponType::Sword,
        250, 0, 20, ElementType::Physical, 30, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #83 Divine Scepter *(existing)* — Priest Scepter (Primordial set)
    s_weapons.push_back({
        "Divine Scepter", Rarity::Legendary, WeaponType::Scepter,
        180, 0, 80, ElementType::Holy, 40, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #84 Shadowstrike Bow *(existing)* — Archer Bow (Primordial set)
    s_weapons.push_back({
        "Shadowstrike Bow", Rarity::Legendary, WeaponType::Bow,
        220, 0, 30, ElementType::Poison, 35, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #85 Gilded Blade *(existing)* — Merchant Sword (Primordial set)
    s_weapons.push_back({
        "Gilded Blade", Rarity::Legendary, WeaponType::Sword,
        200, 0, 40, ElementType::Fire, 25, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #86 Archmage Orb *(existing)* — Offhand Orb (closest: Scepter for now)
    s_weapons.push_back({
        "Archmage Orb", Rarity::Legendary, WeaponType::Scepter,
        80, 0, 200, ElementType::Arcane, 50, 0,
        "The Primordial One", 7,
        ItemPassive::ManaRegen10, ItemPassive::None
    });

    // #87 Archmage Robes *(existing)* — Mage Chest (Primordial set)
    s_armor.push_back({
        "Archmage Robes", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        35, 0, {{ElementType::Arcane, 8}}, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #88 Crown of the Primordial — Helmet (Primordial set)
    s_armor.push_back({
        "Crown of the Primordial", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        30, 0, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 0,
        "The Primordial One", 7,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #89 Primordial Plate — Warrior Chest
    s_armor.push_back({
        "Primordial Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        50, 0, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 0,
        "The Primordial One", -1,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #90 Gloves of Creation — Gloves (Primordial set)
    s_armor.push_back({
        "Gloves of Creation", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        18, 0, {{ElementType::Fire, 6}, {ElementType::Ice, 6}, {ElementType::Lightning, 6}, {ElementType::Arcane, 6}, {ElementType::Poison, 6}, {ElementType::Holy, 6}}, 0,
        "The Primordial One", -1,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // #91 Boots of the First Age — Boots (Primordial set)
    s_armor.push_back({
        "Boots of the First Age", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        16, 0, {{ElementType::Fire, 5}, {ElementType::Ice, 5}, {ElementType::Lightning, 5}, {ElementType::Arcane, 5}, {ElementType::Poison, 5}, {ElementType::Holy, 5}}, 0,
        "The Primordial One", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #92 Ring of the Primordial — Ring (Primordial set)
    s_accessories.push_back({
        "Ring of the Primordial", Rarity::Legendary,
        100, 0, 80, 0, ElementType::Arcane, 20, 0,
        "The Primordial One", 7,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #93 Amulet of the End — Amulet
    s_accessories.push_back({
        "Amulet of the End", Rarity::Legendary,
        80, 0, 100, 0, ElementType::Arcane, 25, 0,
        "The Primordial One", -1,
        ItemPassive::PhoenixRevive, ItemPassive::None
    });

    // ================================================================
    // BOSS 9: Arcane Construct — 11 items (Set: Arcanum)
    // ================================================================

    // #94 Tome of Infinite Spells — Offhand Book (closest: Wand) (Arcanum set)
    s_weapons.push_back({
        "Tome of Infinite Spells", Rarity::Legendary, WeaponType::Wand,
        60, 0, 150, ElementType::Arcane, 45, 0,
        "Arcane Construct", 8,
        ItemPassive::DoubleCast25, ItemPassive::None
    });

    // #95 Construct's Core Staff — Mage Staff (Arcanum set)
    s_weapons.push_back({
        "Construct's Core Staff", Rarity::Legendary, WeaponType::Staff,
        190, 0, 130, ElementType::Arcane, 65, 0,
        "Arcane Construct", 8,
        ItemPassive::ArcaneCrit30, ItemPassive::ManaRegen10
    });

    // #96 Arcane Shard Dagger — Archer Dagger
    s_weapons.push_back({
        "Arcane Shard Dagger", Rarity::Legendary, WeaponType::Dagger,
        170, 0, 20, ElementType::Arcane, 40, 0,
        "Arcane Construct", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #97 Manaheart Crown — Mage Helmet (Arcanum set)
    s_armor.push_back({
        "Manaheart Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        24, 0, {{ElementType::Arcane, 20}}, 0,
        "Arcane Construct", 8,
        ItemPassive::ManaCostReduce15, ItemPassive::ManaRegen5
    });

    // #98 Robes of the Infinite Weave — Mage Chest (Arcanum set)
    s_armor.push_back({
        "Robes of the Infinite Weave", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        28, 0, {{ElementType::Arcane, 18}}, 0,
        "Arcane Construct", 8,
        ItemPassive::DoubleCast15, ItemPassive::SpellDmgBoost15
    });

    // #99 Construct's Plated Carapace — Warrior Chest
    s_armor.push_back({
        "Construct's Plated Carapace", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        45, 0, {{ElementType::Arcane, 12}}, 0,
        "Arcane Construct", -1,
        ItemPassive::Thorns40, ItemPassive::DamageReduce10
    });

    // #100 Spell-Etched Gloves — Gloves (Arcanum set)
    s_armor.push_back({
        "Spell-Etched Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        16, 0, {{ElementType::Arcane, 10}}, 0,
        "Arcane Construct", 8,
        ItemPassive::CooldownReduce1, ItemPassive::None
    });

    // #101 Leggings of the Arcane Path — Pants
    s_armor.push_back({
        "Leggings of the Arcane Path", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        18, 0, {{ElementType::Arcane, 8}}, 0,
        "Arcane Construct", -1,
        ItemPassive::ManaShield30, ItemPassive::None
    });

    // #102 Boots of Mana Conduit — Boots (Arcanum set)
    s_armor.push_back({
        "Boots of Mana Conduit", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        15, 0, {{ElementType::Arcane, 12}}, 0,
        "Arcane Construct", 8,
        ItemPassive::ManaOnKill25, ItemPassive::None
    });

    // #103 Band of the Construct — Ring
    s_accessories.push_back({
        "Band of the Construct", Rarity::Legendary,
        90, 0, 100, 0, ElementType::Arcane, 30, 0,
        "Arcane Construct", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #104 Pendant of the Forgotten Lore — Amulet
    s_accessories.push_back({
        "Pendant of the Forgotten Lore", Rarity::Legendary,
        70, 0, 120, 0, ElementType::Arcane, 40, 0,
        "Arcane Construct", -1,
        ItemPassive::CooldownReduce2, ItemPassive::ManaRegen5
    });

    // ================================================================
    // BOSS 10: Chronos, the Time Ender — 12 items (Set: Chrono)
    // ================================================================

    // #105 Staff of Collapsed Timelines — Mage Staff (Chrono set)
    s_weapons.push_back({
        "Staff of Collapsed Timelines", Rarity::Legendary, WeaponType::Staff,
        220, 0, 150, ElementType::Arcane, 75, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::CooldownReduce2, ItemPassive::DoubleCast15
    });

    // #106 Chronoweaver Wand — Mage Wand (Chrono set)
    s_weapons.push_back({
        "Chronoweaver Wand", Rarity::Legendary, WeaponType::Wand,
        200, 0, 160, ElementType::Ice, 60, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::IceFreezePlus1, ItemPassive::ManaCostReduce15
    });

    // #107 Temporal Greatsword — Warrior Sword
    s_weapons.push_back({
        "Temporal Greatsword", Rarity::Legendary, WeaponType::Sword,
        260, 0, 20, ElementType::Physical, 40, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::CritChance30, ItemPassive::PhysDmgBoost20
    });

    // #108 Epoch Bow — Archer Bow
    s_weapons.push_back({
        "Epoch Bow", Rarity::Legendary, WeaponType::Bow,
        230, 0, 35, ElementType::Lightning, 45, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::AtkSpeed20, ItemPassive::CritChance30
    });

    // #109 Chronolord's Helm — Helmet (Chrono set)
    s_armor.push_back({
        "Chronolord's Helm", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        35, 0, {{ElementType::Fire, 15}, {ElementType::Ice, 15}, {ElementType::Lightning, 15}, {ElementType::Arcane, 15}, {ElementType::Poison, 15}, {ElementType::Holy, 15}}, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::AllResist10, ItemPassive::DamageReduce10
    });

    // #110 Robes of the Timeless Weave — Mage Chest (Chrono set)
    s_armor.push_back({
        "Robes of the Timeless Weave", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 0, {{ElementType::Arcane, 20}}, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::SpellDmgBoost20, ItemPassive::ManaRegen10
    });

    // #111 Temporal Plate of the Infinite — Warrior Chest
    s_armor.push_back({
        "Temporal Plate of the Infinite", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        55, 0, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    // #112 Gauntlets of Frozen Time — Gloves (Chrono set)
    s_armor.push_back({
        "Gauntlets of Frozen Time", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        20, 0, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::StatusExtend2, ItemPassive::None
    });

    // #113 Leggings of the Void Walk — Pants
    s_armor.push_back({
        "Leggings of the Void Walk", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        22, 0, {{ElementType::Arcane, 12}}, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #114 Sandals of Dimensional Rift — Boots
    s_armor.push_back({
        "Sandals of Dimensional Rift", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        18, 0, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::Dodge15, ItemPassive::FreezeImmune
    });

    // #115 Loop of Eternity — Ring (Chrono set)
    s_accessories.push_back({
        "Loop of Eternity", Rarity::Legendary,
        120, 0, 100, 0, ElementType::Arcane, 25, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::AllResist10, ItemPassive::ManaRegen5
    });

    // #116 Pendant of the Time Ender — Amulet
    s_accessories.push_back({
        "Pendant of the Time Ender", Rarity::Legendary,
        100, 0, 130, 0, ElementType::Arcane, 50, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::Revive50, ItemPassive::CooldownReduce1
    });
}

// ---- Set Items (from todoequip.md Sections 7) ----

void UniqueItemRegistry::RegisterSetItems()
{
    // ================================================================
    // Set 10: Archmage's Regalia (Epic, Mage, 4 pieces) — Arcane Construct
    // ================================================================

    // Archmage's Circlet — Helmet (Cloth)
    s_armor.push_back({
        "Archmage's Circlet", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Helmet,
        14, 2, {{ElementType::Arcane, 5}}, 1,
        "Arcane Construct", 10,
        ItemPassive::None, ItemPassive::None
    });

    // Archmage's Mantle — Chest (Cloth)
    s_armor.push_back({
        "Archmage's Mantle", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Chest,
        18, 2, {{ElementType::Arcane, 8}}, 1,
        "Arcane Construct", 10,
        ItemPassive::None, ItemPassive::None
    });

    // Archmage's Handwraps — Gloves (Cloth)
    s_armor.push_back({
        "Archmage's Handwraps", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Gloves,
        11, 2, {{ElementType::Arcane, 4}}, 1,
        "Arcane Construct", 10,
        ItemPassive::None, ItemPassive::None
    });

    // Archmage's Focus — Offhand Orb (closest: Scepter)
    s_weapons.push_back({
        "Archmage's Focus", Rarity::Epic, WeaponType::Scepter,
        8, 1, 30, ElementType::Arcane, 5, 1,
        "Arcane Construct", 10,
        ItemPassive::None, ItemPassive::None
    });

    // ================================================================
    // Set 11: Frostweaver's Raiment (Legendary, Mage, 3 pieces) — Chronos
    // ================================================================

    // Frostweaver's Crown — Helmet (Cloth)
    s_armor.push_back({
        "Frostweaver's Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        25, 0, {{ElementType::Ice, 10}}, 0,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // Frostweaver's Robe — Chest (Cloth)
    s_armor.push_back({
        "Frostweaver's Robe", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 0, {{ElementType::Ice, 12}}, 0,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // Frostweaver's Focus — Amulet
    s_accessories.push_back({
        "Frostweaver's Focus", Rarity::Legendary,
        60, 0, 180, 0, ElementType::Ice, 45, 0,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // ================================================================
    // Set 12: Gilded Fortune (Legendary, Merchant, 3 pieces) — Farm Overseer
    // ================================================================

    // Coin-Lord's Crown — Helmet (Cloth)
    s_armor.push_back({
        "Coin-Lord's Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 0, {{ElementType::Fire, 5}, {ElementType::Ice, 5}, {ElementType::Lightning, 5}, {ElementType::Arcane, 5}, {ElementType::Poison, 5}, {ElementType::Holy, 5}}, 0,
        "Farm Overseer", 12,
        ItemPassive::None, ItemPassive::None
    });

    // Merchant Prince's Vest — Chest (Cloth)
    s_armor.push_back({
        "Merchant Prince's Vest", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 0, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 0,
        "Farm Overseer", 12,
        ItemPassive::None, ItemPassive::None
    });

    // Gilded Signet — Ring
    s_accessories.push_back({
        "Gilded Signet", Rarity::Legendary,
        80, 0, 40, 0, ElementType::Arcane, 10, 0,
        "Farm Overseer", 12,
        ItemPassive::None, ItemPassive::None
    });

    // ================================================================
    // Set 13: Elementalist's Vestments (Epic, Any class, 4 pieces) — Seraphim Council
    // ================================================================

    // Elementalist's Circlet — Helmet (Cloth)
    s_armor.push_back({
        "Elementalist's Circlet", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Helmet,
        12, 2, {{ElementType::Fire, 4}}, 1,
        "Seraphim Council", 13,
        ItemPassive::None, ItemPassive::None
    });

    // Elementalist's Mantle — Chest (Cloth)
    s_armor.push_back({
        "Elementalist's Mantle", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Chest,
        16, 2, {{ElementType::Ice, 4}}, 1,
        "Seraphim Council", 13,
        ItemPassive::None, ItemPassive::None
    });

    // Elementalist's Leggings — Pants (Cloth)
    s_armor.push_back({
        "Elementalist's Leggings", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Pants,
        13, 2, {{ElementType::Lightning, 4}}, 1,
        "Seraphim Council", 13,
        ItemPassive::None, ItemPassive::None
    });

    // Elementalist's Sandals — Boots (Cloth)
    s_armor.push_back({
        "Elementalist's Sandals", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Boots,
        10, 2, {{ElementType::Fire, 3}}, 1,
        "Seraphim Council", 13,
        ItemPassive::None, ItemPassive::None
    });
}

// ---- Create ----

std::shared_ptr<Item> UniqueItemRegistry::Create(const std::string& name, int difficulty)
{
    // Look up weapon
    for (const auto& w : s_weapons)
    {
        if (w.name == name)
        {
            int dmg = ScaledValue(w.baseDamage, w.damagePerDiff, difficulty);
            int elemDmg = ScaledValue(w.baseElementDamage, w.elementDamagePerDiff, difficulty);
            int sellVal = (w.rarity == Rarity::Legendary) ? 250 : 100;
            auto item = std::make_shared<Weapon>(name, dmg, w.manaCost, static_cast<int>(w.rarity),
                                                 w.element, elemDmg, w.weaponType, w.passive1, w.passive2);
            item->sellValue = sellVal;
            item->setId = w.setId;
            return item;
        }
    }

    // Look up armor
    for (const auto& a : s_armor)
    {
        if (a.name == name)
        {
            int def = ScaledValue(a.baseDefense, a.defensePerDiff, difficulty);
            std::map<ElementType, int> resist;
            for (const auto& kv : a.baseResist)
            {
                resist[kv.first] = ScaledValue(kv.second, a.resistPerDiff, difficulty);
            }
            int sellVal = (a.rarity == Rarity::Legendary) ? 250 : 100;
            auto item = std::make_shared<Armor>(name, a.armorType, a.piece, def,
                                                static_cast<int>(a.rarity), resist, a.passive1, a.passive2);
            item->sellValue = sellVal;
            item->setId = a.setId;
            return item;
        }
    }

    // Look up accessory
    for (const auto& ac : s_accessories)
    {
        if (ac.name == name)
        {
            int hp = ScaledValue(ac.baseHealth, ac.healthPerDiff, difficulty);
            int mana = ScaledValue(ac.baseMana, ac.manaPerDiff, difficulty);
            int elemDmg = ScaledValue(ac.baseElementDamage, ac.elementDamagePerDiff, difficulty);
            int sellVal = (ac.rarity == Rarity::Legendary) ? 250 : 100;
            auto item = std::make_shared<Accessory>(name, hp, mana, static_cast<int>(ac.rarity),
                                                     ac.element, elemDmg, ac.passive1, ac.passive2);
            item->sellValue = sellVal;
            item->setId = ac.setId;
            return item;
        }
    }

    return nullptr;
}

// ---- Find (by name) ----

const UniqueWeaponDef* UniqueItemRegistry::FindWeapon(const std::string& name)
{
    for (const auto& w : s_weapons)
        if (w.name == name) return &w;
    return nullptr;
}

const UniqueArmorDef* UniqueItemRegistry::FindArmor(const std::string& name)
{
    for (const auto& a : s_armor)
        if (a.name == name) return &a;
    return nullptr;
}

const UniqueAccessoryDef* UniqueItemRegistry::FindAccessory(const std::string& name)
{
    for (const auto& ac : s_accessories)
        if (ac.name == name) return &ac;
    return nullptr;
}

// ---- Lookup by Drop Source ----

std::vector<const UniqueWeaponDef*> UniqueItemRegistry::GetWeaponsByDropSource(const std::string& enemyName)
{
    std::vector<const UniqueWeaponDef*> result;
    for (const auto& w : s_weapons)
        if (w.dropSource == enemyName) result.push_back(&w);
    return result;
}

std::vector<const UniqueArmorDef*> UniqueItemRegistry::GetArmorByDropSource(const std::string& enemyName)
{
    std::vector<const UniqueArmorDef*> result;
    for (const auto& a : s_armor)
        if (a.dropSource == enemyName) result.push_back(&a);
    return result;
}

std::vector<const UniqueAccessoryDef*> UniqueItemRegistry::GetAccessoriesByDropSource(const std::string& enemyName)
{
    std::vector<const UniqueAccessoryDef*> result;
    for (const auto& ac : s_accessories)
        if (ac.dropSource == enemyName) result.push_back(&ac);
    return result;
}

// ---- Lookup by Rarity ----

std::vector<const UniqueWeaponDef*> UniqueItemRegistry::GetWeaponsByRarity(Rarity r)
{
    std::vector<const UniqueWeaponDef*> result;
    for (const auto& w : s_weapons)
        if (w.rarity == r) result.push_back(&w);
    return result;
}

std::vector<const UniqueArmorDef*> UniqueItemRegistry::GetArmorByRarity(Rarity r)
{
    std::vector<const UniqueArmorDef*> result;
    for (const auto& a : s_armor)
        if (a.rarity == r) result.push_back(&a);
    return result;
}

std::vector<const UniqueAccessoryDef*> UniqueItemRegistry::GetAccessoriesByRarity(Rarity r)
{
    std::vector<const UniqueAccessoryDef*> result;
    for (const auto& ac : s_accessories)
        if (ac.rarity == r) result.push_back(&ac);
    return result;
}
