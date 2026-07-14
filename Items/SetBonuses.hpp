#ifndef SET_BONUSES_HPP
#define SET_BONUSES_HPP

#include <string>
#include <vector>
#include "Equipment/Equipment.hpp"

enum class SetBonusType
{
    DmgBoost,
    DefBoost,
    ManaCostReduce,
    CooldownReduce,
    StatusExtend,
    CritBoost,
    SpellDmgBoost,
    HpPerTurn,
    Dodge,
    HealOnKill,
    Thorns,
    Revive,
    ManaRegen,
    AllResist,
    DoubleCast,
    BurnImmune,
    GoldFind,
    ExpBoost
};

inline const char* SetBonusTypeName(SetBonusType t)
{
    switch (t)
    {
        case SetBonusType::DmgBoost:       return "Physical Damage";
        case SetBonusType::DefBoost:       return "Damage Reduction";
        case SetBonusType::ManaCostReduce: return "Mana Cost";
        case SetBonusType::CooldownReduce: return "Cooldown";
        case SetBonusType::StatusExtend:   return "Status Duration";
        case SetBonusType::CritBoost:      return "Crit Chance";
        case SetBonusType::SpellDmgBoost:  return "Spell Damage";
        case SetBonusType::HpPerTurn:      return "HP per Turn";
        case SetBonusType::Dodge:          return "Dodge";
        case SetBonusType::HealOnKill:     return "Heal on Kill";
        case SetBonusType::Thorns:         return "Thorns";
        case SetBonusType::Revive:         return "Revive";
        case SetBonusType::ManaRegen:      return "Mana per Turn";
        case SetBonusType::AllResist:      return "All Resist";
        case SetBonusType::DoubleCast:     return "Double Cast";
        case SetBonusType::BurnImmune:     return "Burn Immunity";
        case SetBonusType::GoldFind:       return "Gold Find";
        case SetBonusType::ExpBoost:       return "XP Boost";
    }
    return "Unknown";
}

struct SetBonus
{
    SetBonusType type;
    int value;
};

struct SetInfo
{
    std::string name;
    int setId;
    std::vector<std::string> pieceNames;
    std::vector<SetBonus> twoPiece;
    std::vector<SetBonus> fourPiece;
};

class SetBonuses
{
public:
    static void Initialize();

    static const SetInfo* FindSet(const std::string& name);
    static const SetInfo* FindSetById(int id);

    static int CountSetPieces(const Equipment& equip, int setId);
    static std::vector<SetBonus> GetActiveBonuses(const Equipment& equip, int setId);
    static std::vector<SetBonus> GetAllSetBonuses(const Equipment& equip);
    static std::string GetSetBonusDescription(const Equipment& equip);

    static const std::vector<SetInfo>& GetAllSets() { return s_sets; }

private:
    static std::vector<SetInfo> s_sets;
    static bool s_initialized;
};

#endif
