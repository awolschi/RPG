#include "Passives.hpp"
#include "../Characters/Character.hpp"
#include <functional>

static void CollectFromItem(std::vector<ItemPassive>& out, const std::shared_ptr<Item>& item)
{
    if (!item) return;
    if (item->passive1 != ItemPassive::None) out.push_back(item->passive1);
    if (item->passive2 != ItemPassive::None) out.push_back(item->passive2);
}

static void ForEachEquipped(const Equipment& equip, std::function<void(const std::shared_ptr<Item>&)> fn)
{
    fn(equip.weapon);
    fn(equip.offhand);
    fn(equip.helmet);
    fn(equip.chest);
    fn(equip.gloves);
    fn(equip.pants);
    fn(equip.boots);
    fn(equip.ring1);
    fn(equip.ring2);
    fn(equip.amulet);
}

std::vector<ItemPassive> Passives::Collect(const Equipment& equip)
{
    std::vector<ItemPassive> result;
    ForEachEquipped(equip, [&](const std::shared_ptr<Item>& item) { CollectFromItem(result, item); });
    return result;
}

bool Passives::Has(const Equipment& equip, ItemPassive p)
{
    bool found = false;
    ForEachEquipped(equip, [&](const std::shared_ptr<Item>& item)
    {
        if (found) return;
        if (item && (item->passive1 == p || item->passive2 == p)) found = true;
    });
    return found;
}

int Passives::Count(const Equipment& equip, ItemPassive p)
{
    int c = 0;
    ForEachEquipped(equip, [&](const std::shared_ptr<Item>& item)
    {
        if (!item) return;
        if (item->passive1 == p) c++;
        if (item->passive2 == p) c++;
    });
    return c;
}

int Passives::FlatBonus(const Equipment& equip, ItemPassive target,
                        ItemPassive match1, int val1,
                        ItemPassive match2, int val2)
{
    int total = 0;
    ForEachEquipped(equip, [&](const std::shared_ptr<Item>& item)
    {
        if (!item) return;
        if (item->passive1 == target || item->passive2 == target)
        {
            if (target == match1) total += val1;
            else if (match2 != ItemPassive::None && target == match2) total += val2;
        }
    });
    return total;
}

int Passives::GetPhysicalDamageBoostPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::PhysDmgBoost15) * 15;
    total += Count(equip, ItemPassive::PhysDmgBoost20) * 20;
    return total;
}

int Passives::GetSpellDamageBoostPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::SpellDmgBoost15) * 15;
    total += Count(equip, ItemPassive::SpellDmgBoost20) * 20;
    return total;
}

int Passives::GetAttackSpeedPercent(const Equipment& equip)
{
    return Count(equip, ItemPassive::AtkSpeed20) * 20;
}

int Passives::GetCritChancePercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::CritChance20) * 20;
    total += Count(equip, ItemPassive::CritChance30) * 30;
    return total;
}

int Passives::GetDamageReductionPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::DamageReduce10) * 10;
    total += Count(equip, ItemPassive::DamageReduce15) * 15;
    return total;
}

int Passives::GetThornsDamage(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::Thorns25) * 25;
    total += Count(equip, ItemPassive::Thorns40) * 40;
    return total;
}

int Passives::GetHealOnKillPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::HealOnKill10) * 10;
    total += Count(equip, ItemPassive::HealOnKill15) * 15;
    if (Has(equip, ItemPassive::HolyHealOnKill)) total += 10;
    return total;
}

int Passives::GetManaRegenBonus(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::ManaRegen5) * 5;
    total += Count(equip, ItemPassive::ManaRegen10) * 10;
    return total;
}

int Passives::GetManaCostReductionPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::ManaCostReduce10) * 10;
    total += Count(equip, ItemPassive::ManaCostReduce15) * 15;
    return total;
}

int Passives::GetManaOnKill(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::ManaOnKill15) * 15;
    total += Count(equip, ItemPassive::ManaOnKill25) * 25;
    return total;
}

int Passives::GetDoubleCastPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::DoubleCast15) * 15;
    total += Count(equip, ItemPassive::DoubleCast25) * 25;
    return total;
}

int Passives::GetCooldownReductionTurns(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::CooldownReduce1) * 1;
    total += Count(equip, ItemPassive::CooldownReduce2) * 2;
    return total;
}

int Passives::GetDodgePercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::Dodge15) * 15;
    total += Count(equip, ItemPassive::Dodge20) * 20;
    return total;
}

int Passives::GetAllResistBonus(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::AllResist5) * 5;
    total += Count(equip, ItemPassive::AllResist10) * 10;
    return total;
}

int Passives::GetExpBoostPercent(const Equipment& equip)
{
    return Count(equip, ItemPassive::ExpBoost20) * 20;
}

int Passives::GetGoldFindPercent(const Equipment& equip)
{
    return Count(equip, ItemPassive::GoldFind30) * 30;
}

bool Passives::IsStunImmune(const Equipment& equip)  { return Has(equip, ItemPassive::StunImmune); }
bool Passives::IsFreezeImmune(const Equipment& equip) { return Has(equip, ItemPassive::FreezeImmune); }
bool Passives::IsBurnImmune(const Equipment& equip)   { return Has(equip, ItemPassive::BurnImmune); }
bool Passives::IsPoisonImmune(const Equipment& equip) { return Has(equip, ItemPassive::PoisonImmune); }

int Passives::GetStatusExtendTurns(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::StatusExtend1) * 1;
    total += Count(equip, ItemPassive::StatusExtend2) * 2;
    return total;
}

int Passives::GetManaShieldPercent(const Equipment& equip)
{
    return Count(equip, ItemPassive::ManaShield30) * 30;
}

bool Passives::HasPhoenixRevive(const Equipment& equip)  { return Has(equip, ItemPassive::PhoenixRevive); }
bool Passives::HasRevive50(const Equipment& equip)       { return Has(equip, ItemPassive::Revive50); }
bool Passives::HasTauntChance(const Equipment& equip)    { return Has(equip, ItemPassive::TauntChance25); }
bool Passives::HasFireNoCd(const Equipment& equip)       { return Has(equip, ItemPassive::FireNoCd20); }
bool Passives::HasIceFreezePlus(const Equipment& equip)  { return Has(equip, ItemPassive::IceFreezePlus1); }
bool Passives::HasArcaneCrit(const Equipment& equip)     { return Has(equip, ItemPassive::ArcaneCrit30); }
bool Passives::HasPoisonDmgPlus(const Equipment& equip)  { return Has(equip, ItemPassive::PoisonDmgPlus25); }
bool Passives::HasManaOnSkillUse(const Equipment& equip) { return Has(equip, ItemPassive::ManaOnSkillUse10); }
bool Passives::HasHpOnSkillUse(const Equipment& equip)   { return Has(equip, ItemPassive::HpOnSkillUse10); }

bool Passives::HasLifesteal(const Equipment& equip)
{
    return Has(equip, ItemPassive::Lifesteal15) || Has(equip, ItemPassive::Lifesteal25);
}

int Passives::GetLifestealPercent(const Equipment& equip)
{
    int total = 0;
    total += Count(equip, ItemPassive::Lifesteal15) * 15;
    total += Count(equip, ItemPassive::Lifesteal25) * 25;
    return total;
}

bool Passives::HasLowHpBonus(const Equipment& equip)
{
    return Has(equip, ItemPassive::SpellDmgLowHp30) || Has(equip, ItemPassive::PhysDmgLowHp30);
}

int Passives::GetLightningStunChance(const Equipment& equip)
{
    return Count(equip, ItemPassive::LightningStun15) * 15;
}

int Passives::GetManaOnSkillUseAmount(const Equipment& equip)
{
    return Count(equip, ItemPassive::ManaOnSkillUse10) * 10;
}

int Passives::GetHpOnSkillUseAmount(const Equipment& equip)
{
    return Count(equip, ItemPassive::HpOnSkillUse10) * 10;
}

bool Passives::RollProc(int percentChance)
{
    if (percentChance <= 0) return false;
    if (percentChance >= 100) return true;
    return (rand() % 100) < percentChance;
}

void Passives::ApplyPostCombatPassives(Character* attacker, Character* defender,
                                        const Equipment& attackerEquip, const Equipment& defenderEquip)
{
    (void)defenderEquip;
    if (!attacker || !defender) return;

    int thorns = GetThornsDamage(defenderEquip);
    if (thorns > 0)
    {
        int thornsDmg = defender->GetMaxHealth() * thorns / 100;
        if (thornsDmg < 1) thornsDmg = 1;
        attacker->TakeDamage(thornsDmg);
    }
}
