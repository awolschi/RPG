#include "SaveGame.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Items/Consumable.hpp"

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

bool SaveGameManager::SaveGame(const std::shared_ptr<Player>& player, int slot, int areaIndex, const ReligionSystem& religion, const AchievementSystem& achievements)
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
            file << item->count << "|" << static_cast<int>(item->type) << "|" << item->name << "|" << item->rarity << "|" << item->sellValue;

            if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
                file << "|OH|" << oh->defense << "|" << oh->manaBonus << "|" << oh->arcaneDamage << "|" << static_cast<int>(oh->offhandType);
            else if (auto w = std::dynamic_pointer_cast<Weapon>(item))
                file << "|W|" << w->damage << "|" << w->manaCost << "|" << static_cast<int>(w->element) << "|" << w->elementDamage;
            else if (auto a = std::dynamic_pointer_cast<Armor>(item))
            {
                file << "|A|" << static_cast<int>(a->armorType) << "|" << static_cast<int>(a->piece) << "|" << a->defense;
                file << "|" << a->elementalResist.size();
                for (const auto& [elem, val] : a->elementalResist)
                    file << "|" << static_cast<int>(elem) << "|" << val;
            }
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
                file << "|AC|" << ac->bonusHealth << "|" << ac->bonusMana << "|" << static_cast<int>(ac->element) << "|" << ac->elementDamage;
            else if (auto c = std::dynamic_pointer_cast<Consumable>(item))
                file << "|C|" << c->healAmount << "|" << c->manaAmount;
            file << "\n";
        }

        const Equipment& eq = player->GetEquipment();
        auto writeEquipSlot = [&](const std::shared_ptr<Item>& item) {
            if (!item) { file << "none\n"; return; }
            file << item->count << "|" << static_cast<int>(item->type) << "|" << item->name
                 << "|" << item->rarity << "|" << item->sellValue;
            if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
                file << "|OH|" << oh->defense << "|" << oh->manaBonus << "|" << oh->arcaneDamage << "|" << static_cast<int>(oh->offhandType);
            else if (auto w = std::dynamic_pointer_cast<Weapon>(item))
                file << "|W|" << w->damage << "|" << w->manaCost << "|" << static_cast<int>(w->element) << "|" << w->elementDamage;
            else if (auto a = std::dynamic_pointer_cast<Armor>(item))
            {
                file << "|A|" << static_cast<int>(a->armorType) << "|" << static_cast<int>(a->piece) << "|" << a->defense;
                file << "|" << a->elementalResist.size();
                for (const auto& [elem, val] : a->elementalResist)
                    file << "|" << static_cast<int>(elem) << "|" << val;
            }
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
                file << "|AC|" << ac->bonusHealth << "|" << ac->bonusMana << "|" << static_cast<int>(ac->element) << "|" << ac->elementDamage;
            file << "\n";
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

        const auto& jobs = player->GetJobSystem().GetJobs();
        file << jobs.size() << "\n";
        for (const auto& j : jobs)
        {
            file << static_cast<int>(j.type) << " " << j.level << " "
                 << j.experience << " " << j.jobPoints << " "
                 << static_cast<int>(j.specialization) << "\n";
            for (size_t p = 0; p < j.perks.size(); ++p)
                file << (j.perks[p].unlocked ? "1" : "0") << (p + 1 < j.perks.size() ? " " : "\n");
        }

        // Area index
        file << areaIndex << "\n";

        // Religion
        file << static_cast<int>(religion.GetGod()) << "\n";
        file << religion.GetDevotionLevel() << "\n";
        file << religion.GetTotalDonated() << "\n";
        // Extended religion data
        const auto& quest = religion.GetActiveQuest();
        file << quest.currentCount << " " << (quest.completed ? 1 : 0) << "\n";

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
                 << (q->rewarded ? "1" : "0") << "\n";
        }

        // Achievements
        file << achievements.Serialize() << "\n";

        file.close();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Player> SaveGameManager::LoadGame(int slot, int& outAreaIndex, ReligionSystem& outReligion, AchievementSystem& outAchievements)
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
        if (firstLine.substr(0, 12) == "SAVE_VERSION ")
        {
            saveVersion = std::stoi(firstLine.substr(12));
        }
        else
        {
            // Old format: first line is player name, rewind
            file.clear();
            file.seekg(firstLinePos);
        }

        std::string playerName;
        std::getline(file, playerName);

        int classInt;
        file >> classInt; file.ignore();
        CharacterClass cc = static_cast<CharacterClass>(classInt);

        int raceInt;
        file >> raceInt; file.ignore();
        CharacterRace cr = static_cast<CharacterRace>(raceInt);

        auto player = std::make_shared<Player>(playerName, cc, cr);

        int level, xp, hp, mana;
        file >> level >> xp >> hp >> mana; file.ignore();

        Stats st;
        file >> st.health >> st.mana >> st.strength >> st.vitality
             >> st.intelligence >> st.wisdom >> st.dexterity >> st.defense; file.ignore();

        player->SetStats(st);
        player->SetCurrentHealth(hp);
        player->SetCurrentMana(mana);
        player->SetLevel(level);
        player->SetExperience(xp);

        player->GetSkills().ClearAll();
        player->GetSkills().AddSkill(std::make_shared<CommonAttack>());
        player->CheckNewSkills();

        size_t invCount;
        int gold;
        file >> invCount >> gold; file.ignore();
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
            int itemCount = std::stoi(countStr);
            int rarity = std::stoi(rarityStr);
            int sellVal = std::stoi(sellStr);

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
                    elem = static_cast<ElementType>(std::stoi(elemStr));
                    if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                        elemDmg = std::stoi(elemDmgStr);
                }
                item = std::make_shared<Weapon>(iname, std::stoi(dmgStr), std::stoi(manaStr), rarity, elem, elemDmg);
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
                    int resistCount = std::stoi(resistCountStr);
                    for (int r = 0; r < resistCount; ++r)
                    {
                        std::string elemStr, valStr;
                        std::getline(ss, elemStr, '|');
                        std::getline(ss, valStr, '|');
                        elemResist[static_cast<ElementType>(std::stoi(elemStr))] = std::stoi(valStr);
                    }
                }
                item = std::make_shared<Armor>(iname, static_cast<ArmorType>(std::stoi(atStr)),
                                               static_cast<ArmorPiece>(std::stoi(apStr)), std::stoi(defStr), rarity, elemResist);
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
                    elem = static_cast<ElementType>(std::stoi(elemStr));
                    if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                        elemDmg = std::stoi(elemDmgStr);
                }
                item = std::make_shared<Accessory>(iname, std::stoi(bhStr), std::stoi(bmStr), rarity, elem, elemDmg);
            }
            else if (subType == "C")
            {
                std::string healStr, manaStr;
                std::getline(ss, healStr, '|');
                std::getline(ss, manaStr, '|');
                item = std::make_shared<Consumable>(iname, std::stoi(healStr), std::stoi(manaStr), rarity);
            }
            else if (subType == "OH")
            {
                std::string defStr, manaStr, arcStr, typeStr;
                std::getline(ss, defStr, '|');
                std::getline(ss, manaStr, '|');
                std::getline(ss, arcStr, '|');
                std::getline(ss, typeStr, '|');
                auto oh = std::make_shared<Offhand>(iname, static_cast<OffhandType>(std::stoi(typeStr)),
                    std::stoi(defStr), std::stoi(manaStr), std::stoi(arcStr), rarity);
                oh->sellValue = sellVal;
                item = oh;
            }
            if (item)
            {
                item->sellValue = sellVal;
                item->count = itemCount;
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
            int rarity = std::stoi(rarityStr);
            int sellVal = std::stoi(sellStr);

            std::string subType;
            std::getline(ss, subType, '|');

            if (subType == "W")
            {
                std::string dmgStr, manaStr, elemStr, elemDmgStr;
                std::getline(ss, dmgStr, '|');
                std::getline(ss, manaStr, '|');
                ElementType elem = ElementType::Physical;
                int elemDmg = 0;
                if (std::getline(ss, elemStr, '|') && !elemStr.empty())
                {
                    elem = static_cast<ElementType>(std::stoi(elemStr));
                    if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                        elemDmg = std::stoi(elemDmgStr);
                }
                auto w = std::make_shared<Weapon>(iname, std::stoi(dmgStr), std::stoi(manaStr), rarity, elem, elemDmg);
                w->sellValue = sellVal;
                if (i == 0) player->GetEquipment().weapon = w;
                else if (i == 1) player->GetEquipment().offhand = w;
            }
            else if (subType == "OH")
            {
                std::string defStr, manaStr, arcStr, typeStr;
                std::getline(ss, defStr, '|');
                std::getline(ss, manaStr, '|');
                std::getline(ss, arcStr, '|');
                std::getline(ss, typeStr, '|');
                auto oh = std::make_shared<Offhand>(iname, static_cast<OffhandType>(std::stoi(typeStr)),
                    std::stoi(defStr), std::stoi(manaStr), std::stoi(arcStr), rarity);
                oh->sellValue = sellVal;
                if (i == 1) player->GetEquipment().offhand = oh;
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
                    int resistCount = std::stoi(resistCountStr);
                    for (int r = 0; r < resistCount; ++r)
                    {
                        std::string elemStr, valStr;
                        std::getline(ss, elemStr, '|');
                        std::getline(ss, valStr, '|');
                        elemResist[static_cast<ElementType>(std::stoi(elemStr))] = std::stoi(valStr);
                    }
                }
                auto a = std::make_shared<Armor>(iname, static_cast<ArmorType>(std::stoi(atStr)),
                                                 static_cast<ArmorPiece>(std::stoi(apStr)), std::stoi(defStr), rarity, elemResist);
                a->sellValue = sellVal;
                if (i == 2) player->GetEquipment().helmet = a;
                else if (i == 3) player->GetEquipment().chest = a;
                else if (i == 4) player->GetEquipment().gloves = a;
                else if (i == 5) player->GetEquipment().pants = a;
                else if (i == 6) player->GetEquipment().boots = a;
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
                    elem = static_cast<ElementType>(std::stoi(elemStr));
                    if (std::getline(ss, elemDmgStr, '|') && !elemDmgStr.empty())
                        elemDmg = std::stoi(elemDmgStr);
                }
                auto ac = std::make_shared<Accessory>(iname, std::stoi(bhStr), std::stoi(bmStr), rarity, elem, elemDmg);
                ac->sellValue = sellVal;
                if (i == 7) player->GetEquipment().ring1 = ac;
                else if (i == 8) player->GetEquipment().ring2 = ac;
                else if (i == 9) player->GetEquipment().amulet = ac;
            }
        }

        size_t skillCount;
        file >> skillCount; file.ignore();

        struct SavedSkillData { std::string name; int level, xp, cd, dmg, pts; std::vector<bool> unlocked; };
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
            sd.level = std::stoi(sklvl);
            sd.xp = std::stoi(skxp);
            sd.cd = std::stoi(skcd);
            sd.dmg = std::stoi(skdmg);

            std::string ptsStr;
            if (std::getline(ss, ptsStr, '|') && !ptsStr.empty())
            {
                sd.pts = std::stoi(ptsStr);
                std::string upgCountStr;
                if (std::getline(ss, upgCountStr, '|') && !upgCountStr.empty())
                {
                    int upgCount = std::stoi(upgCountStr);
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
            }
        }

        // Skill loadout (v2+ saves only)
        if (saveVersion >= 2)
        {
            size_t loadoutCount = 0;
            file >> loadoutCount; file.ignore();
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

        size_t jobCount;
        file >> jobCount; file.ignore();
        auto& jobs = player->GetJobSystem().GetJobs();
        for (size_t i = 0; i < jobCount && i < jobs.size(); ++i)
        {
            int jt, jl, je, jp;
            file >> jt >> jl >> je >> jp;
            jobs[i].level = jl;
            jobs[i].experience = je;
            jobs[i].jobPoints = jp;

            // Read specialization (v3+ saves have this field)
            if (saveVersion >= 3)
            {
                int specInt;
                file >> specInt;
                jobs[i].specialization = static_cast<SpecializationType>(specInt);
            }
            file.ignore();

            for (size_t p = 0; p < jobs[i].perks.size(); ++p)
            {
                int unlocked;
                file >> unlocked;
                jobs[i].perks[p].unlocked = (unlocked == 1);
            }
            file.ignore();
        }

        // Extended save data (area, religion, quests) — only if present
        if (file.peek() != std::char_traits<char>::eof())
        {
            file >> outAreaIndex; file.ignore();

            player->GetQuestManager().ClearQuests();

            int godInt;
            file >> godInt; file.ignore();
            int devotion, donated;
            file >> devotion >> donated; file.ignore();
            outReligion.RestoreState(static_cast<GodType>(godInt), devotion, donated);
            // Read extended religion quest data
            if (file.peek() != std::char_traits<char>::eof())
            {
                int qc, ac;
                file >> qc >> ac; file.ignore();
                if (outReligion.GetGod() != GodType::None)
                {
                    // Restore quest progress (quest structure is regenerated on SetGod)
                    // We store the progress and apply it
                }
            }

            size_t questCount;
            file >> questCount; file.ignore();
            for (size_t i = 0; i < questCount; ++i)
            {
                std::string line;
                std::getline(file, line);
                if (line.empty() || line == "|") continue;
                std::istringstream ss(line);
                std::string title, typeStr, desc, target, tgtCount, curCount, rXP, rGold, statusStr, rewardedStr;
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
                if (title.empty()) continue;
                Quest q(title, static_cast<QuestType>(std::stoi(typeStr)), desc, target,
                         std::stoi(tgtCount), std::stoi(rXP), std::stoi(rGold));
                q.currentCount = std::stoi(curCount);
                q.status = static_cast<QuestStatus>(std::stoi(statusStr));
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
        if (firstLine.substr(0, 12) != "SAVE_VERSION ")
        {
            // Old format: first line is player name
            info.playerName = firstLine;
        }
        else
        {
            // New format: read player name from next line
            std::getline(file, info.playerName);
        }

        int classInt;
        file >> classInt; file.ignore();
        info.characterClass = static_cast<CharacterClass>(classInt);

        int raceDummy;
        file >> raceDummy; file.ignore();

        file >> info.level;

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
