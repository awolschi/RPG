#include "Area.hpp"
#include "../Enemies/Enemies.hpp"
#include <cstdlib>

Area::Area(const std::string& name, const std::string& description,
           int difficulty, const std::vector<Dungeon>& dungeons)
    : name(name), description(description),
      difficulty(difficulty), dungeons(dungeons), explored(false)
{
}

std::vector<std::shared_ptr<Monster>> Area::spawnMonsters()
{
    std::vector<std::shared_ptr<Monster>> monsters;
    const auto& pool = EnemyDatabase::GetPool(difficulty);
    if (pool.empty())
        return monsters;

    int count = 1 + (rand() % 3);
    for (int i = 0; i < count; ++i)
    {
        // Weighted random pick
        int totalWeight = 0;
        for (const auto& e : pool)
            totalWeight += e.weight;

        int roll = rand() % totalWeight;
        int cumulative = 0;
        for (const auto& e : pool)
        {
            cumulative += e.weight;
            if (roll < cumulative)
            {
                auto m = e.factory();
                if (m)
                {
                    m->ScaleStats(difficulty);
                    monsters.push_back(m);
                }
                break;
            }
        }
    }
    return monsters;
}

std::shared_ptr<Monster> Area::spawnBoss()
{
    // Use the area's dungeon boss if available, otherwise scaled generic boss
    return nullptr; // handled per-dungeon via bossFactory
}
