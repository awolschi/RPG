#include "Mage.hpp"
#include "../../../Characters/Character.hpp"

Fireball::Fireball() : Skill("Fireball", 15, 2, 50)
{
}

void Fireball::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().intelligence / 2);
    target.TakeDamage(damage);
    GainXP(1);
    ResetCooldown();
}

IceBolt::IceBolt() : Skill("Ice Bolt", 12, 1, 35)
{
}

void IceBolt::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().intelligence / 3);
    target.TakeDamage(damage);
    GainXP(1);
    ResetCooldown();
}

Meteor::Meteor() : Skill("Meteor", 40, 4, 80)
{
}

void Meteor::Use(Character& caster, Character& target)
{
    if (!IsReady())
        return;

    int damage = baseDamage + (caster.GetStats().intelligence);
    target.TakeDamage(damage);
    GainXP(3);
    ResetCooldown();
}
