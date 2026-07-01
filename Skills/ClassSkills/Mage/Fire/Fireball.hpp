#include "../../Skill.hpp"

class Fireball : public Skill
{
public:

    Fireball();

    void Use(Character& caster,
             Character& target) override;
};