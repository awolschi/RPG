#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <vector>
#include <memory>
#include <functional>
#include "../../Characters/Monster.hpp"

struct EnemyEntry
{
    std::string name;
    std::function<std::shared_ptr<Monster>()> factory;
    int weight; // higher = more common
};

class EnemyDatabase
{
public:
    // ---- Felrona area (difficulty 1) ----
    static std::shared_ptr<Monster> CreateSlime();
    static std::shared_ptr<Monster> CreateRat();
    static std::shared_ptr<Monster> CreateBandit();
    static std::shared_ptr<Monster> CreateAngryChicken();

    // ---- Dark Woods area (difficulty 2-3) ----
    static std::shared_ptr<Monster> CreateWolf();
    static std::shared_ptr<Monster> CreateGiantSpider();
    static std::shared_ptr<Monster> CreateTreant();
    static std::shared_ptr<Monster> CreateWraith();
    static std::shared_ptr<Monster> CreateDarkElfScout();
    static std::shared_ptr<Monster> CreateFeralBoar();

    // ---- Salty Siren Coast area (difficulty 3) ----
    static std::shared_ptr<Monster> CreatePirate();
    static std::shared_ptr<Monster> CreateSeaSerpent();
    static std::shared_ptr<Monster> CreateGiantCrab();
    static std::shared_ptr<Monster> CreateSiren();

    // ---- Dragon's Peak area (difficulty 4) ----
    static std::shared_ptr<Monster> CreateOrc();
    static std::shared_ptr<Monster> CreateHarpy();
    static std::shared_ptr<Monster> CreateFireElemental();
    static std::shared_ptr<Monster> CreateDrake();
    static std::shared_ptr<Monster> CreateStoneGolem();

    // ---- Aran Highlands area (difficulty 5) ----
    static std::shared_ptr<Monster> CreateHighlandWarrior();
    static std::shared_ptr<Monster> CreateGryphon();
    static std::shared_ptr<Monster> CreateEarthElemental();
    static std::shared_ptr<Monster> CreatePhantom();

    // ---- Shadow Dungeon area (difficulty 6) ----
    static std::shared_ptr<Monster> CreateShadow();
    static std::shared_ptr<Monster> CreateLich();
    static std::shared_ptr<Monster> CreateDemon();
    static std::shared_ptr<Monster> CreateVoidKnight();
    static std::shared_ptr<Monster> CreateBoneGolem();

    // ---- Celestial Spire (difficulty 7) ----
    static std::shared_ptr<Monster> CreateCelestialKnight();
    static std::shared_ptr<Monster> CreateSeraphim();
    static std::shared_ptr<Monster> CreateLightElemental();
    static std::shared_ptr<Monster> CreateAstralMage();

    // ---- Void Beyond (difficulty 8) ----
    static std::shared_ptr<Monster> CreateVoidWalker();
    static std::shared_ptr<Monster> CreateCosmicHorror();
    static std::shared_ptr<Monster> CreateNetherDrake();
    static std::shared_ptr<Monster> CreateStarBeast();

    // ---- Arcane Sanctum (difficulty 9) ----
    static std::shared_ptr<Monster> CreateArcaneGuardian();
    static std::shared_ptr<Monster> CreateManaWisp();
    static std::shared_ptr<Monster> CreateCrystalElemental();
    static std::shared_ptr<Monster> CreateSpellblade();

    // ---- Chronos Depths (difficulty 10) ----
    static std::shared_ptr<Monster> CreateTimeWeaver();
    static std::shared_ptr<Monster> CreateChronoBeast();
    static std::shared_ptr<Monster> CreateTemporalKnight();
    static std::shared_ptr<Monster> CreateVoidPhoenix();

    // ---- Boss monsters ----
    static std::shared_ptr<Monster> CreateFarmOverseer();
    static std::shared_ptr<Monster> CreateTreantKing();
    static std::shared_ptr<Monster> CreateKraken();
    static std::shared_ptr<Monster> CreateElderDragon();
    static std::shared_ptr<Monster> CreateAranWarlord();
    static std::shared_ptr<Monster> CreateVoidLord();
    static std::shared_ptr<Monster> CreateSeraphimCouncil();
    static std::shared_ptr<Monster> CreatePrimordialOne();
    static std::shared_ptr<Monster> CreateArcaneConstruct();
    static std::shared_ptr<Monster> CreateChronos();
    static std::shared_ptr<Monster> CreateDragon(); // keep original

    // ---- Forbidden Citadel bosses (difficulty 11) ----
    static std::shared_ptr<Monster> CreateAbyssalSentinel();
    static std::shared_ptr<Monster> CreateVoidEmpress();
    static std::shared_ptr<Monster> CreateInfernalColossus();
    static std::shared_ptr<Monster> CreateGlacialWraith();
    static std::shared_ptr<Monster> CreateStormArbiter();
    static std::shared_ptr<Monster> CreatePlagueSovereign();
    static std::shared_ptr<Monster> CreateCitadelHolyArbiter();
    static std::shared_ptr<Monster> CreateChronoOverlord();
    static std::shared_ptr<Monster> CreateVoidMonarch();
    static std::shared_ptr<Monster> CreateTheUnbroken();

    // Get weighted enemy pool for a given difficulty tier
    static const std::vector<EnemyEntry>& GetPool(int difficulty);
};

#endif
