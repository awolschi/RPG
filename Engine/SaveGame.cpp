#include "SaveGame.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Items/Consumable.hpp"
#include "../Items/SummoningItem.hpp"
#include "../Items/Resources/Resources.hpp"

static int SafeStoi(const std::string& s, int fallback = 0)
{
    if (s.empty()) return fallback;
    try { return std::stoi(s); }
    catch (...) { return fallback; }
}

template<typename T>
static bool SafeRead(std::ifstream& file, T& value, T fallback = T{})
{
    if (!file.good()) { value = fallback; return false; }
    file >> value;
    if (!file.good()) { value = fallback; return false; }
    return true;
}

template<typename E>
static E SafeCastEnum(int v, int maxValid, E fallback)
{
    if (v < 0 || v > maxValid) return fallback;
    return static_cast<E>(v);
}

static void SerializeItem(std::ostream& file, const std::shared_ptr<Item>& item, bool includeConsumableResource)
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
    else if (includeConsumableResource)
    {
        if (auto c = item_cast<Consumable>(item))
            file << "|C|" << c->healAmount << "|" << c->manaAmount;
        else if (auto r = item_cast<Resource>(item))
            file << "|R|" << r->tier << "|" << static_cast<int>(r->quality) << "|" << r->healAmount << "|" << r->manaAmount;
        else if (auto s = item_cast<SummoningItem>(item))
            file << "|S|" << s->bossName;
    }
    file << "|" << item->requiredLevel << "\n";
}

static std::shared_ptr<Item> DeserializeItem(std::istringstream& ss, int saveVersion, int rarity, int sellVal, int setId,
                                              ItemPassive passive1, ItemPassive passive2, int itemCount)
{
    std::string iname;
    std::string typeStr;
    std::getline(ss, iname, '|');
    std::getline(ss, typeStr, '|');

    std::string subType;
    std::getline(ss, subType, '|');

    std::shared_ptr<Item> item;
    if (subType == "W")
    {
        std::string dmgStr, manaStr, elemStr, elemDmgStr;
        std::getline(ss, dmgStr, '|');
        std::getline(ss, manaStr, '|');
        ElementType elem = ElementType::Physical;
        int elemDmg = 0;
        if (std::getline(ss, elemStr, '|') && !elemStr.empty())
        {
            elem = SafeCastEnum<ElementType>(SafeStoi(elemStr), 6, ElementType::Physical);
            if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                elemDmg = SafeStoi(elemDmgStr);
        }
        WeaponType wType = WeaponType::Sword;
        if (saveVersion >= 15)
        {
            std::string wTypeStr;
            if (std::getline(ss, wTypeStr, '|') && !wTypeStr.empty())
                wType = SafeCastEnum<WeaponType>(SafeStoi(wTypeStr), 8, WeaponType::Sword);
        }
        item = std::make_shared<Weapon>(iname, SafeStoi(dmgStr), SafeStoi(manaStr), rarity, elem, elemDmg, wType);
    }
    else if (subType == "A")
    {
        std::string atStr, apStr, defStr;
        std::getline(ss, atStr, '|');
        std::getline(ss, apStr, '|');
        std::getline(ss, defStr, '|');
        std::map<ElementType, int> elemResist;
        std::string resistCountStr;
        if (std::getline(ss, resistCountStr, '|') && !resistCountStr.empty())
        {
            int resistCount = SafeStoi(resistCountStr);
            for (int r = 0; r < resistCount; ++r)
            {
                std::string elemStr, valStr;
                std::getline(ss, elemStr, '|');
                std::getline(ss, valStr, '|');
                elemResist[SafeCastEnum<ElementType>(SafeStoi(elemStr), 6, ElementType::Physical)] = SafeStoi(valStr);
            }
        }
        item = std::make_shared<Armor>(iname, SafeCastEnum<ArmorType>(SafeStoi(atStr), 2, ArmorType::Cloth),
                                       SafeCastEnum<ArmorPiece>(SafeStoi(apStr), 4, ArmorPiece::Helmet), SafeStoi(defStr), rarity, elemResist);
    }
    else if (subType == "AC")
    {
        std::string bhStr, bmStr, elemStr, elemDmgStr;
        std::getline(ss, bhStr, '|');
        std::getline(ss, bmStr, '|');
        ElementType elem = ElementType::Physical;
        int elemDmg = 0;
        if (std::getline(ss, elemStr, '|') && !elemStr.empty())
        {
            elem = SafeCastEnum<ElementType>(SafeStoi(elemStr), 6, ElementType::Physical);
            if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                elemDmg = SafeStoi(elemDmgStr);
        }
        item = std::make_shared<Accessory>(iname, SafeStoi(bhStr), SafeStoi(bmStr), rarity, elem, elemDmg);
    }
    else if (subType == "C")
    {
        std::string healStr, manaStr;
        std::getline(ss, healStr, '|');
        std::getline(ss, manaStr, '|');
        item = std::make_shared<Consumable>(iname, SafeStoi(healStr), SafeStoi(manaStr), rarity);
    }
    else if (subType == "OH")
    {
        std::string defStr, manaStr, arcStr, ohTypeStr, dmgBonusStr;
        std::getline(ss, defStr, '|');
        std::getline(ss, manaStr, '|');
        std::getline(ss, arcStr, '|');
        std::getline(ss, ohTypeStr, '|');
        int dmgBonus = 0;
        if (std::getline(ss, dmgBonusStr, '|') && !dmgBonusStr.empty())
            dmgBonus = SafeStoi(dmgBonusStr);
        item = std::make_shared<Offhand>(iname, SafeCastEnum<OffhandType>(SafeStoi(ohTypeStr), 4, OffhandType::Shield),
            SafeStoi(defStr), SafeStoi(manaStr), SafeStoi(arcStr), rarity,
            ItemPassive::None, ItemPassive::None, dmgBonus);
    }
    else if (subType == "R")
    {
        std::string tierStr, qualStr, healStr, manaStr;
        std::getline(ss, tierStr, '|');
        std::getline(ss, qualStr, '|');
        std::getline(ss, healStr, '|');
        std::getline(ss, manaStr, '|');
        int rTier = tierStr.empty() ? 1 : SafeStoi(tierStr);
        auto rQual = qualStr.empty() ? ResourceQuality::Normal
                                     : SafeCastEnum<ResourceQuality>(SafeStoi(qualStr), 2, ResourceQuality::Normal);
        int rHeal = healStr.empty() ? 0 : SafeStoi(healStr);
        int rMana = manaStr.empty() ? 0 : SafeStoi(manaStr);
        auto r = std::make_shared<Resource>(iname, rTier, sellVal, rHeal, rMana, ResourceQuality::Normal);
        r->quality = rQual;
        r->healAmount = rHeal;
        r->manaAmount = rMana;
        r->sellValue = sellVal;
        item = r;
    }
    else if (subType == "S")
    {
        std::string bossName;
        std::getline(ss, bossName, '|');
        item = SummoningRegistry::Create(iname);
    }
    else if (static_cast<ItemType>(SafeStoi(typeStr)) == ItemType::Resource)
    {
        auto r = std::make_shared<Resource>(iname, SafeStoi(typeStr), sellVal);
        r->sellValue = sellVal;
        item = r;
    }

    if (item)
    {
        item->sellValue = sellVal;
        item->count = itemCount;
        item->setId = setId;
        item->passive1 = passive1;
        item->passive2 = passive2;
        if (saveVersion >= 12)
        {
            std::string reqLvlStr;
            if (std::getline(ss, reqLvlStr, '|') && !reqLvlStr.empty())
                item->requiredLevel = SafeStoi(reqLvlStr, 1);
        }
    }
    return item;
}

SaveGameManager::SaveGameManager()
{
    saveDirectory = "./saves/";
    if (!std::filesystem::exists(saveDirectory))
    {
        try { std::filesystem::create_directory(saveDirectory); }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create save directory: " << e.what() << std::endl;
        }
    }
}

std::string SaveGameManager::SlotPath(int slot) const
{
    return saveDirectory + "slot" + std::to_string(slot) + ".sav";
}

bool SaveGameManager::SaveGame(const std::shared_ptr<Player>& player, int slot, int areaIndex, const ReligionSystem& religion, const AchievementSystem& achievements, const ReputationSystem& reputation, const PetManager& pets, const Wiki& wiki, bool chronosDefeated, const int* citadelBossKillCounts, uint32_t legendaryRecipesUnlocked)
{
    if (!player) return false;
    if (slot < 1 || slot > SLOT_COUNT) return false;

    std::string filepath = SlotPath(slot);
    try
    {
        std::ofstream file(filepath);
        if (!file.is_open()) return false;

        // Version header
        file << "SAVE_VERSION " << SAVE_VERSION << "\n";

        file << player->GetName() << "\n";
        file << static_cast<int>(player->GetCharacterClass()) << "\n";
        file << static_cast<int>(player->GetRace()) << "\n";
        file << player->GetLevel() << " " << player->GetExperience() << "\n";
        file << player->GetCurrentHealth() << " " << player->GetCurrentMana() << "\n";
        const Stats& s = player->GetStats();
        file << s.health << " " << s.mana << " " << s.strength << " " << s.vitality << " "
             << s.intelligence << " " << s.wisdom << " " << s.dexterity << " " << s.defense << "\n";

        Inventory& inv = player->GetInventory();
        file << inv.GetItemCount() << " " << inv.GetGold() << "\n";
        for (size_t i = 0; i < inv.GetItemCount(); ++i)
        {
            auto item = inv.GetItem(i);
            if (!item) continue;
            SerializeItem(file, item, true);
        }

        const Equipment& eq = player->GetEquipment();
        auto writeEquipSlot = [&](const std::shared_ptr<Item>& item) {
            if (!item) { file << "none\n"; return; }
            SerializeItem(file, item, false);
        };
        writeEquipSlot(eq.weapon); writeEquipSlot(eq.offhand);
        writeEquipSlot(eq.helmet); writeEquipSlot(eq.chest);
        writeEquipSlot(eq.gloves); writeEquipSlot(eq.pants);
        writeEquipSlot(eq.boots);  writeEquipSlot(eq.ring1);
        writeEquipSlot(eq.ring2);  writeEquipSlot(eq.amulet);

        file << player->GetSkills().GetSkillCount() << "\n";
        for (size_t i = 0; i < player->GetSkills().GetSkillCount(); ++i)
        {
            auto sk = player->GetSkills().GetSkill(i);
            if (sk)
            {
                file << sk->name << "|" << sk->level << "|" << sk->experience << "|"
                     << sk->currentCooldown << "|" << sk->baseDamage << "|"
                     << sk->skillPoints << "|" << sk->upgrades.size();
                for (const auto& up : sk->upgrades)
                    file << "|" << (up.unlocked ? "1" : "0");
                file << "|" << sk->masteryXP << "|" << sk->masteryLevel << "|" << sk->masteryPoints;
                for (int b = 0; b < Skill::MASTERY_TREE_BRANCHES; ++b)
                    for (int n = 0; n < Skill::MASTERY_NODES_PER_BRANCH; ++n)
                        file << "|" << (sk->masteryNodes[b][n] ? "1" : "0");
                file << "\n";
            }
        }

        // Skill loadout (save as skill names for robustness)
        const auto& loadout = player->GetSkillLoadout();
        file << loadout.size() << "\n";
        for (int idx : loadout)
        {
            auto sk = player->GetSkills().GetSkill(idx);
            if (sk) file << sk->name << "\n";
        }

        // Attack skill index (v11+)
        file << player->GetAttackSkillIndex() << "\n";

        const auto& jobs = player->GetJobSystem().GetJobs();
        file << jobs.size() << "\n";
        for (const auto& j : jobs)
        {
            file << static_cast<int>(j.type) << " " << j.level << " "
                 << j.experience << " " << j.jobPoints << " " << j.skillPoints << " "
                 << static_cast<int>(j.specialization) << " " << j.fatigue << "\n";
            for (size_t p = 0; p < j.perks.size(); ++p)
                file << (j.perks[p].unlocked ? "1" : "0") << (p + 1 < j.perks.size() ? " " : "\n");
        }

        // Job skill tree (v9+)
        file << player->GetJobSystem().SerializeSkillTree() << "\n";

        // Area index
        file << areaIndex << "\n";

        // Religion
        file << static_cast<int>(religion.GetGod()) << "\n";
        file << religion.GetDevotionLevel() << "\n";
        file << religion.GetTotalDonated() << "\n";
        // Extended religion data
        const auto& quest = religion.GetActiveQuest();
        file << quest.currentCount << " " << (quest.completed ? 1 : 0) << "\n";
        // v12: full quest details
        file << quest.targetEnemy << "|" << quest.targetCount << "|" << quest.rewardDevotion << "|" << quest.description << "\n";

        // Quests
        auto& qm = player->GetQuestManager();
        file << qm.GetQuestCount() << "\n";
        for (size_t i = 0; i < qm.GetQuestCount(); ++i)
        {
            Quest* q = qm.GetQuest(i);
            if (!q) { file << "|\n"; continue; }
            file << q->title << "|" << static_cast<int>(q->type) << "|"
                 << q->description << "|" << q->targetName << "|"
                 << q->targetCount << "|" << q->currentCount << "|"
                 << q->rewardXP << "|" << q->rewardGold << "|"
                 << static_cast<int>(q->status) << "|"
                 << (q->rewarded ? "1" : "0") << "|"
                 << q->gatherItemName << "\n";
        }

        // Achievements
        file << achievements.Serialize() << "\n";

        // Faction Reputation (v4+)
        file << reputation.Serialize() << "\n";

        // Pet system (v5+)
        file << pets.Serialize() << "\n";

        // Wiki discovery state (v10+)
        file << wiki.Serialize() << "\n";

        // Endgame unlock (v14+)
        file << (chronosDefeated ? "1" : "0") << "\n";

        // Citadel boss kill counts (v14+)
        for (int i = 0; i < 10; ++i)
            file << citadelBossKillCounts[i] << " ";
        file << "\n";

        // Legendary recipes unlocked bitmask (v14+)
        file << legendaryRecipesUnlocked << "\n";

        // Character mastery data (v15+)
        file << player->charMasteryXP << "\n";
        file << player->charMasteryLevel << "\n";
        file << player->charMasteryPoints << "\n";
        for (int b = 0; b < Player::CHAR_MASTERY_BRANCHES; ++b)
            for (int n = 0; n < Player::CHAR_MASTERY_NODES_PER_BRANCH; ++n)
                file << (player->charMasteryNodes[b][n] ? "1" : "0") << " ";
        file << "\n";

        // Class evolution flag (v15+)
        file << (player->HasClassEvolved() ? "1" : "0") << "\n";

        // Master class flag (v16+)
        file << (player->HasMastered() ? "1" : "0") << "\n";

        file.close();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Player> SaveGameManager::LoadGame(int slot, int& outAreaIndex, ReligionSystem& outReligion, AchievementSystem& outAchievements, ReputationSystem& outReputation, PetManager& outPets, Wiki& outWiki, bool& outChronosDefeated, int* outCitadelBossKillCounts, uint32_t& outLegendaryRecipesUnlocked)
{
    if (slot < 1 || slot > SLOT_COUNT) return nullptr;

    std::string filepath = SlotPath(slot);
    if (!std::filesystem::exists(filepath)) return nullptr;

    try
    {
        std::ifstream file(filepath);
        if (!file.is_open()) return nullptr;

        // Check for version header (v2+ saves)
        int saveVersion = 1;  // default to v1 (old format)
        std::string firstLine;
        std::streampos firstLinePos = file.tellg();
        std::getline(file, firstLine);
        // "SAVE_VERSION " is 13 characters; the previous substr(0,12) check
        // silently failed for every versioned save, corrupting the load.
        if (firstLine.size() >= 13 && firstLine.compare(0, 13, "SAVE_VERSION ") == 0)
        {
            saveVersion = SafeStoi(firstLine.substr(13));
        }
        else
        {
            // Old format: first line is player name, rewind
            file.clear();
            file.seekg(firstLinePos);
        }

        std::string playerName;
        std::getline(file, playerName);

        int classInt = 0;
        SafeRead(file, classInt);
        file.ignore();
        CharacterClass cc = SafeCastEnum<CharacterClass>(classInt, 4, CharacterClass::Warrior);

        int raceInt = 0;
        SafeRead(file, raceInt);
        file.ignore();
        CharacterRace cr = SafeCastEnum<CharacterRace>(raceInt, 4, CharacterRace::Aran);

        auto player = std::make_shared<Player>(playerName, cc, cr);

        int level = 1, xp = 0, hp = 100, mana = 50;
        SafeRead(file, level);
        SafeRead(file, xp);
        SafeRead(file, hp);
        SafeRead(file, mana);
        file.ignore();

        Stats st = {};
        SafeRead(file, st.health);
        SafeRead(file, st.mana);
        SafeRead(file, st.strength);
        SafeRead(file, st.vitality);
        SafeRead(file, st.intelligence);
        SafeRead(file, st.wisdom);
        SafeRead(file, st.dexterity);
        SafeRead(file, st.defense);
        file.ignore();

        player->SetStats(st);
        player->SetCurrentHealth(hp);
        player->SetCurrentMana(mana);
        player->SetLevel(level);
        player->SetExperience(xp);

        player->GetSkills().ClearAll();
        player->GetSkills().AddSkill(std::make_shared<CommonAttack>());
        player->CheckNewSkills();

        size_t invCount = 0;
        int gold = 0;
        SafeRead(file, invCount);
        SafeRead(file, gold);
        file.ignore();
        player->GetInventory().AddGold(gold);

        for (size_t i = 0; i < invCount; ++i)
        {
            std::string line;
            std::getline(file, line);
            if (line.empty()) continue;
            std::istringstream ss(line);
            std::string countStr, typeStr, iname, rarityStr, sellStr;
            std::getline(ss, countStr, '|');
            std::getline(ss, typeStr, '|');
            std::getline(ss, iname, '|');
            std::getline(ss, rarityStr, '|');
            std::getline(ss, sellStr, '|');
            int itemCount = SafeStoi(countStr);
            int rarity = SafeStoi(rarityStr);
            int sellVal = SafeStoi(sellStr);

            int setId = -1;
            ItemPassive passive1 = ItemPassive::None;
            ItemPassive passive2 = ItemPassive::None;
            if (saveVersion >= 9)
            {
                std::string sidStr, p1Str, p2Str;
                std::getline(ss, sidStr, '|');
                std::getline(ss, p1Str, '|');
                std::getline(ss, p2Str, '|');
                if (!sidStr.empty()) setId = SafeStoi(sidStr);
                if (!p1Str.empty()) passive1 = SafeCastEnum<ItemPassive>(SafeStoi(p1Str), 51, ItemPassive::None);
                if (!p2Str.empty()) passive2 = SafeCastEnum<ItemPassive>(SafeStoi(p2Str), 51, ItemPassive::None);
            }

            // Reconstruct the stream for DeserializeItem
            std::string remaining;
            std::getline(ss, remaining, '\0');
            std::istringstream itemStream(iname + "|" + typeStr + "|" + remaining);

            auto item = DeserializeItem(itemStream, saveVersion, rarity, sellVal, setId, passive1, passive2, itemCount);
            if (item)
            {
                player->GetInventory().AddItem(item);
            }
        }

        for (int i = 0; i < 10; ++i)
        {
            std::string line;
            std::getline(file, line);
            if (line.empty() || line == "none" || line.find('|') == std::string::npos) continue;

            std::istringstream ss(line);
            std::string countStr, typeStr, iname, rarityStr, sellStr;
            std::getline(ss, countStr, '|');
            std::getline(ss, typeStr, '|');
            std::getline(ss, iname, '|');
            std::getline(ss, rarityStr, '|');
            std::getline(ss, sellStr, '|');
            int rarity = SafeStoi(rarityStr);
            int sellVal = SafeStoi(sellStr);

            int setId = -1;
            ItemPassive passive1 = ItemPassive::None;
            ItemPassive passive2 = ItemPassive::None;
            if (saveVersion >= 9)
            {
                std::string sidStr, p1Str, p2Str;
                std::getline(ss, sidStr, '|');
                std::getline(ss, p1Str, '|');
                std::getline(ss, p2Str, '|');
                if (!sidStr.empty()) setId = SafeStoi(sidStr);
                if (!p1Str.empty()) passive1 = SafeCastEnum<ItemPassive>(SafeStoi(p1Str), 51, ItemPassive::None);
                if (!p2Str.empty()) passive2 = SafeCastEnum<ItemPassive>(SafeStoi(p2Str), 51, ItemPassive::None);
            }

            // Reconstruct the stream for DeserializeItem
            std::string remaining;
            std::getline(ss, remaining, '\0');
            std::istringstream itemStream(iname + "|" + typeStr + "|" + remaining);

            auto item = DeserializeItem(itemStream, saveVersion, rarity, sellVal, setId, passive1, passive2, 1);
            if (item)
            {
                // Assign to equipment slot based on index with proper casting
                if (i == 0) player->GetEquipment().weapon = item_cast<Weapon>(item);
                else if (i == 1) player->GetEquipment().offhand = item;
                else if (i == 2) player->GetEquipment().helmet = item_cast<Armor>(item);
                else if (i == 3) player->GetEquipment().chest = item_cast<Armor>(item);
                else if (i == 4) player->GetEquipment().gloves = item_cast<Armor>(item);
                else if (i == 5) player->GetEquipment().pants = item_cast<Armor>(item);
                else if (i == 6) player->GetEquipment().boots = item_cast<Armor>(item);
                else if (i == 7) player->GetEquipment().ring1 = item_cast<Accessory>(item);
                else if (i == 8) player->GetEquipment().ring2 = item_cast<Accessory>(item);
                else if (i == 9) player->GetEquipment().amulet = item_cast<Accessory>(item);
            }
        }

        size_t skillCount = 0;
        SafeRead(file, skillCount);
        file.ignore();

        struct SavedSkillData { std::string name; int level, xp, cd, dmg, pts; std::vector<bool> unlocked;
            int masteryXP = 0, masteryLevel = 0, masteryPoints = 0;
            bool masteryNodes[Skill::MASTERY_TREE_BRANCHES][Skill::MASTERY_NODES_PER_BRANCH] = {};
        };
        std::vector<SavedSkillData> savedSkills;
        for (size_t i = 0; i < skillCount; ++i)
        {
            std::string line;
            std::getline(file, line);
            if (line.empty()) continue;
            std::istringstream ss(line);
            SavedSkillData sd;
            std::string skname, sklvl, skxp, skcd, skdmg;
            std::getline(ss, skname, '|');
            std::getline(ss, sklvl, '|');
            std::getline(ss, skxp, '|');
            std::getline(ss, skcd, '|');
            std::getline(ss, skdmg, '|');
            sd.name = skname;
            sd.level = SafeStoi(sklvl);
            sd.xp = SafeStoi(skxp);
            sd.cd = SafeStoi(skcd);
            sd.dmg = SafeStoi(skdmg);

            std::string ptsStr;
            if (std::getline(ss, ptsStr, '|') && !ptsStr.empty())
            {
                sd.pts = SafeStoi(ptsStr);
                std::string upgCountStr;
                if (std::getline(ss, upgCountStr, '|') && !upgCountStr.empty())
                {
                    int upgCount = SafeStoi(upgCountStr);
                    for (int u = 0; u < upgCount; ++u)
                    {
                        std::string ustr;
                        std::getline(ss, ustr, '|');
                        sd.unlocked.push_back(ustr == "1");
                    }
                }
            }
            else
            {
                sd.pts = 0;
            }

            // Mastery data (v14+)
            if (saveVersion >= 14)
            {
                std::string mxpStr, mlvStr, mptStr;
                if (std::getline(ss, mxpStr, '|') && !mxpStr.empty())
                    sd.masteryXP = SafeStoi(mxpStr);
                if (std::getline(ss, mlvStr, '|') && !mlvStr.empty())
                    sd.masteryLevel = SafeStoi(mlvStr);
                if (std::getline(ss, mptStr, '|') && !mptStr.empty())
                    sd.masteryPoints = SafeStoi(mptStr);
                for (int b = 0; b < Skill::MASTERY_TREE_BRANCHES; ++b)
                    for (int n = 0; n < Skill::MASTERY_NODES_PER_BRANCH; ++n)
                    {
                        std::string nstr;
                        if (std::getline(ss, nstr, '|'))
                            sd.masteryNodes[b][n] = (nstr == "1");
                    }
            }

            savedSkills.push_back(sd);
        }

        for (const auto& sd : savedSkills)
        {
            auto sk = player->GetSkills().GetSkillByName(sd.name);
            if (sk)
            {
                sk->level = sd.level;
                sk->experience = sd.xp;
                sk->currentCooldown = sd.cd;
                sk->baseDamage = sd.dmg;
                sk->skillPoints = sd.pts;
                for (size_t u = 0; u < sd.unlocked.size() && u < sk->upgrades.size(); ++u)
                    sk->upgrades[u].unlocked = sd.unlocked[u];
                sk->masteryXP = sd.masteryXP;
                sk->masteryLevel = sd.masteryLevel;
                sk->masteryPoints = sd.masteryPoints;
                for (int b = 0; b < Skill::MASTERY_TREE_BRANCHES; ++b)
                    for (int n = 0; n < Skill::MASTERY_NODES_PER_BRANCH; ++n)
                        sk->masteryNodes[b][n] = sd.masteryNodes[b][n];
            }
        }

        // Skill loadout (v2+ saves only)
        if (saveVersion >= 2)
        {
            size_t loadoutCount = 0;
            SafeRead(file, loadoutCount);
            file.ignore();
            std::vector<int> loadoutIndices;
            for (size_t l = 0; l < loadoutCount; ++l)
            {
                std::string loadoutName;
                std::getline(file, loadoutName);
                if (loadoutName.empty()) continue;
                for (size_t s = 0; s < player->GetSkills().GetSkillCount(); ++s)
                {
                    auto sk = player->GetSkills().GetSkill(s);
                    if (sk && sk->name == loadoutName)
                    {
                        loadoutIndices.push_back(static_cast<int>(s));
                        break;
                    }
                }
            }
            player->SetSkillLoadout(loadoutIndices);
        }

        // Attack skill index (v11+)
        if (saveVersion >= 11 && file.peek() != std::char_traits<char>::eof())
        {
            int atkIdx = 0;
            SafeRead(file, atkIdx);
            file.ignore();
            player->SetAttackSkillIndex(atkIdx);
        }

        size_t jobCount = 0;
        SafeRead(file, jobCount);
        file.ignore();
        auto& jobs = player->GetJobSystem().GetJobs();
        for (size_t i = 0; i < jobCount && i < jobs.size(); ++i)
        {
            int jt = 0, jl = 1, je = 0, jp = 0, jsp = 0;
            SafeRead(file, jt);
            SafeRead(file, jl);
            SafeRead(file, je);
            SafeRead(file, jp);
            SafeRead(file, jsp);
            jobs[i].level = jl;
            jobs[i].experience = je;
            jobs[i].jobPoints = jp;
            if (saveVersion >= 9)
                jobs[i].skillPoints = jsp;

            // Read specialization (v3+ saves have this field)
            if (saveVersion >= 3)
            {
                int specInt = 0;
                SafeRead(file, specInt);
                jobs[i].specialization = SafeCastEnum<SpecializationType>(specInt, 8, SpecializationType::None);
            }

            // Read fatigue (v13+ saves have this field)
            if (saveVersion >= 13)
            {
                int fatigueVal = 0;
                SafeRead(file, fatigueVal);
                jobs[i].fatigue = std::max(0, std::min(fatigueVal, Job::MAX_FATIGUE));
            }

            file.ignore();

            for (size_t p = 0; p < jobs[i].perks.size(); ++p)
            {
                int unlocked = 0;
                SafeRead(file, unlocked);
                jobs[i].perks[p].unlocked = (unlocked == 1);
            }
            file.ignore();
        }

        // Job skill tree (v9+)
        if (saveVersion >= 9 && file.peek() != std::char_traits<char>::eof())
        {
            std::string stData;
            std::getline(file, stData);
            if (!stData.empty())
                player->GetJobSystem().DeserializeSkillTree(stData);
        }

        // Extended save data (area, religion, quests) — only if present
        if (file.peek() != std::char_traits<char>::eof())
        {
            SafeRead(file, outAreaIndex);
            file.ignore();

            player->GetQuestManager().ClearQuests();

            int godInt = 0;
            SafeRead(file, godInt);
            file.ignore();
            int devotion = 0, donated = 0;
            SafeRead(file, devotion);
            SafeRead(file, donated);
            file.ignore();
            outReligion.RestoreState(SafeCastEnum<GodType>(godInt, 4, GodType::None), devotion, donated);
            // Read extended religion quest data
            if (file.peek() != std::char_traits<char>::eof())
            {
                int qc = 0, ac = 0;
                SafeRead(file, qc);
                SafeRead(file, ac);
                file.ignore();
                if (outReligion.GetGod() != GodType::None)
                {
                    outReligion.EnsureQuest();
                    outReligion.RestoreQuest("", "", 0, qc, 0, ac == 1);
                }
            }
            // v12: full religion quest details
            if (saveVersion >= 12 && file.peek() != std::char_traits<char>::eof())
            {
                std::string rqLine;
                std::getline(file, rqLine);
                if (!rqLine.empty() && outReligion.GetGod() != GodType::None)
                {
                    std::istringstream rqss(rqLine);
                    std::string rqTarget, rqDesc;
                    std::string rqTC, rqRD;
                    std::getline(rqss, rqTarget, '|');
                    std::getline(rqss, rqTC, '|');
                    std::getline(rqss, rqRD, '|');
                    std::getline(rqss, rqDesc, '|');
                    outReligion.EnsureQuest();
                    outReligion.RestoreQuest(rqDesc, rqTarget, SafeStoi(rqTC),
                        outReligion.GetActiveQuest().currentCount, SafeStoi(rqRD),
                        outReligion.GetActiveQuest().completed);
                }
            }

            size_t questCount = 0;
            SafeRead(file, questCount);
            file.ignore();
            for (size_t i = 0; i < questCount; ++i)
            {
                std::string line;
                std::getline(file, line);
                if (line.empty() || line == "|") continue;
                std::istringstream ss(line);
                std::string title, typeStr, desc, target, tgtCount, curCount, rXP, rGold, statusStr, rewardedStr, gatherItem;
                std::getline(ss, title, '|');
                std::getline(ss, typeStr, '|');
                std::getline(ss, desc, '|');
                std::getline(ss, target, '|');
                std::getline(ss, tgtCount, '|');
                std::getline(ss, curCount, '|');
                std::getline(ss, rXP, '|');
                std::getline(ss, rGold, '|');
                std::getline(ss, statusStr, '|');
                std::getline(ss, rewardedStr, '|');
                if (saveVersion >= 12)
                    std::getline(ss, gatherItem, '|');
                if (title.empty()) continue;
                Quest q(title, SafeCastEnum<QuestType>(SafeStoi(typeStr), 3, QuestType::Kill), desc, target,
                         SafeStoi(tgtCount), SafeStoi(rXP), SafeStoi(rGold), gatherItem);
                q.currentCount = SafeStoi(curCount);
                q.status = SafeCastEnum<QuestStatus>(SafeStoi(statusStr), 3, QuestStatus::NotStarted);
                q.rewarded = (rewardedStr == "1");
                player->GetQuestManager().AddQuest(q);
            }

            // Achievements (v3+)
            if (saveVersion >= 3)
            {
                std::string achData;
                std::getline(file, achData);
                outAchievements.Deserialize(achData);
            }

            // Faction Reputation (v4+)
            if (saveVersion >= 4)
            {
                std::string repData;
                std::getline(file, repData);
                outReputation.Deserialize(repData);
            }

            // Pet system (v5+)
            if (saveVersion >= 5)
            {
                std::string petData;
                std::getline(file, petData);
                outPets.Deserialize(petData);
            }

            // Wiki discovery state (v10+)
            if (saveVersion >= 10)
            {
                std::string wikiData;
                std::getline(file, wikiData);
                outWiki.Deserialize(wikiData);
            }

            // Endgame unlock (v14+)
            if (saveVersion >= 14)
            {
                std::string chronoData;
                std::getline(file, chronoData);
                outChronosDefeated = (chronoData == "1");

                // Citadel boss kill counts
                for (int i = 0; i < 10; ++i)
                    SafeRead(file, outCitadelBossKillCounts[i]);
                file.ignore();

                // Legendary recipes unlocked bitmask
                SafeRead(file, outLegendaryRecipesUnlocked);
                file.ignore();

                // Character mastery data (v15+)
                if (saveVersion >= 15 && file.peek() != std::char_traits<char>::eof())
                {
                    int cmXP = 0, cmLevel = 0, cmPoints = 0;
                    SafeRead(file, cmXP);
                    SafeRead(file, cmLevel);
                    SafeRead(file, cmPoints);
                    file.ignore();
                    player->charMasteryXP = cmXP;
                    player->charMasteryLevel = cmLevel;
                    player->charMasteryPoints = cmPoints;
                    for (int b = 0; b < Player::CHAR_MASTERY_BRANCHES; ++b)
                        for (int n = 0; n < Player::CHAR_MASTERY_NODES_PER_BRANCH; ++n)
                        {
                            int val = 0;
                            SafeRead(file, val);
                            player->charMasteryNodes[b][n] = (val == 1);
                        }
                    file.ignore();
                    player->RecalcMasteryBonuses();

                    // Class evolution flag (v15+)
                    int evolvedFlag = 0;
                    SafeRead(file, evolvedFlag);
                    file.ignore();
                    if (evolvedFlag == 1 && !player->HasClassEvolved())
                        player->EvolveClass();

                    // Master class flag (v16+)
                    if (file.peek() != '\n' && !file.eof())
                    {
                        int masteredFlag = 0;
                        SafeRead(file, masteredFlag);
                        file.ignore();
                        if (masteredFlag == 1 && !player->HasMastered())
                            player->MasterClass();
                    }
                }
            }
            else
            {
                outChronosDefeated = false;
                for (int i = 0; i < 10; ++i)
                    outCitadelBossKillCounts[i] = 0;
                outLegendaryRecipesUnlocked = 0;
            }
        }

        file.close();
        return player;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return nullptr;
    }
}

bool SaveGameManager::IsSlotOccupied(int slot)
{
    return std::filesystem::exists(SlotPath(slot));
}

SaveSlotInfo SaveGameManager::GetSlotInfo(int slot)
{
    SaveSlotInfo info;
    info.slot = slot;
    info.occupied = false;

    std::string filepath = SlotPath(slot);
    if (!std::filesystem::exists(filepath)) return info;

    try
    {
        std::ifstream file(filepath);
        if (!file.is_open()) return info;

        // Skip version header if present
        std::string firstLine;
        std::getline(file, firstLine);
        if (firstLine.size() >= 13 && firstLine.substr(0, 13) == "SAVE_VERSION ")
        {
            // New format: read player name from next line
            std::getline(file, info.playerName);
        }
        else
        {
            // Old format: first line is player name
            info.playerName = firstLine;
        }

        int classInt = 0;
        SafeRead(file, classInt);
        file.ignore();
        info.characterClass = SafeCastEnum<CharacterClass>(classInt, 4, CharacterClass::Warrior);

        int raceDummy = 0;
        SafeRead(file, raceDummy);
        file.ignore();

        SafeRead(file, info.level);

        info.occupied = true;
        file.close();
    }
    catch (...)
    {
        info.occupied = false;
    }
    return info;
}

std::vector<SaveSlotInfo> SaveGameManager::ListSlots()
{
    std::vector<SaveSlotInfo> slots;
    for (int i = 1; i <= SLOT_COUNT; ++i)
        slots.push_back(GetSlotInfo(i));
    return slots;
}

std::string SaveGameManager::GetSaveDirectory() const
{
    return saveDirectory;
}
