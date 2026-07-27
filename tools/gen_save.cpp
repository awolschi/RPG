#include "../Items/Uniques/UniqueItems.hpp"
#include "../Items/Consumable.hpp"
#include "../Items/SummoningItem.hpp"
#include "../Items/Resources/Resources.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <array>
#include <algorithm>

struct ClassStats {
    int hp, mp, str, vit, int_stat, wis, dex, def;
};

ClassStats GetLevel50Stats(int classId) {
    // Calculate level 50 stats: base + 49 * per_level
    switch(classId) {
        case 0: // Warrior
            return {150+49*15, 20+49*2, 12+49*3, 10+49*2, 2, 2, 5+49, 8+49*2};
        case 1: // Mage
            return {80+49*5, 150+49*10, 1, 3+49, 14+49*4, 8+49*2, 4, 2};
        case 2: // Priest
            return {90+49*8, 120+49*8, 2, 4+49, 8+49*2, 12+49*3, 5, 3+49};
        case 3: // Archer
            return {100+49*10, 100+49*5, 5+49*2, 8+49*3, 4+49, 6+49*2, 3+49, 4+49};
        case 4: // Merchant
            return {110+49*12, 110+49*6, 4+49, 6+49*2, 6+49*2, 8+49*2, 5+49, 5+49};
        default:
            return {885, 118, 159, 108, 2, 2, 54, 106}; // Warrior default
    }
}

std::string GetClassName(int classId) {
    switch(classId) {
        case 0: return "Warrior";
        case 1: return "Mage";
        case 2: return "Priest";
        case 3: return "Archer";
        case 4: return "Merchant";
        default: return "Unknown";
    }
}

static void SerializeItem(std::ostream& file, const std::shared_ptr<Item>& item)
{
    if (!item) return;
    file << item->count << "|" << static_cast<int>(item->type) << "|" << item->name << "|" << item->rarity << "|" << item->sellValue
         << "|" << item->setId << "|" << static_cast<int>(item->passive1) << "|" << static_cast<int>(item->passive2);

    if (auto oh = item_cast<Offhand>(item))
        file << "|OH|" << oh->defense << "|" << oh->manaBonus << "|" << oh->arcaneDamage << "|" << static_cast<int>(oh->offhandType) << "|" << oh->damageBonus;
    else if (auto w = item_cast<Weapon>(item))
        file << "|W|" << w->damage << "|" << w->manaCost << "|" << static_cast<int>(w->element) << "|" << w->elementDamage
             << "|" << static_cast<int>(w->weaponType);
    else if (auto a = item_cast<Armor>(item))
    {
        file << "|A|" << static_cast<int>(a->armorType) << "|" << static_cast<int>(a->piece) << "|" << a->defense;
        file << "|" << a->elementalResist.size();
        for (const auto& [elem, val] : a->elementalResist)
            file << "|" << static_cast<int>(elem) << "|" << val;
    }
    else if (auto ac = item_cast<Accessory>(item))
        file << "|AC|" << ac->bonusHealth << "|" << ac->bonusMana << "|" << static_cast<int>(ac->element) << "|" << ac->elementDamage;

    file << "|" << item->requiredLevel << "\n";
}

int main(int argc, char* argv[])
{
    int slotNum = 2;
    int classId = 0; // Default: Warrior
    
    if (argc > 1)
    {
        slotNum = std::stoi(argv[1]);
        if (slotNum < 1 || slotNum > 10)
        {
            std::cerr << "Slot number must be between 1 and 10" << std::endl;
            return 1;
        }
    }
    if (argc > 2)
    {
        classId = std::stoi(argv[2]);
        if (classId < 0 || classId > 4)
        {
            std::cerr << "Class ID must be between 0 and 4 (Warrior, Mage, Priest, Archer, Merchant)" << std::endl;
            return 1;
        }
    }

    UniqueItemRegistry::Initialize();

    const int DIFF = 10;
    const int PLAYER_LEVEL = 50;

    // Collect all legendary names from the registry
    std::vector<std::shared_ptr<Item>> legendaryItems;

    for (const auto& w : UniqueItemRegistry::GetWeaponsByRarity(Rarity::Legendary))
        if (auto item = UniqueItemRegistry::Create(w->name, DIFF))
            legendaryItems.push_back(item);

    for (const auto& a : UniqueItemRegistry::GetArmorByRarity(Rarity::Legendary))
        if (auto item = UniqueItemRegistry::Create(a->name, DIFF))
            legendaryItems.push_back(item);

    for (const auto& ac : UniqueItemRegistry::GetAccessoriesByRarity(Rarity::Legendary))
        if (auto item = UniqueItemRegistry::Create(ac->name, DIFF))
            legendaryItems.push_back(item);

    for (const auto& oh : UniqueItemRegistry::GetOffhandsByRarity(Rarity::Legendary))
        if (auto item = UniqueItemRegistry::Create(oh->name, DIFF))
            legendaryItems.push_back(item);

    // Set requiredLevel on all items to 1 so a level 50 can equip any
    for (auto& item : legendaryItems)
        item->requiredLevel = 1;

    std::cout << "Total legendary items: " << legendaryItems.size() << std::endl;

    // Create save directory — use absolute path so it works from any cwd
    std::filesystem::create_directories("./saves");
    std::filesystem::create_directories("../build/saves");
    std::string filename = "./saves/slot" + std::to_string(slotNum) + ".sav";
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << " for writing" << std::endl;
        return 1;
    }

    // Get class-specific stats
    ClassStats stats = GetLevel50Stats(classId);
    std::string className = GetClassName(classId);

    // --- SAVE_VERSION ---
    file << "SAVE_VERSION 15\n";

    // --- Player name ---
    file << "TestHero_" << className << "\n";

    // --- CharacterClass ---
    file << classId << "\n";

    // --- CharacterRace: Aran = 0 ---
    file << "0\n";

    // --- Level & XP ---
    file << PLAYER_LEVEL << " " << 0 << "\n";

    // --- Current HP & Mana (full) ---
    file << stats.hp << " " << stats.mp << "\n";

    // --- Stats (level 50 class-specific) ---
    file << stats.hp << " " << stats.mp << " " << stats.str << " " << stats.vit << " "
         << stats.int_stat << " " << stats.wis << " " << stats.dex << " " << stats.def << "\n";

    // --- Inventory: all legendaries ---
    file << legendaryItems.size() << " " << 999999 << "\n";
    for (const auto& item : legendaryItems)
    {
        SerializeItem(file, item);
    }

    // --- Equipment: all "none" (items in inventory, not equipped) ---
    for (int i = 0; i < 10; ++i)
        file << "none\n";

    // --- Skills ---
    // CommonAttack only - the game will auto-add class skills on load
    file << "1\n";
    // CommonAttack|level|xp|cd|damage|skillPoints|upgradeCount|upgradeStates|masteryXP|masteryLevel|masteryPoints|masteryNodes
    file << "Common Attack|1|0|0|10|0|0|0|0|0";
    for (int b = 0; b < 3; ++b)
        for (int n = 0; n < 4; ++n)
            file << "|0";
    file << "\n";

    // --- Skill Loadout ---
    file << "1\n";
    file << "Common Attack\n";

    // --- Attack skill index ---
    file << "0\n";

    // --- Jobs (4 default jobs) ---
    file << "4\n";
    // Job 0: level 50 (matching the character class)
    file << "0 50 0 0 0 0 0\n";
    file << "0 0 0 0 0\n"; // 5 perks
    // Jobs 1-3: level 1
    file << "1 1 0 0 0 0 0\n";
    file << "0 0 0 0 0\n";
    file << "2 1 0 0 0 0 0\n";
    file << "0 0 0 0 0\n";
    file << "3 1 0 0 0 0 0\n";
    file << "0 0 0 0 0\n";

    // --- Job Skill Tree (v9+) ---
    file << "\n";

    // --- Area index ---
    file << "0\n";

    // --- Religion ---
    file << "0\n";  // GodType::None
    file << "0\n";  // devotion level
    file << "0\n";  // total donated
    file << "0 0\n"; // quest count, completed
    file << "||0||\n"; // quest details

    // --- Quests ---
    file << "0\n";

    // --- Achievements ---
    file << "\n";

    // --- Reputation ---
    file << "\n";

    // --- Pets ---
    file << "\n";

    // --- Wiki ---
    file << "\n";

    // --- Endgame unlock (v14+) ---
    file << "0\n";

    // --- Citadel boss kill counts (10 ints) ---
    for (int i = 0; i < 10; ++i)
        file << "0 ";
    file << "\n";

    // --- Legendary recipes unlocked bitmask ---
    file << "0\n";

    // --- Character mastery data (v15+) ---
    file << "0\n";  // charMasteryXP
    file << "0\n";  // charMasteryLevel
    file << "0\n";  // charMasteryPoints
    // charMasteryNodes: 3 branches * 4 nodes = 12 bools
    for (int i = 0; i < 12; ++i)
        file << "0 ";
    file << "\n";

    // --- Class evolution flag (v15+) ---
    file << "0\n";

    // --- Master class flag (v16+) ---
    file << "0\n";

    file.close();

    std::cout << "Save file written to " << filename << std::endl;
    std::cout << "Player: TestHero_" << className << ", Level " << PLAYER_LEVEL << " " << className << std::endl;
    std::cout << "Stats: HP=" << stats.hp << " MP=" << stats.mp << " STR=" << stats.str 
              << " VIT=" << stats.vit << " INT=" << stats.int_stat << " WIS=" << stats.wis 
              << " DEX=" << stats.dex << " DEF=" << stats.def << std::endl;
    std::cout << "Inventory: " << legendaryItems.size() << " legendary items at difficulty " << DIFF << std::endl;
    std::cout << "Gold: 999999" << std::endl;

    return 0;
}
