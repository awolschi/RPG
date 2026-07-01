#include "SaveGame.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

SaveGameManager::SaveGameManager()
{
    saveDirectory = "./saves/";

    // Create saves directory if it doesn't exist
    if (!std::filesystem::exists(saveDirectory))
    {
        try
        {
            std::filesystem::create_directory(saveDirectory);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create save directory: " << e.what() << std::endl;
        }
    }
}

bool SaveGameManager::SaveGame(const std::shared_ptr<Player>& player, const std::string& filename)
{
    if (!player)
    {
        return false;
    }

    std::string filepath = saveDirectory + filename + ".sav";

    try
    {
        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        SerializePlayer(player, file);
        file.close();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Player> SaveGameManager::LoadGame(const std::string& filename)
{
    std::string filepath = saveDirectory + filename + ".sav";

    try
    {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open())
        {
            return nullptr;
        }

        auto player = DeserializePlayer(file);
        file.close();
        return player;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return nullptr;
    }
}

bool SaveGameManager::GameSaveExists(const std::string& filename)
{
    std::string filepath = saveDirectory + filename + ".sav";
    return std::filesystem::exists(filepath);
}

std::string SaveGameManager::GetSaveDirectory() const
{
    return saveDirectory;
}

void SaveGameManager::SerializePlayer(const std::shared_ptr<Player>& player, std::ofstream& file)
{
    if (!player)
        return;

    // TODO: Implement serialization logic for player data
    // This is a skeleton implementation - full serialization would include:
    // - Player name and class
    // - Stats and current health/mana
    // - Level and experience
    // - Inventory items
    // - Equipment
    // - Skills and their progression
}

std::shared_ptr<Player> SaveGameManager::DeserializePlayer(std::ifstream& file)
{
    // TODO: Implement deserialization logic
    // This is a skeleton implementation - full deserialization would reconstruct the player from saved data
    return nullptr;
}
