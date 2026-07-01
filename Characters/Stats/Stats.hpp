#ifndef STATS_HPP
#define STATS_HPP

struct Stats
{
    int health;
    int mana;
    int strength;
    int vitality;
    int intelligence;
    int wisdom;
    int dexterity;
    int defense;

    // Default constructor
    Stats(int h = 0, int m = 0, int s = 0, int v = 0, int i = 0, int w = 0, int d = 0, int def = 0)
        : health(h), mana(m), strength(s), vitality(v), intelligence(i), wisdom(w), dexterity(d), defense(def)
    {
    }

    // Operator overload for adding stats
    Stats& operator+=(const Stats& other)
    {
        health += other.health;
        mana += other.mana;
        strength += other.strength;
        vitality += other.vitality;
        intelligence += other.intelligence;
        wisdom += other.wisdom;
        dexterity += other.dexterity;
        defense += other.defense;
        return *this;
    }

    Stats operator+(const Stats& other) const
    {
        Stats result = *this;
        result += other;
        return result;
    }
};

#endif
