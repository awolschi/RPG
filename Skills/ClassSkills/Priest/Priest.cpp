#include "Priest.hpp"
#include "../../../Characters/Character.hpp"

HolySmite::HolySmite() : Skill("Holy Smite", 20, 2, 45)
{
}

void HolySmite::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().wisdom / 2);
    target.TakeDamage(damage);
    GainXP(1);
    ResetCooldown();
}

Heal::Heal() : Skill("Heal", 25, 1, 0)
{
}

void Heal::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int healAmount = 40 + (caster.GetStats().wisdom / 2);
    target.RestoreHealth(healAmount);
    GainXP(1);
    ResetCooldown();
}

MassHeal::MassHeal() : Skill("Mass Heal", 50, 3, 0)
{
}

void MassHeal::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int healAmount = 60 + (caster.GetStats().wisdom);
    target.RestoreHealth(healAmount);
    GainXP(2);
    ResetCooldown();
}
