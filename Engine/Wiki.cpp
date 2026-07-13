#include "Wiki.hpp"
#include "../Graphics/Colors.hpp"
#include <algorithm>
#include <sstream>
#include <cstring>

Wiki::Wiki()
    : currentTab(WikiTab::Equipment), page(0), maxPage(0), areas(nullptr)
{
    searchBuf[0] = '\0';
    BuildEquipmentDatabase();
    BuildSkillDatabase();
    BuildEnemyDatabase();
    BuildResourceDatabase();
    BuildCraftingDatabase();
}

void Wiki::SetAreas(const std::vector<Area>& areaList)
{
    areas = &areaList;
    BuildAreaDatabase();
}

// ============================================================
//  EQUIPMENT DATABASE
// ============================================================

void Wiki::BuildEquipmentDatabase()
{
    equipEntries.clear();

    // ===== Common weapons =====
    equipEntries.push_back({"Iron Sword", "Common Weapon", "DMG: 10-45  MP: 0-10", "Drop (any area)", 1});
    equipEntries.push_back({"Steel Axe", "Common Weapon", "DMG: 12-47  MP: 0-10", "Drop (any area)", 1});
    equipEntries.push_back({"Wooden Staff", "Common Weapon", "DMG: 8-43  MP: 5-15", "Drop (any area)", 1});
    equipEntries.push_back({"Dagger", "Common Weapon", "DMG: 5-40  MP: 0", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Wand", "Common Weapon", "DMG: 7-42  MP: 8-18", "Drop (any area)", 1});
    equipEntries.push_back({"Training Staff", "Common Weapon", "DMG: 9-44  MP: 6-16", "Drop (any area)", 1});
    equipEntries.push_back({"Shortbow", "Common Weapon", "DMG: 8-43  MP: 0", "Drop (any area)", 1});
    equipEntries.push_back({"Spellbook", "Common Weapon", "DMG: 6-41  MP: 12-22", "Drop (any area)", 1});

    // ===== Common armor =====
    equipEntries.push_back({"Leather Helmet", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Leather Chest", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Leather Gloves", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Leather Pants", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Leather Boots", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Cloth Cap", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Cloth Robe", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Cloth Gloves", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Cloth Pants", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Cloth Boots", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Hood", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Robe", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Mitts", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Trousers", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});
    equipEntries.push_back({"Apprentice Sandals", "Common Armor", "DEF: 2-9", "Drop (any area)", 1});

    // ===== Rare weapons =====
    equipEntries.push_back({"Enchanted Blade", "Rare Weapon", "DMG: 28-100  MP: 3-19", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Mithril Mace", "Rare Weapon", "DMG: 33-105  MP: 3-19", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Crystal Staff", "Rare Weapon", "DMG: 28-100  MP: 13-29", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Shadowfang", "Rare Weapon", "DMG: 36-108  MP: 1-17", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Thunder Bow", "Rare Weapon", "DMG: 31-103  MP: 8-24", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Runed Greatsword", "Rare Weapon", "DMG: 38-110  MP: 3-19", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Arcane Wand", "Rare Weapon", "DMG: 26-98  MP: 18-34", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Frost Staff", "Rare Weapon", "DMG: 30-102  MP: 11-27", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Fire Staff", "Rare Weapon", "DMG: 33-105  MP: 8-24", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Storm Caller's Staff", "Rare Weapon", "DMG: 31-103  MP: 15-31", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Venomfang Dagger", "Rare Weapon", "DMG: 28-100  MP: 6-22", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Holy Scepter", "Rare Weapon", "DMG: 32-104  MP: 11-27", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Inferno Axe", "Rare Weapon", "DMG: 36-108  MP: 3-19", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Glacial Bow", "Rare Weapon", "DMG: 31-103  MP: 9-25", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Thunderstrike Hammer", "Rare Weapon", "DMG: 38-110  MP: 0-16", "Drop (mid areas+)", 2});

    // ===== Rare armor =====
    equipEntries.push_back({"Plate Helm", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Plate Armor", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Plate Gauntlets", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Plate Leggings", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Plate Boots", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Reinforced Helm", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Reinforced Chest", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Reinforced Gloves", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Reinforced Pants", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Reinforced Boots", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Scholar's Circlet", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Mage's Robe", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Spellbinder Gloves", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Enchanted Leggings", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Traveler's Boots", "Rare Armor", "DEF: 7-35", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Circlet", "Rare Armor (Cloth)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Robe", "Rare Armor (Cloth)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Handwraps", "Rare Armor (Cloth)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Leggings", "Rare Armor (Cloth)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Sandals", "Rare Armor (Cloth)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Ranger's Hood", "Rare Armor (Leather)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Ranger's Vest", "Rare Armor (Leather)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Ranger's Grips", "Rare Armor (Leather)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Ranger's Leggings", "Rare Armor (Leather)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Ranger's Boots", "Rare Armor (Leather)", "DEF: 7-35  +Elemental Res", "Drop (mid areas+)", 2});

    // ===== Boss weapons =====
    equipEntries.push_back({"Flamebrand", "Boss Weapon", "DMG: 40-110  MP: 10-34", "Boss drop", 3});
    equipEntries.push_back({"Frostbite", "Boss Weapon", "DMG: 43-113  MP: 15-39", "Boss drop", 3});
    equipEntries.push_back({"Thunderstrike", "Boss Weapon", "DMG: 45-115  MP: 10-34", "Boss drop", 3});
    equipEntries.push_back({"Doomhammer", "Boss Weapon", "DMG: 50-120  MP: 5-29", "Boss drop", 3});
    equipEntries.push_back({"Arcane Staff", "Boss Weapon", "DMG: 35-105  MP: 30-54", "Boss drop", 3});
    equipEntries.push_back({"Voidreaver", "Boss Weapon", "DMG: 55-125  MP: 20-44", "Boss drop", 4});
    equipEntries.push_back({"Celestial Bow", "Boss Weapon", "DMG: 48-118  MP: 25-49", "Boss drop", 4});
    equipEntries.push_back({"Soul Reaper", "Boss Weapon", "DMG: 60-130  MP: 0-24", "Boss drop", 4});
    equipEntries.push_back({"Staff of Ages", "Boss Weapon", "DMG: 40-110  MP: 35-59", "Boss drop", 4});
    equipEntries.push_back({"Wand of the Void", "Boss Weapon", "DMG: 37-107  MP: 40-64", "Boss drop", 4});
    equipEntries.push_back({"Scepter of Stars", "Boss Weapon", "DMG: 45-115  MP: 30-54", "Boss drop", 4});
    equipEntries.push_back({"Tome of Elements", "Boss Weapon", "DMG: 32-102  MP: 50-74", "Boss drop", 4});

    // ===== Boss armor =====
    equipEntries.push_back({"Dragon Helm", "Boss Armor", "DEF: 11-72", "Boss drop", 3});
    equipEntries.push_back({"Titan Chestplate", "Boss Armor", "DEF: 11-72", "Boss drop", 3});
    equipEntries.push_back({"Gauntlets of Power", "Boss Armor", "DEF: 11-72", "Boss drop", 3});
    equipEntries.push_back({"Guardian Legplates", "Boss Armor", "DEF: 11-72", "Boss drop", 3});
    equipEntries.push_back({"Boots of Swiftness", "Boss Armor", "DEF: 11-72", "Boss drop", 3});
    equipEntries.push_back({"Crown of the Archmage", "Boss Armor", "DEF: 11-72", "Boss drop", 4});
    equipEntries.push_back({"Ethereal Mantle", "Boss Armor", "DEF: 11-72", "Boss drop", 4});
    equipEntries.push_back({"Hands of the Weave", "Boss Armor", "DEF: 11-72", "Boss drop", 4});
    equipEntries.push_back({"Leggings of Eternity", "Boss Armor", "DEF: 11-72", "Boss drop", 4});
    equipEntries.push_back({"Sandals of Transcendence", "Boss Armor", "DEF: 11-72", "Boss drop", 4});

    // ===== Accessories =====
    equipEntries.push_back({"Ring of Health", "Accessory", "HP: 30-90", "Boss drop", 2});
    equipEntries.push_back({"Ring of Mana", "Accessory", "MP: 18-74", "Boss drop", 2});
    equipEntries.push_back({"Amulet of Balance", "Accessory", "HP: 15-45  MP: 9-37", "Boss drop", 2});
    equipEntries.push_back({"Seal of Power", "Accessory", "HP: 40-100  MP: 28-84", "Boss drop", 3});
    equipEntries.push_back({"Band of Fortitude", "Accessory", "HP: 50-110", "Boss drop", 3});
    equipEntries.push_back({"Pendant of Wisdom", "Accessory", "MP: 28-94", "Boss drop", 3});
    equipEntries.push_back({"Tears of Eluna", "Accessory", "HP: 45-105  MP: 35-85", "Boss drop", 4});
    equipEntries.push_back({"Orb of Focus", "Accessory", "MP: 34-110", "Boss drop", 3});
    equipEntries.push_back({"Ring of Spellpower", "Accessory", "HP: 10-70  MP: 28-94", "Boss drop", 3});
    equipEntries.push_back({"Amulet of the Sage", "Accessory", "HP: 5-35  MP: 36-130", "Boss drop", 4});
    equipEntries.push_back({"Arcane Core", "Accessory", "HP: 8-25  MP: 40-120", "Boss drop", 4});
    equipEntries.push_back({"Ember Ring", "Accessory (Fire)", "HP: 15-85  MP: 18-78", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Frost Pendant", "Accessory (Ice)", "HP: 20-90  MP: 23-83", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Storm Amulet", "Accessory (Lightning)", "HP: 25-95  MP: 28-88", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Venom Band", "Accessory (Poison)", "HP: 30-100  MP: 18-78", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Holy Charm", "Accessory (Holy)", "HP: 25-95  MP: 23-83", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Arcane Signet", "Accessory (Arcane)", "HP: 15-85  MP: 38-98", "Drop (mid areas+)", 2});
    equipEntries.push_back({"Elemental Loop", "Accessory (Any)", "HP: 20-90  MP: 18-78", "Drop (mid areas+)", 3});

    // ===== Legendary (class-specific) =====
    equipEntries.push_back({"Titan's Greatsword", "Legendary (Warrior)", "DMG: 250  MP: 20", "The Primordial One", 5});
    equipEntries.push_back({"Titan's Plate", "Legendary (Warrior)", "DEF: 60", "The Primordial One", 5});
    equipEntries.push_back({"Titan's Might", "Legendary (Warrior)", "HP: 200  MP: 50", "The Primordial One", 5});
    equipEntries.push_back({"Divine Scepter", "Legendary (Priest)", "DMG: 180  MP: 80", "The Primordial One", 5});
    equipEntries.push_back({"Divine Robe", "Legendary (Priest)", "DEF: 40", "The Primordial One", 5});
    equipEntries.push_back({"Divine Blessing", "Legendary (Priest)", "HP: 100  MP: 200", "The Primordial One", 5});
    equipEntries.push_back({"Archmage Staff", "Legendary (Mage)", "DMG: 200  MP: 100", "The Primordial One", 5});
    equipEntries.push_back({"Archmage Robes", "Legendary (Mage)", "DEF: 35", "The Primordial One", 5});
    equipEntries.push_back({"Archmage Orb", "Legendary (Mage)", "HP: 50  MP: 250", "The Primordial One", 5});
    equipEntries.push_back({"Shadowstrike Bow", "Legendary (Archer)", "DMG: 220  MP: 30", "The Primordial One", 5});
    equipEntries.push_back({"Shadowstalker Vest", "Legendary (Archer)", "DEF: 45", "The Primordial One", 5});
    equipEntries.push_back({"Shadow Cloak", "Legendary (Archer)", "HP: 150  MP: 100", "The Primordial One", 5});
    equipEntries.push_back({"Gilded Blade", "Legendary (Merchant)", "DMG: 200  MP: 40", "The Primordial One", 5});
    equipEntries.push_back({"Gilded Armor", "Legendary (Merchant)", "DEF: 50", "The Primordial One", 5});
    equipEntries.push_back({"Gilded Pendant", "Legendary (Merchant)", "HP: 180  MP: 120", "The Primordial One", 5});

    // ===== Crafted weapons =====
    equipEntries.push_back({"Copper Dagger", "Crafted Weapon", "DMG: 8  MP: 0", "Craft (Mining Lv.1)", 1});
    equipEntries.push_back({"Iron Sword", "Crafted Weapon", "DMG: 14  MP: 2", "Craft (Mining Lv.3)", 1});
    equipEntries.push_back({"Steel Blade", "Crafted Weapon", "DMG: 22  MP: 5", "Craft (Mining Lv.5)", 2});
    equipEntries.push_back({"Gold Longsword", "Crafted Weapon", "DMG: 30  MP: 8", "Craft (Mining Lv.7)", 3});
    equipEntries.push_back({"Mythril Greatsword", "Crafted Weapon", "DMG: 42  MP: 12", "Craft (Mining Lv.9)", 4});
    equipEntries.push_back({"Wooden Staff", "Crafted Weapon", "DMG: 4  MP: 12", "Craft (Lumber Lv.1)", 1});
    equipEntries.push_back({"Oak Staff", "Crafted Weapon", "DMG: 8  MP: 20", "Craft (Lumber Lv.3)", 1});
    equipEntries.push_back({"Crystal Staff", "Crafted Weapon", "DMG: 12  MP: 30", "Craft (Lumber Lv.5)", 2});
    equipEntries.push_back({"Ebony Wand", "Crafted Weapon", "DMG: 16  MP: 42", "Craft (Lumber Lv.7)", 3});
    equipEntries.push_back({"Magical Scepter", "Crafted Weapon", "DMG: 22  MP: 55", "Craft (Lumber Lv.9)", 4});
    equipEntries.push_back({"Short Bow", "Crafted Weapon", "DMG: 10  MP: 2", "Craft (Lumber Lv.1)", 1});
    equipEntries.push_back({"Reinforced Bow", "Crafted Weapon", "DMG: 16  MP: 4", "Craft (Lumber Lv.3)", 1});
    equipEntries.push_back({"Longbow", "Crafted Weapon", "DMG: 24  MP: 6", "Craft (Lumber Lv.5)", 2});
    equipEntries.push_back({"Ebony Bow", "Crafted Weapon", "DMG: 32  MP: 10", "Craft (Lumber Lv.7)", 3});
    equipEntries.push_back({"Mythril Bow", "Crafted Weapon", "DMG: 40  MP: 14", "Craft (Lumber Lv.9)", 4});

    // ===== Crafted armor / accessories =====
    equipEntries.push_back({"Cloth Armor", "Crafted Armor", "DEF: 4", "Craft (Smith Lv.3)", 1});
    equipEntries.push_back({"Chainmail", "Crafted Armor", "DEF: 14", "Craft (Smith Lv.5)", 2});
    equipEntries.push_back({"Plate Armor", "Crafted Armor", "DEF: 20", "Craft (Smith Lv.7)", 3});
    equipEntries.push_back({"Mythril Armor", "Crafted Armor", "DEF: 28", "Craft (Smith Lv.9)", 4});
    equipEntries.push_back({"Silver Ring", "Crafted Accessory", "HP: 30  MP: 15", "Craft (Fish Lv.5)", 2});
    equipEntries.push_back({"Gold Amulet", "Crafted Accessory", "HP: 50  MP: 30", "Craft (Fish Lv.7)", 3});

    std::sort(equipEntries.begin(), equipEntries.end(),
        [](const WikiEntry& a, const WikiEntry& b) {
            if (a.rarity != b.rarity) return a.rarity < b.rarity;
            if (a.category != b.category) return a.category < b.category;
            return a.name < b.name;
        });
}

// ============================================================
//  SKILL DATABASE
// ============================================================

void Wiki::BuildSkillDatabase()
{
    skillEntries.clear();

    // ===== Common =====
    skillEntries.push_back({"Attack", "Common", "Mana:0  CD:0  Base:STR/2 + weapon", "Basic attack"});

    // ===== Warrior Skills =====
    skillEntries.push_back({"Power Strike", "Warrior (Lv.1)", "Mana:15  CD:1  Base:60 + STR/2", "Powerful strike"});
    skillEntries.push_back({"Whirlwind", "Warrior (Lv.1)", "Mana:30  CD:3  Base:50 + STR/3", "Spin attack (2 hits)"});
    skillEntries.push_back({"Defensive Stance", "Warrior (Lv.3)", "Mana:0  CD:2  DEF+3", "Restore 5 + VIT/2 HP"});
    skillEntries.push_back({"War Cry", "Warrior (Lv.3)", "Mana:20  CD:4  DEF+5", "Heal 20 + VIT HP"});
    skillEntries.push_back({"Shield Bash", "Warrior (Lv.10)", "Mana:20  CD:2  Base:80 + STR/2", "Stunning blow"});
    skillEntries.push_back({"Battle Cry", "Warrior (Lv.20)", "Mana:30  CD:4  DEF+8", "Heal 40 + VIT HP"});
    skillEntries.push_back({"Execute", "Warrior (Lv.30)", "Mana:50  CD:4  Base:180 + STR*2", "Finisher move"});
    skillEntries.push_back({"War Stomp", "Warrior (Lv.40)", "Mana:60  CD:5  Base:120 + STR", "Drains 25 enemy mana"});

    // ===== Priest Skills =====
    skillEntries.push_back({"Holy Smite", "Priest (Lv.1)", "Mana:20  CD:2  Base:45 + WIS/2", "Holy damage"});
    skillEntries.push_back({"Heal", "Priest (Lv.1)", "Mana:25  CD:1  Heal:40 + WIS/2", "Restore HP"});
    skillEntries.push_back({"Mass Heal", "Priest (Lv.3)", "Mana:50  CD:3  Heal:60 + WIS", "Powerful heal"});
    skillEntries.push_back({"Divine Shield", "Priest (Lv.3)", "Mana:30  CD:5  DEF+17", "Heal 50 + WIS HP"});
    skillEntries.push_back({"Smite", "Priest (Lv.8)", "Mana:18  CD:2  Base:55 + WIS/2", "Holy damage"});
    skillEntries.push_back({"Renew", "Priest (Lv.15)", "Mana:35  CD:3  Heal:90 + WIS", "Strong heal"});
    skillEntries.push_back({"Holy Nova", "Priest (Lv.25)", "Mana:45  CD:4  Base:80 + WIS", "Damage + heal 50 + WIS/2"});
    skillEntries.push_back({"Divine Wrath", "Priest (Lv.40)", "Mana:65  CD:5  Base:200 + WIS*2", "Holy nuke"});

    // ===== Archer Skills =====
    skillEntries.push_back({"Piercing Shot", "Archer (Lv.1)", "Mana:10  CD:1  Base:40 + DEX/2", "Poison DoT 3 turns"});
    skillEntries.push_back({"Multi Shot", "Archer (Lv.3)", "Mana:20  CD:3  Base:30 + DEX/3", "Poison (2 hits)"});
    skillEntries.push_back({"Rain of Arrows", "Archer (Lv.5)", "Mana:30  CD:4  Base:60 + DEX", "Poison (2 hits)"});
    skillEntries.push_back({"Quick Shot", "Archer (Lv.5)", "Mana:8  CD:0  Base:30 + DEX/3", "Fast poison shot"});
    skillEntries.push_back({"Snipe", "Archer (Lv.15)", "Mana:25  CD:3  Base:120 + DEX", "Precise poison shot"});
    skillEntries.push_back({"Arrow Storm", "Archer (Lv.25)", "Mana:35  CD:4  Base:60 + DEX/3", "Poison (2 hits)"});
    skillEntries.push_back({"Death Mark", "Archer (Lv.40)", "Mana:55  CD:5  Base:220 + DEX*2", "Poison nuke"});

    // ===== Mage Skills =====
    skillEntries.push_back({"Fireball", "Mage (Lv.1)", "Mana:15  CD:2  Base:50 + Int/2", "Fire damage"});
    skillEntries.push_back({"Ice Bolt", "Mage (Lv.1)", "Mana:12  CD:1  Base:35 + Int/3", "Ice damage"});
    skillEntries.push_back({"Arcane Bolt", "Mage (Lv.2)", "Mana:6  CD:0  Base:20 + Int/4", "Cheap filler spell"});
    skillEntries.push_back({"Meteor", "Mage (Lv.3)", "Mana:40  CD:4  Base:80 + Int", "Heavy fire nuke"});
    skillEntries.push_back({"Frost Ward", "Mage (Lv.4)", "Mana:14  CD:3  DEF+12", "Heal: 15 + Int/4, self-defense"});
    skillEntries.push_back({"Arcane Surge", "Mage (Lv.5)", "Mana:35  CD:5  Base:100 + Int*2", "Big burst damage"});
    skillEntries.push_back({"Mana Siphon", "Mage (Lv.6)", "Mana:10  CD:2  Base:30 + Int/2", "Restore 10 + Int/5 mana"});
    skillEntries.push_back({"Chain Lightning", "Mage (Lv.8)", "Mana:20  CD:3  Base:55 + Int", "Lightning damage"});
    skillEntries.push_back({"Lightning Bolt", "Mage (Lv.10)", "Mana:18  CD:2  Base:60 + Int/2", "Lightning damage"});
    skillEntries.push_back({"Arcane Missiles", "Mage (Lv.12)", "Mana:24  CD:3  Base:70 + Int*1.5", "Arcane barrage"});
    skillEntries.push_back({"Frost Shield", "Mage (Lv.14)", "Mana:18  CD:4  DEF+22", "Restore 25 + Int/3 mana"});
    skillEntries.push_back({"Mana Bomb", "Mage (Lv.16)", "Mana:35  CD:4  Base:90 + Int*2", "Spend 20 extra mana for +3 dmg each"});
    skillEntries.push_back({"Blazing Orb", "Mage (Lv.18)", "Mana:26  CD:3  Base:65 + Int", "Heal: 10 + Int/6"});
    skillEntries.push_back({"Frost Nova", "Mage (Lv.20)", "Mana:28  CD:3  Base:70 + Int", "Drains 20 enemy mana"});
    skillEntries.push_back({"Arcane Barrage", "Mage (Lv.22)", "Mana:30  CD:3  Base:85 + Int*2", "High arcane burst"});
    skillEntries.push_back({"Time Warp", "Mage (Lv.26)", "Mana:28  CD:4  Base:55 + Int", "Self DEF +18"});
    skillEntries.push_back({"Glacial Spike", "Mage (Lv.28)", "Mana:32  CD:4  Base:105+Int*1.5", "1.5x dmg vs low-HP enemies"});
    skillEntries.push_back({"Mana Shield", "Mage (Lv.30)", "Mana:0  CD:5  DEF+25", "Restore 40 + Int/2 mana"});
    skillEntries.push_back({"Pyroblast", "Mage (Lv.32)", "Mana:42  CD:5  Base:135 + Int*2", "Massive fire damage"});
    skillEntries.push_back({"Blizzard", "Mage (Lv.34)", "Mana:48  CD:5  Base:145+Int*1.5", "Drains 15 enemy mana"});
    skillEntries.push_back({"Arcane Power", "Mage (Lv.36)", "Mana:38  CD:6  DEF+35", "Restore 50+Int/2 mana & 30+Int/3 HP"});
    skillEntries.push_back({"Ice Lance", "Mage (Lv.38)", "Mana:36  CD:4  Base:125 + Int*2", "1.6x dmg vs low-mana enemies"});
    skillEntries.push_back({"Inferno", "Mage (Lv.40)", "Mana:70  CD:6  Base:220 + Int*2", "Devastating fire"});
    skillEntries.push_back({"Elemental Fury", "Mage (Lv.44)", "Mana:55  CD:6  Base:200 + Int*3", "Massive mixed damage"});
    skillEntries.push_back({"Archmage's Cataclysm", "Mage (Lv.50)", "Mana:75  CD:8  Base:300+Int*4", "Ultimate — restores 20 mana"});

    // ===== Merchant Skills =====
    skillEntries.push_back({"Throw Coin", "Merchant (Lv.1)", "Mana:5  CD:1  Base:35 + DEX/3", "Physical damage"});
    skillEntries.push_back({"Appraise", "Merchant (Lv.3)", "Mana:0  CD:3  DEF+3", "Restore 15 + WIS/2 mana"});
    skillEntries.push_back({"Haggle", "Merchant (Lv.3)", "Mana:15  CD:4  Heal:25 + VIT", "Drain 10 enemy mana"});
    skillEntries.push_back({"Bribery", "Merchant (Lv.10)", "Mana:20  CD:2  Base:60 + WIS/2", "Physical damage"});
    skillEntries.push_back({"Invest", "Merchant (Lv.15)", "Mana:25  CD:4  DEF+5", "Restore 30 + INT/2 mana"});
    skillEntries.push_back({"Coin Storm", "Merchant (Lv.25)", "Mana:40  CD:4  Base:35 + DEX/4", "Physical (3 hits)"});
    skillEntries.push_back({"Golden Hurricane", "Merchant (Lv.40)", "Mana:55  CD:5  Base:180 + DEX*2", "Physical nuke"});

    std::sort(skillEntries.begin(), skillEntries.end(),
        [](const WikiEntry& a, const WikiEntry& b) {
            auto extractLevel = [](const std::string& cat) -> int {
                auto pos = cat.find("Lv.");
                if (pos == std::string::npos) return 0;
                int level = 0;
                for (size_t i = pos + 3; i < cat.size() && std::isdigit(cat[i]); ++i)
                    level = level * 10 + (cat[i] - '0');
                return level;
            };
            std::string classA = a.category.substr(0, a.category.find(' '));
            std::string classB = b.category.substr(0, b.category.find(' '));
            if (classA != classB) return classA < classB;
            return extractLevel(a.category) < extractLevel(b.category);
        });
}

// ============================================================
//  ENEMY DATABASE
// ============================================================

void Wiki::BuildEnemyDatabase()
{
    enemyEntries.clear();

    // Felrona (tier 1)
    enemyEntries.push_back({"Slime", "Felrona", "HP:20  MP:2  STR:3  DEF:1  XP:25  Gold:8", "Area 1", 0, "Tier 1 drops. Special: Slime Gel"});
    enemyEntries.push_back({"Rat", "Felrona", "HP:15  MP:0  STR:4  DEF:1  XP:20  Gold:5", "Area 1", 0, "Tier 1 drops. Special: Rat Tail"});
    enemyEntries.push_back({"Bandit", "Felrona", "HP:35  MP:5  STR:7  DEF:3  XP:60  Gold:30", "Area 1", 0, "Tier 1 drops. Special: Bandit Mask"});
    enemyEntries.push_back({"Angry Chicken", "Felrona", "HP:10  MP:0  STR:2  DEF:0  XP:15  Gold:3", "Area 1", 0, "Tier 1 drops. Special: Chicken Feather"});

    // Dark Woods (tier 2)
    enemyEntries.push_back({"Wolf", "Dark Woods", "HP:40  MP:5  STR:8  DEF:3  XP:75  Gold:35", "Area 2", 0, "Tier 2 drops. Special: Wolf Pelt"});
    enemyEntries.push_back({"Giant Spider", "Dark Woods", "HP:35  MP:10  STR:9  DEF:4  XP:80  Gold:30", "Area 2", 0, "Tier 2 drops. Special: Spider Silk"});
    enemyEntries.push_back({"Treant", "Dark Woods", "HP:80  MP:20  STR:12  DEF:8  XP:120  Gold:60", "Area 2", 0, "Tier 2 drops. Special: Living Branch"});
    enemyEntries.push_back({"Wraith", "Dark Woods", "HP:30  MP:30  STR:6  DEF:2  XP:110  Gold:40", "Area 2", 0, "Tier 2 drops. Special: Wraith Essence"});
    enemyEntries.push_back({"Dark Elf Scout", "Dark Woods", "HP:45  MP:15  STR:10  DEF:5  XP:100  Gold:55", "Area 2", 0, "Tier 2 drops. Special: Elf Arrow"});
    enemyEntries.push_back({"Feral Boar", "Dark Woods", "HP:55  MP:0  STR:11  DEF:6  XP:90  Gold:40", "Area 2", 0, "Tier 2 drops. Special: Boar Tusk"});

    // Salty Siren Coast (tier 3)
    enemyEntries.push_back({"Pirate", "Salty Siren Coast", "HP:50  MP:10  STR:10  DEF:5  XP:110  Gold:80", "Area 3", 0, "Tier 3 drops. Special: Doubloon"});
    enemyEntries.push_back({"Sea Serpent", "Salty Siren Coast", "HP:70  MP:25  STR:14  DEF:7  XP:150  Gold:90", "Area 3", 0, "Tier 3 drops. Special: Serpent Scale"});
    enemyEntries.push_back({"Giant Crab", "Salty Siren Coast", "HP:60  MP:5  STR:8  DEF:10  XP:100  Gold:60", "Area 3", 0, "Tier 3 drops. Special: Crab Claw"});
    enemyEntries.push_back({"Siren", "Salty Siren Coast", "HP:40  MP:40  STR:7  DEF:4  XP:140  Gold:70", "Area 3", 0, "Tier 3 drops. Special: Siren Song"});

    // Dragon's Peak (tier 4)
    enemyEntries.push_back({"Orc", "Dragon's Peak", "HP:50  MP:15  STR:10  DEF:5  XP:100  Gold:50", "Area 4", 0, "Tier 4 drops. Special: Orc Tusk"});
    enemyEntries.push_back({"Harpy", "Dragon's Peak", "HP:40  MP:15  STR:9  DEF:4  XP:110  Gold:55", "Area 4", 0, "Tier 4 drops. Special: Harpy Feather"});
    enemyEntries.push_back({"Fire Elemental", "Dragon's Peak", "HP:55  MP:50  STR:15  DEF:6  XP:160  Gold:70", "Area 4", 0, "Tier 4 drops. Special: Fire Essence"});
    enemyEntries.push_back({"Drake", "Dragon's Peak", "HP:90  MP:30  STR:16  DEF:10  XP:200  Gold:110", "Area 4", 0, "Tier 4 drops. Special: Drake Scale"});
    enemyEntries.push_back({"Stone Golem", "Dragon's Peak", "HP:120  MP:5  STR:14  DEF:15  XP:180  Gold:80", "Area 4", 0, "Tier 4 drops. Special: Golem Core"});

    // Aran Highlands (tier 5)
    enemyEntries.push_back({"Highland Warrior", "Aran Highlands", "HP:70  MP:15  STR:14  DEF:9  XP:170  Gold:100", "Area 5", 0, "Tier 5 drops. Special: Warrior Crest"});
    enemyEntries.push_back({"Gryphon", "Aran Highlands", "HP:80  MP:20  STR:16  DEF:8  XP:200  Gold:110", "Area 5", 0, "Tier 5 drops. Special: Gryphon Feather"});
    enemyEntries.push_back({"Earth Elemental", "Aran Highlands", "HP:130  MP:10  STR:12  DEF:16  XP:220  Gold:90", "Area 5", 0, "Tier 5 drops. Special: Earth Essence"});
    enemyEntries.push_back({"Phantom", "Aran Highlands", "HP:50  MP:50  STR:11  DEF:6  XP:230  Gold:120", "Area 5", 0, "Tier 5 drops. Special: Phantom Shroud"});

    // Shadow Dungeon (tier 6)
    enemyEntries.push_back({"Shadow", "Shadow Dungeon", "HP:60  MP:30  STR:13  DEF:8  XP:210  Gold:100", "Area 6", 0, "Tier 6 drops. Special: Shadow Essence"});
    enemyEntries.push_back({"Lich", "Shadow Dungeon", "HP:70  MP:100  STR:12  DEF:10  XP:300  Gold:150", "Area 6", 0, "Tier 6 drops. Special: Lich Phylactery"});
    enemyEntries.push_back({"Demon", "Shadow Dungeon", "HP:100  MP:50  STR:20  DEF:14  XP:350  Gold:200", "Area 6", 0, "Tier 6 drops. Special: Demon Horn"});
    enemyEntries.push_back({"Void Knight", "Shadow Dungeon", "HP:120  MP:40  STR:22  DEF:16  XP:400  Gold:250", "Area 6", 0, "Tier 6 drops. Special: Void Shard"});
    enemyEntries.push_back({"Bone Golem", "Shadow Dungeon", "HP:150  MP:10  STR:18  DEF:18  XP:350  Gold:180", "Area 6", 0, "Tier 6 drops. Special: Bone Fragment"});

    // Celestial Spire (tier 7)
    enemyEntries.push_back({"Celestial Knight", "Celestial Spire", "HP:180  MP:60  STR:28  DEF:20  XP:500  Gold:300", "Area 7", 0, "Tier 7 drops. Special: Celestial Ingot"});
    enemyEntries.push_back({"Seraphim", "Celestial Spire", "HP:140  MP:120  STR:24  DEF:16  XP:600  Gold:350", "Area 7", 0, "Tier 7 drops. Special: Seraphim Wing"});
    enemyEntries.push_back({"Light Elemental", "Celestial Spire", "HP:120  MP:150  STR:20  DEF:18  XP:550  Gold:280", "Area 7", 0, "Tier 7 drops. Special: Light Essence"});
    enemyEntries.push_back({"Astral Mage", "Celestial Spire", "HP:130  MP:200  STR:18  DEF:14  XP:650  Gold:400", "Area 7", 0, "Tier 7 drops. Special: Astral Essence"});

    // Void Beyond (tier 8)
    enemyEntries.push_back({"Void Walker", "The Void Beyond", "HP:200  MP:80  STR:32  DEF:22  XP:800  Gold:500", "Area 8", 0, "Tier 8 drops. Special: Void Essence"});
    enemyEntries.push_back({"Cosmic Horror", "The Void Beyond", "HP:250  MP:100  STR:36  DEF:24  XP:1000  Gold:600", "Area 8", 0, "Tier 8 drops. Special: Cosmic Eye"});
    enemyEntries.push_back({"Nether Drake", "The Void Beyond", "HP:280  MP:80  STR:38  DEF:26  XP:1100  Gold:700", "Area 8", 0, "Tier 8 drops. Special: Nether Scale"});
    enemyEntries.push_back({"Star Beast", "The Void Beyond", "HP:220  MP:150  STR:30  DEF:20  XP:1200  Gold:650", "Area 8", 0, "Tier 8 drops. Special: Star Fragment"});

    // Arcane Sanctum (tier 9)
    enemyEntries.push_back({"Arcane Guardian", "Arcane Sanctum", "HP:400  MP:300  STR:45  DEF:35  XP:2500  Gold:1200", "Area 9", 0, "Tier 9 drops. Special: Guardian Core"});
    enemyEntries.push_back({"Mana Wisp", "Arcane Sanctum", "HP:200  MP:500  STR:30  DEF:20  XP:2000  Gold:900", "Area 9", 0, "Tier 9 drops. Special: Mana Crystal"});
    enemyEntries.push_back({"Crystal Elemental", "Arcane Sanctum", "HP:500  MP:100  STR:50  DEF:50  XP:3000  Gold:1500", "Area 9", 0, "Tier 9 drops. Special: Crystal Shard"});
    enemyEntries.push_back({"Spellblade", "Arcane Sanctum", "HP:350  MP:200  STR:55  DEF:40  XP:2800  Gold:1300", "Area 9", 0, "Tier 9 drops. Special: Spellblade Essence"});

    // Chronos Depths (tier 10)
    enemyEntries.push_back({"Time Weaver", "Chronos Depths", "HP:500  MP:600  STR:55  DEF:45  XP:5000  Gold:2500", "Area 10", 0, "Tier 10 drops. Special: Time Essence"});
    enemyEntries.push_back({"Chrono Beast", "Chronos Depths", "HP:800  MP:200  STR:75  DEF:55  XP:6000  Gold:3000", "Area 10", 0, "Tier 10 drops. Special: Temporal Crystal"});
    enemyEntries.push_back({"Temporal Knight", "Chronos Depths", "HP:700  MP:400  STR:70  DEF:55  XP:5500  Gold:2800", "Area 10", 0, "Tier 10 drops. Special: Knight's Sigil"});
    enemyEntries.push_back({"Void Phoenix", "Chronos Depths", "HP:600  MP:500  STR:65  DEF:50  XP:6000  Gold:3200", "Area 10", 0, "Tier 10 drops. Special: Phoenix Feather"});

    // Bosses
    enemyEntries.push_back({"Farm Overseer", "Boss", "HP:120  MP:20  STR:15  DEF:8  XP:300  Gold:150", "Abandoned Farm", 0, "Boss drops only. Special: Overseer's Key"});
    enemyEntries.push_back({"Treant King", "Boss", "HP:250  MP:50  STR:22  DEF:14  XP:600  Gold:300", "Twilight Hollow", 0, "Boss drops only. Special: King's Crown"});
    enemyEntries.push_back({"Kraken", "Boss", "HP:300  MP:80  STR:25  DEF:16  XP:800  Gold:500", "Sunken Grotto", 0, "Boss drops only. Special: Kraken Ink"});
    enemyEntries.push_back({"Elder Dragon", "Boss", "HP:200  MP:100  STR:30  DEF:20  XP:500  Gold:500", "The Summit", 0, "Boss drops only. Special: Dragon Heart"});
    enemyEntries.push_back({"Aran Warlord", "Boss", "HP:350  MP:60  STR:30  DEF:20  XP:1000  Gold:600", "The Citadel", 0, "Boss drops only. Special: Warlord's Horn"});
    enemyEntries.push_back({"Void Lord", "Boss", "HP:500  MP:150  STR:35  DEF:24  XP:2000  Gold:1000", "Abyssal Depths", 0, "Boss drops only. Special: Void Essence"});
    enemyEntries.push_back({"Seraphim Council", "Boss", "HP:800  MP:250  STR:45  DEF:30  XP:5000  Gold:2000", "Heavenly Ascent", 0, "Boss drops only. Special: Council Seal"});
    enemyEntries.push_back({"The Primordial One", "Ultimate Boss", "HP:2000  MP:500  STR:60  DEF:45  XP:15000  Gold:5000", "The Endless Void (Lv.50)", 0, "Boss drops only. Special: Primordial Essence"});
    enemyEntries.push_back({"Arcane Construct", "Boss", "HP:3000  MP:800  STR:80  DEF:55  XP:25000  Gold:8000", "Sanctum of Secrets", 0, "Boss drops only. Special: Construct Core"});
    enemyEntries.push_back({"Chronos, the Time Ender", "Ultimate Boss", "HP:5000  MP:1000  STR:100  DEF:70  XP:50000  Gold:15000", "Chronos Depths (Lv.50)", 0, "Boss drops only. Special: Time Crystal"});
}

// ============================================================
//  RESOURCE DATABASE
// ============================================================

void Wiki::BuildResourceDatabase()
{
    resourceEntries.clear();

    resourceEntries.push_back({"Copper Ore", "Mining", "Tier 1 mining resource", "Mining Lv.1", 1});
    resourceEntries.push_back({"Iron Ore", "Mining", "Tier 2 mining resource", "Mining Lv.3", 2});
    resourceEntries.push_back({"Silver Ore", "Mining", "Tier 3 mining resource", "Mining Lv.5", 3});
    resourceEntries.push_back({"Gold Ore", "Mining", "Tier 4 mining resource", "Mining Lv.7", 4});
    resourceEntries.push_back({"Mythril Ore", "Mining", "Tier 5 mining resource", "Mining Lv.9", 5});

    resourceEntries.push_back({"Wood", "Lumberjacking", "Tier 1 lumber resource", "Lumber Lv.1", 1});
    resourceEntries.push_back({"Oak Wood", "Lumberjacking", "Tier 2 lumber resource", "Lumber Lv.3", 2});
    resourceEntries.push_back({"Maple Wood", "Lumberjacking", "Tier 3 lumber resource", "Lumber Lv.5", 3});
    resourceEntries.push_back({"Ebony Wood", "Lumberjacking", "Tier 4 lumber resource", "Lumber Lv.7", 4});
    resourceEntries.push_back({"Magical Wood", "Lumberjacking", "Tier 5 lumber resource", "Lumber Lv.9", 5});

    resourceEntries.push_back({"Raw Fish", "Fishing", "Tier 1 fishing resource", "Fishing Lv.1", 1});
    resourceEntries.push_back({"Salmon", "Fishing", "Tier 2 fishing resource", "Fishing Lv.3", 2});
    resourceEntries.push_back({"Tuna", "Fishing", "Tier 3 fishing resource", "Fishing Lv.5", 3});
    resourceEntries.push_back({"Exotic Fish", "Fishing", "Tier 4 fishing resource", "Fishing Lv.7", 4});
    resourceEntries.push_back({"Legendary Fish", "Fishing", "Tier 5 fishing resource", "Fishing Lv.9", 5});

    resourceEntries.push_back({"Simple Ingot", "Smithing", "Tier 1 smithing resource", "Smithing Lv.1", 1});
    resourceEntries.push_back({"Steel Ingot", "Smithing", "Tier 2 smithing resource", "Smithing Lv.3", 2});
    resourceEntries.push_back({"Silver Ingot", "Smithing", "Tier 3 smithing resource", "Smithing Lv.5", 3});
    resourceEntries.push_back({"Gold Ingot", "Smithing", "Tier 4 smithing resource", "Smithing Lv.7", 4});
    resourceEntries.push_back({"Mythril Ingot", "Smithing", "Tier 5 smithing resource", "Smithing Lv.9", 5});
}

// ============================================================
//  CRAFTING DATABASE
// ============================================================

void Wiki::BuildCraftingDatabase()
{
    craftEntries.clear();

    craftEntries.push_back({"Copper Dagger", "Weapon", "DMG:8  Materials: Copper Ore x2", "Mining Lv.1", 1});
    craftEntries.push_back({"Wooden Staff", "Weapon", "DMG:4  MP:12  Materials: Wood x2 + Simple Ingot x1", "Lumber Lv.1", 1});
    craftEntries.push_back({"Short Bow", "Weapon", "DMG:10  MP:2  Materials: Wood x2 + Simple Ingot x1", "Lumber Lv.1", 1});
    craftEntries.push_back({"Iron Sword", "Weapon", "DMG:14  MP:2  Materials: Iron Ore x2 + Wood x2", "Mining Lv.3", 1});
    craftEntries.push_back({"Oak Staff", "Weapon", "DMG:8  MP:20  Materials: Oak Wood x2 + Steel Ingot x1", "Lumber Lv.3", 1});
    craftEntries.push_back({"Reinforced Bow", "Weapon", "DMG:16  MP:4  Materials: Oak Wood x2 + Steel Ingot x1", "Lumber Lv.3", 1});
    craftEntries.push_back({"Cloth Armor", "Armor", "DEF:4  Materials: Simple Ingot x2 + Wood x2", "Smith Lv.3", 1});
    craftEntries.push_back({"Steel Blade", "Weapon", "DMG:22  MP:5  Materials: Silver Ore x2 + Oak Wood x2", "Mining Lv.5", 2});
    craftEntries.push_back({"Crystal Staff", "Weapon", "DMG:12  MP:30  Materials: Maple Wood x2 + Silver Ingot x1", "Lumber Lv.5", 2});
    craftEntries.push_back({"Longbow", "Weapon", "DMG:24  MP:6  Materials: Maple Wood x2 + Silver Ingot x1", "Lumber Lv.5", 2});
    craftEntries.push_back({"Chainmail", "Armor", "DEF:14  Materials: Silver Ingot x2 + Maple Wood x2", "Smith Lv.5", 2});
    craftEntries.push_back({"Silver Ring", "Accessory", "HP:30  MP:15  Materials: Salmon x2 + Silver Ore x1", "Fish Lv.5", 2});
    craftEntries.push_back({"Gold Longsword", "Weapon", "DMG:30  MP:8  Materials: Gold Ore x2 + Maple Wood x2", "Mining Lv.7", 3});
    craftEntries.push_back({"Ebony Wand", "Weapon", "DMG:16  MP:42  Materials: Ebony Wood x2 + Gold Ingot x1", "Lumber Lv.7", 3});
    craftEntries.push_back({"Ebony Bow", "Weapon", "DMG:32  MP:10  Materials: Ebony Wood x2 + Gold Ingot x1", "Lumber Lv.7", 3});
    craftEntries.push_back({"Plate Armor", "Armor", "DEF:20  Materials: Gold Ingot x2 + Ebony Wood x2", "Smith Lv.7", 3});
    craftEntries.push_back({"Gold Amulet", "Accessory", "HP:50  MP:30  Materials: Tuna x2 + Gold Ore x1", "Fish Lv.7", 3});
    craftEntries.push_back({"Mythril Greatsword", "Weapon", "DMG:42  MP:12  Materials: Mythril Ore x2 + Ebony Wood x2", "Mining Lv.9", 4});
    craftEntries.push_back({"Magical Scepter", "Weapon", "DMG:22  MP:55  Materials: Magical Wood x2 + Mythril Ingot x1", "Lumber Lv.9", 4});
    craftEntries.push_back({"Mythril Bow", "Weapon", "DMG:40  MP:14  Materials: Magical Wood x2 + Mythril Ingot x1", "Lumber Lv.9", 4});
    craftEntries.push_back({"Mythril Armor", "Armor", "DEF:28  Materials: Mythril Ingot x2 + Magical Wood x2", "Smith Lv.9", 4});
}

// ============================================================
//  AREA DATABASE
// ============================================================

void Wiki::BuildAreaDatabase()
{
    areaEntries.clear();
    if (!areas) return;

    for (const auto& a : *areas)
    {
        std::string info = "Difficulty: " + std::to_string(a.difficulty);
        info += "  |  Dungeons: " + std::to_string(a.dungeons.size());
        info += "  |  " + a.description;

        std::string dungeons;
        for (size_t i = 0; i < a.dungeons.size(); ++i)
        {
            if (i > 0) dungeons += ", ";
            dungeons += a.dungeons[i].name + " (" + std::to_string(a.dungeons[i].rooms.size()) + " rooms)";
        }
        if (!dungeons.empty())
            info += "  |  [" + dungeons + "]";

        areaEntries.push_back({a.name, "Area", info, "", a.difficulty});
    }
}

// ============================================================
//  DRAW
// ============================================================

void Wiki::Draw(GRenderer& renderer)
{
    renderer.DrawPanel(20, 40, GRenderer::W - 40, GRenderer::H - 80, "Codex");

    DrawTabBar(renderer);

    switch (currentTab)
    {
        case WikiTab::Equipment: DrawTabPage(renderer, equipEntries, "Equipment"); break;
        case WikiTab::Skills:    DrawTabPage(renderer, skillEntries, "Skills"); break;
        case WikiTab::Enemies:   DrawTabPage(renderer, enemyEntries, "Enemies"); break;
        case WikiTab::Resources: DrawTabPage(renderer, resourceEntries, "Resources"); break;
        case WikiTab::Crafting:  DrawTabPage(renderer, craftEntries, "Crafting Recipes"); break;
        case WikiTab::Areas:     DrawTabPage(renderer, areaEntries, "Areas"); break;
        default: break;
    }
}

void Wiki::DrawTabBar(GRenderer& renderer)
{
    const char* tabNames[] = { "Equipment", "Skills", "Enemies", "Resources", "Crafting", "Areas" };
    int tabCount = static_cast<int>(WikiTab::COUNT);
    int tabW = 100;
    int startX = renderer.CenterX(tabCount * tabW);
    int y = 55;

    for (int i = 0; i < tabCount; ++i)
    {
        int x = startX + i * tabW;
        bool selected = (i == static_cast<int>(currentTab));
        bool hover = renderer.IsMouseInRect(x, y, tabW - 4, 32);
        Color bg = selected ? CQColors::Gold : (hover ? CQColors::BtnHover : CQColors::BgPanel);
        Color text = selected ? CQColors::BgDark : CQColors::TextLight;
        renderer.DrawRect(x, y, tabW - 4, 32, bg);
        renderer.DrawRectLines(x, y, tabW - 4, 32, CQColors::BorderLight, 1);
        int tw = MeasureText(tabNames[i], 16);
        int textX = x + (tabW - 4 - tw) / 2;
        renderer.DrawText(tabNames[i], textX, y + 8, 16, text);
        if (hover && renderer.IsMouseClickedOn(x, y, tabW - 4, 32))
        {
            currentTab = static_cast<WikiTab>(i);
            page = 0;
            searchQuery.clear();
            searchBuf[0] = '\0';
        }
    }
}

void Wiki::DrawTabPage(GRenderer& renderer, std::vector<WikiEntry>& entries, const std::string& title)
{
    (void)title;

    // ---- Search bar ----
    int searchY = 96;
    renderer.DrawText("Search:", 40, searchY, 14, CQColors::TextDim);

    int c = GetCharPressed();
    while (c > 0)
    {
        if (c >= 32 && c <= 126)
        {
            size_t len = strlen(searchBuf);
            if (len < sizeof(searchBuf) - 1)
            {
                searchBuf[len] = static_cast<char>(c);
                searchBuf[len + 1] = '\0';
            }
        }
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        size_t len = strlen(searchBuf);
        if (len > 0) searchBuf[len - 1] = '\0';
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        searchBuf[0] = '\0';
    }

    renderer.DrawRect(110, searchY - 2, 300, 22, CQColors::BgInput);
    renderer.DrawRectLines(110, searchY - 2, 300, 22, CQColors::BorderLight, 1);
    std::string display = searchBuf[0] ? searchBuf : "";
    if (display.empty())
        renderer.DrawText("Type to filter...", 115, searchY + 1, 13, CQColors::TextDim);
    else
        renderer.DrawText(display, 115, searchY + 1, 13, CQColors::TextLight);

    // Clear button
    if (searchBuf[0] && renderer.Button("X", 415, searchY - 2, 20, 22))
        searchBuf[0] = '\0';

    searchQuery = searchBuf;

    // ---- Filter entries ----
    std::vector<WikiEntry> filtered;
    if (searchQuery.empty())
    {
        filtered = entries;
    }
    else
    {
        std::string lowerQuery = searchQuery;
        for (auto& c : lowerQuery) c = std::tolower(c);
        for (const auto& e : entries)
        {
            std::string lowerName = e.name;
            for (auto& c : lowerName) c = std::tolower(c);
            std::string lowerCat = e.category;
            for (auto& c : lowerCat) c = std::tolower(c);
            if (lowerName.find(lowerQuery) != std::string::npos ||
                lowerCat.find(lowerQuery) != std::string::npos)
            {
                filtered.push_back(e);
            }
        }
    }

    // ---- Column setup ----
    int y = 126;
    int lineH = 18;
    int rowLimit = 4;
    int headerH = 18;
    int separatorH = 3;
    int totalHeaderH = headerH + separatorH;
    int availableH = GRenderer::H - 160 - totalHeaderH;
    int maxLines = availableH / (lineH + rowLimit);
    if (maxLines < 1) maxLines = 1;
    int totalEntries = static_cast<int>(filtered.size());
    maxPage = std::max(0, (totalEntries - 1) / maxLines);
    if (page > maxPage) page = maxPage;

    int startIdx = page * maxLines;
    int endIdx = std::min(startIdx + maxLines, totalEntries);

    // Define column layouts per tab
    struct Col { int x; int w; };
    static constexpr Col baseCols[4] = { {40, 180}, {230, 190}, {430, 250}, {700, 220} };

    int colX[4];
    int colW[4];
    // Adjust for tab-specific column widths
    if (currentTab == WikiTab::Skills)
    {
        colX[0] = 40;   colW[0] = 200;
        colX[1] = 250;  colW[1] = 150;
        colX[2] = 410;  colW[2] = 290;
        colX[3] = 710;  colW[3] = 210;
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            colX[i] = baseCols[i].x;
            colW[i] = baseCols[i].w;
        }
    }

    // ---- Column headers ----
    Color headerColor = CQColors::TextGold;
    switch (currentTab)
    {
        case WikiTab::Equipment:
            renderer.DrawText("Name", colX[0], y, 14, headerColor);
            renderer.DrawText("Type", colX[1], y, 14, headerColor);
            renderer.DrawText("Stats", colX[2], y, 14, headerColor);
            renderer.DrawText("Source", colX[3], y, 14, headerColor);
            break;
        case WikiTab::Skills:
            renderer.DrawText("Skill", colX[0], y, 14, headerColor);
            renderer.DrawText("Class / Lv.", colX[1], y, 14, headerColor);
            renderer.DrawText("Cost / Damage", colX[2], y, 14, headerColor);
            renderer.DrawText("Effect", colX[3], y, 14, headerColor);
            break;
        case WikiTab::Enemies:
            renderer.DrawText("Enemy", colX[0], y, 14, headerColor);
            renderer.DrawText("Area", colX[1], y, 14, headerColor);
            renderer.DrawText("Stats", colX[2], y, 14, headerColor);
            renderer.DrawText("Loot", colX[3], y, 14, headerColor);
            break;
        case WikiTab::Resources:
            renderer.DrawText("Resource", colX[0], y, 14, headerColor);
            renderer.DrawText("Skill", colX[1], y, 14, headerColor);
            renderer.DrawText("Description", colX[2], y, 14, headerColor);
            renderer.DrawText("Unlock", colX[3], y, 14, headerColor);
            break;
        case WikiTab::Crafting:
            renderer.DrawText("Product", colX[0], y, 14, headerColor);
            renderer.DrawText("Product Type", colX[1], y, 14, headerColor);
            renderer.DrawText("Materials", colX[2], y, 14, headerColor);
            renderer.DrawText("Unlock", colX[3], y, 14, headerColor);
            break;
        case WikiTab::Areas:
            renderer.DrawText("Area", colX[0], y, 14, headerColor);
            renderer.DrawText("Info", colX[1], y, 14, headerColor);
            break;
        default: break;
    }

    y += headerH + 2;
    renderer.DrawRect(40, y - 2, GRenderer::W - 80, 1, CQColors::BorderLight);
    y += separatorH;

    // ---- Entries ----
    for (int i = startIdx; i < endIdx; ++i)
    {
        const auto& e = filtered[i];

        // Alternating row background
        if ((i - startIdx) % 2 == 0)
            renderer.DrawRect(38, y - 1, GRenderer::W - 76, lineH + 1, CQColors::BgDark);
        else
            renderer.DrawRect(38, y - 1, GRenderer::W - 76, lineH + 1, CQColors::BgPanel);

        Color textColor = (e.rarity >= 5) ? CQColors::GoldBright
                        : (e.rarity >= 3) ? CQColors::TextGreen
                        : CQColors::TextLight;

        // Draw name (truncate if needed)
        std::string name = e.name;
        int maxNameW = colW[0] - 5;
        while (!name.empty() && MeasureText(name.c_str(), 14) > maxNameW)
            name.pop_back();
        renderer.DrawText(name, colX[0], y, 14, textColor);

        // Draw category
        renderer.DrawText(e.category, colX[1], y, 13, CQColors::TextDim);

        // Draw info (truncate to column width)
        std::string info = e.info;
        int maxInfoW = colW[2] - 5;
        while (!info.empty() && MeasureText(info.c_str(), 12) > maxInfoW)
            info.pop_back();
        renderer.DrawText(info, colX[2], y, 12, CQColors::TextLight);

        // Draw 4th column (source or lootInfo for enemies)
        if (currentTab == WikiTab::Enemies && !e.lootInfo.empty())
            renderer.DrawText(e.lootInfo, colX[3], y, 12, CQColors::TextDim);
        else if (!e.source.empty())
            renderer.DrawText(e.source, colX[3], y, 12, CQColors::TextDim);

        y += lineH + rowLimit;
    }

    // ---- Results summary ----
    std::string resultInfo = std::to_string(filtered.size()) + " entries";
    if (!searchQuery.empty())
        resultInfo += " (filtered)";
    renderer.DrawRightText(resultInfo, GRenderer::W - 50, y + 4, 13, CQColors::TextDim);
    y += 18;

    // ---- Page nav ----
    int navY = GRenderer::H - 90;
    if (page > 0)
    {
        if (renderer.Button("< Prev", renderer.CenterX(200) - 110, navY, 100, 32))
            page--;
    }
    if (page < maxPage)
    {
        if (renderer.Button("Next >", renderer.CenterX(200) + 10, navY, 100, 32))
            page++;
    }
    std::string pg = "Page " + std::to_string(page + 1) + "/" + std::to_string(maxPage + 1);
    renderer.DrawCenteredText(pg, navY + 8, 16, CQColors::TextDim);
}
