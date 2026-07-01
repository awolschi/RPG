#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <memory>
#include "../../Characters/Monster.hpp"

class Area
{
public:
    Area(const std::string& name, const std::string& description, int difficulty);

    std::string name;
    std::string description;
    int difficulty;

    std::vector<std::shared_ptr<Monster>> spawnMonsters();
    bool IsExplored() const { return explored; }
    void SetExplored(bool value) { explored = value; }

private:
    bool explored;
};

#endif
