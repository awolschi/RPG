#ifndef MAGE_HPP
#define MAGE_HPP

#include "../../Skill.hpp"

// === Level 1: Starter spells ===
class Fireball : public Skill
{
public:
    Fireball();
    void Use(Character& caster, Character& target) override;
};

class IceBolt : public Skill
{
public:
    IceBolt();
    void Use(Character& caster, Character& target) override;
};

// === Level 2 ===
class ArcaneBolt : public Skill
{
public:
    ArcaneBolt();
    void Use(Character& caster, Character& target) override;
};

// === Level 3 ===
class Meteor : public Skill
{
public:
    Meteor();
    void Use(Character& caster, Character& target) override;
};

// === Level 4 ===
class FrostWard : public Skill
{
public:
    FrostWard();
    void Use(Character& caster, Character& target) override;
};

// === Level 5 ===
class ArcaneSurge : public Skill
{
public:
    ArcaneSurge();
    void Use(Character& caster, Character& target) override;
};

// === Level 6 ===
class ManaSiphon : public Skill
{
public:
    ManaSiphon();
    void Use(Character& caster, Character& target) override;
};

// === Level 8 ===
class ChainLightning : public Skill
{
public:
    ChainLightning();
    void Use(Character& caster, Character& target) override;
};

// === Level 10 ===
class LightningBolt : public Skill
{
public:
    LightningBolt();
    void Use(Character& caster, Character& target) override;
};

// === Level 12 ===
class ArcaneMissiles : public Skill
{
public:
    ArcaneMissiles();
    void Use(Character& caster, Character& target) override;
};

// === Level 14 ===
class FrostShield : public Skill
{
public:
    FrostShield();
    void Use(Character& caster, Character& target) override;
};

// === Level 16 ===
class ManaBomb : public Skill
{
public:
    ManaBomb();
    void Use(Character& caster, Character& target) override;
};

// === Level 18 ===
class BlazingOrb : public Skill
{
public:
    BlazingOrb();
    void Use(Character& caster, Character& target) override;
};

// === Level 20 ===
class FrostNova : public Skill
{
public:
    FrostNova();
    void Use(Character& caster, Character& target) override;
};

// === Level 22 ===
class ArcaneBarrage : public Skill
{
public:
    ArcaneBarrage();
    void Use(Character& caster, Character& target) override;
};

// === Level 26 ===
class TimeWarp : public Skill
{
public:
    TimeWarp();
    void Use(Character& caster, Character& target) override;
};

// === Level 28 ===
class GlacialSpike : public Skill
{
public:
    GlacialSpike();
    void Use(Character& caster, Character& target) override;
};

// === Level 30 ===
class ManaShield : public Skill
{
public:
    ManaShield();
    void Use(Character& caster, Character& target) override;
};

// === Level 32 ===
class Pyroblast : public Skill
{
public:
    Pyroblast();
    void Use(Character& caster, Character& target) override;
};

// === Level 34 ===
class Blizzard : public Skill
{
public:
    Blizzard();
    void Use(Character& caster, Character& target) override;
};

// === Level 36 ===
class ArcanePower : public Skill
{
public:
    ArcanePower();
    void Use(Character& caster, Character& target) override;
};

// === Level 38 ===
class IceLance : public Skill
{
public:
    IceLance();
    void Use(Character& caster, Character& target) override;
};

// === Level 40 ===
class Inferno : public Skill
{
public:
    Inferno();
    void Use(Character& caster, Character& target) override;
};

// === Level 44 ===
class ElementalFury : public Skill
{
public:
    ElementalFury();
    void Use(Character& caster, Character& target) override;
};

// === Level 50 ===
class ArchmageCataclysm : public Skill
{
public:
    ArchmageCataclysm();
    void Use(Character& caster, Character& target) override;
};

#endif
