#include "Races.hpp"

RaceData RaceDatabase::Get(CharacterRace race)
{
    switch (race)
    {
        case CharacterRace::Thull:
            return {"Thull", "A race known for strength and endurance.", {0, 0, 2, 2, -1, 0, -1, 1}};

        case CharacterRace::Mekkel:
            return {"Mekkel", "A people of nomads and bards, quick and wise.", {0, 0, -1, 0, 0, 2, 2, -1}};

        case CharacterRace::Aran:
            return {"Aran", "Strong-willed masters of the sword.", {0, 0, 2, 0, 1, 0, 1, 0}};

        case CharacterRace::Pesha:
            return {"Pesha", "Honorable peace-lovers attuned to nature.", {0, 0, 0, 1, 0, 2, 0, 1}};

        case CharacterRace::Toran:
            return {"Toran", "Disciplined warriors with iron wills.", {0, 0, 1, 2, 0, -1, 0, 2}};

        default:
            return {"Unknown", "No data.", {}};
    }
}
