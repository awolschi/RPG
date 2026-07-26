#include "UniqueItems.hpp"
#include <algorithm>

// Static member definitions
std::vector<UniqueWeaponDef> UniqueItemRegistry::s_weapons;
std::vector<UniqueArmorDef> UniqueItemRegistry::s_armor;
std::vector<UniqueAccessoryDef> UniqueItemRegistry::s_accessories;
std::vector<UniqueOffhandDef> UniqueItemRegistry::s_offhands;
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
    s_offhands.clear();
    RegisterMageEpics();
    RegisterBossLegendaries();
    RegisterSetItems();
    RegisterMerchantUniques();
    RegisterWarriorLegendaries();
    RegisterWarriorEpics();
    RegisterMerchantEpics();
    RegisterPriestLegendaries();
    RegisterPriestEpics();
    RegisterArcherLegendaries();
    RegisterArcherEpics();
    RegisterAchievementItems();
    RegisterCitadelLegendaries();
    RegisterOffhands();

    // Enforce minimum scaling values so all uniques stay competitive with procedural loot
    // Epic weapon scaling: 35 + difficulty*28, so legendaries need damagePerDiff >= 28
    for (auto& w : s_weapons)
    {
        if (w.damagePerDiff < 28) w.damagePerDiff = 28;
        if (w.elementDamagePerDiff < 6) w.elementDamagePerDiff = 6;
    }
    // Epic armor scaling: 12 + difficulty*6
    for (auto& a : s_armor)
    {
        if (a.defensePerDiff < 5) a.defensePerDiff = 5;
        if (a.resistPerDiff < 2) a.resistPerDiff = 2;
    }
    // Epic accessory scaling
    for (auto& ac : s_accessories)
    {
        if (ac.healthPerDiff < 10) ac.healthPerDiff = 10;
        if (ac.manaPerDiff < 8) ac.manaPerDiff = 8;
        if (ac.elementDamagePerDiff < 2) ac.elementDamagePerDiff = 2;
    }
    // Offhand scaling
    for (auto& oh : s_offhands)
    {
        if (oh.defensePerDiff < 2) oh.defensePerDiff = 2;
        if (oh.damageBonusPerDiff < 3) oh.damageBonusPerDiff = 3;
        if (oh.arcaneDamagePerDiff < 2) oh.arcaneDamagePerDiff = 2;
        if (oh.manaBonusPerDiff < 4) oh.manaBonusPerDiff = 4;
    }

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
        55, 8, 5, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // #2 Pitchfork of the Fields — Warrior Spear (closest: Mace)
    s_weapons.push_back({
        "Pitchfork of the Fields", Rarity::Legendary, WeaponType::Mace,
        50, 8, 0, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::Thorns25, ItemPassive::None
    });

    // #3 Rusty Spellgrimoire — Mage Wand
    s_weapons.push_back({
        "Rusty Spellgrimoire", Rarity::Legendary, WeaponType::Wand,
        60, 8, 30, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::ManaRegen5, ItemPassive::None
    });

    // #4 Scarecrow's Hood — Helmet
    s_armor.push_back({
        "Scarecrow's Hood", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        12, 3, {{ElementType::Fire, 5}}, 1,
        "Farm Overseer", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #5 Straw-Filled Vest — Chest
    s_armor.push_back({
        "Straw-Filled Vest", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        15, 3, {{ElementType::Ice, 3}}, 1,
        "Farm Overseer", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // #6 Work-Worn Gloves — Gloves
    s_armor.push_back({
        "Work-Worn Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        8, 3, {}, 1,
        "Farm Overseer", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #7 Fieldwalker Boots — Boots
    s_armor.push_back({
        "Fieldwalker Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        7, 3, {}, 1,
        "Farm Overseer", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // #8 Ring of the Harvest — Ring
    s_accessories.push_back({
        "Ring of the Harvest", Rarity::Legendary,
        40, 10, 20, 5, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::GoldFind30, ItemPassive::None
    });

    // #9 Pendant of the Landlord — Amulet
    s_accessories.push_back({
        "Pendant of the Landlord", Rarity::Legendary,
        30, 10, 15, 5, ElementType::Physical, 0, 0,
        "Farm Overseer", -1,
        ItemPassive::ExpBoost20, ItemPassive::None
    });

    // ================================================================
    // BOSS 2: Treant King — 11 items (Set: Verdant)
    // ================================================================

    // #10 Briarwood Staff — Mage Staff (Verdant set)
    s_weapons.push_back({
        "Briarwood Staff", Rarity::Legendary, WeaponType::Staff,
        80, 12, 60, ElementType::Fire, 30, 0,
        "Treant King", 1,
        ItemPassive::FireNoCd20, ItemPassive::None
    });

    // #11 Thornwhip — Archer Dagger
    s_weapons.push_back({
        "Thornwhip", Rarity::Legendary, WeaponType::Dagger,
        70, 12, 10, ElementType::Poison, 25, 0,
        "Treant King", -1,
        ItemPassive::PoisonDmgPlus25, ItemPassive::None
    });

    // #12 Living Root Bow — Archer Bow
    s_weapons.push_back({
        "Living Root Bow", Rarity::Legendary, WeaponType::Bow,
        75, 12, 15, ElementType::Poison, 20, 0,
        "Treant King", -1,
        ItemPassive::StatusExtend1, ItemPassive::None
    });

    // #13 Crown of Thorns — Helmet (Verdant set)
    s_armor.push_back({
        "Crown of Thorns", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        18, 4, {{ElementType::Fire, 8}}, 2,
        "Treant King", 1,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #14 Barkheart Chestguard — Warrior Chest (Verdant set)
    s_armor.push_back({
        "Barkheart Chestguard", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        25, 4, {{ElementType::Ice, 6}}, 2,
        "Treant King", 1,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #15 Mantle of the Wild — Mage/Priest Chest (Verdant set)
    s_armor.push_back({
        "Mantle of the Wild", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        16, 4, {{ElementType::Fire, 10}}, 2,
        "Treant King", 1,
        ItemPassive::SpellDmgBoost15, ItemPassive::None
    });

    // #16 Sapweave Gloves — Gloves
    s_armor.push_back({
        "Sapweave Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        10, 4, {{ElementType::Poison, 8}}, 2,
        "Treant King", -1,
        ItemPassive::Lifesteal15, ItemPassive::None
    });

    // #17 Rootwalker Leggings — Pants
    s_armor.push_back({
        "Rootwalker Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        12, 4, {{ElementType::Ice, 5}}, 2,
        "Treant King", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #18 Seedkeeper Boots — Boots (Verdant set)
    s_armor.push_back({
        "Seedkeeper Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        9, 4, {{ElementType::Poison, 6}}, 2,
        "Treant King", 1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // #19 Ring of Living Wood — Ring
    s_accessories.push_back({
        "Ring of Living Wood", Rarity::Legendary,
        50, 15, 25, 8, ElementType::Physical, 0, 0,
        "Treant King", -1,
        ItemPassive::AllResist5, ItemPassive::None
    });

    // #20 Amulet of the Forest Heart — Amulet
    s_accessories.push_back({
        "Amulet of the Forest Heart", Rarity::Legendary,
        35, 15, 40, 8, ElementType::Physical, 0, 0,
        "Treant King", -1,
        ItemPassive::ManaRegen5, ItemPassive::None
    });

    // ================================================================
    // BOSS 3: Kraken — 11 items (Set: Abyssal)
    // ================================================================

    // #21 Tidecaller's Staff — Mage Staff (Abyssal set)
    s_weapons.push_back({
        "Tidecaller's Staff", Rarity::Legendary, WeaponType::Staff,
        100, 16, 70, ElementType::Ice, 35, 0,
        "Kraken", 2,
        ItemPassive::IceFreezePlus1, ItemPassive::None
    });

    // #22 Kraken's Grip — Priest Mace (Abyssal set)
    s_weapons.push_back({
        "Kraken's Grip", Rarity::Legendary, WeaponType::Mace,
        85, 16, 40, ElementType::Ice, 30, 0,
        "Kraken", 2,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #23 Inkshot Longbow — Archer Bow
    s_weapons.push_back({
        "Inkshot Longbow", Rarity::Legendary, WeaponType::Bow,
        90, 16, 20, ElementType::Physical, 0, 0,
        "Kraken", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #24 Abyssal Helm — Warrior Helmet (Abyssal set)
    s_armor.push_back({
        "Abyssal Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        22, 5, {{ElementType::Ice, 12}}, 2,
        "Kraken", 2,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // #25 Tide-Sworn Robes — Mage Chest (Abyssal set)
    s_armor.push_back({
        "Tide-Sworn Robes", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        18, 5, {{ElementType::Ice, 14}}, 2,
        "Kraken", 2,
        ItemPassive::ManaCostReduce10, ItemPassive::None
    });

    // #26 Mantle of the Deep — Priest Chest
    s_armor.push_back({
        "Mantle of the Deep", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        17, 5, {{ElementType::Ice, 10}}, 2,
        "Kraken", -1,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #27 Tentacle-Woven Bracers — Gloves
    s_armor.push_back({
        "Tentacle-Woven Bracers", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        11, 5, {{ElementType::Lightning, 8}}, 2,
        "Kraken", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #28 Pressure-Tossed Leggings — Pants
    s_armor.push_back({
        "Pressure-Tossed Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        14, 5, {{ElementType::Ice, 6}}, 2,
        "Kraken", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // #29 Coral Stalkers — Boots (Abyssal set)
    s_armor.push_back({
        "Coral Stalkers", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        10, 5, {{ElementType::Ice, 8}}, 2,
        "Kraken", 2,
        ItemPassive::FreezeImmune, ItemPassive::None
    });

    // #30 Ring of the Drowned King — Ring
    s_accessories.push_back({
        "Ring of the Drowned King", Rarity::Legendary,
        60, 20, 30, 10, ElementType::Ice, 15, 0,
        "Kraken", -1,
        ItemPassive::AllResist5, ItemPassive::None
    });

    // #31 Abyssal Pearl Pendant — Amulet
    s_accessories.push_back({
        "Abyssal Pearl Pendant", Rarity::Legendary,
        45, 20, 50, 10, ElementType::Ice, 20, 0,
        "Kraken", -1,
        ItemPassive::ManaOnKill15, ItemPassive::None
    });

    // ================================================================
    // BOSS 4: Elder Dragon — 12 items (Set: Dragonfire)
    // ================================================================

    // #32 Staff of the Eternal Flame — Mage Staff (Dragonfire set)
    s_weapons.push_back({
        "Staff of the Eternal Flame", Rarity::Legendary, WeaponType::Staff,
        140, 22, 90, ElementType::Fire, 50, 0,
        "Elder Dragon", 3,
        ItemPassive::FireNoCd20, ItemPassive::None
    });

    // #33 Dragonbone Wand — Mage Wand (Dragonfire set)
    s_weapons.push_back({
        "Dragonbone Wand", Rarity::Legendary, WeaponType::Wand,
        120, 22, 100, ElementType::Fire, 45, 0,
        "Elder Dragon", 3,
        ItemPassive::DoubleCast15, ItemPassive::None
    });

    // #34 Fang of the Elder — Warrior Sword (Dragonfire set)
    s_weapons.push_back({
        "Fang of the Elder", Rarity::Legendary, WeaponType::Sword,
        150, 22, 10, ElementType::Fire, 30, 0,
        "Elder Dragon", 3,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #35 Drake's Eye Scepter — Priest Scepter
    s_weapons.push_back({
        "Drake's Eye Scepter", Rarity::Legendary, WeaponType::Scepter,
        110, 22, 80, ElementType::Holy, 40, 0,
        "Elder Dragon", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #36 Dragonhide Helm — Warrior Helmet (Dragonfire set)
    s_armor.push_back({
        "Dragonhide Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        28, 6, {{ElementType::Fire, 15}}, 3,
        "Elder Dragon", 3,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #37 Drakefire Cowl — Mage Helmet (Dragonfire set)
    s_armor.push_back({
        "Drakefire Cowl", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        18, 6, {{ElementType::Fire, 12}}, 3,
        "Elder Dragon", 3,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #38 Scales of the Eternal — Warrior Chest (Dragonfire set)
    s_armor.push_back({
        "Scales of the Eternal", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        35, 6, {{ElementType::Fire, 10}, {ElementType::Ice, 5}}, 3,
        "Elder Dragon", 3,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #39 Mantle of Living Flame — Mage/Priest Chest
    s_armor.push_back({
        "Mantle of Living Flame", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        20, 6, {{ElementType::Fire, 14}}, 3,
        "Elder Dragon", -1,
        ItemPassive::SpellDmgLowHp30, ItemPassive::None
    });

    // #40 Gauntlets of the Brood — Warrior Gloves
    s_armor.push_back({
        "Gauntlets of the Brood", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        14, 6, {{ElementType::Fire, 8}}, 3,
        "Elder Dragon", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::None
    });

    // #41 Dragonclaw Boots — Boots
    s_armor.push_back({
        "Dragonclaw Boots", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        12, 6, {{ElementType::Fire, 6}}, 3,
        "Elder Dragon", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #42 Ring of the Broodmother — Ring
    s_accessories.push_back({
        "Ring of the Broodmother", Rarity::Legendary,
        70, 30, 35, 12, ElementType::Fire, 20, 0,
        "Elder Dragon", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #43 Heart of the Dragon — Amulet
    s_accessories.push_back({
        "Heart of the Dragon", Rarity::Legendary,
        50, 30, 60, 12, ElementType::Fire, 35, 0,
        "Elder Dragon", -1,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // ================================================================
    // BOSS 5: Aran Warlord — 12 items (Set: Warlord's)
    // ================================================================

    // #44 Warlord's Decree — Mage Staff
    s_weapons.push_back({
        "Warlord's Decree", Rarity::Legendary, WeaponType::Staff,
        150, 28, 80, ElementType::Lightning, 40, 0,
        "Aran Warlord", -1,
        ItemPassive::LightningStun15, ItemPassive::None
    });

    // #45 Blade of the Aran — Warrior Sword (Warlord's set)
    s_weapons.push_back({
        "Blade of the Aran", Rarity::Legendary, WeaponType::Sword,
        170, 28, 5, ElementType::Physical, 0, 0,
        "Aran Warlord", 4,
        ItemPassive::PhysDmgBoost20, ItemPassive::None
    });

    // #46 Warbringer's Mace — Priest Mace (Warlord's set)
    s_weapons.push_back({
        "Warbringer's Mace", Rarity::Legendary, WeaponType::Mace,
        130, 28, 60, ElementType::Holy, 35, 0,
        "Aran Warlord", 4,
        ItemPassive::StunImmune, ItemPassive::None
    });

    // #47 Siegebow of Aran — Archer Bow
    s_weapons.push_back({
        "Siegebow of Aran", Rarity::Legendary, WeaponType::Bow,
        155, 28, 20, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::AtkSpeed20, ItemPassive::None
    });

    // #48 Warlord's Helm — Warrior Helmet (Warlord's set)
    s_armor.push_back({
        "Warlord's Helm", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        30, 7, {{ElementType::Lightning, 10}}, 3,
        "Aran Warlord", 4,
        ItemPassive::TauntChance25, ItemPassive::None
    });

    // #49 Mantle of the Battle-Mage — Mage Chest
    s_armor.push_back({
        "Mantle of the Battle-Mage", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        22, 7, {{ElementType::Lightning, 12}}, 3,
        "Aran Warlord", -1,
        ItemPassive::ManaCostReduce15, ItemPassive::None
    });

    // #50 Battle-Hardened Plate — Warrior Chest (Warlord's set)
    s_armor.push_back({
        "Battle-Hardened Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        40, 7, {{ElementType::Lightning, 8}}, 3,
        "Aran Warlord", 4,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #51 Bracers of Command — Gloves
    s_armor.push_back({
        "Bracers of Command", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        13, 7, {{ElementType::Lightning, 6}}, 3,
        "Aran Warlord", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // #52 Greaves of Conquest — Pants (Warlord's set)
    s_armor.push_back({
        "Greaves of Conquest", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Pants,
        15, 7, {}, 3,
        "Aran Warlord", 4,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // #53 Warboots of the Vanguard — Boots (Warlord's set)
    s_armor.push_back({
        "Warboots of the Vanguard", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Boots,
        11, 7, {}, 3,
        "Aran Warlord", 4,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #54 Signet of the Warlord — Ring
    s_accessories.push_back({
        "Signet of the Warlord", Rarity::Legendary,
        80, 40, 30, 15, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #55 Amulet of Broken Chains — Amulet
    s_accessories.push_back({
        "Amulet of Broken Chains", Rarity::Legendary,
        60, 40, 40, 15, ElementType::Physical, 0, 0,
        "Aran Warlord", -1,
        ItemPassive::StunImmune, ItemPassive::None
    });

    // ================================================================
    // BOSS 6: Void Lord — 13 items (Set: Voidborn)
    // ================================================================

    // #56 Wand of the Void — Mage Wand (Voidborn set)
    s_weapons.push_back({
        "Wand of the Void", Rarity::Legendary, WeaponType::Wand,
        160, 32, 120, ElementType::Arcane, 60, 0,
        "Void Lord", 5,
        ItemPassive::ManaCostReduce10, ItemPassive::ManaRegen5
    });

    // #57 Null-Staff — Mage Staff (Voidborn set)
    s_weapons.push_back({
        "Null-Staff", Rarity::Legendary, WeaponType::Staff,
        145, 32, 100, ElementType::Arcane, 55, 0,
        "Void Lord", 5,
        ItemPassive::ArcaneCrit30, ItemPassive::None
    });

    // #58 Soulreaper's Scythe — Warrior Axe (Voidborn set)
    s_weapons.push_back({
        "Soulreaper's Scythe", Rarity::Legendary, WeaponType::Axe,
        180, 32, 15, ElementType::Arcane, 30, 0,
        "Void Lord", 5,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // #59 Voidchaser Bow — Archer Bow
    s_weapons.push_back({
        "Voidchaser Bow", Rarity::Legendary, WeaponType::Bow,
        165, 32, 25, ElementType::Arcane, 25, 0,
        "Void Lord", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #60 Void-Touched Hood — Mage Helmet (Voidborn set)
    s_armor.push_back({
        "Void-Touched Hood", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 8, {{ElementType::Arcane, 18}}, 4,
        "Void Lord", 5,
        ItemPassive::ManaShield30, ItemPassive::None
    });

    // #61 Helm of the Null Knight — Warrior Helmet (Voidborn set)
    s_armor.push_back({
        "Helm of the Null Knight", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        32, 8, {{ElementType::Arcane, 14}}, 4,
        "Void Lord", 5,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #62 Void Shroud — Mage/Priest Chest (Voidborn set)
    s_armor.push_back({
        "Void Shroud", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        24, 8, {{ElementType::Arcane, 16}}, 4,
        "Void Lord", 5,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #63 Voidforged Plate — Warrior Chest
    s_armor.push_back({
        "Voidforged Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        42, 8, {{ElementType::Arcane, 10}}, 4,
        "Void Lord", -1,
        ItemPassive::Thorns40, ItemPassive::None
    });

    // #64 Null-Grip Gauntlets — Gloves (Voidborn set)
    s_armor.push_back({
        "Null-Grip Gauntlets", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        14, 8, {{ElementType::Arcane, 8}}, 4,
        "Void Lord", 5,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #65 Shadowthread Leggings — Pants
    s_armor.push_back({
        "Shadowthread Leggings", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        16, 8, {{ElementType::Arcane, 6}}, 4,
        "Void Lord", -1,
        ItemPassive::CooldownReduce1, ItemPassive::None
    });

    // #66 Voidstep Treads — Boots (Voidborn set)
    s_armor.push_back({
        "Voidstep Treads", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        13, 8, {{ElementType::Arcane, 10}}, 4,
        "Void Lord", 5,
        ItemPassive::FreezeImmune, ItemPassive::None
    });

    // #67 Band of the Void Lord — Ring
    s_accessories.push_back({
        "Band of the Void Lord", Rarity::Legendary,
        90, 50, 50, 18, ElementType::Arcane, 25, 0,
        "Void Lord", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #68 Pendant of Entropy — Amulet
    s_accessories.push_back({
        "Pendant of Entropy", Rarity::Legendary,
        70, 50, 70, 18, ElementType::Arcane, 30, 0,
        "Void Lord", -1,
        ItemPassive::CooldownReduce2, ItemPassive::None
    });

    // ================================================================
    // BOSS 7: Seraphim Council — 12 items (Set: Seraphic)
    // ================================================================

    // #69 Staff of Radiant Truth — Mage Staff (Seraphic set)
    s_weapons.push_back({
        "Staff of Radiant Truth", Rarity::Legendary, WeaponType::Staff,
        175, 36, 110, ElementType::Holy, 55, 0,
        "Seraphim Council", 6,
        ItemPassive::DoubleCast15, ItemPassive::HolyHealOnKill
    });

    // #70 Scepter of Judgment — Priest Scepter (Seraphic set)
    s_weapons.push_back({
        "Scepter of Judgment", Rarity::Legendary, WeaponType::Scepter,
        150, 36, 100, ElementType::Holy, 60, 0,
        "Seraphim Council", 6,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // #71 Blade of Dawn — Warrior Sword (Seraphic set)
    s_weapons.push_back({
        "Blade of Dawn", Rarity::Legendary, WeaponType::Sword,
        190, 36, 15, ElementType::Holy, 35, 0,
        "Seraphim Council", 6,
        ItemPassive::PhysDmgBoost20, ItemPassive::HealOnKill10
    });

    // #72 Celestial Longbow — Archer Bow
    s_weapons.push_back({
        "Celestial Longbow", Rarity::Legendary, WeaponType::Bow,
        170, 36, 30, ElementType::Holy, 30, 0,
        "Seraphim Council", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #73 Halo of Ascension — Mage/Priest Helmet (Seraphic set)
    s_armor.push_back({
        "Halo of Ascension", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        22, 9, {{ElementType::Holy, 16}}, 4,
        "Seraphim Council", 6,
        ItemPassive::SpellDmgBoost20, ItemPassive::None
    });

    // #74 Helm of the Fallen Seraph — Warrior Helmet (Seraphic set)
    s_armor.push_back({
        "Helm of the Fallen Seraph", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        34, 9, {{ElementType::Holy, 12}}, 4,
        "Seraphim Council", 6,
        ItemPassive::DamageReduce15, ItemPassive::TauntChance25
    });

    // #75 Raiment of the Celestial Choir — Priest Chest (Seraphic set)
    s_armor.push_back({
        "Raiment of the Celestial Choir", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        26, 9, {{ElementType::Holy, 18}}, 4,
        "Seraphim Council", 6,
        ItemPassive::HealOnKill15, ItemPassive::None
    });

    // #76 Mantle of Divine Light — Mage Chest
    s_armor.push_back({
        "Mantle of Divine Light", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        24, 9, {{ElementType::Holy, 14}}, 4,
        "Seraphim Council", -1,
        ItemPassive::ManaCostReduce15, ItemPassive::None
    });

    // #77 Seraphim's Grace Gloves — Gloves (Seraphic set)
    s_armor.push_back({
        "Seraphim's Grace Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        15, 9, {{ElementType::Holy, 10}}, 4,
        "Seraphim Council", 6,
        ItemPassive::Lifesteal15, ItemPassive::None
    });

    // #78 Sandals of the Ascended — Boots (Seraphic set)
    s_armor.push_back({
        "Sandals of the Ascended", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        14, 9, {{ElementType::Holy, 8}}, 4,
        "Seraphim Council", 6,
        ItemPassive::Revive50, ItemPassive::None
    });

    // #79 Radiant Band — Ring
    s_accessories.push_back({
        "Radiant Band", Rarity::Legendary,
        80, 60, 60, 20, ElementType::Holy, 20, 0,
        "Seraphim Council", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #80 Amulet of the Divine Eye — Amulet
    s_accessories.push_back({
        "Amulet of the Divine Eye", Rarity::Legendary,
        65, 60, 80, 20, ElementType::Holy, 30, 0,
        "Seraphim Council", -1,
        ItemPassive::ManaRegen10, ItemPassive::None
    });

    // ================================================================
    // BOSS 8: The Primordial One — 13 items (Set: Primordial)
    // ================================================================

    // #81 Archmage Staff *(existing)* — Mage Staff (Primordial set)
    s_weapons.push_back({
        "Archmage Staff", Rarity::Legendary, WeaponType::Staff,
        200, 40, 100, ElementType::Arcane, 50, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #82 Titan's Greatsword *(existing)* — Warrior Sword (Primordial set)
    s_weapons.push_back({
        "Titan's Greatsword", Rarity::Legendary, WeaponType::Sword,
        250, 40, 20, ElementType::Physical, 30, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #83 Divine Scepter *(existing)* — Priest Scepter (Primordial set)
    s_weapons.push_back({
        "Divine Scepter", Rarity::Legendary, WeaponType::Scepter,
        180, 40, 80, ElementType::Holy, 40, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #84 Shadowstrike Bow *(existing)* — Archer Bow (Primordial set)
    s_weapons.push_back({
        "Shadowstrike Bow", Rarity::Legendary, WeaponType::Bow,
        220, 40, 30, ElementType::Poison, 35, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #85 Gilded Blade *(existing)* — Merchant Sword (Primordial set)
    s_weapons.push_back({
        "Gilded Blade", Rarity::Legendary, WeaponType::Sword,
        200, 40, 40, ElementType::Fire, 25, 0,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #86 Archmage Orb *(existing)* — Offhand Orb (closest: Scepter for now)
    s_weapons.push_back({
        "Archmage Orb", Rarity::Legendary, WeaponType::Scepter,
        130, 40, 200, ElementType::Arcane, 50, 0,
        "The Primordial One", 7,
        ItemPassive::ManaRegen10, ItemPassive::None
    });

    // #87 Archmage Robes *(existing)* — Mage Chest (Primordial set)
    s_armor.push_back({
        "Archmage Robes", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        35, 10, {{ElementType::Arcane, 8}}, 5,
        "The Primordial One", 7,
        ItemPassive::None, ItemPassive::None
    });

    // #88 Crown of the Primordial — Helmet (Primordial set)
    s_armor.push_back({
        "Crown of the Primordial", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        30, 10, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 5,
        "The Primordial One", 7,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #89 Primordial Plate — Warrior Chest
    s_armor.push_back({
        "Primordial Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        50, 10, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 5,
        "The Primordial One", -1,
        ItemPassive::DamageReduce15, ItemPassive::None
    });

    // #90 Gloves of Creation — Gloves (Primordial set)
    s_armor.push_back({
        "Gloves of Creation", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        18, 10, {{ElementType::Fire, 6}, {ElementType::Ice, 6}, {ElementType::Lightning, 6}, {ElementType::Arcane, 6}, {ElementType::Poison, 6}, {ElementType::Holy, 6}}, 5,
        "The Primordial One", -1,
        ItemPassive::Lifesteal25, ItemPassive::None
    });

    // #91 Boots of the First Age — Boots (Primordial set)
    s_armor.push_back({
        "Boots of the First Age", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        16, 10, {{ElementType::Fire, 5}, {ElementType::Ice, 5}, {ElementType::Lightning, 5}, {ElementType::Arcane, 5}, {ElementType::Poison, 5}, {ElementType::Holy, 5}}, 5,
        "The Primordial One", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #92 Ring of the Primordial — Ring (Primordial set)
    s_accessories.push_back({
        "Ring of the Primordial", Rarity::Legendary,
        100, 80, 80, 25, ElementType::Arcane, 20, 0,
        "The Primordial One", 7,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #93 Amulet of the End — Amulet
    s_accessories.push_back({
        "Amulet of the End", Rarity::Legendary,
        80, 80, 100, 25, ElementType::Arcane, 25, 0,
        "The Primordial One", -1,
        ItemPassive::PhoenixRevive, ItemPassive::None
    });

    // ================================================================
    // BOSS 9: Arcane Construct — 11 items (Set: Arcanum)
    // ================================================================

    // #94 Tome of Infinite Spells — Offhand Book (closest: Wand) (Arcanum set)
    s_weapons.push_back({
        "Tome of Infinite Spells", Rarity::Legendary, WeaponType::Wand,
        140, 42, 150, ElementType::Arcane, 45, 0,
        "Arcane Construct", 8,
        ItemPassive::DoubleCast25, ItemPassive::None
    });

    // #95 Construct's Core Staff — Mage Staff (Arcanum set)
    s_weapons.push_back({
        "Construct's Core Staff", Rarity::Legendary, WeaponType::Staff,
        190, 42, 130, ElementType::Arcane, 65, 0,
        "Arcane Construct", 8,
        ItemPassive::ArcaneCrit30, ItemPassive::ManaRegen10
    });

    // #96 Arcane Shard Dagger — Archer Dagger
    s_weapons.push_back({
        "Arcane Shard Dagger", Rarity::Legendary, WeaponType::Dagger,
        170, 42, 20, ElementType::Arcane, 40, 0,
        "Arcane Construct", -1,
        ItemPassive::CritChance30, ItemPassive::None
    });

    // #97 Manaheart Crown — Mage Helmet (Arcanum set)
    s_armor.push_back({
        "Manaheart Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        24, 11, {{ElementType::Arcane, 20}}, 5,
        "Arcane Construct", 8,
        ItemPassive::ManaCostReduce15, ItemPassive::ManaRegen5
    });

    // #98 Robes of the Infinite Weave — Mage Chest (Arcanum set)
    s_armor.push_back({
        "Robes of the Infinite Weave", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        28, 11, {{ElementType::Arcane, 18}}, 5,
        "Arcane Construct", 8,
        ItemPassive::DoubleCast15, ItemPassive::SpellDmgBoost15
    });

    // #99 Construct's Plated Carapace — Warrior Chest
    s_armor.push_back({
        "Construct's Plated Carapace", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        45, 11, {{ElementType::Arcane, 12}}, 5,
        "Arcane Construct", -1,
        ItemPassive::Thorns40, ItemPassive::DamageReduce10
    });

    // #100 Spell-Etched Gloves — Gloves (Arcanum set)
    s_armor.push_back({
        "Spell-Etched Gloves", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        16, 11, {{ElementType::Arcane, 10}}, 5,
        "Arcane Construct", 8,
        ItemPassive::CooldownReduce1, ItemPassive::None
    });

    // #101 Leggings of the Arcane Path — Pants
    s_armor.push_back({
        "Leggings of the Arcane Path", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        18, 11, {{ElementType::Arcane, 8}}, 5,
        "Arcane Construct", -1,
        ItemPassive::ManaShield30, ItemPassive::None
    });

    // #102 Boots of Mana Conduit — Boots (Arcanum set)
    s_armor.push_back({
        "Boots of Mana Conduit", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        15, 11, {{ElementType::Arcane, 12}}, 5,
        "Arcane Construct", 8,
        ItemPassive::ManaOnKill25, ItemPassive::None
    });

    // #103 Band of the Construct — Ring
    s_accessories.push_back({
        "Band of the Construct", Rarity::Legendary,
        90, 100, 100, 28, ElementType::Arcane, 30, 0,
        "Arcane Construct", -1,
        ItemPassive::AllResist10, ItemPassive::None
    });

    // #104 Pendant of the Forgotten Lore — Amulet
    s_accessories.push_back({
        "Pendant of the Forgotten Lore", Rarity::Legendary,
        70, 100, 120, 28, ElementType::Arcane, 40, 0,
        "Arcane Construct", -1,
        ItemPassive::CooldownReduce2, ItemPassive::ManaRegen5
    });

    // ================================================================
    // BOSS 10: Chronos, the Time Ender — 12 items (Set: Chrono)
    // ================================================================

    // #105 Staff of Collapsed Timelines — Mage Staff (Chrono set)
    s_weapons.push_back({
        "Staff of Collapsed Timelines", Rarity::Legendary, WeaponType::Staff,
        220, 42, 150, ElementType::Arcane, 75, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::CooldownReduce2, ItemPassive::DoubleCast15
    });

    // #106 Chronoweaver Wand — Mage Wand (Chrono set)
    s_weapons.push_back({
        "Chronoweaver Wand", Rarity::Legendary, WeaponType::Wand,
        200, 42, 160, ElementType::Ice, 60, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::IceFreezePlus1, ItemPassive::ManaCostReduce15
    });

    // #107 Temporal Greatsword — Warrior Sword
    s_weapons.push_back({
        "Temporal Greatsword", Rarity::Legendary, WeaponType::Sword,
        260, 42, 20, ElementType::Physical, 40, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::CritChance30, ItemPassive::PhysDmgBoost20
    });

    // #108 Epoch Bow — Archer Bow
    s_weapons.push_back({
        "Epoch Bow", Rarity::Legendary, WeaponType::Bow,
        230, 42, 35, ElementType::Lightning, 45, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::AtkSpeed20, ItemPassive::CritChance30
    });

    // #109 Chronolord's Helm — Helmet (Chrono set)
    s_armor.push_back({
        "Chronolord's Helm", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        35, 11, {{ElementType::Fire, 15}, {ElementType::Ice, 15}, {ElementType::Lightning, 15}, {ElementType::Arcane, 15}, {ElementType::Poison, 15}, {ElementType::Holy, 15}}, 5,
        "Chronos, the Time Ender", 9,
        ItemPassive::AllResist10, ItemPassive::DamageReduce10
    });

    // #110 Robes of the Timeless Weave — Mage Chest (Chrono set)
    s_armor.push_back({
        "Robes of the Timeless Weave", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 11, {{ElementType::Arcane, 20}}, 5,
        "Chronos, the Time Ender", 9,
        ItemPassive::SpellDmgBoost20, ItemPassive::ManaRegen10
    });

    // #111 Temporal Plate of the Infinite — Warrior Chest
    s_armor.push_back({
        "Temporal Plate of the Infinite", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        55, 11, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 5,
        "Chronos, the Time Ender", -1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    // #112 Gauntlets of Frozen Time — Gloves (Chrono set)
    s_armor.push_back({
        "Gauntlets of Frozen Time", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        20, 11, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 5,
        "Chronos, the Time Ender", 9,
        ItemPassive::StatusExtend2, ItemPassive::None
    });

    // #113 Leggings of the Void Walk — Pants
    s_armor.push_back({
        "Leggings of the Void Walk", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Pants,
        22, 11, {{ElementType::Arcane, 12}}, 5,
        "Chronos, the Time Ender", -1,
        ItemPassive::Dodge20, ItemPassive::None
    });

    // #114 Sandals of Dimensional Rift — Boots
    s_armor.push_back({
        "Sandals of Dimensional Rift", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Boots,
        18, 11, {{ElementType::Fire, 10}, {ElementType::Ice, 10}, {ElementType::Lightning, 10}, {ElementType::Arcane, 10}, {ElementType::Poison, 10}, {ElementType::Holy, 10}}, 5,
        "Chronos, the Time Ender", -1,
        ItemPassive::Dodge15, ItemPassive::FreezeImmune
    });

    // #115 Loop of Eternity — Ring (Chrono set)
    s_accessories.push_back({
        "Loop of Eternity", Rarity::Legendary,
        120, 100, 100, 28, ElementType::Arcane, 25, 0,
        "Chronos, the Time Ender", 9,
        ItemPassive::AllResist10, ItemPassive::ManaRegen5
    });

    // #116 Pendant of the Time Ender — Amulet
    s_accessories.push_back({
        "Pendant of the Time Ender", Rarity::Legendary,
        100, 100, 130, 28, ElementType::Arcane, 50, 0,
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
        25, 11, {{ElementType::Ice, 10}}, 5,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // Frostweaver's Robe — Chest (Cloth)
    s_armor.push_back({
        "Frostweaver's Robe", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 11, {{ElementType::Ice, 12}}, 5,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // Frostweaver's Focus — Amulet
    s_accessories.push_back({
        "Frostweaver's Focus", Rarity::Legendary,
        60, 100, 180, 28, ElementType::Ice, 45, 0,
        "Chronos, the Time Ender", 11,
        ItemPassive::None, ItemPassive::None
    });

    // ================================================================
    // Set 12: Gilded Fortune (Legendary, Merchant, 3 pieces) — Farm Overseer
    // ================================================================

    // Coin-Lord's Crown — Helmet (Cloth)
    s_armor.push_back({
        "Coin-Lord's Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 3, {{ElementType::Fire, 5}, {ElementType::Ice, 5}, {ElementType::Lightning, 5}, {ElementType::Arcane, 5}, {ElementType::Poison, 5}, {ElementType::Holy, 5}}, 1,
        "Farm Overseer", 12,
        ItemPassive::None, ItemPassive::None
    });

    // Merchant Prince's Vest — Chest (Cloth)
    s_armor.push_back({
        "Merchant Prince's Vest", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        30, 3, {{ElementType::Fire, 8}, {ElementType::Ice, 8}, {ElementType::Lightning, 8}, {ElementType::Arcane, 8}, {ElementType::Poison, 8}, {ElementType::Holy, 8}}, 1,
        "Farm Overseer", 12,
        ItemPassive::None, ItemPassive::None
    });

    // Gilded Signet — Ring
    s_accessories.push_back({
        "Gilded Signet", Rarity::Legendary,
        80, 10, 40, 5, ElementType::Arcane, 10, 0,
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

// ---- Merchant Unique Legendaries ----

void UniqueItemRegistry::RegisterMerchantUniques()
{
    // Merchant-themed items spread across multiple bosses

    // Coinlord's Cudgel — Mace (drops from Farm Overseer)
    s_weapons.push_back({
        "Coinlord's Cudgel", Rarity::Legendary, WeaponType::Mace,
        60, 8, 15, ElementType::Physical, 10, 0,
        "Farm Overseer", 17,
        ItemPassive::GoldFind30, ItemPassive::None
    });

    // Auctioneer's Blade — Sword (drops from Aran Warlord)
    s_weapons.push_back({
        "Auctioneer's Blade", Rarity::Legendary, WeaponType::Sword,
        160, 28, 25, ElementType::Physical, 20, 0,
        "Aran Warlord", -1,
        ItemPassive::CritChance20, ItemPassive::GoldFind30
    });

    // Ledger of Endless Wealth — Wand (drops from Arcane Construct)
    s_weapons.push_back({
        "Ledger of Endless Wealth", Rarity::Legendary, WeaponType::Wand,
        180, 32, 140, ElementType::Arcane, 40, 0,
        "Arcane Construct", -1,
        ItemPassive::ManaRegen10, ItemPassive::ExpBoost20
    });

    // Merchant's Traveling Cloak — Cloth Chest (drops from Void Lord)
    s_armor.push_back({
        "Merchant's Traveling Cloak", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        22, 8, {{ElementType::Arcane, 8}}, 4,
        "Void Lord", 17,
        ItemPassive::Dodge15, ItemPassive::GoldFind30
    });

    // Bullion Greaves — Plate Boots (drops from Kraken)
    s_armor.push_back({
        "Bullion Greaves", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Boots,
        14, 5, {{ElementType::Ice, 6}}, 2,
        "Kraken", -1,
        ItemPassive::HealOnKill10, ItemPassive::GoldFind30
    });

    // Gilded Bracers — Plate Gloves (drops from Elder Dragon)
    s_armor.push_back({
        "Gilded Bracers", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        16, 6, {{ElementType::Fire, 5}}, 3,
        "Elder Dragon", -1,
        ItemPassive::CritChance20, ItemPassive::GoldFind30
    });

    // Ring of the Golden Touch — Ring (drops from Seraphim Council)
    s_accessories.push_back({
        "Ring of the Golden Touch", Rarity::Legendary,
        50, 60, 30, 20, ElementType::Holy, 15, 0,
        "Seraphim Council", 17,
        ItemPassive::GoldFind30, ItemPassive::ExpBoost20
    });

    // Amulet of Prosperity — Amulet (drops from Chronos)
    s_accessories.push_back({
        "Amulet of Prosperity", Rarity::Legendary,
        60, 100, 50, 28, ElementType::Arcane, 20, 0,
        "Chronos, the Time Ender", 17,
        ItemPassive::ExpBoost20, ItemPassive::ManaRegen5
    });
}

// ---- Merchant Unique Epics ----

void UniqueItemRegistry::RegisterMerchantEpics()
{
    // === EPIC WEAPONS (4) ===

    // Gilded Longsword — Sword (drops from Orc)
    s_weapons.push_back({
        "Gilded Longsword", Rarity::Epic, WeaponType::Sword,
        55, 2, 12, ElementType::Physical, 5, 1,
        "Orc", -1,
        ItemPassive::GoldFind30, ItemPassive::PhysDmgBoost15
    });

    // Profitkeeper's Staff — Staff (drops from Lich)
    s_weapons.push_back({
        "Profitkeeper's Staff", Rarity::Epic, WeaponType::Staff,
        42, 2, 45, ElementType::Arcane, 12, 2,
        "Lich", -1,
        ItemPassive::GoldFind30, ItemPassive::SpellDmgBoost15
    });

    // Hoardseeker's Bow — Bow (drops from Drake)
    s_weapons.push_back({
        "Hoardseeker's Bow", Rarity::Epic, WeaponType::Bow,
        50, 2, 18, ElementType::Physical, 4, 1,
        "Drake", -1,
        ItemPassive::GoldFind30, ItemPassive::CritChance20
    });

    // Bargainer's Mace — Mace (drops from Highland Warrior)
    s_weapons.push_back({
        "Bargainer's Mace", Rarity::Epic, WeaponType::Mace,
        58, 2, 10, ElementType::Physical, 8, 1,
        "Highland Warrior", -1,
        ItemPassive::GoldFind30, ItemPassive::DamageReduce10
    });

    // === EPIC ARMOR (4) ===

    // Merchant's Silk Robe — Cloth Chest (drops from Giant Spider)
    s_armor.push_back({
        "Merchant's Silk Robe", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Chest,
        8, 1, {{ElementType::Arcane, 4}}, 1,
        "Giant Spider", -1,
        ItemPassive::GoldFind30, ItemPassive::Dodge15
    });

    // Padded Coin Purse Helm — Leather Helmet (drops from Wolf)
    s_armor.push_back({
        "Padded Coin Purse Helm", Rarity::Epic, ArmorType::Leather, ArmorPiece::Helmet,
        9, 1, {{ElementType::Physical, 3}}, 1,
        "Wolf", -1,
        ItemPassive::GoldFind30, ItemPassive::ExpBoost20
    });

    // Bullion-Plated Pauldrons — Plate Chest (drops from Fire Elemental)
    s_armor.push_back({
        "Bullion-Plated Pauldrons", Rarity::Epic, ArmorType::Plate, ArmorPiece::Chest,
        12, 1, {{ElementType::Fire, 4}}, 1,
        "Fire Elemental", -1,
        ItemPassive::GoldFind30, ItemPassive::DamageReduce10
    });

    // Smuggler's Leather Boots — Leather Boots (drops from Dark Elf Scout)
    s_armor.push_back({
        "Smuggler's Leather Boots", Rarity::Epic, ArmorType::Leather, ArmorPiece::Boots,
        7, 1, {{ElementType::Poison, 3}}, 1,
        "Dark Elf Scout", -1,
        ItemPassive::GoldFind30, ItemPassive::Dodge15
    });

    // === EPIC ACCESSORIES (3) ===

    // Coin Purse Ring — Ring (drops from Shadow)
    s_accessories.push_back({
        "Coin Purse Ring", Rarity::Epic,
        20, 1, 10, 1, ElementType::Physical, 0, 0,
        "Shadow", -1,
        ItemPassive::GoldFind30, ItemPassive::ExpBoost20
    });

    // Ledger Charm — Amulet (drops from Wraith)
    s_accessories.push_back({
        "Ledger Charm", Rarity::Epic,
        15, 1, 25, 2, ElementType::Arcane, 6, 1,
        "Wraith", -1,
        ItemPassive::GoldFind30, ItemPassive::HealOnKill10
    });

    // Merchant's Seal — Ring (drops from Siren)
    s_accessories.push_back({
        "Merchant's Seal", Rarity::Epic,
        25, 1, 15, 1, ElementType::Ice, 4, 1,
        "Siren", -1,
        ItemPassive::GoldFind30, ItemPassive::ManaRegen5
    });
}

// ---- Achievement Reward Items ----

void UniqueItemRegistry::RegisterAchievementItems()
{
    // Items awarded by specific achievements — no drop source

    // Innkeeper's Key — "Inn Regular" (rest 100 times)
    s_weapons.push_back({
        "Innkeeper's Key", Rarity::Legendary, WeaponType::Dagger,
        150, 42, 20, ElementType::Holy, 15, 0,
        "", -1,
        ItemPassive::HealOnKill15, ItemPassive::GoldFind30
    });

    // Gatherer's Pouch — "Gather Legend" (50 gather quests)
    s_accessories.push_back({
        "Gatherer's Pouch", Rarity::Legendary,
        40, 100, 40, 28, ElementType::Physical, 0, 0,
        "", -1,
        ItemPassive::GoldFind30, ItemPassive::ExpBoost20
    });

    // Explorer's Compass — "World Walker" (15 explore quests)
    s_accessories.push_back({
        "Explorer's Compass", Rarity::Legendary,
        50, 100, 30, 28, ElementType::Physical, 0, 0,
        "", -1,
        ItemPassive::Dodge20, ItemPassive::ExpBoost20
    });

    // World Map — "World Explorer" (visit all areas)
    s_accessories.push_back({
        "World Map", Rarity::Legendary,
        60, 100, 60, 28, ElementType::Physical, 0, 0,
        "", -1,
        ItemPassive::AllResist10, ItemPassive::ManaRegen10
    });

    // Shadow Cloak — "Secret Master" (10 secret areas)
    s_armor.push_back({
        "Shadow Cloak", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        22, 11, {{ElementType::Arcane, 12}}, 5,
        "", -1,
        ItemPassive::Dodge20, ItemPassive::ManaRegen5
    });

    // Hero's Crown — "Legendary Hero" (level 50)
    s_armor.push_back({
        "Hero's Crown", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        30, 11, {{ElementType::Holy, 10}}, 5,
        "", -1,
        ItemPassive::DamageReduce15, ItemPassive::ExpBoost20
    });

    // Tycoon's Ring — "Tycoon" (100k gold)
    s_accessories.push_back({
        "Tycoon's Ring", Rarity::Legendary,
        70, 100, 40, 28, ElementType::Physical, 0, 0,
        "", -1,
        ItemPassive::GoldFind30, ItemPassive::GoldFind30
    });

    // Archivist's Satchel — "Archivist" (100 unique items)
    s_accessories.push_back({
        "Archivist's Satchel", Rarity::Legendary,
        50, 100, 50, 28, ElementType::Arcane, 10, 0,
        "", -1,
        ItemPassive::ExpBoost20, ItemPassive::ManaRegen5
    });

    // Merchant's Crown — "Trade Lord" (sell 1000 items)
    s_armor.push_back({
        "Merchant's Crown", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 11, {{ElementType::Fire, 5}, {ElementType::Ice, 5}, {ElementType::Lightning, 5}, {ElementType::Arcane, 5}, {ElementType::Poison, 5}, {ElementType::Holy, 5}}, 5,
        "", -1,
        ItemPassive::GoldFind30, ItemPassive::ExpBoost20
    });
}

// ---- Warrior Legendary Items ----

void UniqueItemRegistry::RegisterWarriorLegendaries()
{
    // ================================================================
    // WARRIOR LEGENDARIES (~10 items) — Boss drops
    // Theme: high defense, thorns, damage reduction, stun, physical damage
    // ================================================================

    // === WEAPONS (4) ===

    // Ironclad Warblade — Sword (drops from Aran Warlord)
    s_weapons.push_back({
        "Ironclad Warblade", Rarity::Legendary, WeaponType::Sword,
        180, 28, 20, ElementType::Physical, 0, 0,
        "Aran Warlord", 14,
        ItemPassive::PhysDmgBoost20, ItemPassive::Thorns25
    });

    // Abyssal Cleaver — Axe (drops from Kraken)
    s_weapons.push_back({
        "Abyssal Cleaver", Rarity::Legendary, WeaponType::Axe,
        165, 16, 15, ElementType::Ice, 20, 0,
        "Kraken", -1,
        ItemPassive::DamageReduce15, ItemPassive::PhysDmgBoost20
    });

    // Thunderstrike Mace — Mace (drops from Elder Dragon)
    s_weapons.push_back({
        "Thunderstrike Mace", Rarity::Legendary, WeaponType::Mace,
        170, 22, 25, ElementType::Lightning, 15, 0,
        "Elder Dragon", -1,
        ItemPassive::StunImmune, ItemPassive::Thorns40
    });

    // Earthenbreaker — Hammer (drops from The Primordial One)
    s_weapons.push_back({
        "Earthenbreaker", Rarity::Legendary, WeaponType::Hammer,
        200, 40, 30, ElementType::Physical, 10, 0,
        "The Primordial One", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::DamageReduce15
    });

    // === ARMOR (4) ===

    // Bulwark Plate — Plate Chest (drops from Kraken)
    s_armor.push_back({
        "Bulwark Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        30, 5, {{ElementType::Physical, 10}}, 2,
        "Kraken", 14,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    // Warlord's Casque — Plate Helmet (drops from Aran Warlord)
    s_armor.push_back({
        "Warlord's Casque", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Helmet,
        28, 7, {{ElementType::Lightning, 8}}, 3,
        "Aran Warlord", 14,
        ItemPassive::TauntChance25, ItemPassive::DamageReduce15
    });

    // Drakeplate Gauntlets — Plate Gloves (drops from Elder Dragon)
    s_armor.push_back({
        "Drakeplate Gauntlets", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Gloves,
        16, 6, {{ElementType::Fire, 6}}, 3,
        "Elder Dragon", 14,
        ItemPassive::Thorns25, ItemPassive::StunImmune
    });

    // Primordial Leather Vest — Leather Chest (drops from The Primordial One)
    s_armor.push_back({
        "Primordial Leather Vest", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Chest,
        22, 10, {{ElementType::Physical, 5}, {ElementType::Lightning, 5}}, 5,
        "The Primordial One", -1,
        ItemPassive::Dodge20, ItemPassive::PhysDmgBoost15
    });

    // === ACCESSORIES (2) ===

    // Warlord's Signet — Ring (drops from Void Lord)
    s_accessories.push_back({
        "Warlord's Signet", Rarity::Legendary,
        80, 50, 20, 18, ElementType::Physical, 0, 0,
        "Void Lord", -1,
        ItemPassive::Thorns40, ItemPassive::AllResist10
    });

    // Amulet of Iron Will — Amulet (drops from Seraphim Council)
    s_accessories.push_back({
        "Amulet of Iron Will", Rarity::Legendary,
        60, 60, 30, 20, ElementType::Physical, 0, 0,
        "Seraphim Council", -1,
        ItemPassive::StunImmune, ItemPassive::DamageReduce10
    });
}

// ---- Warrior Epic Items ----

void UniqueItemRegistry::RegisterWarriorEpics()
{
    // ================================================================
    // WARRIOR EPICS (~10 items) — Mid-game enemy drops
    // Theme: sturdy defense, physical power, survivability
    // ================================================================

    // === WEAPONS (4) ===

    // Wolfbane — Sword (drops from Wolf)
    s_weapons.push_back({
        "Wolfbane", Rarity::Epic, WeaponType::Sword,
        50, 3, 10, ElementType::Physical, 5, 1,
        "Wolf", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // Orc-Cleaver — Axe (drops from Orc)
    s_weapons.push_back({
        "Orc-Cleaver", Rarity::Epic, WeaponType::Axe,
        55, 3, 12, ElementType::Physical, 8, 1,
        "Orc", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // Highland Mace — Mace (drops from Highland Warrior)
    s_weapons.push_back({
        "Highland Mace", Rarity::Epic, WeaponType::Mace,
        48, 3, 15, ElementType::Physical, 6, 1,
        "Highland Warrior", -1,
        ItemPassive::Thorns25, ItemPassive::None
    });

    // Drakebone Hammer — Hammer (drops from Drake)
    s_weapons.push_back({
        "Drakebone Hammer", Rarity::Epic, WeaponType::Hammer,
        52, 3, 20, ElementType::Fire, 10, 1,
        "Drake", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // === ARMOR (4) ===

    // Shadow-Woven Plate — Plate Helmet (drops from Shadow)
    s_armor.push_back({
        "Shadow-Woven Plate", Rarity::Epic, ArmorType::Plate, ArmorPiece::Helmet,
        14, 2, {{ElementType::Arcane, 4}}, 1,
        "Shadow", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // Spider-Silk Chestguard — Plate Chest (drops from Giant Spider)
    s_armor.push_back({
        "Spider-Silk Chestguard", Rarity::Epic, ArmorType::Plate, ArmorPiece::Chest,
        18, 2, {{ElementType::Poison, 5}}, 1,
        "Giant Spider", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // Fireforged Gauntlets — Plate Gloves (drops from Fire Elemental)
    s_armor.push_back({
        "Fireforged Gauntlets", Rarity::Epic, ArmorType::Plate, ArmorPiece::Gloves,
        12, 2, {{ElementType::Fire, 6}}, 1,
        "Fire Elemental", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // Dark Leather Boots — Leather Boots (drops from Dark Elf Scout)
    s_armor.push_back({
        "Dark Leather Boots", Rarity::Epic, ArmorType::Leather, ArmorPiece::Boots,
        10, 2, {{ElementType::Poison, 3}}, 1,
        "Dark Elf Scout", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // === ACCESSORIES (2) ===

    // Band of the Vanguard — Ring (drops from Highland Warrior)
    s_accessories.push_back({
        "Band of the Vanguard", Rarity::Epic,
        30, 5, 15, 3, ElementType::Physical, 8, 1,
        "Highland Warrior", -1,
        ItemPassive::PhysDmgLowHp30, ItemPassive::None
    });

    // Emberheart Pendant — Amulet (drops from Fire Elemental)
    s_accessories.push_back({
        "Emberheart Pendant", Rarity::Epic,
        35, 5, 20, 3, ElementType::Fire, 10, 1,
        "Fire Elemental", -1,
        ItemPassive::Lifesteal15, ItemPassive::None
    });
}

// ---- Priest Class-Specific Legendaries ----

void UniqueItemRegistry::RegisterPriestLegendaries()
{
    // ================================================================
    // PRIEST LEGENDARIES — ~10 items, themed: healing, holy, mana, revive
    // ================================================================

    // === LEGENDARY WEAPONS (4) ===

    // Arbiter's Mace — Mace (drops from Seraphim Council)
    s_weapons.push_back({
        "Arbiter's Mace", Rarity::Legendary, WeaponType::Mace,
        140, 36, 35, ElementType::Holy, 30, 0,
        "Seraphim Council", 15,
        ItemPassive::HolyHealOnKill, ItemPassive::HealOnKill15
    });

    // Scepter of the Faithful — Scepter (drops from Elder Dragon)
    s_weapons.push_back({
        "Scepter of the Faithful", Rarity::Legendary, WeaponType::Scepter,
        120, 22, 60, ElementType::Holy, 45, 0,
        "Elder Dragon", -1,
        ItemPassive::ManaRegen10, ItemPassive::Revive50
    });

    // Staff of Benediction — Staff (drops from Arcane Construct)
    s_weapons.push_back({
        "Staff of Benediction", Rarity::Legendary, WeaponType::Staff,
        130, 40, 80, ElementType::Holy, 50, 0,
        "Arcane Construct", -1,
        ItemPassive::DoubleCast15, ItemPassive::SpellDmgBoost20
    });

    // Mace of the Penance — Mace (drops from Chronos, the Time Ender)
    s_weapons.push_back({
        "Mace of the Penance", Rarity::Legendary, WeaponType::Mace,
        160, 42, 40, ElementType::Holy, 35, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::DamageReduce15, ItemPassive::Lifesteal15
    });

    // === LEGENDARY ARMOR (4) ===

    // Vestments of the Martyr — Cloth Chest (drops from Arcane Construct)
    s_armor.push_back({
        "Vestments of the Martyr", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        18, 10, {{ElementType::Holy, 12}}, 5,
        "Arcane Construct", 15,
        ItemPassive::HealOnKill15, ItemPassive::Revive50
    });

    // Halo of the Redeemed — Cloth Helmet (drops from Seraphim Council)
    s_armor.push_back({
        "Halo of the Redeemed", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Helmet,
        20, 9, {{ElementType::Holy, 14}}, 4,
        "Seraphim Council", 15,
        ItemPassive::DamageReduce10, ItemPassive::ManaRegen10
    });

    // Gauntlets of Grace — Cloth Gloves (drops from Void Lord)
    s_armor.push_back({
        "Gauntlets of Grace", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        12, 8, {{ElementType::Holy, 8}}, 4,
        "Void Lord", 15,
        ItemPassive::HealOnKill10, ItemPassive::ManaCostReduce10
    });

    // Treads of the Pilgrim — Leather Boots (drops from Treant King)
    s_armor.push_back({
        "Treads of the Pilgrim", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Boots,
        14, 4, {{ElementType::Holy, 6}}, 2,
        "Treant King", -1,
        ItemPassive::AllResist10, ItemPassive::FreezeImmune
    });

    // === LEGENDARY ACCESSORIES (2) ===

    // Pendant of Devotion — Amulet (drops from Chronos, the Time Ender)
    s_accessories.push_back({
        "Pendant of Devotion", Rarity::Legendary,
        60, 100, 80, 28, ElementType::Holy, 20, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::Revive50, ItemPassive::ManaRegen10
    });

    // Band of the Oathkeeper — Ring (drops from Kraken)
    s_accessories.push_back({
        "Band of the Oathkeeper", Rarity::Legendary,
        50, 20, 60, 10, ElementType::Holy, 15, 0,
        "Kraken", -1,
        ItemPassive::PhoenixRevive, ItemPassive::DamageReduce10
    });
}

// ---- Priest Class-Specific Epics ----

void UniqueItemRegistry::RegisterPriestEpics()
{
    // ================================================================
    // PRIEST EPICS — ~10 items, mid-game enemy drops
    // ================================================================

    // === EPIC WEAPONS (4) ===

    // Lightbringer's Mace — Mace (drops from Lich)
    s_weapons.push_back({
        "Lightbringer's Mace", Rarity::Epic, WeaponType::Mace,
        50, 3, 25, ElementType::Holy, 12, 2,
        "Lich", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // Scepter of the Dawn — Scepter (drops from Fire Elemental)
    s_weapons.push_back({
        "Scepter of the Dawn", Rarity::Epic, WeaponType::Scepter,
        40, 3, 40, ElementType::Holy, 15, 2,
        "Fire Elemental", -1,
        ItemPassive::ManaRegen5, ItemPassive::None
    });

    // Staff of Quiet Faith — Staff (drops from Giant Spider)
    s_weapons.push_back({
        "Staff of Quiet Faith", Rarity::Epic, WeaponType::Staff,
        35, 3, 50, ElementType::Holy, 10, 2,
        "Giant Spider", -1,
        ItemPassive::ManaCostReduce10, ItemPassive::None
    });

    // Censer of the Lost — Scepter (drops from Wraith)
    s_weapons.push_back({
        "Censer of the Lost", Rarity::Epic, WeaponType::Scepter,
        45, 3, 35, ElementType::Holy, 14, 2,
        "Wraith", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    // === EPIC ARMOR (4) ===

    // Cowl of the Whispering Light — Cloth Helmet (drops from Wolf)
    s_armor.push_back({
        "Cowl of the Whispering Light", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Helmet,
        10, 2, {{ElementType::Holy, 4}}, 1,
        "Wolf", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // Acolyte's Vestments — Cloth Chest (drops from Drake)
    s_armor.push_back({
        "Acolyte's Vestments", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Chest,
        14, 2, {{ElementType::Holy, 5}}, 1,
        "Drake", -1,
        ItemPassive::HealOnKill10, ItemPassive::None
    });

    // Handwraps of the Order — Cloth Gloves (drops from Shadow)
    s_armor.push_back({
        "Handwraps of the Order", Rarity::Epic, ArmorType::Cloth, ArmorPiece::Gloves,
        8, 2, {{ElementType::Holy, 3}}, 1,
        "Shadow", -1,
        ItemPassive::ManaOnKill15, ItemPassive::None
    });

    // Sandpilgrim's Stride — Leather Boots (drops from Siren)
    s_armor.push_back({
        "Sandpilgrim's Stride", Rarity::Epic, ArmorType::Leather, ArmorPiece::Boots,
        9, 2, {{ElementType::Holy, 3}}, 1,
        "Siren", -1,
        ItemPassive::AllResist5, ItemPassive::None
    });

    // === EPIC ACCESSORIES (2) ===

    // Ring of the Penitent — Ring (drops from Sea Serpent)
    s_accessories.push_back({
        "Ring of the Penitent", Rarity::Epic,
        25, 5, 40, 8, ElementType::Holy, 8, 2,
        "Sea Serpent", -1,
        ItemPassive::Revive50, ItemPassive::None
    });

    // Holyworn Charm — Amulet (drops from Lich)
    s_accessories.push_back({
        "Holyworn Charm", Rarity::Epic,
        35, 5, 50, 8, ElementType::Holy, 10, 2,
        "Lich", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });
}

// ---- Archer Legendary Items ----

void UniqueItemRegistry::RegisterArcherLegendaries()
{
    // ================================================================
    // ARCHER LEGENDARIES (~10 items) — Boss drops
    // Theme: crit chance, attack speed, dodge, poison, physical damage
    // ================================================================

    // === WEAPONS (4) ===

    // Windrunner's Bow — Bow (drops from Elder Dragon)
    s_weapons.push_back({
        "Windrunner's Bow", Rarity::Legendary, WeaponType::Bow,
        170, 22, 10, ElementType::Physical, 0, 0,
        "Elder Dragon", 16,
        ItemPassive::CritChance30, ItemPassive::AtkSpeed20
    });

    // Venomfang Recurve — Bow (drops from Kraken)
    s_weapons.push_back({
        "Venomfang Recurve", Rarity::Legendary, WeaponType::Bow,
        155, 16, 15, ElementType::Poison, 30, 0,
        "Kraken", -1,
        ItemPassive::PoisonDmgPlus25, ItemPassive::AtkSpeed20
    });

    // Nightblade's Fang — Dagger (drops from Void Lord)
    s_weapons.push_back({
        "Nightblade's Fang", Rarity::Legendary, WeaponType::Dagger,
        160, 32, 20, ElementType::Arcane, 20, 0,
        "Void Lord", -1,
        ItemPassive::CritChance30, ItemPassive::Dodge20
    });

    // Stormstring Longbow — Bow (drops from Aran Warlord)
    s_weapons.push_back({
        "Stormstring Longbow", Rarity::Legendary, WeaponType::Bow,
        165, 28, 12, ElementType::Lightning, 20, 0,
        "Aran Warlord", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::AtkSpeed20
    });

    // === ARMOR (4) ===

    // Pathfinder's Cap — Leather Helmet (drops from Treant King)
    s_armor.push_back({
        "Pathfinder's Cap", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Helmet,
        14, 4, {{ElementType::Poison, 8}}, 2,
        "Treant King", 16,
        ItemPassive::Dodge20, ItemPassive::CritChance20
    });

    // Wyrmhide Jerkin — Leather Chest (drops from Elder Dragon)
    s_armor.push_back({
        "Wyrmhide Jerkin", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Chest,
        20, 6, {{ElementType::Fire, 6}}, 3,
        "Elder Dragon", 16,
        ItemPassive::PhysDmgBoost15, ItemPassive::Dodge15
    });

    // Shadowstalker Grips — Leather Gloves (drops from Seraphim Council)
    s_armor.push_back({
        "Shadowstalker Grips", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Gloves,
        12, 9, {{ElementType::Physical, 5}}, 4,
        "Seraphim Council", 16,
        ItemPassive::CritChance20, ItemPassive::PoisonDmgPlus25
    });

    // Galewalkers — Leather Boots (drops from Chronos, the Time Ender)
    s_armor.push_back({
        "Galewalkers", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Boots,
        10, 11, {{ElementType::Lightning, 5}}, 5,
        "Chronos, the Time Ender", -1,
        ItemPassive::Dodge20, ItemPassive::AtkSpeed20
    });

    // === ACCESSORIES (2) ===

    // Ranger's Focus — Ring (drops from Farm Overseer)
    s_accessories.push_back({
        "Ranger's Focus", Rarity::Legendary,
        40, 10, 15, 5, ElementType::Physical, 10, 0,
        "Farm Overseer", -1,
        ItemPassive::CritChance30, ItemPassive::PhysDmgBoost20
    });

    // Pendant of the Hunt — Amulet (drops from Arcane Construct)
    s_accessories.push_back({
        "Pendant of the Hunt", Rarity::Legendary,
        55, 100, 20, 28, ElementType::Poison, 15, 0,
        "Arcane Construct", -1,
        ItemPassive::AtkSpeed20, ItemPassive::CritChance20
    });
}

// ---- Archer Epic Items ----

void UniqueItemRegistry::RegisterArcherEpics()
{
    // ================================================================
    // ARCHER EPICS (~10 items) — Mid-game enemy drops
    // Theme: crit, speed, poison, physical damage, mobility
    // ================================================================

    // === WEAPONS (4) ===

    // Gloomflight Bow — Bow (drops from Shadow)
    s_weapons.push_back({
        "Gloomflight Bow", Rarity::Epic, WeaponType::Bow,
        48, 3, 12, ElementType::Arcane, 8, 1,
        "Shadow", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // Wolf's Bane — Dagger (drops from Wolf)
    s_weapons.push_back({
        "Wolf's Bane", Rarity::Epic, WeaponType::Dagger,
        42, 3, 8, ElementType::Physical, 5, 1,
        "Wolf", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // Spider Silk Longbow — Bow (drops from Giant Spider)
    s_weapons.push_back({
        "Spider Silk Longbow", Rarity::Epic, WeaponType::Bow,
        50, 3, 15, ElementType::Poison, 10, 1,
        "Giant Spider", -1,
        ItemPassive::AtkSpeed20, ItemPassive::None
    });

    // Orc-Slayer Dagger — Dagger (drops from Orc)
    s_weapons.push_back({
        "Orc-Slayer Dagger", Rarity::Epic, WeaponType::Dagger,
        45, 3, 10, ElementType::Physical, 6, 1,
        "Orc", -1,
        ItemPassive::PhysDmgLowHp30, ItemPassive::None
    });

    // === ARMOR (4) ===

    // Shadowhide Cowl — Leather Helmet (drops from Dark Elf Scout)
    s_armor.push_back({
        "Shadowhide Cowl", Rarity::Epic, ArmorType::Leather, ArmorPiece::Helmet,
        10, 2, {{ElementType::Poison, 4}}, 1,
        "Dark Elf Scout", -1,
        ItemPassive::Dodge15, ItemPassive::None
    });

    // Spiderweave Vest — Leather Chest (drops from Giant Spider)
    s_armor.push_back({
        "Spiderweave Vest", Rarity::Epic, ArmorType::Leather, ArmorPiece::Chest,
        14, 2, {{ElementType::Poison, 5}}, 1,
        "Giant Spider", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    // Wraith-Touched Grips — Leather Gloves (drops from Wraith)
    s_armor.push_back({
        "Wraith-Touched Grips", Rarity::Epic, ArmorType::Leather, ArmorPiece::Gloves,
        9, 2, {{ElementType::Arcane, 4}}, 1,
        "Wraith", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    // Siren's Whisper Boots — Leather Boots (drops from Siren)
    s_armor.push_back({
        "Siren's Whisper Boots", Rarity::Epic, ArmorType::Leather, ArmorPiece::Boots,
        8, 2, {{ElementType::Ice, 3}}, 1,
        "Siren", -1,
        ItemPassive::HealOnKill10, ItemPassive::Dodge15
    });

    // === ACCESSORIES (2) ===

    // Highland Hunter's Ring — Ring (drops from Highland Warrior)
    s_accessories.push_back({
        "Highland Hunter's Ring", Rarity::Epic,
        25, 5, 10, 2, ElementType::Physical, 6, 1,
        "Highland Warrior", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    // Drake-Eye Pendant — Amulet (drops from Drake)
    s_accessories.push_back({
        "Drake-Eye Pendant", Rarity::Epic,
        30, 5, 15, 2, ElementType::Fire, 8, 1,
        "Drake", -1,
        ItemPassive::CritChance20, ItemPassive::AtkSpeed20
    });
}

// ---- Forbidden Citadel Legendaries (difficulty 11) ----
// The strongest items in the game. Each drops from a specific Citadel boss.

void UniqueItemRegistry::RegisterCitadelLegendaries()
{
    // ================================================================
    // CITADEL WEAPONS (10) — One per boss
    // ================================================================

    // Abyssal Sentinel's Edge — Warrior Sword
    s_weapons.push_back({
        "Abyssal Sentinel's Edge", Rarity::Legendary, WeaponType::Sword,
        80, 32, 20, ElementType::Arcane, 25, 10,
        "Abyssal Sentinel", -1,
        ItemPassive::CritChance30, ItemPassive::Dodge20
    });

    // Void Empress's Grimoire — Mage Wand
    s_weapons.push_back({
        "Void Empress's Grimoire", Rarity::Legendary, WeaponType::Wand,
        65, 30, 50, ElementType::Arcane, 40, 12,
        "Void Empress", -1,
        ItemPassive::DoubleCast25, ItemPassive::ManaRegen10
    });

    // Colossus Breaker — Warrior Mace
    s_weapons.push_back({
        "Colossus Breaker", Rarity::Legendary, WeaponType::Mace,
        90, 35, 15, ElementType::Fire, 15, 8,
        "Infernal Colossus", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::DamageReduce15
    });

    // Frostbound Requiem — Priest Staff
    s_weapons.push_back({
        "Frostbound Requiem", Rarity::Legendary, WeaponType::Staff,
        60, 28, 60, ElementType::Ice, 35, 10,
        "Glacial Wraith", -1,
        ItemPassive::IceFreezePlus1, ItemPassive::SpellDmgBoost20
    });

    // Thunderlord's Decree — Archer Bow
    s_weapons.push_back({
        "Thunderlord's Decree", Rarity::Legendary, WeaponType::Bow,
        70, 30, 10, ElementType::Lightning, 30, 12,
        "Storm Arbiter", -1,
        ItemPassive::AtkSpeed20, ItemPassive::LightningStun15
    });

    // Plaguebearer's Kiss — Archer Dagger
    s_weapons.push_back({
        "Plaguebearer's Kiss", Rarity::Legendary, WeaponType::Dagger,
        65, 30, 5, ElementType::Poison, 35, 12,
        "Plague Sovereign", -1,
        ItemPassive::PoisonDmgPlus25, ItemPassive::Lifesteal25
    });

    // Radiant Arbiter's Blade — Priest Mace
    s_weapons.push_back({
        "Radiant Arbiter's Blade", Rarity::Legendary, WeaponType::Mace,
        75, 32, 25, ElementType::Holy, 30, 10,
        "Holy Arbiter", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::HealOnKill15
    });

    // Chrono Shatter — Mage Staff
    s_weapons.push_back({
        "Chrono Shatter", Rarity::Legendary, WeaponType::Staff,
        70, 32, 65, ElementType::Arcane, 45, 14,
        "Chrono Overlord", -1,
        ItemPassive::DoubleCast15, ItemPassive::CooldownReduce2
    });

    // Monarch's Ruin — Warrior Axe
    s_weapons.push_back({
        "Monarch's Ruin", Rarity::Legendary, WeaponType::Axe,
        95, 38, 20, ElementType::Arcane, 20, 10,
        "Void Monarch", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::CritChance30
    });

    // The Unbroken Oath — Priest Mace
    s_weapons.push_back({
        "The Unbroken Oath", Rarity::Legendary, WeaponType::Mace,
        85, 36, 30, ElementType::Holy, 35, 12,
        "The Unbroken", -1,
        ItemPassive::Revive50, ItemPassive::DamageReduce15
    });

    // ================================================================
    // CITADEL ARMOR (5) — Spread across bosses
    // ================================================================

    // Abyssal Aegis — Warrior Plate Chest
    s_armor.push_back({
        "Abyssal Aegis", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Chest,
        25, 8, {{ElementType::Arcane, 12}, {ElementType::Holy, 8}}, 4,
        "Abyssal Sentinel", -1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    // Void Empress's Mantle — Mage Cloth Chest
    s_armor.push_back({
        "Void Empress's Mantle", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Chest,
        18, 6, {{ElementType::Arcane, 15}}, 4,
        "Void Empress", -1,
        ItemPassive::ManaShield30, ItemPassive::SpellDmgBoost20
    });

    // Stormforged Plate — Warrior Plate Boots
    s_armor.push_back({
        "Stormforged Plate", Rarity::Legendary, ArmorType::Plate, ArmorPiece::Boots,
        18, 7, {{ElementType::Lightning, 10}}, 3,
        "Storm Arbiter", -1,
        ItemPassive::Dodge20, ItemPassive::StunImmune
    });

    // Plagueveil Shroud — Archer Leather Chest
    s_armor.push_back({
        "Plagueveil Shroud", Rarity::Legendary, ArmorType::Leather, ArmorPiece::Chest,
        20, 7, {{ElementType::Poison, 12}}, 3,
        "Plague Sovereign", -1,
        ItemPassive::Lifesteal15, ItemPassive::AllResist10
    });

    // Chrono-Woven Wraps — Mage Cloth Gloves
    s_armor.push_back({
        "Chrono-Woven Wraps", Rarity::Legendary, ArmorType::Cloth, ArmorPiece::Gloves,
        14, 6, {{ElementType::Arcane, 10}}, 3,
        "Chrono Overlord", -1,
        ItemPassive::CooldownReduce2, ItemPassive::ManaOnKill25
    });

    // ================================================================
    // CITADEL ACCESSORIES (3)
    // ================================================================

    // Monarch's Coronet — Ring
    s_accessories.push_back({
        "Monarch's Coronet", Rarity::Legendary,
        80, 20, 50, 12, ElementType::Arcane, 25, 8,
        "Void Monarch", -1,
        ItemPassive::AllResist10, ItemPassive::ManaRegen10
    });

    // Unbroken Will — Amulet
    s_accessories.push_back({
        "Unbroken Will", Rarity::Legendary,
        100, 25, 40, 10, ElementType::Holy, 20, 8,
        "The Unbroken", -1,
        ItemPassive::PhoenixRevive, ItemPassive::DamageReduce10
    });

    // Chrono Paradox — Ring
    s_accessories.push_back({
        "Chrono Paradox", Rarity::Legendary,
        70, 18, 60, 15, ElementType::Arcane, 30, 10,
        "Chrono Overlord", -1,
        ItemPassive::DoubleCast15, ItemPassive::ManaOnSkillUse10
    });
}

// ---- Epic & Legendary Offhands ----

void UniqueItemRegistry::RegisterOffhands()
{
    // ================================================================
    // EPIC OFFHANDS — Mid-game boss drops (difficulty 5-8)
    // ================================================================

    // --- Warrior Shields (Epic) ---

    s_offhands.push_back({
        "Bulwark of the Fallen", Rarity::Epic, OffhandType::Shield,
        18, 4, 0, 0, 0, 0, 0, 0,
        "Shadow Knight", -1,
        ItemPassive::DamageReduce10, ItemPassive::None
    });

    s_offhands.push_back({
        "Ironclad Bastion", Rarity::Epic, OffhandType::Shield,
        22, 5, 0, 0, 0, 0, 0, 0,
        "Orc Warlord", -1,
        ItemPassive::Thorns25, ItemPassive::None
    });

    s_offhands.push_back({
        "Aegis of the Dire Wolf", Rarity::Epic, OffhandType::Shield,
        25, 5, 0, 0, 0, 0, 0, 0,
        "Dire Wolf Alpha", -1,
        ItemPassive::StunImmune, ItemPassive::DamageReduce10
    });

    s_offhands.push_back({
        "Stormwall Guard", Rarity::Epic, OffhandType::Shield,
        28, 6, 0, 0, 0, 0, 0, 0,
        "Storm Giant", -1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns25
    });

    // --- Mage Orbs (Epic) ---

    s_offhands.push_back({
        "Orb of Pyroclasm", Rarity::Epic, OffhandType::Orb,
        3, 2, 15, 4, 12, 3, 0, 0,
        "Fire Elemental", -1,
        ItemPassive::FireNoCd20, ItemPassive::None
    });

    s_offhands.push_back({
        "Frostweaver's Eye", Rarity::Epic, OffhandType::Orb,
        4, 2, 20, 5, 15, 4, 0, 0,
        "Frost Wyrm", -1,
        ItemPassive::IceFreezePlus1, ItemPassive::ManaRegen5
    });

    s_offhands.push_back({
        "Stormheart Core", Rarity::Epic, OffhandType::Orb,
        3, 2, 18, 4, 18, 5, 0, 0,
        "Thunder Drake", -1,
        ItemPassive::LightningStun15, ItemPassive::None
    });

    s_offhands.push_back({
        "Voidglass Sphere", Rarity::Epic, OffhandType::Orb,
        5, 3, 25, 6, 20, 5, 0, 0,
        "Void Knight", -1,
        ItemPassive::DoubleCast15, ItemPassive::ManaOnKill15
    });

    // --- Priest Books (Epic) ---

    s_offhands.push_back({
        "Tome of Restoration", Rarity::Epic, OffhandType::Book,
        3, 2, 18, 5, 8, 3, 0, 0,
        "High Priestess", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::None
    });

    s_offhands.push_back({
        "Scripture of Shadows", Rarity::Epic, OffhandType::Book,
        4, 2, 22, 5, 12, 4, 0, 0,
        "Dark Bishop", -1,
        ItemPassive::Lifesteal15, ItemPassive::ManaRegen5
    });

    s_offhands.push_back({
        "Codex of the Sacred Flame", Rarity::Epic, OffhandType::Book,
        4, 3, 25, 6, 14, 4, 0, 0,
        "Holy Arbiter", -1,
        ItemPassive::HealOnKill15, ItemPassive::ManaCostReduce10
    });

    s_offhands.push_back({
        "Grimoire of Twilight", Rarity::Epic, OffhandType::Book,
        5, 3, 28, 6, 16, 5, 0, 0,
        "Lich", -1,
        ItemPassive::SpellDmgBoost20, ItemPassive::ManaOnKill25
    });

    // --- Archer Quivers (Epic) ---

    s_offhands.push_back({
        "Quiver of the Flaming Arrow", Rarity::Epic, OffhandType::Quiver,
        4, 2, 0, 0, 0, 0, 12, 3,
        "Fire Elemental", -1,
        ItemPassive::PhysDmgBoost15, ItemPassive::None
    });

    s_offhands.push_back({
        "Venomtip Quiver", Rarity::Epic, OffhandType::Quiver,
        5, 2, 0, 0, 0, 0, 15, 4,
        "Giant Spider", -1,
        ItemPassive::CritChance20, ItemPassive::None
    });

    s_offhands.push_back({
        "Quiver of the Stormwind", Rarity::Epic, OffhandType::Quiver,
        4, 2, 0, 0, 0, 0, 18, 4,
        "Sea Serpent", -1,
        ItemPassive::AtkSpeed20, ItemPassive::PhysDmgBoost15
    });

    s_offhands.push_back({
        "Shadowstalker Bandolier", Rarity::Epic, OffhandType::Quiver,
        6, 3, 0, 0, 0, 0, 22, 5,
        "Shadow Knight", -1,
        ItemPassive::CritChance30, ItemPassive::Dodge15
    });

    // ================================================================
    // LEGENDARY OFFHANDS — Boss drops (difficulty 8+)
    // ================================================================

    // --- Warrior Shields (Legendary) ---

    s_offhands.push_back({
        "Sentinel's Bulwark", Rarity::Legendary, OffhandType::Shield,
        35, 7, 0, 0, 0, 0, 0, 0,
        "Treant King", 1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    s_offhands.push_back({
        "Dragonfire Aegis", Rarity::Legendary, OffhandType::Shield,
        38, 7, 0, 0, 0, 0, 0, 0,
        "Ancient Drake", -1,
        ItemPassive::BurnImmune, ItemPassive::DamageReduce15
    });

    s_offhands.push_back({
        "Bulwark of the Void", Rarity::Legendary, OffhandType::Shield,
        42, 8, 0, 0, 0, 0, 0, 0,
        "Void Knight", -1,
        ItemPassive::DamageReduce15, ItemPassive::StunImmune
    });

    s_offhands.push_back({
        "Titan's Ward", Rarity::Legendary, OffhandType::Shield,
        45, 8, 0, 0, 0, 0, 0, 0,
        "Chronos, the Time Ender", -1,
        ItemPassive::DamageReduce15, ItemPassive::Thorns40
    });

    s_offhands.push_back({
        "Aegis of the Eternal", Rarity::Legendary, OffhandType::Shield,
        50, 9, 0, 0, 0, 0, 0, 0,
        "Primordial One", -1,
        ItemPassive::Revive50, ItemPassive::DamageReduce15
    });

    // --- Mage Orbs (Legendary) ---

    s_offhands.push_back({
        "Orb of Eternal Flame", Rarity::Legendary, OffhandType::Orb,
        5, 3, 35, 7, 30, 7, 0, 0,
        "Fire Elemental", -1,
        ItemPassive::FireNoCd20, ItemPassive::ManaRegen10
    });

    s_offhands.push_back({
        "Glacial Heart", Rarity::Legendary, OffhandType::Orb,
        6, 3, 40, 8, 35, 8, 0, 0,
        "Frost Wyrm", -1,
        ItemPassive::IceFreezePlus1, ItemPassive::SpellDmgBoost20
    });

    s_offhands.push_back({
        "Stormlord's Focus", Rarity::Legendary, OffhandType::Orb,
        5, 3, 38, 7, 32, 7, 0, 0,
        "Thunder Drake", -1,
        ItemPassive::LightningStun15, ItemPassive::DoubleCast15
    });

    s_offhands.push_back({
        "Voidbinder Shard", Rarity::Legendary, OffhandType::Orb,
        7, 4, 45, 8, 38, 8, 0, 0,
        "Void Monarch", -1,
        ItemPassive::DoubleCast25, ItemPassive::ManaOnKill25
    });

    s_offhands.push_back({
        "Archmage's Focus", Rarity::Legendary, OffhandType::Orb,
        8, 4, 50, 9, 42, 9, 0, 0,
        "Primordial One", -1,
        ItemPassive::DoubleCast25, ItemPassive::ManaCostReduce15
    });

    // --- Priest Books (Legendary) ---

    s_offhands.push_back({
        "Tome of Divine Light", Rarity::Legendary, OffhandType::Book,
        5, 3, 35, 7, 20, 6, 0, 0,
        "Holy Arbiter", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::HealOnKill15
    });

    s_offhands.push_back({
        "Gospel of the Undying", Rarity::Legendary, OffhandType::Book,
        6, 3, 40, 8, 25, 7, 0, 0,
        "Lich", -1,
        ItemPassive::Revive50, ItemPassive::Lifesteal25
    });

    s_offhands.push_back({
        "Scripture of the Faithful", Rarity::Legendary, OffhandType::Book,
        7, 4, 45, 8, 28, 7, 0, 0,
        "The Unbroken", -1,
        ItemPassive::ManaShield30, ItemPassive::SpellDmgBoost20
    });

    s_offhands.push_back({
        "Codex of Infinite Mercy", Rarity::Legendary, OffhandType::Book,
        8, 4, 50, 9, 30, 8, 0, 0,
        "Primordial One", -1,
        ItemPassive::HolyHealOnKill, ItemPassive::ManaCostReduce15
    });

    // --- Archer Quivers (Legendary) ---

    s_offhands.push_back({
        "Quiver of the Crimson Flame", Rarity::Legendary, OffhandType::Quiver,
        6, 3, 0, 0, 0, 0, 28, 6,
        "Fire Elemental", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::CritChance20
    });

    s_offhands.push_back({
        "Quiver of the Frozen Wind", Rarity::Legendary, OffhandType::Quiver,
        7, 3, 0, 0, 0, 0, 32, 7,
        "Frost Wyrm", -1,
        ItemPassive::CritChance30, ItemPassive::AtkSpeed20
    });

    s_offhands.push_back({
        "Quiver of the Storm King", Rarity::Legendary, OffhandType::Quiver,
        6, 3, 0, 0, 0, 0, 35, 7,
        "Thunder Drake", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::AtkSpeed20
    });

    s_offhands.push_back({
        "Shadowveil Quiver", Rarity::Legendary, OffhandType::Quiver,
        8, 4, 0, 0, 0, 0, 40, 8,
        "Shadow Knight", -1,
        ItemPassive::CritChance30, ItemPassive::Dodge20
    });

    s_offhands.push_back({
        "Windrunner's Legacy", Rarity::Legendary, OffhandType::Quiver,
        9, 4, 0, 0, 0, 0, 45, 9,
        "Primordial One", -1,
        ItemPassive::PhysDmgBoost20, ItemPassive::CritChance30
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

    // Look up offhand
    for (const auto& oh : s_offhands)
    {
        if (oh.name == name)
        {
            int def = ScaledValue(oh.baseDefense, oh.defensePerDiff, difficulty);
            int mana = ScaledValue(oh.baseManaBonus, oh.manaBonusPerDiff, difficulty);
            int arcDmg = ScaledValue(oh.baseArcaneDamage, oh.arcaneDamagePerDiff, difficulty);
            int dmgBonus = ScaledValue(oh.baseDamageBonus, oh.damageBonusPerDiff, difficulty);
            int sellVal = (oh.rarity == Rarity::Legendary) ? 250 : 100;
            auto item = std::make_shared<Offhand>(name, oh.offhandType, def, mana, arcDmg,
                                                   static_cast<int>(oh.rarity), oh.passive1, oh.passive2, dmgBonus);
            item->sellValue = sellVal;
            item->setId = oh.setId;
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

const UniqueOffhandDef* UniqueItemRegistry::FindOffhand(const std::string& name)
{
    for (const auto& oh : s_offhands)
        if (oh.name == name) return &oh;
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

std::vector<const UniqueOffhandDef*> UniqueItemRegistry::GetOffhandsByDropSource(const std::string& enemyName)
{
    std::vector<const UniqueOffhandDef*> result;
    for (const auto& oh : s_offhands)
        if (oh.dropSource == enemyName) result.push_back(&oh);
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

std::vector<const UniqueOffhandDef*> UniqueItemRegistry::GetOffhandsByRarity(Rarity r)
{
    std::vector<const UniqueOffhandDef*> result;
    for (const auto& oh : s_offhands)
        if (oh.rarity == r) result.push_back(&oh);
    return result;
}


