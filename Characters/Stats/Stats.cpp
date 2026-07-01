class Stats
{
public:
    int maxHealth = 0;
    int maxMana = 0;
    int strength = 0;
    int vitality = 0;
    int intelligence = 0;
    int wisdom = 0;
    int dexterity = 0;
    int defense = 0;

    Stats& operator+=(const Stats& other)
    {
        maxHealth += other.maxHealth;
        maxMana += other.maxMana;
        strength += other.strength;
        vitality += other.vitality;
        intelligence += other.intelligence;
        wisdom += other.wisdom;
        dexterity += other.dexterity;
        defense += other.defense;
        return *this;
    }
};