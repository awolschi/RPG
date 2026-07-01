#include "Skill.hpp"
#include "../Characters/Character.hpp"

const int Skill::XP_TO_LEVEL = 100;

void Skill::Use(Character& caster, Character& target)
{
    // Base skill does nothing - overridden by subclasses
}

void Skill::ReduceCooldown()
{
    if (currentCooldown > 0)
    {
        currentCooldown--;
    }
}

bool Skill::IsReady() const
{
    return currentCooldown == 0;
}

void Skill::ResetCooldown()
{
    currentCooldown = cooldown;
}

void Skill::GainXP(int xp)
{
    experience += xp;
    if (experience >= XP_TO_LEVEL)
    {
        LevelUp();
    }
}

void Skill::LevelUp()
{
    level++;
    experience = 0;
    // Increase effectiveness with level
    baseDamage += (baseDamage / 10) + 1;
    manaCost += (manaCost / 20);
}
