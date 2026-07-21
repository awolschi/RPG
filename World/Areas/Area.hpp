#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../../Characters/Monster.hpp"

struct DungeonRoom
{
    std::string description;
    int minEnemies;
    int maxEnemies;
};

struct Dungeon
{
    std::string name;
    std::string description;
    std::vector<DungeonRoom> rooms;
    std::function<std::shared_ptr<Monster>()> bossFactory;
};

class Area
{
public:
    Area(const std::string& name, const std::string& description, int difficulty,
         const std::vector<Dungeon>& dungeons = {},
         const std::vector<int>& connections = {});

    std::string name;
    std::string description;
    int difficulty;
    std::vector<Dungeon> dungeons;
    std::vector<int> connections; // indices of adjacent areas

    std::vector<std::shared_ptr<Monster>> spawnMonsters();
    std::shared_ptr<Monster> spawnBoss();
    bool IsExplored() const { return explored; }
    void SetExplored(bool value) { explored = value; }

private:
    bool explored;
};

#endif
