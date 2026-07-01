#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <vector>
#include <memory>
#include "../../Characters/Monster.hpp"

class EnemyDatabase
{
public:
    static std::shared_ptr<Monster> CreateGoblin();
    static std::shared_ptr<Monster> CreateOrc();
    static std::shared_ptr<Monster> CreateWolf();
    static std::shared_ptr<Monster> CreateDragon();
};

#endif
