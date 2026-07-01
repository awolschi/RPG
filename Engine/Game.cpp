#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Items/Loot.hpp"
#include "../World/Enemies/Enemies.hpp"

Game::Game() : currentState(GameState::MainMenu), combatSystem(std::make_shared<CombatSystem>())
{
    srand(static_cast<unsigned>(time(nullptr)));
}

void Game::Run()
{
    while (currentState != GameState::Exit)
    {
        switch (currentState)
        {
            case GameState::MainMenu:
                MainMenu();
                break;
            case GameState::CharacterCreation:
                CharacterCreation();
                break;
            case GameState::Exploring:
                ExploreMenu();
                break;
            case GameState::Inventory:
                InventoryMenu();
                break;
            case GameState::Stats:
                StatsMenu();
                break;
            case GameState::Jobs:
                JobsMenu();
                break;
            case GameState::EnemySelection:
                EnemySelectionMenu();
                break;
            case GameState::Exit:
                break;
            default:
                currentState = GameState::MainMenu;
                break;
        }
    }

    std::cout << "\nThanks for playing!" << std::endl;
}

void Game::MainMenu()
{
    ClearScreen();
    DisplayWelcome();

    std::cout << "\nMain Menu:" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            currentState = GameState::CharacterCreation;
            break;
        case 2:
            currentState = GameState::Exit;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }
}

void Game::CharacterCreation()
{
    ClearScreen();
    std::cout << "================================\n";
    std::cout << "    CHARACTER CREATION\n";
    std::cout << "================================\n\n";

    std::string playerName;
    std::cout << "Enter your character name: ";
    std::cin.ignore();
    std::getline(std::cin, playerName);

    std::cout << "\nChoose your class:\n";
    std::cout << "1. Warrior - High HP, High Defense\n";
    std::cout << "2. Priest - Balanced, Healing abilities\n";
    std::cout << "3. Mage - High Intelligence, Powerful spells\n";
    std::cout << "4. Archer - High Dexterity, Ranged attacks\n";
    std::cout << "> ";

    int classChoice;
    std::cin >> classChoice;

    CharacterClass playerClass = CharacterClass::Warrior;
    switch (classChoice)
    {
        case 1:
            playerClass = CharacterClass::Warrior;
            break;
        case 2:
            playerClass = CharacterClass::Priest;
            break;
        case 3:
            playerClass = CharacterClass::Mage;
            break;
        case 4:
            playerClass = CharacterClass::Archer;
            break;
        default:
            std::cout << "Invalid choice! Warrior selected.\n";
            playerClass = CharacterClass::Warrior;
    }

    player = std::make_shared<Player>(playerName, playerClass);

    ClearScreen();
    std::cout << "Character created!\n\n";
    player->DisplayStats();

    currentState = GameState::Exploring;
}

void Game::ExploreMenu()
{
    if (!player)
    {
        currentState = GameState::MainMenu;
        return;
    }

    ClearScreen();
    std::cout << "================================\n";
    std::cout << "       EXPLORATION MENU\n";
    std::cout << "================================\n";
    std::cout << "Player: " << player->GetName() << " (Level " << player->GetLevel() << ")\n";
    std::cout << "Health: " << player->GetCurrentHealth() << "/" << player->GetStats().health << "\n";
    std::cout << "Mana: " << player->GetCurrentMana() << "/" << player->GetStats().mana << "\n";
    std::cout << "Gold: " << player->GetInventory().GetGold() << "\n";
    std::cout << "\nWhat would you like to do?\n";
    std::cout << "1. Search for enemies (random)\n";
    std::cout << "2. Choose your opponent\n";
    std::cout << "3. View inventory\n";
    std::cout << "4. View stats\n";
    std::cout << "5. View jobs\n";
    std::cout << "6. Rest (restore health/mana)\n";
    std::cout << "7. Return to main menu\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            StartRandomEncounter();
            break;
        case 2:
            currentState = GameState::EnemySelection;
            break;
        case 3:
            currentState = GameState::Inventory;
            break;
        case 4:
            currentState = GameState::Stats;
            break;
        case 5:
            currentState = GameState::Jobs;
            break;
        case 6:
            player->RestoreHealth(player->GetStats().health);
            player->RestoreMana(player->GetStats().mana);
            std::cout << "You rest and recover your health and mana!\n";
            break;
        case 7:
            if (player->IsAlive())
            {
                currentState = GameState::MainMenu;
            }
            else
            {
                std::cout << "You are defeated! Game Over.\n";
                currentState = GameState::MainMenu;
                player.reset();
            }
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
    }
}

void Game::InventoryMenu()
{
    if (!player)
    {
        currentState = GameState::Exploring;
        return;
    }

    ClearScreen();
    std::cout << "================================\n";
    std::cout << "        INVENTORY\n";
    std::cout << "================================\n";
    std::cout << "Gold: " << player->GetInventory().GetGold() << "\n";
    std::cout << "Items: " << player->GetInventory().GetItemCount() << "/" << player->GetInventory().GetMaxSlots() << "\n\n";

    if (player->GetInventory().GetItemCount() == 0)
    {
        std::cout << "Your inventory is empty.\n";
    }
    else
    {
        for (size_t i = 0; i < player->GetInventory().GetItemCount(); ++i)
        {
            auto item = player->GetInventory().GetItem(i);
            if (item)
            {
                std::cout << i + 1 << ". " << item->name << " (Rarity: " << item->rarity << ")\n";
            }
        }
    }

    std::cout << "\nPress Enter to return to exploration...\n";
    std::cin.ignore();
    std::cin.get();

    currentState = GameState::Exploring;
}

void Game::StatsMenu()
{
    if (!player)
    {
        currentState = GameState::Exploring;
        return;
    }

    ClearScreen();
    player->DisplayStats();

    std::cout << "\nSkills:\n";
    for (size_t i = 0; i < player->GetSkills().GetSkillCount(); ++i)
    {
        auto skill = player->GetSkills().GetSkill(i);
        if (skill)
        {
            std::cout << i + 1 << ". " << skill->name << " (Level " << skill->level << ", XP: " << skill->experience << "/100)\n";
        }
    }

    std::cout << "\nPress Enter to return to exploration...\n";
    std::cin.ignore();
    std::cin.get();

    currentState = GameState::Exploring;
}

void Game::JobsMenu()
{
    if (!player)
    {
        currentState = GameState::Exploring;
        return;
    }

    ClearScreen();
    std::cout << "================================\n";
    std::cout << "           JOBS MENU\n";
    std::cout << "================================\n";

    player->GetJobSystem().DisplayAllJobs();

    std::cout << "\nWhat would you like to do?\n";
    std::cout << "1. Work at Mining\n";
    std::cout << "2. Work at Lumberjacking\n";
    std::cout << "3. Work at Fishing\n";
    std::cout << "4. Work at Smithing\n";
    std::cout << "5. Return to exploration\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            player->GetJobSystem().WorkJob(JobType::Mining, 3);
            break;
        case 2:
            player->GetJobSystem().WorkJob(JobType::Lumberjacking, 3);
            break;
        case 3:
            player->GetJobSystem().WorkJob(JobType::Fishing, 3);
            break;
        case 4:
            player->GetJobSystem().WorkJob(JobType::Smithing, 3);
            break;
        case 5:
            currentState = GameState::Exploring;
            return;
        default:
            std::cout << "Invalid choice!\n";
            break;
    }

    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore();
    std::cin.get();

    currentState = GameState::Exploring;
}

void Game::EnemySelectionMenu()
{
    if (!player || !player->IsAlive())
    {
        currentState = GameState::Exploring;
        return;
    }

    ClearScreen();
    std::cout << "================================\n";
    std::cout << "      CHOOSE YOUR OPPONENT\n";
    std::cout << "================================\n";
    std::cout << "\nSelect an enemy to fight:\n";
    std::cout << "1. Goblin (Weak)\n";
    std::cout << "2. Wolf (Medium)\n";
    std::cout << "3. Orc (Strong)\n";
    std::cout << "4. Dragon (Very Strong)\n";
    std::cout << "5. Back to exploration\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    std::shared_ptr<Monster> enemy;

    switch (choice)
    {
        case 1:
            enemy = EnemyDatabase::CreateGoblin();
            break;
        case 2:
            enemy = EnemyDatabase::CreateWolf();
            break;
        case 3:
            enemy = EnemyDatabase::CreateOrc();
            break;
        case 4:
            enemy = EnemyDatabase::CreateDragon();
            break;
        case 5:
            currentState = GameState::Exploring;
            return;
        default:
            std::cout << "Invalid choice!\n";
            currentState = GameState::Exploring;
            return;
    }

    if (enemy)
    {
        enemy->GetSkills().AddSkill(std::make_shared<CommonAttack>());
        StartCombatWithEnemy(enemy);
    }
    else
    {
        currentState = GameState::Exploring;
    }
}

void Game::StartRandomEncounter()
{
    if (!player || !player->IsAlive())
    {
        return;
    }

    int difficulty = 1 + (player->GetLevel() - 1) / 5;
    CreateTestMonster(difficulty);
}

void Game::CreateTestMonster(int difficulty)
{
    // Create a test monster with stats based on difficulty
    Stats monsterStats(30 + difficulty * 20, 10 + difficulty * 5, 5 + difficulty * 2, 4 + difficulty, 3 + difficulty, 3 + difficulty, 4 + difficulty, 2 + difficulty);

    auto monster = std::make_shared<Monster>("Goblin", monsterStats, 50 * difficulty, 25 * difficulty);

    // Add basic attack skill to monster
    monster->GetSkills().AddSkill(std::make_shared<CommonAttack>());

    StartCombatWithEnemy(monster);
}

void Game::StartCombatWithEnemy(std::shared_ptr<Monster> enemy)
{
    if (!player || !enemy || !player->IsAlive() || !enemy->IsAlive())
    {
        currentState = GameState::Exploring;
        return;
    }

    if (combatSystem->StartCombat(player, enemy))
    {
        // Victory
        player->GainXP(enemy->GetExperienceReward());
        player->GetInventory().AddGold(enemy->GetGoldReward());
        std::cout << "You gained " << enemy->GetExperienceReward() << " XP and " << enemy->GetGoldReward() << " gold!\n";

        // Handle loot drops
        DisplayLootReward(enemy);
    }
    else
    {
        // Defeat
        std::cout << "You were defeated...\n";
        player.reset();
    }

    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore();
    std::cin.get();

    currentState = GameState::Exploring;
}

void Game::DisplayLootReward(std::shared_ptr<Monster> enemy)
{
    if (!player || !enemy)
        return;

    int difficulty = 1 + (player->GetLevel() - 1) / 5;
    auto loot = LootTable::GenerateLoot(difficulty);

    if (loot.empty())
    {
        std::cout << "\nNo loot dropped.\n";
        return;
    }

    std::cout << "\n=== LOOT DROPS ===" << std::endl;
    for (size_t i = 0; i < loot.size(); ++i)
    {
        auto item = loot[i];
        std::cout << "* " << item->name << " (Rarity: " << item->rarity << ")\n";

        // Try to add to inventory
        if (player->GetInventory().AddItem(item))
        {
            std::cout << "  → Added to inventory\n";
        }
        else
        {
            std::cout << "  → Inventory full! Item not taken.\n";
        }
    }
}

void Game::ClearScreen()
{
    std::system("clear || cls");
}

void Game::DisplayWelcome()
{
    std::cout << "================================\n";
    std::cout << "       Welcome to Text RPG\n";
    std::cout << "================================\n";
}
