#include "Skill.hpp"
#include "../Characters/Character.hpp"

void Skill::Use(Character& caster, Character& target)
{
}

void Skill::ReduceCooldown()
{
    if (currentCooldown > 0)
        currentCooldown--;
}

bool Skill::IsReady() const
{
    return currentCooldown == 0;
}

void Skill::ResetCooldown()
{
    currentCooldown = cooldown;
}

int Skill::GetXPToLevel(int currentLevel)
{
    return currentLevel * 80;
}

void Skill::GainXP(int xp)
{
    experience += xp;
    int needed = GetXPToLevel(level);
    while (experience >= needed)
    {
        experience -= needed;
        LevelUp();
        needed = GetXPToLevel(level);
        if (level >= 50) { experience = 0; break; }
    }
}

void Skill::LevelUp()
{
    level++;
    baseDamage = (int)(baseDamage * 1.12f) + 1;
    if (level % 5 == 0 && cooldown > 1)
        cooldown--;
}
