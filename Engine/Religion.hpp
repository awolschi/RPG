#ifndef RELIGION_HPP
#define RELIGION_HPP

#include <string>
#include <memory>

class Character;

enum class GodType
{
    None,
    Shaim,
    Karosh,
    Amala,
    Tordo
};

struct GodData
{
    std::string name;
    std::string description;
    int devotionLevel;
    int totalDonated;
};

class ReligionSystem
{
public:
    ReligionSystem();

    void SetGod(GodType god);
    GodType GetGod() const { return currentGod; }
    int GetDevotionLevel() const { return devotionLevel; }
    int GetTotalDonated() const { return totalDonated; }

    bool Donate(int amount);
    std::string GetGodName() const;
    std::string GetGodDescription() const;
    int GetNextDevotionCost() const;
    void ApplyDevotionBonus(std::shared_ptr<Character> player) const;
    void RestoreState(GodType god, int devotion, int donated);

private:
    GodType currentGod;
    int devotionLevel;
    int totalDonated;
};

#endif
