#include "Religion.hpp"
#include "../Characters/Character.hpp"
#include <iostream>

ReligionSystem::ReligionSystem()
    : currentGod(GodType::None), devotionLevel(0), totalDonated(0)
{
}

void ReligionSystem::SetGod(GodType god)
{
    currentGod = god;
    devotionLevel = 0;
    totalDonated = 0;
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
            return "The god of purity and rebirth. Grants health bonuses.";
        case GodType::Karosh:
            return "The god of despair and destruction. Grants attack power.";
        case GodType::Amala:
            return "The iron god of craftsmanship. Grants defense and crafting bonuses.";
        case GodType::Tordo:
            return "The god of strength. Grants raw power at a cost.";
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

void ReligionSystem::ApplyDevotionBonus(std::shared_ptr<Character> player) const
{
    if (!player || currentGod == GodType::None || devotionLevel == 0)
        return;

    std::cout << "\n" << GetGodName() << "'s blessing empowers you (Devotion " << devotionLevel << ")!\n";

    switch (currentGod)
    {
        case GodType::Shaim:
            player->RestoreHealth(devotionLevel * 3);
            break;
        case GodType::Karosh:
            player->SetAttackBonus(devotionLevel * 2);
            std::cout << "Your attacks deal +" << (devotionLevel * 2) << " damage.\n";
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
}
