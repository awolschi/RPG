#include "Area.hpp"

Area::Area(const std::string& name, const std::string& description, int difficulty)
    : name(name), description(description), difficulty(difficulty), explored(false)
{
}

std::vector<std::shared_ptr<Monster>> Area::spawnMonsters()
{
    // TODO: Implement monster spawning based on area difficulty
    std::vector<std::shared_ptr<Monster>> monsters;
    return monsters;
}
