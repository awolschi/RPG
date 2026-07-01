#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include <string>
#include <memory>
#include "../Characters/Player.hpp"

class SaveGameManager
{
public:
    SaveGameManager();

    // Save and load functionality
    bool SaveGame(const std::shared_ptr<Player>& player, const std::string& filename);
    std::shared_ptr<Player> LoadGame(const std::string& filename);

    // Utility
    bool GameSaveExists(const std::string& filename);
    std::string GetSaveDirectory() const;

private:
    std::string saveDirectory;

    // Helper functions for serialization
    void SerializePlayer(const std::shared_ptr<Player>& player, std::ofstream& file);
    std::shared_ptr<Player> DeserializePlayer(std::ifstream& file);
};

#endif
