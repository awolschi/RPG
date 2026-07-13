#ifndef RACES_HPP
#define RACES_HPP

#include <string>
#include "../Stats/Stats.hpp"

enum class CharacterRace
{
    Thull,
    Mekkel,
    Aran,
    Pesha,
    Toran
};

struct RaceData
{
    std::string name;
    std::string description;
    Stats statModifier;
};

class RaceDatabase
{
public:
    static RaceData Get(CharacterRace race);
};

#endif
