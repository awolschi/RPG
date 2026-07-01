#include "Classes.hpp"

ClassData ClassDatabase::Get(CharacterClass c)
{
    switch(c)
    {
        case CharacterClass::Warrior:
            return
            {
                {
                    150, 20,
                    12, 10, 2, 2, 5, 8
                },
                {
                    15, 2,
                    3, 2, 0, 0, 1, 2
                }
            };

        case CharacterClass::Priest:
            return
            {
                {
                    90, 120,
                    2, 4, 8, 12, 5, 3
                },
                {
                    8, 8,
                    0, 1, 2, 3, 0, 1
                }
            };

        case CharacterClass::Mage:
            return
            {
                {
                    80, 150,
                    1, 3, 14, 8, 4, 2
                },
                {
                    5, 10,
                    0, 1, 4, 2, 0, 0
                }
            };

        case CharacterClass::Archer:
            return
            {
                {
                    100, 100,
                    5, 8, 4, 6, 3, 4
                },
                {
                    10, 5,
                    2, 3, 1, 2, 1, 1
                }
            };

        default:
            return {};
    }
}