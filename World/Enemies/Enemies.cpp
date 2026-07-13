#include "Enemies.hpp"
#include <cstdlib>
#include "../../Characters/Monster.hpp"

// ============================================================
//  FELRONA
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateSlime()
{
    Stats s(30, 2, 3, 2, 1, 1, 2, 1);
    auto m = std::make_shared<Monster>("Slime", s, 25, 8);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Fire, 0.75f);  // Slimes weak to fire
    m->SetResistance(ElementType::Ice, 1.25f);   // Resists ice
    m->SetDropTier(1);
    m->AddSpecialDrop("Slime Gel", 40, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateRat()
{
    Stats s(22, 0, 4, 3, 1, 1, 6, 1);
    auto m = std::make_shared<Monster>("Rat", s, 20, 5);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Physical, 0.75f);   // Small and hard to hit
    m->SetResistance(ElementType::Poison, 1.5f);      // Weak to poison
    m->SetDropTier(1);
    m->AddSpecialDrop("Rat Tail", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateBandit()
{
    Stats s(50, 5, 7, 5, 2, 2, 5, 3);
    auto m = std::make_shared<Monster>("Bandit", s, 60, 30);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Physical, 0.75f);   // Armored
    m->SetResistance(ElementType::Holy, 1.25f);       // Evil-doers weak to holy
    m->SetDropTier(1);
    m->AddSpecialDrop("Bandit Mask", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateAngryChicken()
{
    Stats s(15, 0, 2, 1, 1, 1, 8, 0);
    auto m = std::make_shared<Monster>("Angry Chicken", s, 15, 3);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Fire, 1.5f);        // Feathered, weak to fire
    m->SetResistance(ElementType::Physical, 1.25f);   // Fragile
    m->SetDropTier(1);
    m->AddSpecialDrop("Feather", 50, 1, 3);
    return m;
}

// ============================================================
//  DARK WOODS
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateWolf()
{
    Stats s(55, 5, 8, 6, 2, 3, 8, 3);
    auto m = std::make_shared<Monster>("Wolf", s, 75, 35);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Ice, 0.75f);        // Thick fur resists cold
    m->SetResistance(ElementType::Fire, 1.5f);        // Weak to fire (fur)
    m->SetDropTier(2);
    m->AddSpecialDrop("Wolf Pelt", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateGiantSpider()
{
    Stats s(45, 10, 9, 5, 3, 4, 7, 4);
    auto m = std::make_shared<Monster>("Giant Spider", s, 80, 30);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Ice, 0.75f);   // Spiders weak to ice
    m->SetResistance(ElementType::Fire, 1.25f);  // Resists fire
    m->SetDropTier(2);
    m->AddSpecialDrop("Spider Silk", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateTreant()
{
    Stats s(100, 20, 12, 10, 5, 6, 3, 8);
    auto m = std::make_shared<Monster>("Treant", s, 120, 60);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Fire, 1.5f);   // Trees weak to fire
    m->SetResistance(ElementType::Ice, 0.75f);   // Resists ice
    m->SetDropTier(2);
    m->AddSpecialDrop("Ancient Bark", 45, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateWraith()
{
    Stats s(40, 30, 6, 5, 8, 10, 10, 2);
    auto m = std::make_shared<Monster>("Wraith", s, 110, 40);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Lightning, 0.5f);  // Wraiths weak to lightning
    m->SetResistance(ElementType::Physical, 0.75f);   // Resists physical
    m->SetDropTier(2);
    m->AddSpecialDrop("Ectoplasm", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateDarkElfScout()
{
    Stats s(60, 15, 10, 7, 6, 5, 9, 5);
    auto m = std::make_shared<Monster>("Dark Elf Scout", s, 100, 55);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Fire, 1.25f);  // Dark elves resist fire
    m->SetDropTier(2);
    m->AddSpecialDrop("Elf Arrow", 30, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateFeralBoar()
{
    Stats s(70, 0, 11, 8, 2, 2, 5, 6);
    auto m = std::make_shared<Monster>("Feral Boar", s, 90, 40);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Physical, 0.75f);   // Tough hide
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(2);
    m->AddSpecialDrop("Boar Tusk", 40, 1, 2);
    return m;
}

// ============================================================
//  SALTY SIREN COAST
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreatePirate()
{
    Stats s(50, 10, 10, 7, 4, 3, 6, 5);
    auto m = std::make_shared<Monster>("Pirate", s, 110, 80);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Physical, 0.75f);   // Armored
    m->SetResistance(ElementType::Lightning, 1.25f);  // Sea association, weak to lightning
    m->SetResistance(ElementType::Ice, 0.75f);        // Coastal, resists cold
    m->SetDropTier(3);
    m->AddSpecialDrop("Doubloon", 30, 1, 3);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateSeaSerpent()
{
    Stats s(70, 25, 14, 9, 7, 5, 8, 7);
    auto m = std::make_shared<Monster>("Sea Serpent", s, 150, 90);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Lightning, 1.5f);  // Sea creatures weak to lightning
    m->SetResistance(ElementType::Ice, 0.75f);       // Resists ice
    m->SetDropTier(3);
    m->AddSpecialDrop("Serpent Scale", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateGiantCrab()
{
    Stats s(60, 5, 8, 10, 3, 3, 4, 10);
    auto m = std::make_shared<Monster>("Giant Crab", s, 100, 60);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Lightning, 1.5f);  // Crabs weak to lightning
    m->SetResistance(ElementType::Physical, 0.75f);   // Hard shell resists physical
    m->SetDropTier(3);
    m->AddSpecialDrop("Crab Shell", 40, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateSiren()
{
    Stats s(40, 40, 7, 5, 10, 12, 9, 4);
    auto m = std::make_shared<Monster>("Siren", s, 140, 70);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Ice, 1.25f);       // Sirens resist ice
    m->SetResistance(ElementType::Lightning, 0.75f);  // Weak to lightning
    m->SetDropTier(3);
    m->AddSpecialDrop("Siren Crystal", 25, 1, 1);
    return m;
}

// ============================================================
//  DRAGON'S PEAK
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateOrc()
{
    Stats s(50, 15, 10, 8, 4, 4, 5, 5);
    auto m = std::make_shared<Monster>("Orc", s, 100, 50);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Poison, 0.75f);     // Tough constitution
    m->SetResistance(ElementType::Holy, 1.25f);       // Evil humanoid, weak to holy
    m->SetDropTier(4);
    m->AddSpecialDrop("Orc Tusk", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateHarpy()
{
    Stats s(40, 15, 9, 6, 5, 4, 12, 4);
    auto m = std::make_shared<Monster>("Harpy", s, 110, 55);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Lightning, 1.25f);  // Flying creatures resist lightning
    m->SetDropTier(4);
    m->AddSpecialDrop("Harpy Feather", 30, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateFireElemental()
{
    Stats s(55, 50, 15, 8, 10, 6, 7, 6);
    auto m = std::make_shared<Monster>("Fire Elemental", s, 160, 70);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Fire, 0.25f);       // Immune to fire
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetResistance(ElementType::Lightning, 0.75f);  // Resists lightning
    m->SetDropTier(4);
    m->AddSpecialDrop("Fire Essence", 40, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateDrake()
{
    Stats s(90, 30, 16, 12, 8, 6, 8, 10);
    auto m = std::make_shared<Monster>("Drake", s, 200, 110);
    m->SetAIType(EnemyAIType::Berserker);
    m->SetResistance(ElementType::Fire, 0.5f);        // Drakes resist fire
    m->SetResistance(ElementType::Ice, 1.25f);        // Weak to ice
    m->SetDropTier(4);
    m->AddSpecialDrop("Drake Scale", 35, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateStoneGolem()
{
    Stats s(120, 5, 14, 15, 3, 3, 3, 15);
    auto m = std::make_shared<Monster>("Stone Golem", s, 180, 80);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Physical, 0.5f);    // Stone resists physical
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(4);
    m->AddSpecialDrop("Golem Core", 50, 1, 1);
    return m;
}

// ============================================================
//  ARAN HIGHLANDS
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateHighlandWarrior()
{
    Stats s(70, 15, 14, 11, 6, 5, 7, 9);
    auto m = std::make_shared<Monster>("Highland Warrior", s, 170, 100);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Physical, 0.75f);   // Heavy armor
    m->SetResistance(ElementType::Ice, 0.75f);        // Highland dwelling, resists cold
    m->SetResistance(ElementType::Lightning, 1.25f);  // Metal armor conductive
    m->SetDropTier(5);
    m->AddSpecialDrop("Warrior Emblem", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateGryphon()
{
    Stats s(80, 20, 16, 10, 7, 7, 12, 8);
    auto m = std::make_shared<Monster>("Gryphon", s, 200, 110);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Lightning, 1.25f);  // Flying creatures resist lightning
    m->SetDropTier(5);
    m->AddSpecialDrop("Gryphon Plume", 35, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateEarthElemental()
{
    Stats s(130, 10, 12, 16, 6, 5, 4, 16);
    auto m = std::make_shared<Monster>("Earth Elemental", s, 220, 90);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Physical, 0.5f);    // Earth resists physical
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetDropTier(5);
    m->AddSpecialDrop("Earth Essence", 40, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreatePhantom()
{
    Stats s(50, 50, 11, 8, 12, 14, 11, 6);
    auto m = std::make_shared<Monster>("Phantom", s, 230, 120);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Physical, 0.5f);    // Phantoms resist physical
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(5);
    m->AddSpecialDrop("Phantom Shard", 30, 1, 1);
    return m;
}

// ============================================================
//  SHADOW DUNGEON
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateShadow()
{
    Stats s(60, 30, 13, 9, 9, 8, 13, 8);
    auto m = std::make_shared<Monster>("Shadow", s, 210, 100);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Physical, 0.5f);    // Shadows resist physical
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(6);
    m->AddSpecialDrop("Shadow Essence", 35, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateLich()
{
    Stats s(70, 100, 12, 10, 18, 16, 7, 10);
    auto m = std::make_shared<Monster>("Lich", s, 300, 150);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Ice, 0.5f);         // Liches resist ice
    m->SetResistance(ElementType::Fire, 1.5f);        // Weak to fire
    m->SetDropTier(6);
    m->AddSpecialDrop("Lich Phylactery", 25, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateDemon()
{
    Stats s(100, 50, 20, 14, 12, 10, 10, 14);
    auto m = std::make_shared<Monster>("Demon", s, 350, 200);
    m->SetAIType(EnemyAIType::Berserker);
    m->SetResistance(ElementType::Fire, 0.25f);       // Demons immune to fire
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetDropTier(6);
    m->AddSpecialDrop("Demon Horn", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateVoidKnight()
{
    Stats s(120, 40, 22, 16, 10, 10, 8, 16);
    auto m = std::make_shared<Monster>("Void Knight", s, 400, 250);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Void knights resist arcane
    m->SetDropTier(6);
    m->AddSpecialDrop("Void Crystal", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateBoneGolem()
{
    Stats s(150, 10, 18, 18, 5, 5, 5, 18);
    auto m = std::make_shared<Monster>("Bone Golem", s, 350, 180);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Physical, 0.5f);    // Bones resist physical
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(6);
    m->AddSpecialDrop("Bone Fragment", 40, 1, 1);
    return m;
}

// ============================================================
//  BOSSES
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateFarmOverseer()
{
    Stats s(120, 20, 15, 12, 6, 5, 7, 8);
    auto m = std::make_shared<Monster>("Farm Overseer", s, 300, 150, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Physical, 0.75f);   // Armored boss
    m->SetResistance(ElementType::Fire, 1.25f);       // Weak to fire
    m->SetDropTier(3);
    m->AddSpecialDrop("Farm Key", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateTreantKing()
{
    Stats s(250, 50, 22, 18, 10, 12, 6, 14);
    auto m = std::make_shared<Monster>("Treant King", s, 600, 300, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Fire, 1.5f);        // Trees weak to fire
    m->SetResistance(ElementType::Ice, 0.5f);         // Resists ice
    m->SetDropTier(4);
    m->AddSpecialDrop("Treant Heart", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateKraken()
{
    Stats s(300, 80, 25, 20, 14, 12, 10, 16);
    auto m = std::make_shared<Monster>("Kraken", s, 800, 500, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Lightning, 1.5f);   // Sea creature weak to lightning
    m->SetResistance(ElementType::Ice, 0.75f);        // Resists ice
    m->SetDropTier(5);
    m->AddSpecialDrop("Kraken Ink", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateElderDragon()
{
    // Original dragon is the boss of Dragon's Peak
    Stats s(200, 100, 30, 20, 25, 20, 15, 20);
    auto m = std::make_shared<Monster>("Elder Dragon", s, 500, 500, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Fire, 0.25f);       // Dragon immune to fire
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetDropTier(6);
    m->AddSpecialDrop("Dragon Heart", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateAranWarlord()
{
    Stats s(350, 60, 30, 24, 14, 12, 12, 20);
    auto m = std::make_shared<Monster>("Aran Warlord", s, 1000, 600, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Physical, 0.5f);    // Heavy armor boss
    m->SetResistance(ElementType::Ice, 1.25f);        // Weak to ice
    m->SetResistance(ElementType::Holy, 1.25f);       // Evil warlord, weak to holy
    m->SetDropTier(7);
    m->AddSpecialDrop("Warlord Crest", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateVoidLord()
{
    Stats s(500, 150, 35, 28, 22, 20, 15, 24);
    auto m = std::make_shared<Monster>("Void Lord", s, 2000, 1000, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Void lords resist arcane
    m->SetResistance(ElementType::Physical, 0.75f);   // Resists physical
    m->SetDropTier(8);
    m->AddSpecialDrop("Void Core", 100, 1, 1);
    return m;
}

// ============================================================
//  CELESTIAL SPIRE
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateCelestialKnight()
{
    Stats s(180, 60, 28, 22, 12, 14, 14, 20);
    auto m = std::make_shared<Monster>("Celestial Knight", s, 500, 300);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Arcane, 0.75f);     // Celestial beings resist arcane
    m->SetDropTier(7);
    m->AddSpecialDrop("Celestial Shard", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateSeraphim()
{
    Stats s(140, 120, 24, 18, 20, 22, 18, 16);
    auto m = std::make_shared<Monster>("Seraphim", s, 600, 350);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Angels resist arcane
    m->SetResistance(ElementType::Fire, 1.25f);       // Weak to fire
    m->SetDropTier(7);
    m->AddSpecialDrop("Seraphim Tear", 25, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateLightElemental()
{
    Stats s(120, 150, 20, 16, 24, 20, 16, 18);
    auto m = std::make_shared<Monster>("Light Elemental", s, 550, 280);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Light resists arcane
    m->SetResistance(ElementType::Fire, 1.25f);       // Weak to fire
    m->SetDropTier(7);
    m->AddSpecialDrop("Light Essence", 35, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateAstralMage()
{
    Stats s(130, 200, 18, 14, 28, 24, 12, 14);
    auto m = std::make_shared<Monster>("Astral Mage", s, 650, 400);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Astral mages resist arcane
    m->SetDropTier(7);
    m->AddSpecialDrop("Astral Fragment", 30, 1, 1);
    return m;
}

// ============================================================
//  VOID BEYOND
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateVoidWalker()
{
    Stats s(200, 80, 32, 24, 16, 14, 18, 22);
    auto m = std::make_shared<Monster>("Void Walker", s, 800, 500);
    m->SetAIType(EnemyAIType::Berserker);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Void walkers resist arcane
    m->SetResistance(ElementType::Physical, 0.75f);   // Resists physical
    m->SetDropTier(8);
    m->AddSpecialDrop("Void Essence", 35, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateCosmicHorror()
{
    Stats s(250, 100, 36, 28, 18, 16, 14, 24);
    auto m = std::make_shared<Monster>("Cosmic Horror", s, 1000, 600);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Arcane, 0.25f);     // Cosmic horrors resist arcane
    m->SetResistance(ElementType::Physical, 0.5f);    // Resists physical
    m->SetDropTier(8);
    m->AddSpecialDrop("Cosmic Fragment", 25, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateNetherDrake()
{
    Stats s(280, 80, 38, 30, 14, 12, 16, 26);
    auto m = std::make_shared<Monster>("Nether Drake", s, 1100, 700);
    m->SetAIType(EnemyAIType::Berserker);
    m->SetResistance(ElementType::Fire, 0.25f);       // Nether drakes immune to fire
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetDropTier(8);
    m->AddSpecialDrop("Nether Scale", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateStarBeast()
{
    Stats s(220, 150, 30, 22, 22, 20, 20, 20);
    auto m = std::make_shared<Monster>("Star Beast", s, 1200, 650);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Arcane, 0.75f);     // Star beasts resist arcane
    m->SetDropTier(8);
    m->AddSpecialDrop("Star Core", 30, 1, 1);
    return m;
}

// ============================================================
//  NEW BOSSES
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateSeraphimCouncil()
{
    Stats s(800, 250, 45, 35, 35, 30, 25, 30);
    auto m = std::make_shared<Monster>("Seraphim Council", s, 5000, 2000, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Angels resist arcane
    m->SetResistance(ElementType::Fire, 1.25f);       // Weak to fire
    m->SetDropTier(9);
    m->AddSpecialDrop("Seraph Crown", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreatePrimordialOne()
{
    Stats s(2000, 500, 60, 50, 50, 45, 40, 45);
    auto m = std::make_shared<Monster>("The Primordial One", s, 15000, 5000, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Arcane, 0.25f);     // Primordial resists arcane
    m->SetResistance(ElementType::Physical, 0.5f);    // Resists physical
    m->SetDropTier(10);
    return m;
}

// ============================================================
//  ARCANE SANCTUM (difficulty 9)
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateArcaneGuardian()
{
    Stats s(400, 300, 45, 35, 50, 40, 25, 35);
    auto m = std::make_shared<Monster>("Arcane Guardian", s, 2500, 1200);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Arcane guardian resists arcane
    m->SetDropTier(9);
    m->AddSpecialDrop("Arcane Fragment", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateManaWisp()
{
    Stats s(200, 500, 30, 20, 60, 50, 40, 20);
    auto m = std::make_shared<Monster>("Mana Wisp", s, 2000, 900);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Arcane, 0.25f);     // Mana wisps immune to arcane
    m->SetResistance(ElementType::Physical, 0.75f);   // Resists physical
    m->SetDropTier(9);
    m->AddSpecialDrop("Mana Crystal", 35, 1, 2);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateCrystalElemental()
{
    Stats s(500, 100, 50, 55, 30, 25, 20, 50);
    auto m = std::make_shared<Monster>("Crystal Elemental", s, 3000, 1500);
    m->SetAIType(EnemyAIType::Defensive);
    m->SetResistance(ElementType::Physical, 0.5f);    // Crystal resists physical
    m->SetResistance(ElementType::Lightning, 1.5f);   // Weak to lightning
    m->SetDropTier(9);
    m->AddSpecialDrop("Crystal Shard", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateSpellblade()
{
    Stats s(350, 200, 55, 40, 45, 35, 35, 40);
    auto m = std::make_shared<Monster>("Spellblade", s, 2800, 1300);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Arcane, 0.75f);     // Trained in arcana
    m->SetResistance(ElementType::Physical, 0.75f);   // Skilled warrior
    m->SetDropTier(9);
    m->AddSpecialDrop("Spell Essence", 25, 1, 1);
    return m;
}

// ============================================================
//  CHRONOS DEPTHS (difficulty 10)
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateTimeWeaver()
{
    Stats s(500, 600, 55, 45, 70, 60, 45, 45);
    auto m = std::make_shared<Monster>("Time Weaver", s, 5000, 2500);
    m->SetAIType(EnemyAIType::Caster);
    m->SetResistance(ElementType::Arcane, 0.5f);      // Time weavers resist arcane
    m->SetDropTier(10);
    m->AddSpecialDrop("Time Fragment", 30, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateChronoBeast()
{
    Stats s(800, 200, 75, 60, 40, 35, 50, 55);
    auto m = std::make_shared<Monster>("Chrono Beast", s, 6000, 3000);
    m->SetAIType(EnemyAIType::Aggressive);
    m->SetResistance(ElementType::Physical, 0.75f);   // Chrono beasts resist physical
    m->SetDropTier(10);
    m->AddSpecialDrop("Chrono Scale", 25, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateTemporalKnight()
{
    Stats s(700, 400, 70, 55, 55, 45, 40, 55);
    auto m = std::make_shared<Monster>("Temporal Knight", s, 5500, 2800);
    m->SetAIType(EnemyAIType::Balanced);
    m->SetResistance(ElementType::Arcane, 0.75f);     // Temporal knights resist arcane
    m->SetDropTier(10);
    m->AddSpecialDrop("Temporal Emblem", 25, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateVoidPhoenix()
{
    Stats s(600, 500, 65, 50, 60, 55, 60, 50);
    auto m = std::make_shared<Monster>("Void Phoenix", s, 6000, 3200);
    m->SetAIType(EnemyAIType::Berserker);
    m->SetResistance(ElementType::Fire, 0.25f);       // Phoenixes immune to fire
    m->SetResistance(ElementType::Ice, 1.5f);         // Weak to ice
    m->SetDropTier(10);
    m->AddSpecialDrop("Phoenix Ash", 20, 1, 1);
    return m;
}

// ============================================================
//  NEW BOSSES
// ============================================================

std::shared_ptr<Monster> EnemyDatabase::CreateArcaneConstruct()
{
    Stats s(3000, 800, 80, 60, 80, 60, 50, 55);
    auto m = std::make_shared<Monster>("Arcane Construct", s, 25000, 8000, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Arcane, 0.25f);     // Arcane construct immune to arcane
    m->SetDropTier(11);
    m->AddSpecialDrop("Arcane Core", 100, 1, 1);
    return m;
}

std::shared_ptr<Monster> EnemyDatabase::CreateChronos()
{
    Stats s(5000, 1000, 100, 80, 100, 80, 70, 70);
    auto m = std::make_shared<Monster>("Chronos, the Time Ender", s, 50000, 15000, true);
    m->SetAIType(EnemyAIType::Boss);
    m->SetResistance(ElementType::Arcane, 0.25f);     // Chronos resists arcane
    m->SetResistance(ElementType::Physical, 0.5f);    // Resists physical
    m->SetDropTier(12);
    m->AddSpecialDrop("Chronos Eye", 100, 1, 1);
    return m;
}

// Keep original for backwards compat
std::shared_ptr<Monster> EnemyDatabase::CreateDragon()
{
    return CreateElderDragon();
}

// ============================================================
//  WEIGHTED POOLS (per difficulty tier)
// ============================================================

const std::vector<EnemyEntry>& EnemyDatabase::GetPool(int difficulty)
{
    static const std::vector<EnemyEntry> tier1 = {
        { "Slime",        []{ return CreateSlime(); },        4 },
        { "Rat",          []{ return CreateRat(); },          4 },
        { "Bandit",       []{ return CreateBandit(); },       2 },
        { "Angry Chicken",[]{ return CreateAngryChicken(); }, 3 },
    };

    static const std::vector<EnemyEntry> tier2 = {
        { "Wolf",          []{ return CreateWolf(); },          3 },
        { "Giant Spider",  []{ return CreateGiantSpider(); },   3 },
        { "Treant",        []{ return CreateTreant(); },        1 },
        { "Wraith",        []{ return CreateWraith(); },        2 },
        { "Dark Elf Scout",[]{ return CreateDarkElfScout(); },  2 },
        { "Feral Boar",    []{ return CreateFeralBoar(); },     3 },
    };

    static const std::vector<EnemyEntry> tier3 = {
        { "Pirate",       []{ return CreatePirate(); },       3 },
        { "Sea Serpent",  []{ return CreateSeaSerpent(); },   2 },
        { "Giant Crab",   []{ return CreateGiantCrab(); },    3 },
        { "Siren",        []{ return CreateSiren(); },        2 },
    };

    static const std::vector<EnemyEntry> tier4 = {
        { "Orc",            []{ return CreateOrc(); },           3 },
        { "Harpy",          []{ return CreateHarpy(); },         2 },
        { "Fire Elemental", []{ return CreateFireElemental(); }, 2 },
        { "Drake",          []{ return CreateDrake(); },         2 },
        { "Stone Golem",    []{ return CreateStoneGolem(); },    1 },
    };

    static const std::vector<EnemyEntry> tier5 = {
        { "Highland Warrior", []{ return CreateHighlandWarrior(); }, 3 },
        { "Gryphon",          []{ return CreateGryphon(); },         2 },
        { "Earth Elemental",  []{ return CreateEarthElemental(); },  2 },
        { "Phantom",          []{ return CreatePhantom(); },         2 },
    };

    static const std::vector<EnemyEntry> tier6 = {
        { "Shadow",     []{ return CreateShadow(); },     3 },
        { "Lich",       []{ return CreateLich(); },       2 },
        { "Demon",      []{ return CreateDemon(); },      2 },
        { "Void Knight",[]{ return CreateVoidKnight(); }, 2 },
        { "Bone Golem", []{ return CreateBoneGolem(); },  1 },
    };

    static const std::vector<EnemyEntry> tier7 = {
        { "Celestial Knight", []{ return CreateCelestialKnight(); }, 3 },
        { "Seraphim",         []{ return CreateSeraphim(); },        2 },
        { "Light Elemental",  []{ return CreateLightElemental(); },  2 },
        { "Astral Mage",      []{ return CreateAstralMage(); },      2 },
    };

    static const std::vector<EnemyEntry> tier8 = {
        { "Void Walker",   []{ return CreateVoidWalker(); },   3 },
        { "Cosmic Horror", []{ return CreateCosmicHorror(); }, 2 },
        { "Nether Drake",  []{ return CreateNetherDrake(); },  2 },
        { "Star Beast",    []{ return CreateStarBeast(); },    2 },
    };

    static const std::vector<EnemyEntry> tier9 = {
        { "Arcane Guardian",  []{ return CreateArcaneGuardian(); },  3 },
        { "Mana Wisp",        []{ return CreateManaWisp(); },        2 },
        { "Crystal Elemental",[]{ return CreateCrystalElemental(); }, 2 },
        { "Spellblade",       []{ return CreateSpellblade(); },      2 },
    };

    static const std::vector<EnemyEntry> tier10 = {
        { "Time Weaver",     []{ return CreateTimeWeaver(); },    3 },
        { "Chrono Beast",    []{ return CreateChronoBeast(); },    2 },
        { "Temporal Knight", []{ return CreateTemporalKnight(); }, 2 },
        { "Void Phoenix",    []{ return CreateVoidPhoenix(); },    2 },
    };

    if (difficulty <= 1) return tier1;
    if (difficulty == 2) return tier2;
    if (difficulty == 3) return tier3;
    if (difficulty == 4) return tier4;
    if (difficulty == 5) return tier5;
    if (difficulty == 6) return tier6;
    if (difficulty == 7) return tier7;
    if (difficulty == 8) return tier8;
    if (difficulty == 9) return tier9;
    return tier10;
}
