#ifndef SKILL_HPP
#define SKILL_HPP

#include <string>

class Character;

class Skill
{
public:
    Skill(const std::string& name = "", int manaCost = 0, int cooldown = 0, int baseDamage = 0)
        : name(name), manaCost(manaCost), cooldown(cooldown), currentCooldown(0), baseDamage(baseDamage), level(1), experience(0)
    {
    }

    virtual ~Skill() = default;

    virtual void Use(Character& caster, Character& target);

    void ReduceCooldown();
    bool IsReady() const;
    void ResetCooldown();

    // Skill progression
    void GainXP(int xp);
    void LevelUp();

    std::string name;
    int manaCost;
    int cooldown;
    int currentCooldown;
    int baseDamage;
    int level;
    int experience;
    static const int XP_TO_LEVEL;
};

#endif
