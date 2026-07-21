#include "Religion.hpp"
#include "../Characters/Character.hpp"
#include <sstream>

ReligionSystem::ReligionSystem()
    : currentGod(GodType::None), devotionLevel(0), totalDonated(0),
      prayersToday(0), lastPrayerDay(0), godAbility1Used(false), godAbility2Used(false)
{
    activeQuest = { "", "", 0, 0, 0, false };
}

void ReligionSystem::SetGod(GodType god)
{
    currentGod = god;
    devotionLevel = 0;
    totalDonated = 0;
    prayersToday = 0;
    activeQuest = { "", "", 0, 0, 0, false };
    if (god != GodType::None)
        GenerateQuest();
}

bool ReligionSystem::Donate(int amount)
{
    if (amount <= 0 || currentGod == GodType::None)
        return false;

    totalDonated += amount;
    int needed = GetNextDevotionCost();
    if (totalDonated >= needed && devotionLevel < 10)
    {
        devotionLevel++;
        totalDonated = 0;
        return true;
    }
    return false;
}

std::string ReligionSystem::GetGodName() const
{
    switch (currentGod)
    {
        case GodType::Shaim:  return "Shaim";
        case GodType::Karosh: return "Karosh";
        case GodType::Amala:  return "Amala";
        case GodType::Tordo:  return "Tordo";
        default:              return "None";
    }
}

std::string ReligionSystem::GetGodDescription() const
{
    switch (currentGod)
    {
        case GodType::Shaim:
            return "The god of purity and rebirth. Grants healing and defense.";
        case GodType::Karosh:
            return "The god of despair and destruction. Grants attack power but lowers defense.";
        case GodType::Amala:
            return "The iron god of craftsmanship. Grants defense and crafting bonuses.";
        case GodType::Tordo:
            return "The god of strength and hate. Grants raw power at a cost.";
        default:
            return "No deity chosen.";
    }
}

int ReligionSystem::GetNextDevotionCost() const
{
    return 50 + (devotionLevel * 30);
}

void ReligionSystem::RestoreState(GodType god, int devotion, int donated)
{
    currentGod = god;
    devotionLevel = devotion;
    totalDonated = donated;
}

std::string ReligionSystem::ApplyDevotionBonus(std::shared_ptr<Character> player) const
{
    if (!player || currentGod == GodType::None || devotionLevel == 0)
        return "";

    std::string msg = GetGodName() + "'s blessing empowers you (Devotion " + std::to_string(devotionLevel) + ")!";

    switch (currentGod)
    {
        case GodType::Shaim:
            player->RestoreHealth(devotionLevel * 3);
            break;
        case GodType::Karosh:
            player->SetAttackBonus(devotionLevel * 2);
            msg += " Your attacks deal +" + std::to_string(devotionLevel * 2) + " damage.";
            break;
        case GodType::Amala:
            player->IncreaseTempDefense(devotionLevel * 2);
            break;
        case GodType::Tordo:
            player->IncreaseTempDefense(devotionLevel);
            player->RestoreHealth(devotionLevel * 5);
            break;
        default:
            break;
    }
    return msg;
}

// Prayer system
bool ReligionSystem::CanPray() const
{
    if (currentGod == GodType::None) return false;
    if (prayersToday >= 3) return false; // Max 3 prayers per "day"
    return true;
}

std::string ReligionSystem::Pray(std::shared_ptr<Character> player)
{
    if (!CanPray()) return "You cannot pray right now.";

    prayersToday++;
    std::string result;

    switch (currentGod)
    {
        case GodType::Shaim:
        {
            int heal = 10 + devotionLevel * 2;
            player->RestoreHealth(heal);
            result = "Shaim's light washes over you. Restored " + std::to_string(heal) + " HP.";
            break;
        }
        case GodType::Karosh:
        {
            int bonus = 3 + devotionLevel;
            player->SetAttackBonus(player->GetAttackBonus() + bonus);
            result = "Karosh's rage fills you. +" + std::to_string(bonus) + " attack for this fight.";
            break;
        }
        case GodType::Amala:
        {
            int def = 3 + devotionLevel;
            player->IncreaseTempDefense(def);
            result = "Amala's iron will strengthens you. +" + std::to_string(def) + " defense for this fight.";
            break;
        }
        case GodType::Tordo:
        {
            int heal = 5 + devotionLevel * 3;
            int def = devotionLevel;
            player->RestoreHealth(heal);
            player->IncreaseTempDefense(def);
            result = "Tordo's fury empowers you. +" + std::to_string(def) + " DEF, restored " + std::to_string(heal) + " HP.";
            break;
        }
        default:
            break;
    }

    // Small devotion gain from praying
    if (prayersToday == 1 && devotionLevel < 10)
    {
        devotionLevel++;
        result += " (Devotion +1)";
    }

    return result;
}

int ReligionSystem::GetPrayerCooldown() const
{
    return 3 - prayersToday; // Prayers remaining today
}

void ReligionSystem::ResetPrayers()
{
    prayersToday = 0;
}

bool ReligionSystem::CanUseGodAbility1() const
{
    return currentGod != GodType::None && devotionLevel >= 3 && !godAbility1Used;
}

bool ReligionSystem::CanUseGodAbility2() const
{
    return currentGod != GodType::None && devotionLevel >= 7 && !godAbility2Used;
}

void ReligionSystem::UseGodAbility1()
{
    godAbility1Used = true;
}

void ReligionSystem::UseGodAbility2()
{
    godAbility2Used = true;
}

void ReligionSystem::ResetGodAbilityCooldowns()
{
    godAbility1Used = false;
    godAbility2Used = false;
}

std::string ReligionSystem::ExecuteGodAbility1(std::shared_ptr<Character> player)
{
    if (!player || !CanUseGodAbility1()) return "";

    UseGodAbility1();
    std::string result;

    switch (currentGod)
    {
        case GodType::Shaim:
        {
            int heal = player->GetStats().health / 3;
            player->RestoreHealth(heal);
            result = "Divine Light! Restored " + std::to_string(heal) + " HP.";
            break;
        }
        case GodType::Karosh:
        {
            int bonus = player->GetAttackBonus() + player->GetStats().strength / 4;
            player->SetAttackBonus(bonus);
            result = "Blood Rage! +25% damage for this combat.";
            break;
        }
        case GodType::Amala:
        {
            int def = 15 + devotionLevel;
            player->IncreaseTempDefense(def);
            result = "Fortify! +" + std::to_string(def) + " DEF for this combat.";
            break;
        }
        case GodType::Tordo:
        {
            int dmgBonus = player->GetAttackBonus() + player->GetStats().strength / 10;
            player->SetAttackBonus(dmgBonus);
            int def = 10 + devotionLevel;
            player->IncreaseTempDefense(def);
            result = "War Cry! +" + std::to_string(dmgBonus) + " ATK, +" + std::to_string(def) + " DEF.";
            break;
        }
        default:
            break;
    }

    return result;
}

std::string ReligionSystem::ExecuteGodAbility2(std::shared_ptr<Character> player)
{
    if (!player || !CanUseGodAbility2()) return "";

    UseGodAbility2();
    std::string result;

    switch (currentGod)
    {
        case GodType::Shaim:
        {
            int maxHp = player->GetStats().health;
            player->RestoreHealth(maxHp);
            result = "Rebirth! Restored to full HP!";
            break;
        }
        case GodType::Karosh:
        {
            int bonus = player->GetAttackBonus() + player->GetStats().strength / 2;
            player->SetAttackBonus(bonus);
            int defLoss = player->GetStats().defense / 4;
            player->IncreaseTempDefense(-defLoss);
            result = "Annihilate! +50% damage, -25% DEF for this combat.";
            break;
        }
        case GodType::Amala:
        {
            int def = 30 + devotionLevel * 2;
            player->IncreaseTempDefense(def);
            result = "Iron Skin! +" + std::to_string(def) + " DEF for this combat.";
            break;
        }
        case GodType::Tordo:
        {
            int bonus = player->GetAttackBonus() + player->GetStats().strength * 2 / 5;
            player->SetAttackBonus(bonus);
            int hpLoss = player->GetStats().health / 7;
            player->TakeDamage(hpLoss, ElementType::Physical);
            result = "Titan's Strength! +40% damage, sacrificed " + std::to_string(hpLoss) + " HP.";
            break;
        }
        default:
            break;
    }

    return result;
}

// God-specific quests
void ReligionSystem::GenerateQuest()
{
    if (currentGod == GodType::None) return;

    // Generate a simple kill quest based on god
    switch (currentGod)
    {
        case GodType::Shaim:
            activeQuest = { "Purify 5 undead creatures", "Wraith", 5, 0, 1, false };
            break;
        case GodType::Karosh:
            activeQuest = { "Slay 10 enemies in combat", "", 10, 0, 2, false };
            break;
        case GodType::Amala:
            activeQuest = { "Defeat 3 construct enemies", "Golem", 3, 0, 1, false };
            break;
        case GodType::Tordo:
            activeQuest = { "Defeat 7 enemies without healing", "", 7, 0, 2, false };
            break;
        default:
            break;
    }
}

void ReligionSystem::EnsureQuest()
{
    if (currentGod == GodType::None) return;
    if (activeQuest.targetEnemy.empty() && activeQuest.targetCount == 0 && activeQuest.description.empty())
        GenerateQuest();
}

void ReligionSystem::RestoreQuest(const std::string& desc, const std::string& target, int targetCount,
                                   int currentCount, int rewardDevotion, bool completed)
{
    if (currentGod == GodType::None) return;
    if (!desc.empty())
    {
        activeQuest.description = desc;
        activeQuest.targetEnemy = target;
        activeQuest.targetCount = targetCount;
        activeQuest.rewardDevotion = rewardDevotion;
    }
    activeQuest.currentCount = currentCount;
    activeQuest.completed = completed;
}

bool ReligionSystem::ProgressQuest(const std::string& enemyName)
{
    if (activeQuest.completed || currentGod == GodType::None) return false;

    // For quests with a specific target, check name match
    if (!activeQuest.targetEnemy.empty())
    {
        if (enemyName.find(activeQuest.targetEnemy) == std::string::npos)
            return false;
    }

    activeQuest.currentCount++;
    if (activeQuest.currentCount >= activeQuest.targetCount)
    {
        activeQuest.completed = true;
        return true; // Quest completed!
    }
    return false;
}

void ReligionSystem::CompleteQuest()
{
    if (!activeQuest.completed) return;
    devotionLevel = std::min(10, devotionLevel + activeQuest.rewardDevotion);
    GenerateQuest(); // Generate next quest
}

GodAbility ReligionSystem::GetGodAbility1() const
{
    switch (currentGod)
    {
        case GodType::Shaim:
            return { "Divine Light", "Heal 30% max HP (Devotion 3)", 3 };
        case GodType::Karosh:
            return { "Blood Rage", "+25% damage for 3 turns (Devotion 3)", 3 };
        case GodType::Amala:
            return { "Fortify", "+15 DEF for 3 turns (Devotion 3)", 3 };
        case GodType::Tordo:
            return { "War Cry", "+10% damage & +10 DEF (Devotion 3)", 3 };
        default:
            return { "None", "", 0 };
    }
}

GodAbility ReligionSystem::GetGodAbility2() const
{
    switch (currentGod)
    {
        case GodType::Shaim:
            return { "Rebirth", "Restore to full HP once per combat (Devotion 7)", 7 };
        case GodType::Karosh:
            return { "Annihilate", "+50% damage, -25% DEF for 3 turns (Devotion 7)", 7 };
        case GodType::Amala:
            return { "Iron Skin", "+30 DEF for 3 turns (Devotion 7)", 7 };
        case GodType::Tordo:
            return { "Titan's Strength", "+40% damage, -15% max HP (Devotion 7)", 7 };
        default:
            return { "None", "", 0 };
    }
}

bool ReligionSystem::HasAbility1() const
{
    return devotionLevel >= GetGodAbility1().requiredDevotion;
}

bool ReligionSystem::HasAbility2() const
{
    return devotionLevel >= GetGodAbility2().requiredDevotion;
}

std::string ReligionSystem::Serialize() const
{
    return std::to_string(static_cast<int>(currentGod)) + " "
         + std::to_string(devotionLevel) + " "
         + std::to_string(totalDonated) + " "
         + std::to_string(prayersToday) + " "
         + std::to_string(lastPrayerDay) + " "
         + std::to_string(activeQuest.currentCount) + " "
         + std::to_string(activeQuest.completed ? 1 : 0) + " "
         + std::to_string(godAbility1Used ? 1 : 0) + " "
         + std::to_string(godAbility2Used ? 1 : 0);
}

void ReligionSystem::Deserialize(const std::string& data)
{
    std::istringstream ss(data);
    int godInt, dp, td, pt, ld, qc, ac, ab1, ab2;
    ss >> godInt >> dp >> td >> pt >> ld >> qc >> ac;
    currentGod = static_cast<GodType>(godInt);
    devotionLevel = dp;
    totalDonated = td;
    prayersToday = pt;
    lastPrayerDay = ld;
    activeQuest.currentCount = qc;
    activeQuest.completed = (ac == 1);
    godAbility1Used = false;
    godAbility2Used = false;
    if (ss >> ab1 >> ab2)
    {
        godAbility1Used = (ab1 == 1);
        godAbility2Used = (ab2 == 1);
    }
}
