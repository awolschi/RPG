#include "SetBonuses.hpp"
#include <sstream>

std::vector<SetInfo> SetBonuses::s_sets;
bool SetBonuses::s_initialized = false;

static void AddBonus(std::vector<SetBonus>& list, SetBonusType type, int value)
{
    list.push_back({type, value});
}

void SetBonuses::Initialize()
{
    if (s_initialized) return;
    s_sets.clear();

    // ================================================================
    // Set 1: Verdant (Treant King) — 5 pieces
    // ================================================================
    {
        SetInfo s;
        s.name = "Verdant";
        s.setId = 1;
        s.pieceNames = {
            "Briarwood Staff",
            "Crown of Thorns",
            "Barkheart Chestguard",
            "Mantle of the Wild",
            "Seedkeeper Boots"
        };
        AddBonus(s.twoPiece, SetBonusType::SpellDmgBoost, 15);
        AddBonus(s.fourPiece, SetBonusType::Thorns, 25);
        AddBonus(s.fourPiece, SetBonusType::HealOnKill, 10);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 2: Abyssal (Kraken) — 5 pieces
    // Bonuses from Section 7: Abyssal Marksman
    // ================================================================
    {
        SetInfo s;
        s.name = "Abyssal";
        s.setId = 2;
        s.pieceNames = {
            "Tidecaller's Staff",
            "Kraken's Grip",
            "Abyssal Helm",
            "Tide-Sworn Robes",
            "Coral Stalkers"
        };
        AddBonus(s.twoPiece, SetBonusType::CritBoost, 20);
        AddBonus(s.fourPiece, SetBonusType::DmgBoost, 15);
        AddBonus(s.fourPiece, SetBonusType::Dodge, 15);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 3: Dragonfire (Elder Dragon) — 6 pieces
    // Bonuses from Section 7: Dragonfire Plate
    // ================================================================
    {
        SetInfo s;
        s.name = "Dragonfire";
        s.setId = 3;
        s.pieceNames = {
            "Staff of the Eternal Flame",
            "Dragonbone Wand",
            "Fang of the Elder",
            "Dragonhide Helm",
            "Drakefire Cowl",
            "Scales of the Eternal"
        };
        AddBonus(s.twoPiece, SetBonusType::DmgBoost, 15);
        AddBonus(s.fourPiece, SetBonusType::Thorns, 25);
        AddBonus(s.fourPiece, SetBonusType::BurnImmune, 1);
        AddBonus(s.fourPiece, SetBonusType::CritBoost, 10);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 4: Warlord's (Aran Warlord) — 6 pieces
    // Bonuses from Section 7: Warlord's Battlegear
    // ================================================================
    {
        SetInfo s;
        s.name = "Warlord's";
        s.setId = 4;
        s.pieceNames = {
            "Blade of the Aran",
            "Warbringer's Mace",
            "Warlord's Helm",
            "Battle-Hardened Plate",
            "Greaves of Conquest",
            "Warboots of the Vanguard"
        };
        AddBonus(s.twoPiece, SetBonusType::DmgBoost, 15);
        AddBonus(s.twoPiece, SetBonusType::CritBoost, 10);
        AddBonus(s.fourPiece, SetBonusType::DefBoost, 10);
        AddBonus(s.fourPiece, SetBonusType::DmgBoost, 25);
        AddBonus(s.fourPiece, SetBonusType::HealOnKill, 10);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 5: Voidborn (Void Lord) — 8 pieces
    // Bonuses from Section 7: Voidborn Shroud
    // ================================================================
    {
        SetInfo s;
        s.name = "Voidborn";
        s.setId = 5;
        s.pieceNames = {
            "Wand of the Void",
            "Null-Staff",
            "Soulreaper's Scythe",
            "Void-Touched Hood",
            "Helm of the Null Knight",
            "Void Shroud",
            "Null-Grip Gauntlets",
            "Voidstep Treads"
        };
        AddBonus(s.twoPiece, SetBonusType::SpellDmgBoost, 15);
        AddBonus(s.twoPiece, SetBonusType::ManaCostReduce, 10);
        AddBonus(s.twoPiece, SetBonusType::ManaRegen, 10);
        AddBonus(s.fourPiece, SetBonusType::DoubleCast, 15);
        AddBonus(s.fourPiece, SetBonusType::AllResist, 15);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 6: Seraphic (Seraphim Council) — 8 pieces
    // Bonuses from Section 7: Seraphic Vestments
    // ================================================================
    {
        SetInfo s;
        s.name = "Seraphic";
        s.setId = 6;
        s.pieceNames = {
            "Staff of Radiant Truth",
            "Scepter of Judgment",
            "Blade of Dawn",
            "Halo of Ascension",
            "Helm of the Fallen Seraph",
            "Raiment of the Celestial Choir",
            "Seraphim's Grace Gloves",
            "Sandals of the Ascended"
        };
        AddBonus(s.twoPiece, SetBonusType::SpellDmgBoost, 15);
        AddBonus(s.twoPiece, SetBonusType::HealOnKill, 10);
        AddBonus(s.fourPiece, SetBonusType::Revive, 50);
        AddBonus(s.fourPiece, SetBonusType::AllResist, 15);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 7: Primordial (The Primordial One) — 9 pieces
    // Bonuses from Section 7: Primordial Vanguard
    // ================================================================
    {
        SetInfo s;
        s.name = "Primordial";
        s.setId = 7;
        s.pieceNames = {
            "Archmage Staff",
            "Titan's Greatsword",
            "Divine Scepter",
            "Shadowstrike Bow",
            "Gilded Blade",
            "Archmage Orb",
            "Archmage Robes",
            "Crown of the Primordial",
            "Ring of the Primordial"
        };
        AddBonus(s.twoPiece, SetBonusType::DmgBoost, 15);
        AddBonus(s.twoPiece, SetBonusType::AllResist, 10);
        AddBonus(s.fourPiece, SetBonusType::DmgBoost, 25);
        AddBonus(s.fourPiece, SetBonusType::DefBoost, 10);
        AddBonus(s.fourPiece, SetBonusType::Thorns, 25);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 8: Arcanum (Arcane Construct) — 6 pieces
    // ================================================================
    {
        SetInfo s;
        s.name = "Arcanum";
        s.setId = 8;
        s.pieceNames = {
            "Tome of Infinite Spells",
            "Construct's Core Staff",
            "Manaheart Crown",
            "Robes of the Infinite Weave",
            "Spell-Etched Gloves",
            "Boots of Mana Conduit"
        };
        AddBonus(s.twoPiece, SetBonusType::SpellDmgBoost, 15);
        AddBonus(s.twoPiece, SetBonusType::ManaRegen, 10);
        AddBonus(s.fourPiece, SetBonusType::DoubleCast, 15);
        AddBonus(s.fourPiece, SetBonusType::CooldownReduce, 1);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 9: Chrono (Chronos) — 6 pieces
    // Bonuses from Section 7: Chrono Warden
    // ================================================================
    {
        SetInfo s;
        s.name = "Chrono";
        s.setId = 9;
        s.pieceNames = {
            "Staff of Collapsed Timelines",
            "Chronoweaver Wand",
            "Chronolord's Helm",
            "Robes of the Timeless Weave",
            "Gauntlets of Frozen Time",
            "Loop of Eternity"
        };
        AddBonus(s.twoPiece, SetBonusType::AllResist, 10);
        AddBonus(s.twoPiece, SetBonusType::CooldownReduce, 1);
        AddBonus(s.fourPiece, SetBonusType::Dodge, 15);
        AddBonus(s.fourPiece, SetBonusType::DmgBoost, 15);
        AddBonus(s.fourPiece, SetBonusType::AllResist, 20);
        AddBonus(s.fourPiece, SetBonusType::Revive, 50);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 10: Archmage's Regalia (Epic, Mage, 4 pieces)
    // Bonuses from Section 7: Archmage's Regalia
    // ================================================================
    {
        SetInfo s;
        s.name = "Archmage's Regalia";
        s.setId = 10;
        s.pieceNames = {
            "Archmage's Circlet",
            "Archmage's Mantle",
            "Archmage's Handwraps",
            "Archmage's Focus"
        };
        AddBonus(s.twoPiece, SetBonusType::SpellDmgBoost, 10);
        AddBonus(s.fourPiece, SetBonusType::ManaCostReduce, 15);
        AddBonus(s.fourPiece, SetBonusType::SpellDmgBoost, 20);
        AddBonus(s.fourPiece, SetBonusType::ManaRegen, 10);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 11: Frostweaver's Raiment (Legendary, Mage, 3 pieces)
    // Bonuses from Section 7: Frostweaver's Raiment
    // ================================================================
    {
        SetInfo s;
        s.name = "Frostweaver's Raiment";
        s.setId = 11;
        s.pieceNames = {
            "Frostweaver's Crown",
            "Frostweaver's Robe",
            "Frostweaver's Focus"
        };
        AddBonus(s.twoPiece, SetBonusType::DmgBoost, 25);
        AddBonus(s.fourPiece, SetBonusType::StatusExtend, 2);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 12: Gilded Fortune (Legendary, Merchant, 3 pieces)
    // Bonuses from Section 7: Gilded Fortune
    // ================================================================
    {
        SetInfo s;
        s.name = "Gilded Fortune";
        s.setId = 12;
        s.pieceNames = {
            "Coin-Lord's Crown",
            "Merchant Prince's Vest",
            "Gilded Signet"
        };
        AddBonus(s.twoPiece, SetBonusType::GoldFind, 50);
        AddBonus(s.fourPiece, SetBonusType::GoldFind, 100);
        AddBonus(s.fourPiece, SetBonusType::ExpBoost, 20);
        s_sets.push_back(s);
    }

    // ================================================================
    // Set 13: Elementalist's Vestments (Epic, Any, 4 pieces)
    // Bonuses from Section 7: Elementalist's Vestments
    // ================================================================
    {
        SetInfo s;
        s.name = "Elementalist's Vestments";
        s.setId = 13;
        s.pieceNames = {
            "Elementalist's Circlet",
            "Elementalist's Mantle",
            "Elementalist's Leggings",
            "Elementalist's Sandals"
        };
        AddBonus(s.twoPiece, SetBonusType::DmgBoost, 8);
        AddBonus(s.fourPiece, SetBonusType::AllResist, 15);
        AddBonus(s.fourPiece, SetBonusType::DoubleCast, 20);
        s_sets.push_back(s);
    }

    s_initialized = true;
}

const SetInfo* SetBonuses::FindSet(const std::string& name)
{
    for (const auto& s : s_sets)
        if (s.name == name) return &s;
    return nullptr;
}

const SetInfo* SetBonuses::FindSetById(int id)
{
    for (const auto& s : s_sets)
        if (s.setId == id) return &s;
    return nullptr;
}

int SetBonuses::CountSetPieces(const Equipment& equip, int setId)
{
    int count = 0;

    auto checkItem = [&](const std::shared_ptr<Item>& item) {
        if (item && item->setId == setId)
            count++;
    };

    checkItem(equip.weapon);
    checkItem(equip.offhand);
    checkItem(equip.helmet);
    checkItem(equip.chest);
    checkItem(equip.gloves);
    checkItem(equip.pants);
    checkItem(equip.boots);
    checkItem(equip.ring1);
    checkItem(equip.ring2);
    checkItem(equip.amulet);

    return count;
}

std::vector<SetBonus> SetBonuses::GetActiveBonuses(const Equipment& equip, int setId)
{
    std::vector<SetBonus> result;
    const SetInfo* set = FindSetById(setId);
    if (!set) return result;

    int pieces = CountSetPieces(equip, setId);
    int fullThreshold = static_cast<int>(set->pieceNames.size());

    if (pieces >= 2)
    {
        for (const auto& b : set->twoPiece)
            result.push_back(b);
    }

    if (pieces >= fullThreshold)
    {
        for (const auto& b : set->fourPiece)
            result.push_back(b);
    }

    return result;
}

std::vector<SetBonus> SetBonuses::GetAllSetBonuses(const Equipment& equip)
{
    std::vector<SetBonus> all;
    for (const auto& s : s_sets)
    {
        auto bonuses = GetActiveBonuses(equip, s.setId);
        for (const auto& b : bonuses)
            all.push_back(b);
    }
    return all;
}

std::string SetBonuses::GetSetBonusDescription(const Equipment& equip)
{
    std::ostringstream ss;
    bool anyActive = false;

    for (const auto& s : s_sets)
    {
        int pieces = CountSetPieces(equip, s.setId);
        if (pieces < 2) continue;

        int fullThreshold = static_cast<int>(s.pieceNames.size());
        if (!anyActive) anyActive = true;

        ss << s.name << " (" << pieces << "/" << fullThreshold << ")\n";

        auto bonuses = GetActiveBonuses(equip, s.setId);
        for (const auto& b : bonuses)
        {
            ss << "  + " << b.value;
            switch (b.type)
            {
                case SetBonusType::DmgBoost:       ss << "% physical damage"; break;
                case SetBonusType::DefBoost:       ss << "% damage reduction"; break;
                case SetBonusType::ManaCostReduce: ss << "% mana cost reduction"; break;
                case SetBonusType::CooldownReduce: ss << " turn(s) cooldown reduction"; break;
                case SetBonusType::StatusExtend:   ss << " turn(s) status duration"; break;
                case SetBonusType::CritBoost:      ss << "% crit chance"; break;
                case SetBonusType::SpellDmgBoost:  ss << "% spell damage"; break;
                case SetBonusType::HpPerTurn:      ss << " HP per turn"; break;
                case SetBonusType::Dodge:          ss << "% dodge chance"; break;
                case SetBonusType::HealOnKill:     ss << "% heal on kill"; break;
                case SetBonusType::Thorns:         ss << "% damage reflected"; break;
                case SetBonusType::Revive:         ss << "% HP on revive"; break;
                case SetBonusType::ManaRegen:      ss << " mana per turn"; break;
                case SetBonusType::AllResist:      ss << " all resist"; break;
                case SetBonusType::DoubleCast:     ss << "% double cast chance"; break;
                case SetBonusType::BurnImmune:     ss << " - immune to burn"; break;
                case SetBonusType::GoldFind:       ss << "% gold from kills"; break;
                case SetBonusType::ExpBoost:       ss << "% experience from kills"; break;
            }
            ss << "\n";
        }
    }

    if (!anyActive)
        return "No set bonuses active.";

    return ss.str();
}
