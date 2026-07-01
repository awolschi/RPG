#include "Warrior.hpp"
#include "../../../Characters/Character.hpp"

PowerStrike::PowerStrike() : Skill("Power Strike", 15, 1, 60)
{
}

void PowerStrike::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().strength / 2);
    target.TakeDamage(damage);
    GainXP(1);
    ResetCooldown();
}

Whirlwind::Whirlwind() : Skill("Whirlwind", 30, 3, 50)
{
}

void Whirlwind::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().strength / 3);
    target.TakeDamage(damage);
    target.TakeDamage(damage / 2);
    GainXP(2);
    ResetCooldown();
}

DefensiveStance::DefensiveStance() : Skill("Defensive Stance", 0, 2, 0)
{
}

void DefensiveStance::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    // Increases player's defense temporarily (simplified)
    caster.IncreaseTempDefense(10);
    GainXP(1);
    ResetCooldown();
}
