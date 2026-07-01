#include "Combat.hpp"
#include <cstdlib>
#include <iomanip>
#include <string>

bool CombatSystem::StartCombat(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy)
{
    if (!player || !enemy || !player->IsAlive() || !enemy->IsAlive())
    {
        return false;
    }

    std::cout << "\n=== COMBAT START ===" << std::endl;
    std::cout << player->GetName() << " VS " << enemy->GetName() << std::endl;
    std::cout << "================================\n" << std::endl;

    // Combat loop
    while (player->IsAlive() && enemy->IsAlive())
    {
        DisplayCombatStatus(player, enemy);

        // Player turn
        std::cout << "\nYour turn! Choose action:" << std::endl;
        std::cout << "1. Attack" << std::endl;
        std::cout << "2. Use Skill" << std::endl;
        std::cout << "3. Defend" << std::endl;
        std::cout << "4. Flee" << std::endl;
        std::cout << "> ";

        int choice;
        std::cin >> choice;

        CombatAction playerAction = CombatAction::Attack;
        int skillIndex = 0;

        switch (choice)
        {
            case 1:
                playerAction = CombatAction::Attack;
                ExecuteTurn(player, enemy, playerAction);
                break;
            case 2:
                DisplaySkillOptions(player);
                std::cout << "Choose skill (0-" << player->GetSkills().GetSkillCount() - 1 << "): ";
                std::cin >> skillIndex;
                if (skillIndex >= 0 && skillIndex < static_cast<int>(player->GetSkills().GetSkillCount()))
                {
                    ExecuteTurn(player, enemy, CombatAction::UseSkill, skillIndex);
                }
                else
                {
                    std::cout << "Invalid skill!" << std::endl;
                }
                break;
            case 3:
                playerAction = CombatAction::Defend;
                player->IncreaseTempDefense(5);
                DisplayCombatLog(player->GetName() + " takes a defensive stance!");
                break;
            case 4:
                std::cout << "You fled from combat!" << std::endl;
                return false;
            default:
                std::cout << "Invalid action!" << std::endl;
                continue;
        }

        if (!enemy->IsAlive())
        {
            break;
        }

        // Enemy turn - simple AI
        std::cout << "\n" << enemy->GetName() << "'s turn!" << std::endl;

        int enemyChoice = rand() % 3;
        switch (enemyChoice)
        {
            case 0:
                ExecuteTurn(enemy, player, CombatAction::Attack);
                break;
            case 1:
                if (enemy->GetSkills().GetSkillCount() > 1)
                {
                    ExecuteTurn(enemy, player, CombatAction::UseSkill, 1 + rand() % (enemy->GetSkills().GetSkillCount() - 1));
                }
                else
                {
                    ExecuteTurn(enemy, player, CombatAction::Attack);
                }
                break;
            default:
                DisplayCombatLog(enemy->GetName() + " is preparing an attack!");
                break;
        }

        std::cout << std::endl;
    }

    // Combat end
    if (player->IsAlive())
    {
        std::cout << "\n=== YOU WON! ===" << std::endl;
        std::cout << "Victory!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "\n=== YOU LOST ===" << std::endl;
        std::cout << "You have been defeated..." << std::endl;
        return false;
    }
}

bool CombatSystem::ExecuteTurn(std::shared_ptr<Character> attacker, std::shared_ptr<Character> defender, CombatAction action, int skillIndex)
{
    if (!attacker || !defender)
        return false;

    switch (action)
    {
        case CombatAction::Attack:
        {
            auto attackSkill = attacker->GetSkills().GetSkill(0); // First skill is usually basic attack
            if (attackSkill)
            {
                attackSkill->Use(*attacker, *defender);
                DisplayCombatLog(attacker->GetName() + " attacks " + defender->GetName() + "!");
            }
            break;
        }

        case CombatAction::UseSkill:
        {
            auto skill = attacker->GetSkills().GetSkill(skillIndex);
            if (skill)
            {
                if (attacker->GetCurrentMana() >= skill->manaCost)
                {
                    if (skill->IsReady())
                    {
                        skill->Use(*attacker, *defender);
                        attacker->ReduceMana(skill->manaCost);
                        DisplayCombatLog(attacker->GetName() + " uses " + skill->name + "!");
                    }
                    else
                    {
                        DisplayCombatLog(skill->name + " is on cooldown! (" + std::to_string(skill->currentCooldown) + " turns)");
                    }
                }
                else
                {
                    DisplayCombatLog("Not enough mana for " + skill->name + "!");
                }
            }
            break;
        }

        case CombatAction::Defend:
            attacker->IncreaseTempDefense(5);
            DisplayCombatLog(attacker->GetName() + " takes a defensive stance!");
            break;

        default:
            break;
    }

    attacker->GetSkills().UpdateCooldowns();
    return true;
}

void CombatSystem::DisplayCombatStatus(std::shared_ptr<Character> player, std::shared_ptr<Character> enemy)
{
    int playerHealthBar = (player->GetCurrentHealth() * 20) / player->GetStats().health;
    int enemyHealthBar = (enemy->GetCurrentHealth() * 20) / enemy->GetStats().health;

    std::cout << "\n" << player->GetName() << " [";
    for (int i = 0; i < 20; ++i)
    {
        std::cout << (i < playerHealthBar ? "=" : " ");
    }
    std::cout << "] " << player->GetCurrentHealth() << "/" << player->GetStats().health << std::endl;

    std::cout << enemy->GetName() << " [";
    for (int i = 0; i < 20; ++i)
    {
        std::cout << (i < enemyHealthBar ? "=" : " ");
    }
    std::cout << "] " << enemy->GetCurrentHealth() << "/" << enemy->GetStats().health << std::endl;
}

void CombatSystem::DisplaySkillOptions(std::shared_ptr<Character> character)
{
    if (!character)
        return;

    std::cout << "\nAvailable Skills:" << std::endl;
    for (size_t i = 0; i < character->GetSkills().GetSkillCount(); ++i)
    {
        auto skill = character->GetSkills().GetSkill(i);
        if (skill)
        {
            std::cout << i << ". " << skill->name << " (Mana: " << skill->manaCost << ", Cooldown: " << skill->currentCooldown << "/" << skill->cooldown << ")" << std::endl;
        }
    }
}

void CombatSystem::DisplayCombatLog(const std::string& message)
{
    std::cout << "> " << message << std::endl;
}
