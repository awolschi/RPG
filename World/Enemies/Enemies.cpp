#include "Enemies.hpp"

std::shared_ptr<Monster> EnemyDatabase::CreateGoblin()
{
    Stats stats(30, 10, 5, 4, 3, 3, 4, 2);
    return std::make_shared<Monster>("Goblin", stats, 50, 25);
}

std::shared_ptr<Monster> EnemyDatabase::CreateOrc()
{
    Stats stats(50, 15, 10, 8, 4, 4, 5, 5);
    return std::make_shared<Monster>("Orc", stats, 100, 50);
}

std::shared_ptr<Monster> EnemyDatabase::CreateWolf()
{
    Stats stats(40, 5, 8, 6, 2, 3, 8, 3);
    return std::make_shared<Monster>("Wolf", stats, 75, 35);
}

std::shared_ptr<Monster> EnemyDatabase::CreateDragon()
{
    Stats stats(200, 100, 30, 20, 25, 20, 15, 20);
    return std::make_shared<Monster>("Dragon", stats, 500, 500);
}