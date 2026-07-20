#include "Loot.hpp"
#include "Uniques/UniqueItems.hpp"
#include <cstdlib>
#include "Consumable.hpp"

static ElementType RandomElement()
{
    int roll = rand() % 5;
    switch (roll)
    {
        case 0: return ElementType::Fire;
        case 1: return ElementType::Ice;
        case 2: return ElementType::Lightning;
        case 3: return ElementType::Poison;
        case 4: return ElementType::Holy;
        default: return ElementType::Physical;
    }
}

static std::map<ElementType, int> RandomElementalResist(int difficulty)
{
    std::map<ElementType, int> resist;
    if (rand() % 100 < 25 + difficulty * 5)
    {
        ElementType elem = RandomElement();
        resist[elem] = 1 + difficulty + rand() % (difficulty + 1);
    }
    return resist;
}

static int RollRarity(int commonPct, int uncommonPct, int rarePct, int epicPct, int /*legendaryPct*/)
{
    int roll = rand() % 100;
    if (roll < commonPct)    return 1;
    if (roll < commonPct + uncommonPct)  return 2;
    if (roll < commonPct + uncommonPct + rarePct)  return 3;
    if (roll < commonPct + uncommonPct + rarePct + epicPct)  return 4;
    return 5;
}

static int RollNormalRarity(int dropTier)
{
    if (dropTier <= 2)  return RollRarity(80, 20, 0, 0, 0);
    if (dropTier <= 4)  return RollRarity(60, 35, 5, 0, 0);
    if (dropTier <= 6)  return RollRarity(40, 40, 20, 0, 0);
    if (dropTier <= 8)  return RollRarity(0, 50, 40, 10, 0);
    if (dropTier <= 10) return RollRarity(0, 30, 50, 20, 0);
    if (dropTier <= 12) return RollRarity(0, 0, 50, 40, 10);
    if (dropTier <= 14) return RollRarity(0, 0, 30, 50, 20);
    if (dropTier <= 16) return RollRarity(0, 0, 0, 50, 30);
    if (dropTier <= 18) return RollRarity(0, 0, 0, 40, 40);
    return RollRarity(0, 0, 0, 30, 50);
}

static int RollBossRarity(int dropTier)
{
    if (dropTier <= 2)  return RollRarity(0, 70, 30, 0, 0);
    if (dropTier <= 4)  return RollRarity(0, 50, 50, 0, 0);
    if (dropTier <= 6)  return RollRarity(0, 0, 80, 20, 0);
    if (dropTier <= 8)  return RollRarity(0, 0, 60, 40, 0);
    if (dropTier <= 10) return RollRarity(0, 0, 40, 60, 0);
    if (dropTier <= 12) return RollRarity(0, 0, 0, 70, 30);
    if (dropTier <= 14) return RollRarity(0, 0, 0, 50, 50);
    return RollRarity(0, 0, 0, 0, 100);
}

static std::shared_ptr<Item> CreateWeaponOfRarity(int rarity, int difficulty)
{
    switch (rarity)
    {
        case 1: return LootTable::CreateCommonWeapon(difficulty);
        case 2: return LootTable::CreateRareWeapon(difficulty);
        case 3: return LootTable::CreateBossWeapon(difficulty);
        case 4: return LootTable::CreateEpicWeapon(difficulty);
        case 5: return LootTable::CreateLegendaryWeapon(difficulty);
        default: return LootTable::CreateCommonWeapon(difficulty);
    }
}

static std::shared_ptr<Item> CreateArmorOfRarity(int rarity, int difficulty)
{
    switch (rarity)
    {
        case 1: return LootTable::CreateCommonArmor(difficulty);
        case 2: return LootTable::CreateRareArmor(difficulty);
        case 3: return LootTable::CreateBossArmor(difficulty);
        case 4: return LootTable::CreateEpicArmor(difficulty);
        case 5: return LootTable::CreateLegendaryArmor(difficulty);
        default: return LootTable::CreateCommonArmor(difficulty);
    }
}

std::vector<std::shared_ptr<Item>> LootTable::GenerateLoot(int difficulty, int dropTier, int roll)
{
    std::vector<std::shared_ptr<Item>> loot;

    if (roll == -1) roll = rand() % 100;
    int baseDrop = 40 + (difficulty * 5);

    if (roll < baseDrop)
    {
        int rarity = RollNormalRarity(dropTier);
        loot.push_back(CreateWeaponOfRarity(rarity, difficulty));
        roll = rand() % 100;
    }
    if (roll < 35 + (difficulty * 2))
    {
        int rarity = RollNormalRarity(dropTier);
        loot.push_back(CreateArmorOfRarity(rarity, difficulty));
        roll = rand() % 100;
    }
    if (roll < 15 + (difficulty * 2))
    {
        int rarity = RollNormalRarity(dropTier);
        if (rand() % 2 == 0)
            loot.push_back(CreateWeaponOfRarity(rarity, difficulty));
        else
            loot.push_back(CreateArmorOfRarity(rarity, difficulty));
    }
    if (rand() % 100 < 30 + (difficulty * 3))
        loot.push_back(CreatePotion(difficulty));
    return loot;
}

std::vector<std::shared_ptr<Item>> LootTable::GenerateBossLoot(int difficulty, int dropTier, int /*roll*/)
{
    std::vector<std::shared_ptr<Item>> loot;

    // Guaranteed minimum rarity per spec Section 11:
    // Area 1-2 (dropTier 1-2): Uncommon guaranteed, 30% Rare
    // Area 3-4 (dropTier 3-4): Rare guaranteed, 20-40% Epic
    // Area 5-6 (dropTier 5-6): Rare guaranteed, 40-60% Epic
    // Area 7+ (dropTier 7+): Epic guaranteed, up to Legendary
    int guaranteedMin = 2;
    if (dropTier >= 3) guaranteedMin = 3;
    if (dropTier >= 7) guaranteedMin = 4;
    if (dropTier >= 11) guaranteedMin = 5;

    int rarity = RollBossRarity(dropTier);
    if (rarity < guaranteedMin) rarity = guaranteedMin;
    loot.push_back(CreateWeaponOfRarity(rarity, difficulty));

    if (rand() % 2 == 0)
    {
        int armorRarity = RollBossRarity(dropTier);
        if (armorRarity < guaranteedMin) armorRarity = guaranteedMin;
        loot.push_back(CreateArmorOfRarity(armorRarity, difficulty));
    }

    if (rand() % 100 < 60)
        loot.push_back(CreateAccessory(difficulty));

    return loot;
}

// ---- Common ----

std::shared_ptr<Item> LootTable::CreateCommonWeapon(int difficulty)
{
    int dmg = 10 + (difficulty * 4);
    int mana = rand() % (difficulty * 2);
    ElementType elem = (rand() % 3 == 0) ? RandomElement() : ElementType::Physical;
    int elemDmg = (elem != ElementType::Physical) ? 2 + difficulty + rand() % (difficulty + 2) : 0;
    switch (rand() % 8)
    {
        case 0: return std::make_shared<Weapon>("Iron Sword", dmg, mana, 1, elem, elemDmg, WeaponType::Sword);
        case 1: return std::make_shared<Weapon>("Steel Axe", dmg + 2, mana, 1, elem, elemDmg, WeaponType::Axe);
        case 2: return std::make_shared<Weapon>("Wooden Staff", dmg - 2, mana + 5, 1, elem, elemDmg, WeaponType::Staff);
        case 3: return std::make_shared<Weapon>("Dagger", dmg - 5, 0, 1, elem, elemDmg, WeaponType::Dagger);
        case 4: return std::make_shared<Weapon>("Apprentice Wand", dmg - 3, mana + 8, 1, elem, elemDmg, WeaponType::Wand);
        case 5: return std::make_shared<Weapon>("Training Staff", dmg - 1, mana + 6, 1, elem, elemDmg, WeaponType::Staff);
        case 6: return std::make_shared<Weapon>("Shortbow", dmg - 2, 0, 1, elem, elemDmg, WeaponType::Bow);
        case 7: return std::make_shared<Weapon>("Spellbook", dmg - 4, mana + 12, 1, elem, elemDmg, WeaponType::Wand);
        default: return std::make_shared<Weapon>("Sword", dmg, mana, 1, elem, elemDmg, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateCommonArmor(int difficulty)
{
    int def = 1 + difficulty;
    ArmorType type = (rand() % 2 == 0) ? ArmorType::Leather : ArmorType::Cloth;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);
    std::string name;
    switch (piece)
    {
        case ArmorPiece::Helmet:
            name = (type == ArmorType::Leather) ? "Leather Helmet" : ((rand() % 2) ? "Cloth Cap" : "Apprentice Hood");
            break;
        case ArmorPiece::Chest:
            name = (type == ArmorType::Leather) ? "Leather Chest" : ((rand() % 2) ? "Cloth Robe" : "Apprentice Robe");
            break;
        case ArmorPiece::Gloves:
            name = (type == ArmorType::Leather) ? "Leather Gloves" : ((rand() % 2) ? "Cloth Gloves" : "Apprentice Mitts");
            break;
        case ArmorPiece::Pants:
            name = (type == ArmorType::Leather) ? "Leather Pants" : ((rand() % 2) ? "Cloth Pants" : "Apprentice Trousers");
            break;
        case ArmorPiece::Boots:
            name = (type == ArmorType::Leather) ? "Leather Boots" : ((rand() % 2) ? "Cloth Boots" : "Apprentice Sandals");
            break;
    }
    return std::make_shared<Armor>(name, type, piece, def, 1, RandomElementalResist(difficulty));
}

// ---- Rare ----

std::shared_ptr<Item> LootTable::CreateRareWeapon(int difficulty)
{
    int dmg = 8 + (difficulty * 12);
    int mana = 3 + (difficulty * 2);
    ElementType elem = RandomElement();
    int elemDmg = 3 + difficulty * 2 + rand() % (difficulty * 2 + 3);
    switch (rand() % 16)
    {
        case 0: return std::make_shared<Weapon>("Enchanted Blade", dmg, mana, 2, elem, elemDmg, WeaponType::Sword);
        case 1: return std::make_shared<Weapon>("Mithril Mace", dmg + 5, mana, 2, elem, elemDmg, WeaponType::Mace);
        case 2: return std::make_shared<Weapon>("Crystal Staff", dmg, mana + 10, 2, elem, elemDmg, WeaponType::Staff);
        case 3: return std::make_shared<Weapon>("Shadowfang", dmg + 8, mana - 2, 2, elem, elemDmg, WeaponType::Dagger);
        case 4: return std::make_shared<Weapon>("Thunder Bow", dmg + 3, mana + 5, 2, elem, elemDmg, WeaponType::Bow);
        case 5: return std::make_shared<Weapon>("Runed Greatsword", dmg + 10, mana, 2, elem, elemDmg, WeaponType::Sword);
        case 6: return std::make_shared<Weapon>("Arcane Wand", dmg - 2, mana + 15, 2, elem, elemDmg, WeaponType::Wand);
        case 7: return std::make_shared<Weapon>("Frost Staff", dmg + 2, mana + 8, 2, ElementType::Ice, elemDmg, WeaponType::Staff);
        case 8: return std::make_shared<Weapon>("Fire Staff", dmg + 5, mana + 5, 2, ElementType::Fire, elemDmg, WeaponType::Staff);
        case 9: return std::make_shared<Weapon>("Storm Caller's Staff", dmg + 3, mana + 12, 2, ElementType::Lightning, elemDmg, WeaponType::Staff);
        case 10: return std::make_shared<Weapon>("Venomfang Dagger", dmg + 2, mana + 3, 2, ElementType::Poison, elemDmg + 2, WeaponType::Dagger);
        case 11: return std::make_shared<Weapon>("Holy Scepter", dmg + 4, mana + 8, 2, ElementType::Holy, elemDmg + 1, WeaponType::Scepter);
        case 12: return std::make_shared<Weapon>("Inferno Axe", dmg + 8, mana, 2, ElementType::Fire, elemDmg + 3, WeaponType::Axe);
        case 13: return std::make_shared<Weapon>("Glacial Bow", dmg + 3, mana + 6, 2, ElementType::Ice, elemDmg + 2, WeaponType::Bow);
        case 14: return std::make_shared<Weapon>("Thunderstrike Hammer", dmg + 10, mana - 3, 2, ElementType::Lightning, elemDmg + 2, WeaponType::Hammer);
        default: return std::make_shared<Weapon>("Legendary Sword", dmg, mana, 2, elem, elemDmg, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateRareArmor(int difficulty)
{
    int def = 3 + (difficulty * 2);
    int roll = rand() % 5;
    ArmorType type;
    if (roll == 0) type = ArmorType::Plate;
    else if (roll == 1) type = ArmorType::Leather;
    else if (roll == 2) type = ArmorType::Cloth;
    else if (roll == 3) type = ArmorType::Cloth;
    else type = ArmorType::Leather;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);
    std::string name;
    std::map<ElementType, int> resist;
    if (type == ArmorType::Cloth && roll == 2)
    {
        ElementType elem = RandomElement();
        resist[elem] = 2 + difficulty;
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Elemental Circlet"; break;
            case ArmorPiece::Chest:  name = "Elemental Robe"; break;
            case ArmorPiece::Gloves: name = "Elemental Handwraps"; break;
            case ArmorPiece::Pants:  name = "Elemental Leggings"; break;
            case ArmorPiece::Boots:  name = "Elemental Sandals"; break;
        }
        return std::make_shared<Armor>(name, type, piece, def, 2, resist);
    }
    else if (type == ArmorType::Leather && roll == 3)
    {
        ElementType elem = RandomElement();
        resist[elem] = 2 + difficulty;
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Ranger's Hood"; break;
            case ArmorPiece::Chest:  name = "Ranger's Vest"; break;
            case ArmorPiece::Gloves: name = "Ranger's Grips"; break;
            case ArmorPiece::Pants:  name = "Ranger's Leggings"; break;
            case ArmorPiece::Boots:  name = "Ranger's Boots"; break;
        }
        return std::make_shared<Armor>(name, type, piece, def, 2, resist);
    }
    if (type == ArmorType::Cloth)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Scholar's Circlet"; break;
            case ArmorPiece::Chest:  name = "Mage's Robe"; break;
            case ArmorPiece::Gloves: name = "Spellbinder Gloves"; break;
            case ArmorPiece::Pants:  name = "Enchanted Leggings"; break;
            case ArmorPiece::Boots:  name = "Traveler's Boots"; break;
        }
    }
    else
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = (type == ArmorType::Plate) ? "Plate Helm" : "Reinforced Helm"; break;
            case ArmorPiece::Chest:  name = (type == ArmorType::Plate) ? "Plate Armor" : "Reinforced Chest"; break;
            case ArmorPiece::Gloves: name = (type == ArmorType::Plate) ? "Plate Gauntlets" : "Reinforced Gloves"; break;
            case ArmorPiece::Pants:  name = (type == ArmorType::Plate) ? "Plate Leggings" : "Reinforced Pants"; break;
            case ArmorPiece::Boots:  name = (type == ArmorType::Plate) ? "Plate Boots" : "Reinforced Boots"; break;
        }
    }
    return std::make_shared<Armor>(name, type, piece, def, 2, RandomElementalResist(difficulty));
}

// ---- Boss ----

std::shared_ptr<Item> LootTable::CreateBossWeapon(int difficulty)
{
    int dmg = 20 + (difficulty * 16);
    int mana = 10 + (difficulty * 3);
    int rarity = 3;
    int elemDmg = 5 + difficulty * 4;
    switch (rand() % 12)
    {
        case 0: return std::make_shared<Weapon>("Flamebrand", dmg, mana, rarity, ElementType::Fire, elemDmg, WeaponType::Sword);
        case 1: return std::make_shared<Weapon>("Frostbite", dmg + 3, mana + 5, rarity, ElementType::Ice, elemDmg, WeaponType::Sword);
        case 2: return std::make_shared<Weapon>("Thunderstrike", dmg + 5, mana, rarity, ElementType::Lightning, elemDmg, WeaponType::Hammer);
        case 3: return std::make_shared<Weapon>("Doomhammer", dmg + 10, mana - 5, rarity, ElementType::Poison, elemDmg, WeaponType::Hammer);
        case 4: return std::make_shared<Weapon>("Arcane Staff", dmg - 5, mana + 20, rarity, ElementType::Arcane, elemDmg, WeaponType::Staff);
        case 5: return std::make_shared<Weapon>("Voidreaver", dmg + 15, mana + 10, rarity, ElementType::Arcane, elemDmg, WeaponType::Axe);
        case 6: return std::make_shared<Weapon>("Celestial Bow", dmg + 8, mana + 15, rarity, ElementType::Holy, elemDmg, WeaponType::Bow);
        case 7: return std::make_shared<Weapon>("Soul Reaper", dmg + 20, mana - 10, rarity, ElementType::Poison, elemDmg, WeaponType::Axe);
        case 8: return std::make_shared<Weapon>("Staff of Ages", dmg, mana + 25, rarity, ElementType::Fire, elemDmg, WeaponType::Staff);
        case 9: return std::make_shared<Weapon>("Wand of the Void", dmg - 3, mana + 30, rarity, ElementType::Arcane, elemDmg, WeaponType::Wand);
        case 10: return std::make_shared<Weapon>("Scepter of Stars", dmg + 5, mana + 20, rarity, ElementType::Holy, elemDmg, WeaponType::Scepter);
        case 11: return std::make_shared<Weapon>("Tome of Elements", dmg - 8, mana + 40, rarity, ElementType::Ice, elemDmg, WeaponType::Wand);
        default: return std::make_shared<Weapon>("Boss Weapon", dmg, mana, rarity, ElementType::Fire, elemDmg, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateBossArmor(int difficulty)
{
    int def = 6 + (difficulty * 3);
    int rarity = 3;
    int roll = rand() % 3;
    ArmorType type;
    if (roll == 0) type = ArmorType::Plate;
    else if (roll == 1) type = ArmorType::Leather;
    else type = ArmorType::Cloth;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);
    std::string name;
    if (type == ArmorType::Cloth)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Crown of the Archmage"; break;
            case ArmorPiece::Chest:  name = "Ethereal Mantle"; break;
            case ArmorPiece::Gloves: name = "Hands of the Weave"; break;
            case ArmorPiece::Pants:  name = "Leggings of Eternity"; break;
            case ArmorPiece::Boots:  name = "Sandals of Transcendence"; break;
        }
    }
    else
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Dragon Helm"; break;
            case ArmorPiece::Chest:  name = "Titan Chestplate"; break;
            case ArmorPiece::Gloves: name = "Gauntlets of Power"; break;
            case ArmorPiece::Pants:  name = "Guardian Legplates"; break;
            case ArmorPiece::Boots:  name = "Boots of Swiftness"; break;
        }
    }
    return std::make_shared<Armor>(name, type, piece, def, rarity, RandomElementalResist(difficulty));
}

// ---- Epic ----

std::shared_ptr<Item> LootTable::CreateEpicWeapon(int difficulty)
{
    int dmg = 35 + (difficulty * 28);
    int mana = 15 + (difficulty * 5);
    int rarity = 4;
    int elemDmg = 10 + difficulty * 6;
    switch (rand() % 14)
    {
        case 0: return std::make_shared<Weapon>("Inferno Greatsword", dmg + 10, mana, rarity, ElementType::Fire, elemDmg, WeaponType::Sword);
        case 1: return std::make_shared<Weapon>("Glacial Halberd", dmg + 8, mana + 10, rarity, ElementType::Ice, elemDmg, WeaponType::Sword);
        case 2: return std::make_shared<Weapon>("Stormcaller's Hammer", dmg + 15, mana + 5, rarity, ElementType::Lightning, elemDmg, WeaponType::Hammer);
        case 3: return std::make_shared<Weapon>("Plaguebringer's Scythe", dmg + 20, mana - 5, rarity, ElementType::Poison, elemDmg + 5, WeaponType::Axe);
        case 4: return std::make_shared<Weapon>("Arcane Dominance Staff", dmg - 2, mana + 40, rarity, ElementType::Arcane, elemDmg, WeaponType::Staff);
        case 5: return std::make_shared<Weapon>("Celestial Longbow", dmg + 12, mana + 20, rarity, ElementType::Holy, elemDmg, WeaponType::Bow);
        case 6: return std::make_shared<Weapon>("Soulreaver", dmg + 25, mana - 10, rarity, ElementType::Fire, elemDmg + 3, WeaponType::Axe);
        case 7: return std::make_shared<Weapon>("Eternal Frost Staff", dmg, mana + 35, rarity, ElementType::Ice, elemDmg + 2, WeaponType::Staff);
        case 8: return std::make_shared<Weapon>("Thunderfury", dmg + 18, mana + 15, rarity, ElementType::Lightning, elemDmg, WeaponType::Sword);
        case 9: return std::make_shared<Weapon>("Wand of Worlds", dmg - 5, mana + 50, rarity, ElementType::Arcane, elemDmg + 3, WeaponType::Wand);
        case 10: return std::make_shared<Weapon>("Scepter of Dawn", dmg + 5, mana + 30, rarity, ElementType::Holy, elemDmg + 2, WeaponType::Scepter);
        case 11: return std::make_shared<Weapon>("Venomstrike Dagger", dmg + 6, mana + 8, rarity, ElementType::Poison, elemDmg + 5, WeaponType::Dagger);
        case 12: return std::make_shared<Weapon>("Tome of Arcane Secrets", dmg - 8, mana + 60, rarity, ElementType::Arcane, elemDmg, WeaponType::Wand);
        default: return std::make_shared<Weapon>("Epic Weapon", dmg, mana, rarity, ElementType::Fire, elemDmg, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateEpicArmor(int difficulty)
{
    int def = 8 + (difficulty * 4);
    int rarity = 4;
    int roll = rand() % 3;
    ArmorType type;
    if (roll == 0) type = ArmorType::Plate;
    else if (roll == 1) type = ArmorType::Leather;
    else type = ArmorType::Cloth;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);
    std::string name;
    auto resist = RandomElementalResist(difficulty);
    resist[RandomElement()] += 3 + difficulty;

    if (type == ArmorType::Cloth)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Crown of the Archmage"; break;
            case ArmorPiece::Chest:  name = "Ethereal Mantle"; break;
            case ArmorPiece::Gloves: name = "Hands of the Weave"; break;
            case ArmorPiece::Pants:  name = "Leggings of Eternity"; break;
            case ArmorPiece::Boots:  name = "Sandals of Transcendence"; break;
        }
    }
    else if (type == ArmorType::Leather)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Shadow Stalker Hood"; break;
            case ArmorPiece::Chest:  name = "Shadow Stalker Vest"; break;
            case ArmorPiece::Gloves: name = "Shadow Stalker Grips"; break;
            case ArmorPiece::Pants:  name = "Shadow Stalker Leggings"; break;
            case ArmorPiece::Boots:  name = "Shadow Stalker Boots"; break;
        }
    }
    else
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Titan's Helm"; break;
            case ArmorPiece::Chest:  name = "Titan's Plate"; break;
            case ArmorPiece::Gloves: name = "Titan's Gauntlets"; break;
            case ArmorPiece::Pants:  name = "Titan's Greaves"; break;
            case ArmorPiece::Boots:  name = "Titan's Sabatons"; break;
        }
    }
    return std::make_shared<Armor>(name, type, piece, def, rarity, resist);
}

// ---- Legendary ----

std::shared_ptr<Item> LootTable::CreateLegendaryWeapon(int difficulty)
{
    int dmg = 55 + (difficulty * 40);
    int mana = 25 + (difficulty * 8);
    int rarity = 5;
    int elemDmg = 15 + difficulty * 8;
    switch (rand() % 12)
    {
        case 0: return std::make_shared<Weapon>("Worldsplitter", dmg + 20, mana + 10, rarity, ElementType::Fire, elemDmg + 5, WeaponType::Sword);
        case 1: return std::make_shared<Weapon>("Frostmourne's Echo", dmg + 15, mana + 20, rarity, ElementType::Ice, elemDmg + 8, WeaponType::Sword);
        case 2: return std::make_shared<Weapon>("Stormbringer", dmg + 25, mana + 15, rarity, ElementType::Lightning, elemDmg + 3, WeaponType::Hammer);
        case 3: return std::make_shared<Weapon>("Pestilence", dmg + 30, mana, rarity, ElementType::Poison, elemDmg + 10, WeaponType::Axe);
        case 4: return std::make_shared<Weapon>("Staff of Creation", dmg + 5, mana + 60, rarity, ElementType::Arcane, elemDmg + 5, WeaponType::Staff);
        case 5: return std::make_shared<Weapon>("Dawnbreaker", dmg + 18, mana + 25, rarity, ElementType::Holy, elemDmg + 7, WeaponType::Bow);
        case 6: return std::make_shared<Weapon>("Doomhammer's Legacy", dmg + 35, mana - 8, rarity, ElementType::Fire, elemDmg + 6, WeaponType::Hammer);
        case 7: return std::make_shared<Weapon>("Scepter of the Ancients", dmg + 8, mana + 45, rarity, ElementType::Arcane, elemDmg + 4, WeaponType::Scepter);
        case 8: return std::make_shared<Weapon>("Voidrender", dmg + 40, mana + 5, rarity, ElementType::Poison, elemDmg + 8, WeaponType::Axe);
        case 9: return std::make_shared<Weapon>("Tome of Infinite Wisdom", dmg - 5, mana + 80, rarity, ElementType::Arcane, elemDmg + 3, WeaponType::Wand);
        case 10: return std::make_shared<Weapon>("Frostfire Blade", dmg + 22, mana + 18, rarity, ElementType::Ice, elemDmg + 10, WeaponType::Sword);
        case 11: return std::make_shared<Weapon>("Celestial Judgement", dmg + 28, mana + 30, rarity, ElementType::Holy, elemDmg + 6, WeaponType::Scepter);
        default: return std::make_shared<Weapon>("Legendary Weapon", dmg, mana, rarity, ElementType::Physical, elemDmg, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateLegendaryArmor(int difficulty)
{
    int def = 12 + (difficulty * 6);
    int rarity = 5;
    int roll = rand() % 3;
    ArmorType type;
    if (roll == 0) type = ArmorType::Plate;
    else if (roll == 1) type = ArmorType::Leather;
    else type = ArmorType::Cloth;
    ArmorPiece piece = static_cast<ArmorPiece>(rand() % 5);
    std::string name;
    auto resist = RandomElementalResist(difficulty);
    resist[RandomElement()] += 5 + difficulty * 2;

    if (type == ArmorType::Cloth)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Crown of the God-King"; break;
            case ArmorPiece::Chest:  name = "Mantle of the Cosmos"; break;
            case ArmorPiece::Gloves: name = "Hands of Fate"; break;
            case ArmorPiece::Pants:  name = "Legwraps of Infinity"; break;
            case ArmorPiece::Boots:  name = "Treads of the Wind"; break;
        }
    }
    else if (type == ArmorType::Leather)
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Umbral Stalker Hood"; break;
            case ArmorPiece::Chest:  name = "Umbral Stalker Vest"; break;
            case ArmorPiece::Gloves: name = "Umbral Stalker Claws"; break;
            case ArmorPiece::Pants:  name = "Umbral Stalker Legs"; break;
            case ArmorPiece::Boots:  name = "Umbral Stalker Treads"; break;
        }
    }
    else
    {
        switch (piece)
        {
            case ArmorPiece::Helmet: name = "Helm of the Worldbearer"; break;
            case ArmorPiece::Chest:  name = "Chestplate of Eternity"; break;
            case ArmorPiece::Gloves: name = "Gauntlets of the Colossus"; break;
            case ArmorPiece::Pants:  name = "Legplates of the Guardian"; break;
            case ArmorPiece::Boots:  name = "Sabatons of the Leviathan"; break;
        }
    }
    return std::make_shared<Armor>(name, type, piece, def, rarity, resist);
}

std::shared_ptr<Item> LootTable::CreateAccessory(int difficulty)
{
    int hp = 20 + (difficulty * 10);
    int mana = 10 + (difficulty * 8);
    int rarity = 2 + (difficulty > 3 ? 1 : 0);
    int elemDmg = 2 + difficulty + rand() % (difficulty + 3);
    switch (rand() % 18)
    {
        case 0: return std::make_shared<Accessory>("Ring of Health", hp, 0, rarity);
        case 1: return std::make_shared<Accessory>("Ring of Mana", 0, mana, rarity);
        case 2: return std::make_shared<Accessory>("Amulet of Balance", hp / 2, mana / 2, rarity);
        case 3: return std::make_shared<Accessory>("Seal of Power", hp + 10, mana + 10, rarity + 1);
        case 4: return std::make_shared<Accessory>("Band of Fortitude", hp + 20, 0, rarity);
        case 5: return std::make_shared<Accessory>("Pendant of Wisdom", 0, mana + 20, rarity);
        case 6: return std::make_shared<Accessory>("Tears of Eluna", hp + 15, mana + 15, rarity + 1);
        case 7: return std::make_shared<Accessory>("Orb of Focus", 0, mana + 30, rarity);
        case 8: return std::make_shared<Accessory>("Ring of Spellpower", hp - 10, mana + 25, rarity);
        case 9: return std::make_shared<Accessory>("Amulet of the Sage", hp / 2, mana * 2, rarity);
        case 10: return std::make_shared<Accessory>("Arcane Core", hp / 3, mana + 35, rarity + 1);
        case 11: return std::make_shared<Accessory>("Ember Ring", hp - 5, mana + 10, rarity, ElementType::Fire, elemDmg);
        case 12: return std::make_shared<Accessory>("Frost Pendant", hp, mana + 15, rarity, ElementType::Ice, elemDmg);
        case 13: return std::make_shared<Accessory>("Storm Amulet", hp + 5, mana + 20, rarity, ElementType::Lightning, elemDmg);
        case 14: return std::make_shared<Accessory>("Venom Band", hp + 10, mana, rarity, ElementType::Poison, elemDmg);
        case 15: return std::make_shared<Accessory>("Holy Charm", hp + 5, mana + 15, rarity, ElementType::Holy, elemDmg);
        case 16: return std::make_shared<Accessory>("Arcane Signet", hp - 5, mana + 30, rarity, ElementType::Arcane, elemDmg);
        case 17: return std::make_shared<Accessory>("Elemental Loop", hp, mana + 10, rarity + 1, RandomElement(), elemDmg + 2);
        default: return std::make_shared<Accessory>("Mystic Ring", hp, mana, rarity);
    }
}

// ---- Potions ----

std::shared_ptr<Item> LootTable::CreatePotion(int difficulty)
{
    switch (rand() % 5)
    {
        case 0: return std::make_shared<Consumable>("Minor Health Potion", 30 + difficulty * 5, 0, 1);
        case 1: return std::make_shared<Consumable>("Health Potion", 60 + difficulty * 10, 0, 2);
        case 2: return std::make_shared<Consumable>("Minor Mana Potion", 0, 20 + difficulty * 4, 1);
        case 3: return std::make_shared<Consumable>("Mana Potion", 0, 50 + difficulty * 8, 2);
        case 4: return std::make_shared<Consumable>("Refreshing Draft", 30 + difficulty * 3, 20 + difficulty * 3, 2);
        default: return std::make_shared<Consumable>("Minor Health Potion", 30 + difficulty * 5, 0, 1);
    }
}

// ============================================================
//  LEGENDARY CLASS-SPECIFIC LOOT (Primordial One)
// ============================================================

std::vector<std::shared_ptr<Item>> LootTable::GenerateLegendaryClassLoot(CharacterClass charClass)
{
    std::vector<std::shared_ptr<Item>> loot;
    loot.push_back(CreateLegendaryWeapon(charClass));
    loot.push_back(CreateLegendaryChest(charClass));
    loot.push_back(CreateLegendaryAccessory(charClass));
    return loot;
}

std::shared_ptr<Item> LootTable::CreateLegendaryWeapon(CharacterClass charClass)
{
    switch (charClass)
    {
        case CharacterClass::Warrior:
            return std::make_shared<Weapon>("Titan's Greatsword", 250, 20, 5, ElementType::Physical, 30, WeaponType::Sword);
        case CharacterClass::Priest:
            return std::make_shared<Weapon>("Divine Scepter", 180, 80, 5, ElementType::Holy, 40, WeaponType::Scepter);
        case CharacterClass::Mage:
            return std::make_shared<Weapon>("Archmage Staff", 200, 100, 5, ElementType::Arcane, 50, WeaponType::Staff);
        case CharacterClass::Archer:
            return std::make_shared<Weapon>("Shadowstrike Bow", 220, 30, 5, ElementType::Poison, 35, WeaponType::Bow);
        case CharacterClass::Merchant:
            return std::make_shared<Weapon>("Gilded Blade", 200, 40, 5, ElementType::Fire, 25, WeaponType::Sword);
        default:
            return std::make_shared<Weapon>("Legendary Weapon", 200, 30, 5, ElementType::Physical, 30, WeaponType::Sword);
    }
}

std::shared_ptr<Item> LootTable::CreateLegendaryChest(CharacterClass charClass)
{
    switch (charClass)
    {
        case CharacterClass::Warrior:
            return std::make_shared<Armor>("Titan's Plate", ArmorType::Plate, ArmorPiece::Chest, 60, 5,
                std::map<ElementType, int>{{ElementType::Fire, 5}, {ElementType::Ice, 5}});
        case CharacterClass::Priest:
            return std::make_shared<Armor>("Divine Robe", ArmorType::Cloth, ArmorPiece::Chest, 40, 5,
                std::map<ElementType, int>{{ElementType::Holy, 8}});
        case CharacterClass::Mage:
            return std::make_shared<Armor>("Archmage Robes", ArmorType::Cloth, ArmorPiece::Chest, 35, 5,
                std::map<ElementType, int>{{ElementType::Arcane, 8}});
        case CharacterClass::Archer:
            return std::make_shared<Armor>("Shadowstalker Vest", ArmorType::Leather, ArmorPiece::Chest, 45, 5,
                std::map<ElementType, int>{{ElementType::Poison, 6}, {ElementType::Lightning, 3}});
        case CharacterClass::Merchant:
            return std::make_shared<Armor>("Gilded Armor", ArmorType::Plate, ArmorPiece::Chest, 50, 5,
                std::map<ElementType, int>{{ElementType::Fire, 4}, {ElementType::Ice, 4}, {ElementType::Lightning, 4}});
        default:
            return std::make_shared<Armor>("Legendary Chest", ArmorType::Plate, ArmorPiece::Chest, 45, 5);
    }
}

std::shared_ptr<Item> LootTable::CreateLegendaryAccessory(CharacterClass charClass)
{
    switch (charClass)
    {
        case CharacterClass::Warrior:
            return std::make_shared<Accessory>("Titan's Might", 200, 50, 5, ElementType::Physical, 30);
        case CharacterClass::Priest:
            return std::make_shared<Accessory>("Divine Blessing", 100, 200, 5, ElementType::Holy, 40);
        case CharacterClass::Mage:
            return std::make_shared<Accessory>("Archmage Orb", 50, 250, 5, ElementType::Arcane, 50);
        case CharacterClass::Archer:
            return std::make_shared<Accessory>("Shadow Cloak", 150, 100, 5, ElementType::Poison, 35);
        case CharacterClass::Merchant:
            return std::make_shared<Accessory>("Gilded Pendant", 180, 120, 5, ElementType::Fire, 25);
        default:
            return std::make_shared<Accessory>("Legendary Ring", 150, 150, 5, ElementType::Arcane, 30);
    }
}

// ============================================================
//  UNIQUE ITEM DROP (from UniqueItemRegistry)
// ============================================================

std::shared_ptr<Item> LootTable::GenerateUniqueDrop(
    const std::string& enemyName, int difficulty, bool /*isBoss*/)
{
    // Collect all uniques from this enemy's drop pool
    auto weapons = UniqueItemRegistry::GetWeaponsByDropSource(enemyName);
    auto armor = UniqueItemRegistry::GetArmorByDropSource(enemyName);
    auto accessories = UniqueItemRegistry::GetAccessoriesByDropSource(enemyName);

    // Build a flat pool of unique names
    std::vector<std::string> pool;
    for (auto* w : weapons) pool.push_back(w->name);
    for (auto* a : armor) pool.push_back(a->name);
    for (auto* ac : accessories) pool.push_back(ac->name);

    if (pool.empty()) return nullptr;

    // Pick one at random
    std::string chosen = pool[rand() % pool.size()];

    // Create it scaled to difficulty
    return UniqueItemRegistry::Create(chosen, difficulty);
}
