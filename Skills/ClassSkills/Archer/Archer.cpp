#include "Archer.hpp"
#include "../../../Characters/Character.hpp"

PiercingShot::PiercingShot() : Skill("Piercing Shot", 10, 1, 40)
{
}

void PiercingShot::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().dexterity / 2);
    target.TakeDamage(damage);
    GainXP(1);
    ResetCooldown();
}

MultiShot::MultiShot() : Skill("Multi Shot", 20, 3, 30)
{
}

void MultiShot::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().dexterity / 3);
    target.TakeDamage(damage);
    target.TakeDamage(damage / 2);
    GainXP(2);
    ResetCooldown();
}
