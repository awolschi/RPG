#ifndef PASSIVES_HPP
#define PASSIVES_HPP

#include "Item.hpp"
#include "Equipment/Equipment.hpp"
#include <vector>
#include <cstdlib>

class Character;

class Passives
{
public:
    static std::vector<ItemPassive> Collect(const Equipment& equip);

    static bool Has(const Equipment& equip, ItemPassive p);

    static int Count(const Equipment& equip, ItemPassive p);

    static int FlatBonus(const Equipment& equip, ItemPassive target,
                         ItemPassive match1, int val1,
                         ItemPassive match2 = ItemPassive::None, int val2 = 0);

    static int GetPhysicalDamageBoostPercent(const Equipment& equip);
    static int GetSpellDamageBoostPercent(const Equipment& equip);
    static int GetAttackSpeedPercent(const Equipment& equip);
    static int GetCritChancePercent(const Equipment& equip);
    static int GetDamageReductionPercent(const Equipment& equip);
    static int GetThornsDamage(const Equipment& equip);
    static int GetHealOnKillPercent(const Equipment& equip);
    static int GetManaRegenBonus(const Equipment& equip);
    static int GetManaCostReductionPercent(const Equipment& equip);
    static int GetManaOnKill(const Equipment& equip);
    static int GetDoubleCastPercent(const Equipment& equip);
    static int GetCooldownReductionTurns(const Equipment& equip);
    static int GetDodgePercent(const Equipment& equip);
    static int GetAllResistBonus(const Equipment& equip);
    static int GetExpBoostPercent(const Equipment& equip);
    static int GetGoldFindPercent(const Equipment& equip);

    static bool IsStunImmune(const Equipment& equip);
    static bool IsFreezeImmune(const Equipment& equip);
    static bool IsBurnImmune(const Equipment& equip);
    static bool IsPoisonImmune(const Equipment& equip);

    static int GetStatusExtendTurns(const Equipment& equip);
    static int GetManaShieldPercent(const Equipment& equip);

    static bool HasPhoenixRevive(const Equipment& equip);
    static bool HasRevive50(const Equipment& equip);
    static bool HasTauntChance(const Equipment& equip);
    static bool HasLifesteal(const Equipment& equip);
    static int GetLifestealPercent(const Equipment& equip);
    static bool HasLowHpBonus(const Equipment& equip);
    static bool HasFireNoCd(const Equipment& equip);
    static bool HasIceFreezePlus(const Equipment& equip);
    static int GetLightningStunChance(const Equipment& equip);
    static bool HasArcaneCrit(const Equipment& equip);
    static bool HasPoisonDmgPlus(const Equipment& equip);
    static bool HasManaOnSkillUse(const Equipment& equip);
    static bool HasHpOnSkillUse(const Equipment& equip);
    static int GetManaOnSkillUseAmount(const Equipment& equip);
    static int GetHpOnSkillUseAmount(const Equipment& equip);

    static bool RollProc(int percentChance);

    static void ApplyPostCombatPassives(Character* attacker, Character* defender,
                                         const Equipment& attackerEquip, const Equipment& defenderEquip);
};

#endif
