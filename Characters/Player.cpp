#include "Player.hpp"
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Skills/ClassSkills/Warrior/Warrior.hpp"
#include "../Skills/ClassSkills/Mage/Mage.hpp"
#include "../Skills/ClassSkills/Priest/Priest.hpp"
#include "../Skills/ClassSkills/Archer/Archer.hpp"

Player::Player(const std::string& name, CharacterClass characterClass)
    : Character(name, ClassDatabase::Get(characterClass).startingStats),
      characterClass(characterClass),
      inventory(20)
{
    // Add common attack to all players
    skills.AddSkill(std::make_shared<CommonAttack>());

    // Add class-specific skills
    InitializeClassSkills();

    // Initialize all jobs
    jobSystem.AddJob(JobType::Mining);
    jobSystem.AddJob(JobType::Lumberjacking);
    jobSystem.AddJob(JobType::Fishing);
    jobSystem.AddJob(JobType::Smithing);
}

void Player::LevelUp()
{
    Character::LevelUp();

    // Add class-specific stat gains
    ClassData classData = ClassDatabase::Get(characterClass);
    stats += classData.levelUpGain;

    // Restore health and mana on level up
    currentHealth = stats.health;
    currentMana = stats.mana;
}

void Player::InitializeClassSkills()
{
    switch (characterClass)
    {
        case CharacterClass::Warrior:
            skills.AddSkill(std::make_shared<PowerStrike>());
            skills.AddSkill(std::make_shared<Whirlwind>());
            skills.AddSkill(std::make_shared<DefensiveStance>());
            break;

        case CharacterClass::Mage:
            skills.AddSkill(std::make_shared<Fireball>());
            skills.AddSkill(std::make_shared<IceBolt>());
            skills.AddSkill(std::make_shared<Meteor>());
            break;

        case CharacterClass::Priest:
            skills.AddSkill(std::make_shared<HolySmite>());
            skills.AddSkill(std::make_shared<Heal>());
            skills.AddSkill(std::make_shared<MassHeal>());
            break;

        case CharacterClass::Archer:
            skills.AddSkill(std::make_shared<PiercingShot>());
            skills.AddSkill(std::make_shared<MultiShot>());
            break;

        default:
            break;
    }
}
