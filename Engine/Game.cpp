#include "Game.hpp"
#include "KeyboardNav.hpp"
#include "RNG.hpp"
#include "../Graphics/Colors.hpp"
#include "../Graphics/IconRenderer.hpp"
#include "../Graphics/BattleRenderer.hpp"
#include "../Graphics/BattleLayout.hpp"
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Items/Loot.hpp"
#include "../Items/Passives.hpp"
#include "../Items/SetBonuses.hpp"
#include "../Items/Consumable.hpp"
#include "../Items/Resources/Resources.hpp"
#include "../Items/Uniques/UniqueItems.hpp"
#include "../Items/SummoningItem.hpp"
#include "../World/Enemies/Enemies.hpp"
#include "../World/NPCs/NPC.hpp"
#include <algorithm>
#include <cmath>

Game::Game()
    : renderer("Eluna — A CozyQuest Tale"),
      currentState(GameState::MainMenu),
      combatPhase(CombatPhase::PlayerTurn),
      combatSystem(std::make_shared<CombatSystem>()),
      currentAreaIndex(0),
      enemyActionTime(0),
      currentDungeonIndex(0),
      currentDungeonRoom(0),
      currentRoomEnemyIndex(0),
      dungeonRoomEnemyCount(0),
      isDungeonBossFight(false),
      inDungeonEncounter(false),
      leveledUpThisCombat(false),
      dungeonCompleted(false),
      currentNPC("", NPCDialogue{}, -1),
      npcDialoguePhase(0),
      questTab(0),
      questsChecked(false),
      questPage(0),
      questMaxPage(0),
      craftPage(0),
      craftMaxPage(0),
      skillOverviewPage(0),
      skillLoadoutPage(0),
      lastUsedSkillIdx(-1),
      selectedJobIdx(-1),
      jobPerkPage(0),
      inventoryTab(0),
      inventoryPage(0),
      skillStatsPage(0),
      shopSellPage(0),
      achievementsPage(0),
      enemyFlashTimer(0),
      previousState(GameState::MainMenu)
{
    UniqueItemRegistry::Initialize();
    SummoningRegistry::Initialize();
    InitializeAreas();
    wiki.SetAreas(areas);
}

void Game::InitializeAreas()
{
    // ---- Felrona ----
    {
        Dungeon farm;
        farm.name = "Abandoned Farm";
        farm.description = "An old farm overrun by bandits and wild animals.";
        farm.rooms = {
            { "The dusty barn entrance. Hay bales are scattered about.", 1, 2 },
            { "The main farmhouse. Furniture is overturned and looted.", 1, 2 },
            { "The back courtyard. The Farm Overseer waits here.", 0, 0 },
        };
        farm.bossFactory = []{ return EnemyDatabase::CreateFarmOverseer(); };

        areas.emplace_back("Felrona",
            "The Fair City of Felrona, a peaceful starting hub.",
            1, std::vector<Dungeon>{ farm }, std::vector<int>{1, 2});
    }

    // ---- Dark Woods ----
    {
        Dungeon hollow;
        hollow.name = "Twilight Hollow";
        hollow.description = "A dark cave system beneath the forest canopy.";
        hollow.rooms = {
            { "Moss-covered stone stairs descend into gloom.", 1, 2 },
            { "A wide chamber with glowing mushrooms on the walls.", 1, 3 },
            { "A narrow tunnel littered with bones.", 1, 2 },
            { "The Ancient Grove. The Treant King stands guard.", 0, 0 },
        };
        hollow.bossFactory = []{ return EnemyDatabase::CreateTreantKing(); };

        areas.emplace_back("Dark Woods",
            "A shadowy forest where strange creatures lurk.",
            2, std::vector<Dungeon>{ hollow }, std::vector<int>{0, 2, 3});
    }

    // ---- Salty Siren Coast ----
    {
        Dungeon grotto;
        grotto.name = "Sunken Grotto";
        grotto.description = "A flooded cavern along the rocky coastline.";
        grotto.rooms = {
            { "A sandy beach inside a sea cave. Strange shells litter the ground.", 1, 2 },
            { "An underwater tunnel opens into an air pocket with old ship wreckage.", 1, 2 },
            { "A deep pool with an eerie blue glow.", 1, 2 },
            { "The Abyssal Throne. The Kraken stirs.", 0, 0 },
        };
        grotto.bossFactory = []{ return EnemyDatabase::CreateKraken(); };

        areas.emplace_back("Salty Siren Coast",
            "A rocky coastline where pirates and sea monsters roam.",
            3, std::vector<Dungeon>{ grotto }, std::vector<int>{0, 1, 3});
    }

    // ---- Dragon's Peak ----
    {
        Dungeon summit;
        summit.name = "The Summit";
        summit.description = "The treacherous path to the dragon's lair.";
        summit.rooms = {
            { "A winding mountain trail with sheer drops on either side.", 1, 2 },
            { "A collapsed temple halfway up the mountain.", 1, 3 },
            { "A frozen mountain pass. The wind howls fiercely.", 1, 2 },
            { "A massive stone bridge over a bottomless chasm.", 1, 2 },
            { "The Dragon's Nest. The Elder Dragon hoards its treasure here.", 0, 0 },
        };
        summit.bossFactory = []{ return EnemyDatabase::CreateElderDragon(); };

        areas.emplace_back("Dragon's Peak",
            "A dangerous mountain where orcs and dragons roam.",
            4, std::vector<Dungeon>{ summit }, std::vector<int>{1, 2, 4});
    }

    // ---- Aran Highlands ----
    {
        Dungeon citadel;
        citadel.name = "The Citadel";
        citadel.description = "An ancient fortress of the Aran warriors.";
        citadel.rooms = {
            { "The outer gate, flanked by towering stone statues.", 1, 2 },
            { "A vast training yard with weapon racks and dummies.", 1, 3 },
            { "The armory, containing remnants of old Aran weaponry.", 1, 2 },
            { "The great hall, lined with banners of fallen houses.", 1, 2 },
            { "The Throne Room. The Aran Warlord awaits.", 0, 0 },
        };
        citadel.bossFactory = []{ return EnemyDatabase::CreateAranWarlord(); };

        areas.emplace_back("Aran Highlands",
            "Wind-swept highlands home to the proud Aran people.",
            5, std::vector<Dungeon>{ citadel }, std::vector<int>{3, 5});
    }

    // ---- Shadow Dungeon ----
    {
        Dungeon abyss;
        abyss.name = "Abyssal Depths";
        abyss.description = "The deepest reaches of darkness beneath Eluna.";
        abyss.rooms = {
            { "A crumbling staircase spirals into absolute darkness.", 1, 2 },
            { "A torture chamber filled with ancient implements.", 1, 3 },
            { "A vast cavern where shadows seem to move on their own.", 1, 2 },
            { "A bridge of bones over a river of molten rock.", 1, 2 },
            { "A ritual chamber with dark altars and eldritch symbols.", 1, 3 },
            { "The Void Throne. The Void Lord commands the darkness.", 0, 0 },
        };
        abyss.bossFactory = []{ return EnemyDatabase::CreateVoidLord(); };

        areas.emplace_back("Shadow Dungeon",
            "The ultimate challenge beneath the land of Eluna.",
            6, std::vector<Dungeon>{ abyss }, std::vector<int>{4, 6});
    }

    // ---- Celestial Spire ----
    {
        Dungeon ascent;
        ascent.name = "The Heavenly Ascent";
        ascent.description = "A sacred tower piercing the heavens.";
        ascent.rooms = {
            { "Marble steps ascend through clouds. Golden light filters down.", 1, 2 },
            { "A hall of stained glass depicting ancient saints.", 1, 2 },
            { "A celestial garden with floating platforms.", 1, 3 },
            { "A library of divine knowledge guarded by angelic sentinels.", 1, 2 },
            { "The Summit of Heaven. The Seraphim Council awaits.", 0, 0 },
        };
        ascent.bossFactory = []{ return EnemyDatabase::CreateSeraphimCouncil(); };

        areas.emplace_back("Celestial Spire",
            "A towering spire of pure light reaching into the heavens.",
            7, std::vector<Dungeon>{ ascent }, std::vector<int>{5, 7});
    }

    // ---- The Void Beyond ----
    {
        Dungeon endless;
        endless.name = "The Endless Void";
        endless.description = "The final frontier of darkness beyond existence.";
        endless.rooms = {
            { "A rift in reality opens into a starless abyss.", 1, 2 },
            { "Floating debris of forgotten worlds drifts by.", 1, 2 },
            { "A nexus of twisting void energies.", 1, 3 },
            { "The remains of an ancient cosmic battle.", 1, 2 },
            { "A fortress of pure shadow.", 1, 3 },
            { "The Primordial Throne. The Primordial One waits beyond time.", 0, 0 },
        };
        endless.bossFactory = []{ return EnemyDatabase::CreatePrimordialOne(); };

        areas.emplace_back("The Void Beyond",
            "The edge of all existence, where the Primordial One dwells.",
            8, std::vector<Dungeon>{ endless }, std::vector<int>{6, 8});
    }

    // ---- Arcane Sanctum ----
    {
        Dungeon sanctum;
        sanctum.name = "Sanctum of Secrets";
        sanctum.description = "An ancient mage fortress floating among shards of raw magic.";
        sanctum.rooms = {
            { "A grand library where books hover in mid-air, pages rustling.", 1, 2 },
            { "A laboratory crackling with unstable arcane energy.", 1, 2 },
            { "A chamber of floating crystal platforms above a mana vortex.", 1, 3 },
            { "The Arcane Core. The Arcane Construct guards the heart.", 0, 0 },
        };
        sanctum.bossFactory = []{ return EnemyDatabase::CreateArcaneConstruct(); };

        areas.emplace_back("Arcane Sanctum",
            "A floating fortress of pure magic, hidden beyond a forgotten rift.",
            9, std::vector<Dungeon>{ sanctum }, std::vector<int>{7, 9});
    }

    // ---- Chronos Depths ----
    {
        Dungeon chronos;
        chronos.name = "Chronos Depths";
        chronos.description = "A rift in time where past, present, and future collide.";
        chronos.rooms = {
            { "A fractured timeline where the same moment repeats endlessly.", 1, 2 },
            { "A battlefield frozen in mid-war, soldiers suspended in time.", 1, 2 },
            { "A desert of glass where future cities once stood.", 1, 3 },
            { "A cavern of crystallized time, each shard showing a different era.", 1, 2 },
            { "The End of Time. Chronos, the Time Ender, awaits.", 0, 0 },
        };
        chronos.bossFactory = []{ return EnemyDatabase::CreateChronos(); };

        areas.emplace_back("Chronos Depths",
            "The final frontier — a wound in reality where time itself has been broken.",
            10, std::vector<Dungeon>{ chronos }, std::vector<int>{8});
    }

    // ---- Forbidden Citadel (Area 11) — Boss-only gauntlet, requires Chronos defeated ----
    {
        // No regular dungeons — the Citadel is a boss select screen
        areas.emplace_back("Forbidden Citadel",
            "A prison of god-like beings. Only the one who slew Chronos may enter.",
            11, std::vector<Dungeon>{}, std::vector<int>{9});
    }
}

void Game::Run()
{
    while (!renderer.ShouldClose() && currentState != GameState::Exit)
    {
        renderer.BeginFrame();
        renderer.Clear(CQColors::BgDark);

        // Trigger slide-in animation on state entry
        if (currentState != previousState)
        {
            switch (currentState)
            {
                case GameState::Inventory:
                case GameState::SkillLoadout:
                case GameState::SkillUpgrade:
                case GameState::JobPerks:
                case GameState::Crafting:
                case GameState::Religion:
                case GameState::Shop:
                case GameState::Wiki:
                case GameState::QuestLog:
                    renderer.StartSlideIn();
                    break;
                default:
                    break;
            }
            previousState = currentState;
        }

        // Global keyboard: Escape goes back to exploring (from most sub-screens)
        if (IsKeyPressed(KEY_ESCAPE))
        {
            switch (currentState)
            {
                case GameState::Inventory:
                case GameState::Stats:
                case GameState::Jobs:
                case GameState::Crafting:
                case GameState::Religion:
                case GameState::Shop:
                case GameState::AreaSelect:
                case GameState::QuestLog:
                case GameState::SkillLoadout:
                case GameState::SkillUpgrade:
                case GameState::Wiki:
                case GameState::Achievements:
                case GameState::DungeonSelect:
                case GameState::CitadelBossSelect:
                case GameState::SavePrompt:
                case GameState::Evolution:
                case GameState::PetDetail:
                    currentState = GameState::Exploring;
                    break;
                case GameState::JobPerks:
                case GameState::JobSkillTree:
                case GameState::JobUpgrades:
                    currentState = GameState::Jobs;
                    break;
                case GameState::DungeonExplore:
                case GameState::DungeonComplete:
                case GameState::InCombat:
                    currentState = GameState::Exploring;
                    break;
                default:
                    break;
            }
        }

        switch (currentState)
        {
            case GameState::MainMenu:          StateMainMenu(); break;
            case GameState::CharacterCreation: StateCharacterCreation(); break;
            case GameState::Exploring:         StateExplore(); break;
            case GameState::InCombat:          StateCombat(); break;
            case GameState::Inventory:         StateInventory(); break;
            case GameState::Stats:             StateStats(); break;
            case GameState::Jobs:              StateJobs(); break;
            case GameState::Crafting:          StateCraft(); break;
            case GameState::Religion:          StateReligion(); break;
            case GameState::Shop:              StateShop(); break;
            case GameState::AreaSelect:        StateAreaSelect(); break;
            case GameState::QuestLog:          StateQuestLog(); break;
            case GameState::DungeonSelect:     StateDungeonSelect(); break;
            case GameState::DungeonExplore:    StateDungeonExplore(); break;
            case GameState::DungeonComplete:   StateDungeonComplete(); break;
            case GameState::SavePrompt:        SaveGamePrompt(); break;
            case GameState::LoadPrompt:        LoadGamePrompt(); break;
            case GameState::SkillLoadout:      StateSkillLoadout(); break;
            case GameState::SkillUpgrade:      StateSkillUpgrade(); break;
            case GameState::JobPerks:          StateJobPerks(); break;
            case GameState::JobSkillTree:      StateJobSkillTree(); break;
            case GameState::JobUpgrades:       StateJobUpgrades(); break;
            case GameState::Wiki:              StateWiki(); break;
            case GameState::NPCDialogue:        StateNPCDialogue(); break;
            case GameState::Achievements:       StateAchievements(); break;
            case GameState::Reputation:         StateReputation(); break;
            case GameState::CitadelBossSelect:   StateCitadelBossSelect(); break;
            case GameState::Pets:              StatePets(); break;
            case GameState::Evolution:          StateEvolution(); break;
            case GameState::PetDetail:          StatePetDetail(); break;
            case GameState::JobQuests:          StateJobQuests(); break;
            case GameState::ResourceChain:      StateResourceChain(); break;
            case GameState::MasterClass:        StateMasterClass(); break;
            case GameState::Escort:             StateEscort(); break;
            case GameState::Exit:              break;
            default:                           currentState = GameState::MainMenu;
        }

        DrawFloatingTexts();
        DrawParticles();
        CheckAchievementNotifications();
        DrawAchievementNotifications();
        DrawQuestRewardNotifications();
        reputationSystem.UpdateNotifications(GetFrameTime());
        DrawReputationNotifications();
        petManager.UpdateNotifications(GetFrameTime());
        DrawPetNotifications();
        DrawTutorialHints();
        achievementSystem.Tick(GetFrameTime());
        achievementSystem.CheckUnlockables();

        // Draw screen transition overlay
        renderer.DrawTransition();

        renderer.EndFrame();
    }
}

// ============================================================
//  DRAWING HELPERS
// ============================================================

void Game::AddFloatingText(const std::string& text, float x, float y, Color color, int fontSize)
{
    floatingTexts.emplace_back(text, x, y, color, fontSize);
}

void Game::DrawFloatingTexts()
{
    float dt = GetFrameTime();
    for (int i = static_cast<int>(floatingTexts.size()) - 1; i >= 0; --i)
    {
        auto& ft = floatingTexts[i];
        ft.y += ft.vy * dt;
        ft.life -= dt;

        if (ft.life <= 0.0f)
        {
            floatingTexts.erase(floatingTexts.begin() + i);
            continue;
        }

        float alpha = ft.life / ft.maxLife;
        Color c = ft.color;
        c.a = static_cast<unsigned char>(255.0f * alpha);

        // Scale up slightly at start, then back to normal
        float scale = 1.0f;
        if (alpha > 0.8f)
            scale = 1.0f + (alpha - 0.8f) * 2.5f;

        int fs = static_cast<int>(ft.fontSize * scale);
        int tw = MeasureText(ft.text.c_str(), fs);
        int tx = static_cast<int>(ft.x) - tw / 2;
        int ty = static_cast<int>(ft.y);

        // Text shadow for readability
        DrawText(ft.text.c_str(), tx + 1, ty + 1, fs, {0, 0, 0, static_cast<unsigned char>(180 * alpha)});
        DrawText(ft.text.c_str(), tx, ty, fs, c);
    }
}

void Game::AddParticleBurst(float x, float y, Color color, int count)
{
    for (int i = 0; i < count; ++i)
    {
        float angle = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
        float speed = 40.0f + static_cast<float>(GetRandomValue(0, 80));
        float vx = std::cos(angle) * speed;
        float vy = std::sin(angle) * speed;
        float sz = 2.0f + static_cast<float>(GetRandomValue(0, 4));
        float dur = 0.5f + static_cast<float>(GetRandomValue(0, 50)) / 100.0f;
        particles.emplace_back(x, y, vx, vy, color, sz, dur);
    }
}

void Game::AddHealParticles(float x, float y)
{
    for (int i = 0; i < 8; ++i)
    {
        float vx = static_cast<float>(GetRandomValue(-20, 20));
        float vy = -30.0f - static_cast<float>(GetRandomValue(0, 40));
        particles.emplace_back(x + static_cast<float>(GetRandomValue(-10, 10)), y, vx, vy, GREEN, 3.0f, 0.7f);
    }
}

void Game::AddCriticalParticles(float x, float y)
{
    for (int i = 0; i < 15; ++i)
    {
        float angle = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
        float speed = 60.0f + static_cast<float>(GetRandomValue(0, 100));
        float vx = std::cos(angle) * speed;
        float vy = std::sin(angle) * speed;
        particles.emplace_back(x, y, vx, vy, YELLOW, 4.0f, 0.6f);
    }
}

void Game::DrawParticles()
{
    float dt = GetFrameTime();
    for (int i = static_cast<int>(particles.size()) - 1; i >= 0; --i)
    {
        auto& p = particles[i];
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        p.vy += 60.0f * dt; // gravity
        p.life -= dt;

        if (p.life <= 0.0f)
        {
            particles.erase(particles.begin() + i);
            continue;
        }

        float alpha = p.life / p.maxLife;
        Color c = p.color;
        c.a = static_cast<unsigned char>(255.0f * alpha);
        float sz = p.size * alpha;

        DrawCircle(static_cast<int>(p.x), static_cast<int>(p.y), sz, c);
    }
}

void Game::CheckAchievementNotifications()
{
    while (achievementSystem.HasUnnotifiedAchievement())
    {
        AchievementProgress ap = achievementSystem.GetNextUnnotified();
        const AchievementDefinition* def = achievementSystem.GetDefinition(ap.achievementId);
        if (def)
        {
            achNotifications.emplace_back(def->name, def->description);
            // Grant item reward if defined
            if (player && !def->reward.itemId.empty())
            {
                auto item = UniqueItemRegistry::Create(def->reward.itemId, player->GetLevel());
                if (item)
                {
                    player->GetInventory().AddItem(item);
                    AddCombatLog("Achievement Reward: " + item->name + "!");
                }
            }
        }
        achievementSystem.MarkNotified(ap.achievementId);
    }
}

void Game::DrawAchievementNotifications()
{
    float dt = GetFrameTime();
    float yOffset = 60.0f;

    for (int i = static_cast<int>(achNotifications.size()) - 1; i >= 0; --i)
    {
        auto& n = achNotifications[i];
        n.life -= dt;

        if (n.life <= 0.0f)
        {
            achNotifications.erase(achNotifications.begin() + i);
            continue;
        }

        float alpha = 1.0f;
        if (n.life < 0.5f)
            alpha = n.life / 0.5f;

        int panelW = 320;
        int panelH = 50;
        int px = GRenderer::W - panelW - 10;
        int py = static_cast<int>(yOffset);

        // Slide in from right
        float slideProgress = 1.0f;
        if (n.life > n.maxLife - 0.3f)
            slideProgress = (n.maxLife - n.life) / 0.3f;
        px = GRenderer::W - static_cast<int>((panelW + 10) * slideProgress);

        Color bg = {30, 25, 15, static_cast<unsigned char>(220 * alpha)};
        Color border = {200, 170, 80, static_cast<unsigned char>(255 * alpha)};
        Color goldText = {255, 215, 0, static_cast<unsigned char>(255 * alpha)};
        Color whiteText = {220, 220, 210, static_cast<unsigned char>(255 * alpha)};

        DrawRectangle(px, py, panelW, panelH, bg);
        DrawRectangleLines(px, py, panelW, panelH, border);

        // Star icon
        DrawText("*", px + 8, py + 6, 18, goldText);
        DrawText("ACHIEVEMENT UNLOCKED", px + 24, py + 6, 12, goldText);
        DrawText(n.name.c_str(), px + 10, py + 22, 14, goldText);
        DrawText(n.description.c_str(), px + 10, py + 38, 11, whiteText);

        yOffset += panelH + 6.0f;
    }
}

void Game::AddTutorialHint(const std::string& title, const std::string& message)
{
    if (!tutorialEnabled) return;
    tutorialHints.emplace_back(title, message);
}

void Game::DrawTutorialHints()
{
    float dt = GetFrameTime();
    float yOffset = 60.0f;

    for (int i = static_cast<int>(tutorialHints.size()) - 1; i >= 0; --i)
    {
        auto& h = tutorialHints[i];
        h.life -= dt;

        if (h.life <= 0.0f)
        {
            tutorialHints.erase(tutorialHints.begin() + i);
            continue;
        }

        float alpha = 1.0f;
        if (h.life < 0.5f)
            alpha = h.life / 0.5f;

        int panelW = 340;
        int panelH = 56;
        int px = GRenderer::W - panelW - 10;
        int py = static_cast<int>(yOffset);

        // Slide in from right
        float slideProgress = 1.0f;
        if (h.life > h.maxLife - 0.3f)
            slideProgress = (h.maxLife - h.life) / 0.3f;
        px = GRenderer::W - static_cast<int>((panelW + 10) * slideProgress);

        Color bg = {25, 30, 40, static_cast<unsigned char>(220 * alpha)};
        Color border = {100, 160, 220, static_cast<unsigned char>(255 * alpha)};
        Color titleColor = {140, 200, 255, static_cast<unsigned char>(255 * alpha)};
        Color msgColor = {200, 210, 220, static_cast<unsigned char>(255 * alpha)};

        DrawRectangle(px, py, panelW, panelH, bg);
        DrawRectangleLines(px, py, panelW, panelH, border);

        DrawText("?", px + 8, py + 6, 18, titleColor);
        DrawText(h.title.c_str(), px + 26, py + 6, 13, titleColor);
        DrawText(h.message.c_str(), px + 10, py + 24, 12, msgColor);

        yOffset += panelH + 6.0f;
    }
}

void Game::DrawQuestRewardNotifications()
{
    float dt = GetFrameTime();
    float yOffset = 60.0f;

    for (int i = static_cast<int>(questRewardNotifications.size()) - 1; i >= 0; --i)
    {
        auto& n = questRewardNotifications[i];
        n.life -= dt;

        if (n.life <= 0.0f)
        {
            questRewardNotifications.erase(questRewardNotifications.begin() + i);
            continue;
        }

        float alpha = 1.0f;
        if (n.life < 0.5f)
            alpha = n.life / 0.5f;

        int panelW = 320;
        int panelH = 60;
        int px = GRenderer::W - panelW - 10;
        int py = static_cast<int>(yOffset);

        // Slide in from right
        float slideProgress = 1.0f;
        if (n.life > n.maxLife - 0.3f)
            slideProgress = (n.maxLife - n.life) / 0.3f;
        px = GRenderer::W - static_cast<int>((panelW + 10) * slideProgress);

        Color bg = {20, 30, 20, static_cast<unsigned char>(220 * alpha)};
        Color border = {100, 200, 100, static_cast<unsigned char>(255 * alpha)};
        Color titleText = {100, 255, 100, static_cast<unsigned char>(255 * alpha)};
        Color rewardText = {255, 215, 0, static_cast<unsigned char>(255 * alpha)};
        Color dimText = {180, 180, 170, static_cast<unsigned char>(255 * alpha)};

        DrawRectangle(px, py, panelW, panelH, bg);
        DrawRectangleLines(px, py, panelW, panelH, border);

        DrawText("QUEST COMPLETE", px + 10, py + 6, 12, titleText);
        std::string titleLine = n.questTitle;
        int maxTitleW = panelW - 20;
        while (!titleLine.empty() && MeasureText(titleLine.c_str(), 14) > maxTitleW)
            titleLine.pop_back();
        DrawText(titleLine.c_str(), px + 10, py + 22, 14, titleText);

        std::string rewardLine = "+" + std::to_string(n.xpEarned) + " XP   +" + std::to_string(n.goldEarned) + " Gold";
        DrawText(rewardLine.c_str(), px + 10, py + 40, 13, rewardText);

        yOffset += panelH + 6.0f;
    }
}

void Game::DrawTopBar()
{
    if (!player) return;
    renderer.DrawRect(0, 0, GRenderer::W, 50, CQColors::BgPanel);
    renderer.DrawRectLines(0, 0, GRenderer::W, 50, CQColors::BorderLight, 1);

    int x = 10;
    renderer.DrawText(player->GetName(), x, 5, 20, CQColors::TextGold);
    x += MeasureText(player->GetName().c_str(), 20) + 10;

    std::string cls;
    switch (player->GetCharacterClass()) {
        case CharacterClass::Warrior:  cls = "Warrior";  break;
        case CharacterClass::Priest:   cls = "Priest";   break;
        case CharacterClass::Mage:     cls = "Mage";     break;
        case CharacterClass::Archer:   cls = "Archer";   break;
        case CharacterClass::Merchant: cls = "Merchant"; break;
    }
    std::string lvl = "Lv." + std::to_string(player->GetLevel()) + " " + cls;
    renderer.DrawText(lvl, x, 5, 18, CQColors::TextLight);
    x += MeasureText(lvl.c_str(), 18) + 10;

    RaceData rd = RaceDatabase::Get(player->GetRace());
    renderer.DrawText(rd.name, x, 5, 16, CQColors::TextDim);
    x += MeasureText(rd.name.c_str(), 16) + 20;

    // Achievement title
    std::string title = achievementSystem.GetBestTitle();
    if (!title.empty())
    {
        renderer.DrawText("\"" + title + "\"", x, 5, 14, CQColors::TextGold);
        x += MeasureText(title.c_str(), 14) + 30;
    }

    renderer.DrawBarLabeled(player->GetCurrentHealth(), player->GetMaxHealth(),
                             x, 5, 180, 18, CQColors::HpFg, CQColors::HpBg, "HP");
    x += 190;
    renderer.DrawBarLabeled(player->GetCurrentMana(), player->GetMaxMana(),
                             x, 5, 180, 18, CQColors::ManaFg, CQColors::ManaBg, "MP");
    x += 190;
    int xpNext = Character::CalculateRequiredXP(player->GetLevel());
    renderer.DrawBarLabeled(player->GetExperience(), xpNext,
                             x, 5, 140, 18, CQColors::XpFg, CQColors::XpBg, "XP");
    std::string gold = "Gold: " + std::to_string(player->GetInventory().GetGold());
    renderer.DrawRightText(gold, GRenderer::W - 10, 15, 18, CQColors::TextGold);
    if (religion.GetGod() != GodType::None)
    {
        std::string f = "Faith: " + religion.GetGodName()
            + " (Dv." + std::to_string(religion.GetDevotionLevel()) + ")";
        renderer.DrawRightText(f, GRenderer::W - 10, 32, 14, CQColors::TextDim);
    }
}

void Game::DrawMessagePage(const std::string& title,
                            const std::vector<std::string>& lines,
                            GameState returnState)
{
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 100, title);
    int y = 110;
    for (const auto& l : lines)
    {
        renderer.DrawText(l, 70, y, 18, CQColors::TextLight);
        y += 26;
    }
    keyboardNav.SetFocusCount(1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 0))
        currentState = returnState;
}

void Game::DrawCharacterCard(int x, int y, int w, int h,
                              const std::string& name, const std::string& subtitle,
                              const std::string& desc, bool selected, bool hover)
{
    Color bg = selected ? CQColors::BtnHover : (hover ? CQColors::CardBg : CQColors::BgPanel);
    Color border = selected ? CQColors::Gold : CQColors::BorderLight;
    renderer.DrawRect(x, y, w, h, bg);
    renderer.DrawRectLines(x, y, w, h, border, selected ? 2 : 1);
    renderer.DrawText(name, x + 8, y + 6, 18, CQColors::TextGold);
    renderer.DrawText(subtitle, x + 8, y + 28, 14, CQColors::TextDim);
    renderer.DrawText(desc, x + 8, y + 48, 13, CQColors::TextLight);
}

// ============================================================
//  MAIN MENU
// ============================================================

void Game::StateMainMenu()
{
    renderer.DrawPanel(200, 100, GRenderer::W - 400, 200, "");
    renderer.DrawCenteredText("ELUNA", 130, 64, CQColors::Gold);
    renderer.DrawCenteredText("A  C o z y Q u e s t  T a l e", 200, 28, CQColors::TextDim);
    std::vector<std::string> opts = { "New Game", "Load Game", "Quit" };
    keyboardNav.SetFocusCount(static_cast<int>(opts.size()));
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    int c = renderer.ButtonList(opts, renderer.CenterX(260), 340, 260, 48, 10, 0);
    if (c >= 0 && c < static_cast<int>(opts.size()))
    {
        if (c == 0) { currentState = GameState::CharacterCreation; renderer.StartTransition(); }
        else if (c == 1) { currentState = GameState::LoadPrompt; renderer.StartTransition(); }
        else if (c == 2) currentState = GameState::Exit;
    }
    renderer.DrawCenteredText("v1.0  |  Inspired by CozyQuest (Nils Munch)",
                              GRenderer::H - 30, 14, CQColors::TextDim);
}

// ============================================================
//  CHARACTER CREATION
// ============================================================

void Game::StateCharacterCreation()
{
    renderer.DrawPanel(30, 50, GRenderer::W - 60, GRenderer::H - 90, "Character Creation");
    static std::string name = "";
    static CharacterRace selectedRace = CharacterRace::Aran;
    static CharacterClass selectedClass = CharacterClass::Warrior;
    static bool initialized = false;
    if (!initialized) { name = ""; selectedRace = CharacterRace::Aran; selectedClass = CharacterClass::Warrior; initialized = true; }
    int cx = renderer.CenterX(400);
    std::string result = renderer.InputBox("Character Name:", cx, 95, 400);
    if (!result.empty()) name = result;

    renderer.DrawText("Choose Race:", 50, 140, 20, CQColors::TextGold);
    struct RC { CharacterRace r; const char* n; const char* sub; const char* d; };
    RC races[5] = {
        { CharacterRace::Thull,  "Thull",  "Strong & Enduring",  "+2 Str, +2 Vit" },
        { CharacterRace::Mekkel, "Mekkel", "Nomads & Bards",     "+2 Wis, +2 Dex" },
        { CharacterRace::Aran,   "Aran",   "Sword Masters",      "+2 Str, +1 Int" },
        { CharacterRace::Pesha,  "Pesha",  "Peace-Loving",       "+2 Wis, +1 Vit" },
        { CharacterRace::Toran,  "Toran",  "Iron Warriors",      "+2 Vit, +2 Def" },
    };
    for (int i = 0; i < 5; ++i)
    {
        bool sel = races[i].r == selectedRace;
        bool hover = renderer.IsMouseInRect(50 + i * 190, 170, 180, 80);
        DrawCharacterCard(50 + i * 190, 170, 180, 80, races[i].n, races[i].sub, races[i].d, sel, hover);
        if (hover && renderer.IsMouseClickedOn(50 + i * 190, 170, 180, 80))
            selectedRace = races[i].r;
    }
    renderer.DrawText("Choose Class:", 50, 270, 20, CQColors::TextGold);
    struct CC { CharacterClass c; const char* n; const char* sub; const char* d; };
    CC classes[5] = {
        { CharacterClass::Warrior,  "Warrior",  "High HP & Defense",  "+2 Str, +2 Vit" },
        { CharacterClass::Priest,   "Priest",   "Healing Magic",      "+2 Wis, +1 Vit" },
        { CharacterClass::Mage,     "Mage",     "Powerful Spells",    "+2 Int, +1 Wis" },
        { CharacterClass::Archer,   "Archer",   "Ranged Precision",   "+2 Dex, +1 Str" },
        { CharacterClass::Merchant, "Merchant", "Gold & Crafting",    "+1 all, gold bonus" },
    };
    for (int i = 0; i < 5; ++i)
    {
        bool sel = classes[i].c == selectedClass;
        bool hover = renderer.IsMouseInRect(50 + i * 188, 300, 180, 90);
        DrawCharacterCard(50 + i * 188, 300, 180, 90, classes[i].n, classes[i].sub, classes[i].d, sel, hover);
        if (hover && renderer.IsMouseClickedOn(50 + i * 188, 300, 180, 90))
            selectedClass = classes[i].c;
    }
    RaceData rd = RaceDatabase::Get(selectedRace);
    ClassData cd = ClassDatabase::Get(selectedClass);
    std::string cn;
    switch (selectedClass) {
        case CharacterClass::Warrior:  cn = "Warrior";  break;
        case CharacterClass::Priest:   cn = "Priest";   break;
        case CharacterClass::Mage:     cn = "Mage";     break;
        case CharacterClass::Archer:   cn = "Archer";   break;
        case CharacterClass::Merchant: cn = "Merchant"; break;
    }
    std::string info = rd.name + " " + rd.description + "  |  " + cn
        + "  HP:" + std::to_string(cd.startingStats.health)
        + " MP:" + std::to_string(cd.startingStats.mana);
    renderer.DrawText(info, 50, 410, 16, CQColors::TextLight);
    if (name.empty())
        renderer.DrawText("Enter a name to create your character.", 50, 450, 16, CQColors::TextDim);
    else if (renderer.Button("Create " + name, renderer.CenterX(200), 460, 200, 44))
    {
        player = std::make_shared<Player>(name, selectedClass, selectedRace);
        player->GetQuestManager().InitializeDefaultQuests();
        currentAreaIndex = 0;
        selectedSkillIdx = -1;
        viewingMasteryIdx = -1;
        selectedAchievementCategory = -1;
        initialized = false;
        ClearLog();
        currentState = GameState::Exploring;
    }
}

// ============================================================
//  EXPLORE
// ============================================================

void Game::StateExplore()
{
    if (!player) { currentState = GameState::MainMenu; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    DrawTopBar();

    // Area info panel
    renderer.DrawPanel(10, 60, 320, 160, areas[currentAreaIndex].name);
    renderer.DrawText(areas[currentAreaIndex].description, 20, 100, 16, CQColors::TextLight);
    renderer.DrawText("Difficulty: " + std::to_string(areas[currentAreaIndex].difficulty),
                      20, 180, 14, CQColors::TextDim);

    // Dungeon button if available
    int extraButtons = 0;
    if (currentAreaIndex == 10 && chronosDefeated)
    {
        // Forbidden Citadel — boss select
        if (renderer.Button("Face the Citadel", GRenderer::W - 240, 60, 220, 38, 15))
        {
            citadelSelectedBoss = -1;
            currentState = GameState::CitadelBossSelect;
        }
        extraButtons++;
    }
    else if (!areas[currentAreaIndex].dungeons.empty())
    {
        if (renderer.Button("Enter Dungeon", GRenderer::W - 240, 60, 220, 38, 15))
            currentState = GameState::DungeonSelect;
        extraButtons++;
    }

    // NPC button
    if (renderer.Button("Talk to NPC", 20, 220, 150, 36, 15 + extraButtons))
    {
        // Get NPCs for current area (up to 3 NPCs per area)
        NPC npc("", NPCDialogue{}, -1);
        switch (currentAreaIndex)
        {
            case 0: npc = NPCDatabase::CreateFelronaBlacksmith(); break;
            case 1: npc = NPCDatabase::CreateDarkWoodsRanger(); break;
            case 2: npc = NPCDatabase::CreateCoastPirateHunter(); break;
            case 3: npc = NPCDatabase::CreateDragonPeakScholar(); break;
            case 4: npc = NPCDatabase::CreateHighlandsShieldbearer(); break;
            case 5: npc = NPCDatabase::CreateShadowDungeonPriest(); break;
            default: npc = NPCDatabase::CreateFelronaBlacksmith(); break;
        }
        StartNPCDialogue(npc);
    }

    std::vector<std::string> labels = {
        "Explore Area", "Travel", "Quests",
        "Inventory", "Stats", "Jobs",
        "Skills", "Skill Upgrades", "Crafting", "Pray",
        "Shop", "Rest at Inn", "Evolution", "Codex", "Achievements", "Reputation", "Pets", "Job Quests", "Chain Crafting", "Escort", "Save Game", "Main Menu"
    };

    int bx = GRenderer::W - 240;
    int by = 100;
    int btnH = 34;
    int btnSpacing = 36;
    keyboardNav.SetFocusCount(static_cast<int>(labels.size()) + extraButtons);
    for (size_t i = 0; i < labels.size(); ++i)
    {
        if (renderer.Button(labels[i], bx, by + static_cast<int>(i) * btnSpacing, 220, btnH, static_cast<int>(i)))
        {
            if (i == 0) StartAreaEncounter();
            else if (i == 10) { currentState = GameState::Shop; renderer.StartTransition(); }
            else if (i == 11) {
                if (!tutorialRestedAtInn) { tutorialRestedAtInn = true; AddTutorialHint("Rest", "Resting restores HP/MP and reduces fatigue. Visit the inn often!"); }
                player->RestoreHealth(player->GetMaxHealth());
                player->RestoreMana(player->GetMaxMana());
                player->ResetTempDefense();
                // Advance day for weather and other daily effects
                player->GetJobSystem().GetEnvironment().UpdateDaily();
                // Reset job fatigue
                player->GetJobSystem().ResetAllFatigue();
                // Generate new daily job quests
                jobQuestSystem.GenerateDailyQuests(player->GetJobSystem().GetJobs());
                // Restock shops
                shopItems.clear();
            }
            else if (i == 12) { currentState = GameState::Evolution; renderer.StartTransition(); }
            else if (i == 13) { currentState = GameState::Wiki; renderer.StartTransition(); }
            else if (i == 14) { currentState = GameState::Achievements; renderer.StartTransition(); }
            else if (i == 15) { currentState = GameState::Reputation; renderer.StartTransition(); }
            else if (i == 16) { currentState = GameState::Pets; petListPage = 0; renderer.StartTransition(); }
            else if (i == 17) { currentState = GameState::JobQuests; jobQuestPage = 0; renderer.StartTransition(); }
            else if (i == 18) { currentState = GameState::ResourceChain; chainPage = 0; renderer.StartTransition(); }
            else if (i == 19) { currentState = GameState::Escort; renderer.StartTransition(); }
            else if (i == 20) currentState = GameState::SavePrompt;
            else if (i == 21) {
                currentState = GameState::MainMenu;
            }
            else {
                static std::vector<GameState> tgts = {
                    GameState::AreaSelect, GameState::QuestLog,
                    GameState::Inventory, GameState::Stats, GameState::Jobs,
                    GameState::SkillLoadout, GameState::SkillUpgrade, GameState::Crafting, GameState::Religion
                };
                int idx = static_cast<int>(i) - 1;
                if (idx >= 0 && idx < static_cast<int>(tgts.size()))
                {
                    if (idx == 0 && !tutorialTraveled) { tutorialTraveled = true; AddTutorialHint("Travel", "Visit connected areas on the world map. New areas mean new loot!"); }
                    currentState = tgts[idx];
                    renderer.StartTransition();
                }
            }
        }
    }

    // Number key shortcuts for menu (1-9)
    for (int k = 0; k < 9 && k < static_cast<int>(labels.size()); ++k)
    {
        if (IsKeyPressed(static_cast<KeyboardKey>(KEY_ONE + k)))
        {
            if (k == 0) StartAreaEncounter();
            else {
                static std::vector<GameState> menuTgts = {
                    GameState::AreaSelect, GameState::QuestLog,
                    GameState::Inventory, GameState::Stats, GameState::Jobs,
                    GameState::SkillLoadout, GameState::SkillUpgrade, GameState::Crafting, GameState::Religion
                };
                int idx = k - 1;
                if (idx >= 0 && idx < static_cast<int>(menuTgts.size()))
                {
                    currentState = menuTgts[idx];
                    renderer.StartTransition();
                }
            }
            break;
        }
    }
}

// ============================================================
//  DUNGEON SELECT
// ============================================================

void Game::StateDungeonSelect()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    const auto& dungeons = areas[currentAreaIndex].dungeons;
    renderer.DrawPanel(100, 60, GRenderer::W - 200, GRenderer::H - 130, "Dungeons");

    int y = 110;
    keyboardNav.SetFocusCount(static_cast<int>(dungeons.size()) + 1); // +1 for Back
    for (size_t i = 0; i < dungeons.size(); ++i)
    {
        const auto& d = dungeons[i];
        if (renderer.Button(d.name, 130, y, 250, 36, static_cast<int>(i)))
        {
            currentDungeonIndex = static_cast<int>(i);
            currentDungeonRoom = 0;
            currentRoomEnemyIndex = 0;
            dungeonCompleted = false;
            isDungeonBossFight = false;
            roomQueue.clear();
            {
                const auto& rooms = d.rooms;
                dungeonRoomEnemyCount = rooms[0].minEnemies
                    + RNG::Next(rooms[0].maxEnemies - rooms[0].minEnemies + 1);
            }
            currentState = GameState::DungeonExplore;
        }
        renderer.DrawText(d.description, 400, y + 8, 14, CQColors::TextLight);
        renderer.DrawText(std::to_string(d.rooms.size()) + " rooms",
                          400, y + 26, 13, CQColors::TextDim);
        y += 44;
    }

    if (renderer.Button("Back", renderer.CenterX(120), y + 20, 120, 40, static_cast<int>(dungeons.size())))
        currentState = GameState::Exploring;
}

// ============================================================
//  DUNGEON EXPLORE
// ============================================================

void Game::StateDungeonExplore()
{
    if (!player) { currentState = GameState::Exploring; return; }

    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    const auto& dungeon = areas[currentAreaIndex].dungeons[currentDungeonIndex];
    bool isBossRoom = (currentDungeonRoom == static_cast<int>(dungeon.rooms.size()) - 1)
                      && dungeon.rooms.back().minEnemies == 0;

    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120,
                        dungeon.name + " — Room " + std::to_string(currentDungeonRoom + 1));

    renderer.DrawText(dungeon.rooms[currentDungeonRoom].description,
                      70, 110, 18, CQColors::TextLight);

    std::string progress = "Room " + std::to_string(currentDungeonRoom + 1)
        + " / " + std::to_string(dungeon.rooms.size());
    renderer.DrawText(progress, 70, 145, 16, CQColors::TextDim);

    keyboardNav.SetFocusCount(2);

    // Player health
    renderer.DrawBarLabeled(player->GetCurrentHealth(), player->GetMaxHealth(),
                             70, 170, 300, 22, CQColors::HpFg, CQColors::HpBg, "HP");
    renderer.DrawBarLabeled(player->GetCurrentMana(), player->GetMaxMana(),
                             390, 170, 300, 22, CQColors::ManaFg, CQColors::ManaBg, "MP");

    int by = 220;
    if (isBossRoom)
    {
        // Check if this is an ultimate boss requiring level 50
        bool needsLevel50 = (areas[currentAreaIndex].name == "The Void Beyond"
                          || areas[currentAreaIndex].name == "Chronos Depths");

        if (needsLevel50 && player->GetLevel() < Character::MAX_LEVEL)
        {
            renderer.DrawText("The Primordial One - Requires Level 50", 70, by, 22, CQColors::TextRed);
            by += 30;
            renderer.DrawText("You are not yet strong enough to face this foe.", 70, by, 16, CQColors::TextDim);
        }
        else
        {
            renderer.DrawText("A powerful boss awaits!", 70, by, 24, CQColors::TextRed);
            by += 36;
            if (renderer.Button("Face the Boss", renderer.CenterX(200), by, 200, 44, 0))
            {
                auto boss = dungeon.bossFactory();
                if (boss)
                {
                    boss->GetSkills().AddSkill(std::make_shared<CommonAttack>());
                    boss->ScaleStats(areas[currentAreaIndex].difficulty);
                    roomQueue.clear();
                    isDungeonBossFight = true;
                    inDungeonEncounter = true;
                    StartCombatWithEnemy(boss, true);
                }
            }
        }
    }
    else
    {
        int n = dungeonRoomEnemyCount;
        if (renderer.Button("Fight!", renderer.CenterX(160), by, 160, 44, 0))
        {
            // Generate the room's enemies
            roomQueue.clear();
            const auto& pool = EnemyDatabase::GetPool(areas[currentAreaIndex].difficulty);
            for (int i = 0; i < n; ++i)
            {
                if (!pool.empty())
                {
                    int idx = RNG::Next(static_cast<int>(pool.size()));
                    auto m = pool[idx].factory();
                    if (m)
                    {
                        m->GetSkills().AddSkill(std::make_shared<CommonAttack>());
                        m->ScaleStats(areas[currentAreaIndex].difficulty);
                        roomQueue.push_back(m);
                    }
                }
            }
            currentRoomEnemyIndex = 0;
            isDungeonBossFight = false;
            inDungeonEncounter = true;
            if (!roomQueue.empty())
                StartCombatWithEnemy(roomQueue[0], false);
        }
    }

    if (renderer.Button("Flee Dungeon", 70, GRenderer::H - 90, 160, 36, 1))
        currentState = GameState::Exploring;
}

// ============================================================
//  DUNGEON COMPLETE
// ============================================================

void Game::StateDungeonComplete()
{
    if (!player) { currentState = GameState::Exploring; return; }

    const auto& dungeon = areas[currentAreaIndex].dungeons[currentDungeonIndex];

    if (!questsChecked)
    {
        player->GetQuestManager().CompleteExploreQuest(dungeon.name);
        auto& qm = player->GetQuestManager();
        for (size_t i = 0; i < qm.GetQuestCount(); ++i)
        {
            Quest* q = qm.GetQuest(i);
            if (q && q->status == QuestStatus::Completed && !q->rewarded)
            {
                int xpReward = q->rewardXP;
                float achQuestXPBonus = achievementSystem.GetQuestXPBonusByTier();
                if (achQuestXPBonus > 0.0f)
                {
                    int achBonus = static_cast<int>(xpReward * achQuestXPBonus);
                    if (achBonus > 0)
                        xpReward += achBonus;
                }
                player->GainXP(xpReward);
                player->GetInventory().AddGold(q->rewardGold);
                q->rewarded = true;

                // Show quest reward notification
                questRewardNotifications.emplace_back(q->title, xpReward, q->rewardGold);

                // Reputation from quest completion
                FactionID qFaction = reputationSystem.GetFactionByArea(currentAreaIndex);
                reputationSystem.OnQuestCompleted(qFaction, 10 + q->rewardXP / 10);

                // Check for Legend-rank faction pet
                if (reputationSystem.GetRank(qFaction) == RepRank::Legend)
                {
                    std::string petID = petManager.GetFactionLegendPetID(qFaction);
                    if (!petID.empty())
                    {
                        Pet* p = petManager.GetPet(petID);
                        if (p && !p->obtained)
                        {
                            petManager.ObtainPet(petID); wiki.MarkPetObtained(petID);
                            questRewardNotifications.emplace_back("Pet Unlocked: " + p->name, 0, 0);
                            petManager.AddNotification(p->name, "Legend rank reward!");
                        }
                    }
                }

                // Track quest completion achievements
                achievementSystem.UpdateProgress("quest_first");
                achievementSystem.UpdateProgress("quest_5");
                achievementSystem.UpdateProgress("quest_10");
                achievementSystem.UpdateProgress("quest_25");
                achievementSystem.UpdateProgress("quest_50");
                achievementSystem.UpdateProgress("quest_100");
                if (q->type == QuestType::Kill)
                {
                    achievementSystem.UpdateProgress("quest_kill_10");
                    achievementSystem.UpdateProgress("quest_kill_25");
                    achievementSystem.UpdateProgress("quest_kill_50");
                }
                else if (q->type == QuestType::Collect)
                {
                    achievementSystem.UpdateProgress("quest_gather_10");
                    achievementSystem.UpdateProgress("quest_gather_25");
                    achievementSystem.UpdateProgress("quest_gather_50");
                }
                else if (q->type == QuestType::Explore)
                {
                    achievementSystem.UpdateProgress("quest_explore_5");
                    achievementSystem.UpdateProgress("quest_explore_10");
                    achievementSystem.UpdateProgress("quest_explore_15");
                }
            }
        }
        questsChecked = true;

        // Dungeon completion reputation
        FactionID dunFaction = reputationSystem.GetFactionByArea(currentAreaIndex);
        reputationSystem.AddDungeonReputation(dunFaction, areas[currentAreaIndex].difficulty);

        // Check for Legend-rank faction pet
        if (reputationSystem.GetRank(dunFaction) == RepRank::Legend)
        {
            std::string petID = petManager.GetFactionLegendPetID(dunFaction);
            if (!petID.empty())
            {
                Pet* p = petManager.GetPet(petID);
                if (p && !p->obtained)
                {
                    petManager.ObtainPet(petID); wiki.MarkPetObtained(petID);
                    questRewardNotifications.emplace_back("Pet Unlocked: " + p->name, 0, 0);
                    petManager.AddNotification(p->name, "Legend rank reward!");
                }
            }
        }
    }

    keyboardNav.SetFocusCount(1);
    renderer.DrawPanel(200, 100, GRenderer::W - 400, 400, "Dungeon Complete!");

    renderer.DrawCenteredText(dungeon.name + " conquered!", 200, 28, CQColors::TextGold);
    renderer.DrawCenteredText("All enemies cleared. The dungeon is safe once more.",
                              250, 18, CQColors::TextLight);

    if (renderer.Button("Return", renderer.CenterX(160), 420, 160, 44, 0))
    {
        questsChecked = false;
        dungeonCompleted = false;
        currentState = GameState::Exploring;
    }
}

// ============================================================
//  FORBIDDEN CITADEL — BOSS SELECT
// ============================================================

void Game::StateCitadelBossSelect()
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (!chronosDefeated) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    struct CitadelBossInfo
    {
        std::string name;
        std::string element;
        int hp;
        int str;
        std::function<std::shared_ptr<Monster>()> factory;
    };

    std::vector<CitadelBossInfo> bosses = {
        { "Abyssal Sentinel",  "Physical",  8000, 120, []{ return EnemyDatabase::CreateAbyssalSentinel(); } },
        { "Void Empress",      "Arcane",    9000, 110, []{ return EnemyDatabase::CreateVoidEmpress(); } },
        { "Infernal Colossus", "Fire",     10000, 130, []{ return EnemyDatabase::CreateInfernalColossus(); } },
        { "Glacial Wraith",    "Ice",       8500, 100, []{ return EnemyDatabase::CreateGlacialWraith(); } },
        { "Storm Arbiter",     "Lightning", 9500, 125, []{ return EnemyDatabase::CreateStormArbiter(); } },
        { "Plague Sovereign",  "Poison",   11000,  90, []{ return EnemyDatabase::CreatePlagueSovereign(); } },
        { "Holy Arbiter",      "Holy",     12000, 115, []{ return EnemyDatabase::CreateCitadelHolyArbiter(); } },
        { "Chrono Overlord",   "Arcane",   13000, 140, []{ return EnemyDatabase::CreateChronoOverlord(); } },
        { "Void Monarch",      "Arcane",   15000, 150, []{ return EnemyDatabase::CreateVoidMonarch(); } },
        { "The Unbroken",      "Physical", 20000, 170, []{ return EnemyDatabase::CreateTheUnbroken(); } },
    };

    Color bossColors[] = {
        {180, 60, 60, 255}, {120, 40, 180, 255}, {220, 100, 20, 255}, {100, 180, 220, 255},
        {180, 180, 40, 255}, {60, 140, 60, 255}, {240, 220, 140, 255}, {160, 80, 200, 255},
        {40, 20, 80, 255}, {200, 200, 200, 255},
    };

    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, "Forbidden Citadel — Boss Gauntlet");
    renderer.DrawText("Choose a boss to challenge. Each is a legendary being of immense power.", 70, 80, 13, CQColors::TextDim);

    int y = 110;
    int bossCount = static_cast<int>(bosses.size());
    keyboardNav.SetFocusCount(bossCount + 1); // +1 for Back

    for (int i = 0; i < bossCount; ++i)
    {
        const auto& b = bosses[i];
        Color col = bossColors[i % 10];

        // Boss card
        Color cardBg = {25, 25, 35, 255};
        renderer.DrawRect(70, y, GRenderer::W - 160, 50, cardBg);
        renderer.DrawRectLines(70, y, GRenderer::W - 160, 50, col);

        renderer.DrawText(b.name, 80, y + 4, 16, col);
        renderer.DrawText(b.element, 340, y + 4, 13, CQColors::TextDim);
        renderer.DrawText("HP: " + std::to_string(b.hp) + "  STR: " + std::to_string(b.str),
                          440, y + 4, 13, CQColors::TextGold);

        if (renderer.Button("Challenge##cb" + std::to_string(i), GRenderer::W - 220, y + 8, 140, 32, i))
        {
            auto boss = b.factory();
            if (boss)
            {
                boss->GetSkills().AddSkill(std::make_shared<CommonAttack>());
                boss->ScaleStats(areas[currentAreaIndex].difficulty);
                citadelSelectedBoss = i;
                StartCombatWithEnemy(boss, true);
            }
        }

        y += 58;
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 70, 120, 40, bossCount))
    {
        currentState = GameState::Exploring;
    }
}

// ============================================================
//  INVENTORY / STATS / JOBS / CRAFT / RELIGION / AREA / QUESTS
// ============================================================

static std::string GetSetNameFromItem(const std::shared_ptr<Item>& item)
{
    if (!item || item->setId < 0) return "";
    const SetInfo* set = SetBonuses::FindSetById(item->setId);
    return set ? set->name : "";
}

static std::string GetPassiveText(const std::shared_ptr<Item>& item)
{
    if (!item) return "";
    std::string result;
    if (item->passive1 != ItemPassive::None)
        result += PassiveName(item->passive1);
    if (item->passive2 != ItemPassive::None)
    {
        if (!result.empty()) result += " | ";
        result += PassiveName(item->passive2);
    }
    return result;
}

static void DrawSetBonusSection(GRenderer& renderer, const Equipment& eq, int x, int y, int maxW)
{
    struct SetCount { int setId; const char* name; int count; int total; };
    std::vector<SetCount> sets;

    auto checkItem = [&](const std::shared_ptr<Item>& item) {
        if (!item || item->setId < 0) return;
        for (auto& sc : sets)
        {
            if (sc.setId == item->setId) { sc.count++; return; }
        }
        const SetInfo* si = SetBonuses::FindSetById(item->setId);
        int total = si ? static_cast<int>(si->pieceNames.size()) : 0;
        sets.push_back({item->setId, si ? si->name.c_str() : "Unknown", 1, total});
    };

    checkItem(eq.weapon);
    checkItem(eq.offhand);
    checkItem(eq.helmet);
    checkItem(eq.chest);
    checkItem(eq.gloves);
    checkItem(eq.pants);
    checkItem(eq.boots);
    checkItem(eq.ring1);
    checkItem(eq.ring2);
    checkItem(eq.amulet);

    if (sets.empty()) return;

    renderer.DrawText("Set Bonuses:", x, y, 16, CQColors::TextGold);
    y += 22;

    for (const auto& sc : sets)
    {
        if (sc.count < 2) continue;

        std::string header = sc.name;
        header += " (" + std::to_string(sc.count) + "/" + std::to_string(sc.total) + ")";
        renderer.DrawText(header, x + 5, y, 14, CQColors::TextGreen);
        y += 18;

        auto bonuses = SetBonuses::GetActiveBonuses(eq, sc.setId);
        for (const auto& b : bonuses)
        {
            std::string line = "  + " + std::to_string(b.value);
            switch (b.type)
            {
                case SetBonusType::DmgBoost:       line += "% physical damage"; break;
                case SetBonusType::DefBoost:       line += "% damage reduction"; break;
                case SetBonusType::ManaCostReduce: line += "% mana cost reduction"; break;
                case SetBonusType::CooldownReduce: line += " turn(s) cooldown reduction"; break;
                case SetBonusType::StatusExtend:   line += " turn(s) status duration"; break;
                case SetBonusType::CritBoost:      line += "% crit chance"; break;
                case SetBonusType::SpellDmgBoost:  line += "% spell damage"; break;
                case SetBonusType::HpPerTurn:      line += " HP per turn"; break;
                case SetBonusType::Dodge:          line += "% dodge chance"; break;
                case SetBonusType::HealOnKill:     line += "% heal on kill"; break;
                case SetBonusType::Thorns:         line += "% damage reflected"; break;
                case SetBonusType::Revive:         line += "% HP on revive"; break;
                case SetBonusType::ManaRegen:      line += " mana per turn"; break;
                case SetBonusType::AllResist:      line += " all resist"; break;
                case SetBonusType::DoubleCast:     line += "% double cast chance"; break;
                case SetBonusType::BurnImmune:     line += " - immune to burn"; break;
            }
            while (!line.empty() && MeasureText(line.c_str(), 13) > maxW)
                line.pop_back();
            renderer.DrawText(line, x + 15, y, 13, CQColors::TextLight);
            y += 17;
        }
    }
}

static const char* SlotDisplayName(const std::string& key)
{
    if (key == "weapon") return "Weapon";
    if (key == "helmet") return "Helmet";
    if (key == "chest") return "Chest";
    if (key == "gloves") return "Gloves";
    if (key == "pants") return "Pants";
    if (key == "boots") return "Boots";
    if (key == "ring1") return "Ring 1";
    if (key == "ring2") return "Ring 2";
    if (key == "amulet") return "Amulet";
    return key.c_str();
}

static std::string SlotLabel(const std::string& slot, const std::shared_ptr<Item>& item)
{
    std::string display = SlotDisplayName(slot);
    if (!item) return display + ": (none)";
    std::string s = display + ": " + item->name;

    // Show set tag
    std::string setName = GetSetNameFromItem(item);
    if (!setName.empty())
        s += " [" + setName + "]";

    if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
    {
        s += " (" + std::string(OffhandTypeName(oh->offhandType)) + ")";
        if (oh->defense > 0) s += " DEF:" + std::to_string(oh->defense);
        if (oh->damageBonus > 0) s += " DMG:" + std::to_string(oh->damageBonus);
        if (oh->manaBonus > 0) s += " MP:" + std::to_string(oh->manaBonus);
        if (oh->arcaneDamage > 0) s += " [Arcane:" + std::to_string(oh->arcaneDamage) + "]";
    }
    else if (auto w = std::dynamic_pointer_cast<Weapon>(item))
    {
        s += " (DMG:" + std::to_string(w->damage) + ")";
        if (w->element != ElementType::Physical)
            s += " [" + std::string(ElementName(w->element)) + ":" + std::to_string(w->elementDamage) + "]";
    }
    else if (auto a = std::dynamic_pointer_cast<Armor>(item))
    {
        s += " (DEF:" + std::to_string(a->defense) + ")";
        for (const auto& [elem, val] : a->elementalResist)
            s += " [" + std::string(ElementName(elem)) + "Res:" + std::to_string(val) + "]";
    }
    else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
    {
        s += " (HP:" + std::to_string(ac->bonusHealth) + " MP:" + std::to_string(ac->bonusMana) + ")";
        if (ac->element != ElementType::Physical)
            s += " [" + std::string(ElementName(ac->element)) + ":" + std::to_string(ac->elementDamage) + "]";
    }
    return s;
}

void Game::StateInventory()
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (!tutorialInventoryOpened) { tutorialInventoryOpened = true; AddTutorialHint("Inventory", "Equip gear to boost stats. Green arrows mean upgrades."); }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 50, GRenderer::W - 100, GRenderer::H - 110, "Inventory");

    auto& inv = player->GetInventory();
    auto& eq = player->GetEquipment();

    int xLeft = 70;
    int xRight = GRenderer::W / 2 + 60;
    int y = 100;

    // Static state for item comparison
    static int selectedItemIndex = -1;
    static std::shared_ptr<Item> selectedItem = nullptr;
    static int inventoryFocusRow = 0;

    // Tab buttons
    struct InvTab { const char* label; };
    static const InvTab tabs[] = {
        {"All"}, {"Weapons"}, {"Armor"}, {"Acc."}, {"Use"}, {"Other"}
    };
    static constexpr int tabCount = 6;

    // Number key shortcuts for tabs (1-6)
    for (int k = 0; k < tabCount; ++k)
    {
        if (IsKeyPressed(static_cast<KeyboardKey>(KEY_ONE + k)))
        {
            inventoryTab = k;
            inventoryFocusRow = 0;
            inventoryPage = 0;
            selectedItemIndex = -1;
            selectedItem = nullptr;
        }
    }
    // Tab / Shift+Tab to cycle tabs
    if (IsKeyPressed(KEY_TAB))
    {
        if (IsKeyDown(KEY_LEFT_SHIFT))
            inventoryTab = (inventoryTab - 1 + tabCount) % tabCount;
        else
            inventoryTab = (inventoryTab + 1) % tabCount;
        inventoryFocusRow = 0;
        inventoryPage = 0;
        selectedItemIndex = -1;
        selectedItem = nullptr;
    }

    int tabBtnW = 72;
    int tabBtnH = 24;
    int tabSpacing = 3;
    int tabStartX = xLeft;
    for (int t = 0; t < tabCount; ++t)
    {
        int tx = tabStartX + t * (tabBtnW + tabSpacing);
        Color bg = (t == inventoryTab) ? CQColors::Gold : CQColors::BgPanel;
        Color fg = (t == inventoryTab) ? CQColors::TextDark : CQColors::TextDim;
        if (renderer.Button(tabs[t].label, tx, y, tabBtnW, tabBtnH, t))
        {
            inventoryTab = t;
            inventoryPage = 0;
            inventoryFocusRow = 0;
            selectedItemIndex = -1;
            selectedItem = nullptr;
        }
    }
    y += 34;

    // Sort buttons
    static const char* sortLabels[] = {"Rarity", "Name", "Level", "Type"};
    static constexpr int sortCount = 4;
    int sortBtnW = 60;
    int sortBtnH = 20;
    int sortBtnSpacing = 3;
    int sortStartX = xLeft;
    renderer.DrawText("Sort:", sortStartX, y, 14, CQColors::TextDim);
    for (int s = 0; s < sortCount; ++s)
    {
        int sx = sortStartX + 40 + s * (sortBtnW + sortBtnSpacing);
        if (renderer.Button(sortLabels[s], sx, y, sortBtnW, sortBtnH, s))
        {
            if (inventorySortMode == s)
                inventorySortDirection *= -1;
            else
                inventorySortDirection = 1;
            inventorySortMode = s;
            inventoryPage = 0;
            inventoryFocusRow = 0;
        }
    }
    y += 24;

    // Gold & item count
    renderer.DrawText("Gold: " + std::to_string(inv.GetGold()), xLeft, y, 20, CQColors::TextGold);
    y += 30;

    // Vertical separator between panels
    int sepX = xRight - 20;
    renderer.DrawRect(sepX, 95, 1, GRenderer::H - 170, CQColors::BorderLight);

    // Build filtered list based on tab
    std::vector<size_t> filteredIndices;
    for (size_t i = 0; i < inv.GetItemCount(); ++i)
    {
        auto item = inv.GetItem(i);
        if (!item) continue;
        bool show = false;
        switch (inventoryTab)
        {
            case 0: show = true; break; // All
            case 1: show = (item->type == ItemType::Weapon || item->type == ItemType::Offhand); break;
            case 2: show = (item->type == ItemType::Armor); break;
            case 3: show = (item->type == ItemType::Accessory); break;
            case 4: show = (item->type == ItemType::Consumable || item->type == ItemType::Summoning); break;
            case 5: show = (item->type == ItemType::Resource || item->type == ItemType::QuestItem); break;
        }
        if (show) filteredIndices.push_back(i);
    }

    // Sort filtered items
    if (!filteredIndices.empty())
    {
        std::sort(filteredIndices.begin(), filteredIndices.end(),
            [&](size_t a, size_t b)
            {
                auto ia = inv.GetItem(a);
                auto ib = inv.GetItem(b);
                if (!ia || !ib) return false;
                int dir = inventorySortDirection;
                switch (inventorySortMode)
                {
                    case 0: // Rarity (highest first, or reversed)
                        return dir > 0 ? ia->rarity > ib->rarity : ia->rarity < ib->rarity;
                    case 1: // Name (alphabetical)
                        return dir > 0 ? ia->name < ib->name : ia->name > ib->name;
                    case 2: // Required level (highest first)
                        return dir > 0 ? ia->requiredLevel > ib->requiredLevel : ia->requiredLevel < ib->requiredLevel;
                    case 3: // Type, then rarity
                        if (ia->type != ib->type) return static_cast<int>(ia->type) < static_cast<int>(ib->type);
                        return dir > 0 ? ia->rarity > ib->rarity : ia->rarity < ib->rarity;
                    default:
                        return false;
                }
            });
    }

    int itemCount = static_cast<int>(filteredIndices.size());

    // Page bounds for the item list — declared up here so the arrow-handler
    // below can scroll across pages before the rendering loop runs.
    const int itemsPerPage = INVENTORY_ITEMS_PER_PAGE;
    int invMaxPage = (itemCount > itemsPerPage) ? ((itemCount - 1) / itemsPerPage) : 0;
    if (inventoryPage < 0) inventoryPage = 0;
    if (inventoryPage > invMaxPage) inventoryPage = invMaxPage;
    int invStart = inventoryPage * itemsPerPage;
    int invEnd = std::min(invStart + itemsPerPage, itemCount);

    // Keep focus row clamped to a valid index inside the filtered list
    if (itemCount > 0)
    {
        if (inventoryFocusRow < 0) inventoryFocusRow = invStart;
        if (inventoryFocusRow >= itemCount) inventoryFocusRow = itemCount - 1;
    }

    // Arrow key navigation for item list — also scrolls the page when focus
    // crosses the visible window.
    if (IsKeyPressed(KEY_DOWN) && inventoryFocusRow < itemCount - 1)
    {
        inventoryFocusRow++;
        if (inventoryFocusRow >= invEnd && inventoryPage < invMaxPage) inventoryPage++;
    }
    if (IsKeyPressed(KEY_UP) && inventoryFocusRow > 0)
    {
        inventoryFocusRow--;
        if (inventoryFocusRow < invStart && inventoryPage > 0) inventoryPage--;
    }
    // Re-clamp page after potential auto-advance above
    invStart = inventoryPage * itemsPerPage;
    invEnd = std::min(invStart + itemsPerPage, itemCount);
    if (IsKeyPressed(KEY_ENTER) && itemCount > 0 && inventoryFocusRow < static_cast<int>(filteredIndices.size()))
    {
        size_t i = filteredIndices[inventoryFocusRow];
        auto item = inv.GetItem(i);
        if (item)
        {
            if (selectedItemIndex == static_cast<int>(i) && item == selectedItem)
            {
                selectedItemIndex = -1;
                selectedItem = nullptr;
            }
            else
            {
                selectedItemIndex = static_cast<int>(i);
                selectedItem = item;
            }
        }
    }
    if (IsKeyPressed(KEY_DELETE) && itemCount > 0 && inventoryFocusRow < static_cast<int>(filteredIndices.size()))
    {
        size_t i = filteredIndices[inventoryFocusRow];
        inv.RemoveOneItem(i);
        if (selectedItemIndex == static_cast<int>(i))
        {
            selectedItemIndex = -1;
            selectedItem = nullptr;
        }
        if (inventoryFocusRow >= itemCount - 1 && inventoryFocusRow > 0)
            inventoryFocusRow--;
        invMaxPage = (itemCount > itemsPerPage) ? ((itemCount - 1) / itemsPerPage) : 0;
        if (inventoryPage > invMaxPage) inventoryPage = invMaxPage;
    }

    int visibleCount = invEnd - invStart;
    // Keep focus row inside the current page while still indexing into filteredIndices
    if (itemCount > 0)
    {
        if (inventoryFocusRow < invStart) inventoryFocusRow = invStart;
        if (inventoryFocusRow >= invEnd) inventoryFocusRow = invEnd - 1;
    }

    // Pre-compute action button count per visible item for focus indexing
    std::vector<int> actionCountPerItem(visibleCount, 1); // every item has at least "D"
    for (int idx = 0; idx < visibleCount; ++idx)
    {
        size_t i = filteredIndices[invStart + idx];
        auto item = inv.GetItem(i);
        if (!item) continue;
        if (item->type == ItemType::Weapon || item->type == ItemType::Armor || item->type == ItemType::Accessory || item->type == ItemType::Offhand)
        {
            actionCountPerItem[idx] = 1; // D
            if (player->CanEquip(item)) actionCountPerItem[idx]++; // Eq
            actionCountPerItem[idx]++; // ?
        }
        else if (item->type == ItemType::Consumable)
        {
            actionCountPerItem[idx] = 2; // Use + D
        }
        else if (item->type == ItemType::Summoning)
        {
            actionCountPerItem[idx] = 2; // Summon + D
        }
        else if (item->type == ItemType::Resource)
        {
            auto res = std::dynamic_pointer_cast<Resource>(item);
            actionCountPerItem[idx] = (res && res->healAmount > 0) ? 2 : 1; // Eat+D or D
        }
    }
    int totalActionButtons = 0;
    for (int c : actionCountPerItem) totalActionButtons += c;
    keyboardNav.SetFocusCount(tabCount + totalActionButtons + 2 + 1); // tabs + actions + Prev/Next + Back

    renderer.DrawText("Items: " + std::to_string(itemCount), xLeft, y, 16, CQColors::TextDim);
    y += 26;

    // ---- Inventory items (left side) ----
    if (itemCount == 0)
    {
        renderer.DrawText("No items in this category.", xLeft, y, 18, CQColors::TextLight);
    }
    else
    {
        int startY = y;
        int itemFocusIdx = tabCount; // action buttons start after tabs
        const int rowH = 50;
        const int cardW = 420;
        for (int idx = 0; idx < visibleCount; ++idx)
        {
            size_t i = filteredIndices[invStart + idx];
            auto item = inv.GetItem(i);
            if (!item) continue;
            int iy = startY + idx * rowH;

            // Check if this item is selected for comparison
            bool isSelected = (static_cast<int>(i) == selectedItemIndex && item == selectedItem);
            Color rarityColor = RarityColor(static_cast<Rarity>(item->rarity));
            Color cardBg = CQColors::BgDark;
            cardBg.a = 200;

            // Highlight keyboard-focused item
            bool isFocused = (idx == (inventoryFocusRow - invStart));
            if (isFocused)
            {
                renderer.DrawRect(xLeft - 4, iy - 4, cardW, rowH - 4, Color{40, 40, 70, 160});
                keyboardNav.DrawFocusRect(xLeft - 4, iy - 4, cardW, rowH - 4, CQColors::TextDim);
            }

            // Card background
            renderer.DrawRect(xLeft - 4, iy - 4, cardW, rowH - 4, cardBg);

            // Rarity-colored left border bar (4px wide)
            Color barColor = rarityColor;
            barColor.a = 255;
            renderer.DrawRect(xLeft - 4, iy - 4, 4, rowH - 4, barColor);

            // Rarity glow effect for Epic/Legendary items
            if (item->rarity >= static_cast<int>(Rarity::Epic))
            {
                unsigned char glowAlpha = static_cast<unsigned char>(
                    40 + 30.0 * std::sin(renderer.GetTime() * 3.0));
                Color glowColor = rarityColor;
                glowColor.a = glowAlpha;
                renderer.DrawRect(xLeft - 4, iy - 4, cardW, rowH - 4, glowColor);
            }

            // Card border (dim if normal, rarity-colored for rare+)
            Color borderColor = (item->rarity >= static_cast<int>(Rarity::Rare)) ? rarityColor : CQColors::BorderLight;
            borderColor.a = (item->rarity >= static_cast<int>(Rarity::Epic)) ? 200 : 120;
            renderer.DrawRectLines(xLeft - 4, iy - 4, cardW, rowH - 4, borderColor, 1);

            // Icon with rarity background
            DrawItemIcon(item->name, item->rarity, xLeft + 8, iy, 28);

            // Line 1: Name (rarity color, bold)
            Color nameColor = isSelected ? CQColors::TextGold : rarityColor;
            std::string name = item->name;
            if (item->count > 1) name += " x" + std::to_string(item->count);
            int maxNameW = cardW - 80;
            while (!name.empty() && MeasureText(name.c_str(), 16) > maxNameW)
                name.pop_back();
            renderer.DrawText(name, xLeft + 44, iy + 2, 16, nameColor);

            // Set name badge next to item name
            std::string setName = GetSetNameFromItem(item);
            if (!setName.empty())
            {
                int nameW = MeasureText(name.c_str(), 16);
                std::string setBadge = "[" + setName + "]";
                int badgeX = xLeft + 44 + nameW + 6;
                renderer.DrawText(setBadge, badgeX, iy + 4, 11, CQColors::TextGreen);
            }

            // Line 2: Stats summary (dim)
            std::string stats;
            if (auto w = std::dynamic_pointer_cast<Weapon>(item))
            {
                stats = std::string(WeaponTypeName(w->weaponType)) + " | DMG:" + std::to_string(w->damage);
                if (w->element != ElementType::Physical)
                    stats += " | " + std::string(ElementName(w->element)) + ":" + std::to_string(w->elementDamage);
                if (w->manaCost > 0) stats += " | MP:" + std::to_string(w->manaCost);
            }
            else if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
            {
                stats = std::string(OffhandTypeName(oh->offhandType));
                if (oh->defense > 0) stats += " | DEF:" + std::to_string(oh->defense);
                if (oh->damageBonus > 0) stats += " | DMG:" + std::to_string(oh->damageBonus);
                if (oh->manaBonus > 0) stats += " | MP:" + std::to_string(oh->manaBonus);
                if (oh->arcaneDamage > 0) stats += " | Arcane:" + std::to_string(oh->arcaneDamage);
            }
            else if (auto a = std::dynamic_pointer_cast<Armor>(item))
            {
                std::string pn;
                switch (a->piece) {
                    case ArmorPiece::Helmet: pn = "Helmet"; break;
                    case ArmorPiece::Chest:  pn = "Chest"; break;
                    case ArmorPiece::Gloves: pn = "Gloves"; break;
                    case ArmorPiece::Pants:  pn = "Pants"; break;
                    case ArmorPiece::Boots:  pn = "Boots"; break;
                }
                stats = pn + " | DEF:" + std::to_string(a->defense);
                for (const auto& [elem, val] : a->elementalResist)
                    stats += " | " + std::string(ElementName(elem)) + " Res:" + std::to_string(val);
            }
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
            {
                stats = "Accessory";
                if (ac->bonusHealth > 0) stats += " | HP:" + std::to_string(ac->bonusHealth);
                if (ac->bonusMana > 0) stats += " | MP:" + std::to_string(ac->bonusMana);
                if (ac->element != ElementType::Physical)
                    stats += " | " + std::string(ElementName(ac->element)) + ":" + std::to_string(ac->elementDamage);
            }
            else if (auto con = std::dynamic_pointer_cast<Consumable>(item))
            {
                stats = con->GetDescription();
            }
            else if (auto sum = std::dynamic_pointer_cast<SummoningItem>(item))
            {
                stats = "Summon " + sum->bossName;
            }
            else if (auto res = std::dynamic_pointer_cast<Resource>(item))
            {
                stats = "Resource";
                if (res->healAmount > 0) stats += " | Heals:" + std::to_string(res->healAmount);
                if (res->manaAmount > 0) stats += " | MP:" + std::to_string(res->manaAmount);
                if (res->quality != ResourceQuality::Normal)
                    stats += " | " + std::string(ResourceQualityName(res->quality));
            }
            int maxStatsW = cardW - 80;
            while (!stats.empty() && MeasureText(stats.c_str(), 12) > maxStatsW)
                stats.pop_back();
            renderer.DrawText(stats, xLeft + 44, iy + 22, 12, CQColors::TextDim);

            // Show passive text if any
            std::string passiveText = GetPassiveText(item);
            if (!passiveText.empty())
            {
                std::string pt = "Passive: " + passiveText;
                while (!pt.empty() && MeasureText(pt.c_str(), 11) > maxStatsW)
                    pt.pop_back();
                renderer.DrawText(pt, xLeft + 44, iy + 36, 11, CQColors::TextGreen);
            }

            // Tooltip on hover over icon
            if (renderer.IsMouseInRect(xLeft - 2, iy - 4, 36, rowH - 4))
            {
                std::string tip = item->name;
                tip += "\nRarity: " + std::string(RarityName(static_cast<Rarity>(item->rarity)));
                if (item->requiredLevel > 1)
                    tip += "\nReq. Level: " + std::to_string(item->requiredLevel);
                if (auto w = std::dynamic_pointer_cast<Weapon>(item))
                {
                    tip += "\nDamage: " + std::to_string(w->damage);
                    tip += "\nWeapon: " + std::string(WeaponTypeName(w->weaponType));
                    if (w->element != ElementType::Physical)
                        tip += "\n" + std::string(ElementName(w->element)) + " Damage: " + std::to_string(w->elementDamage);
                }
                else if (auto a = std::dynamic_pointer_cast<Armor>(item))
                {
                    tip += "\nDefense: " + std::to_string(a->defense);
                    for (const auto& [elem, val] : a->elementalResist)
                        if (val > 0) tip += "\n" + std::string(ElementName(elem)) + " Resist: " + std::to_string(val);
                }
                else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
                {
                    if (ac->bonusHealth > 0) tip += "\n+" + std::to_string(ac->bonusHealth) + " HP";
                    if (ac->bonusMana > 0) tip += "\n+" + std::to_string(ac->bonusMana) + " MP";
                    if (ac->element != ElementType::Physical)
                        tip += "\n" + std::string(ElementName(ac->element)) + " Damage: " + std::to_string(ac->elementDamage);
                }
                else if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
                {
                    if (oh->defense > 0) tip += "\nDefense: " + std::to_string(oh->defense);
                    if (oh->damageBonus > 0) tip += "\nDamage: +" + std::to_string(oh->damageBonus);
                    if (oh->manaBonus > 0) tip += "\n+" + std::to_string(oh->manaBonus) + " MP";
                    if (oh->arcaneDamage > 0) tip += "\nArcane: " + std::to_string(oh->arcaneDamage);
                }
                else if (auto con = std::dynamic_pointer_cast<Consumable>(item))
                {
                    tip += "\n" + con->GetDescription();
                }
                else if (auto sum = std::dynamic_pointer_cast<SummoningItem>(item))
                {
                    tip += "\n" + sum->GetDescription();
                    tip += "\nUse from Explore to start the fight";
                }
                else if (auto res = std::dynamic_pointer_cast<Resource>(item))
                {
                    if (res->healAmount > 0) tip += "\nHeals: " + std::to_string(res->healAmount) + " HP";
                    if (res->manaAmount > 0) tip += "\nRestores: " + std::to_string(res->manaAmount) + " MP";
                }
                if (item->count > 1) tip += "\nQuantity: " + std::to_string(item->count);
                renderer.DrawTooltip(tip, static_cast<int>(GetMousePosition().x), static_cast<int>(GetMousePosition().y));
            }

            // Action buttons (right side of card)
            int btnX = xLeft + cardW - 110;
            int btnY = iy + 8;
            if (item->type == ItemType::Weapon || item->type == ItemType::Armor || item->type == ItemType::Accessory || item->type == ItemType::Offhand)
            {
                bool canEquip = player->CanEquip(item);
                if (canEquip)
                {
                    if (renderer.Button("Equip", btnX, btnY, 50, 22, itemFocusIdx++))
                        player->EquipItem(item);
                }
                else
                {
                    Color c = CQColors::TextDim; c.a = 100;
                    renderer.DrawText("Equip", btnX + 4, btnY + 4, 12, c);
                }
                btnX += 56;
                // Compare button
                if (renderer.Button("?", btnX, btnY, 22, 22, itemFocusIdx++))
                {
                    if (isSelected)
                    {
                        selectedItemIndex = -1;
                        selectedItem = nullptr;
                    }
                    else
                    {
                        selectedItemIndex = static_cast<int>(i);
                        selectedItem = item;
                    }
                }
                btnX += 26;
            }
            else if (item->type == ItemType::Consumable)
            {
                auto con = std::dynamic_pointer_cast<Consumable>(item);
                if (con && renderer.Button("Use", btnX, btnY, 50, 22, itemFocusIdx++))
                {
                    con->Use(*player);
                    inv.RemoveOneItem(i);
                    if (selectedItemIndex == static_cast<int>(i))
                    {
                        selectedItemIndex = -1;
                        selectedItem = nullptr;
                    }
                }
                btnX += 56;
            }
            else if (item->type == ItemType::Summoning)
            {
                auto sum = std::dynamic_pointer_cast<SummoningItem>(item);
                if (sum && renderer.Button("Summon", btnX, btnY, 60, 22, itemFocusIdx++))
                {
                    auto boss = sum->bossFactory();
                    inv.RemoveOneItem(i);
                    if (selectedItemIndex == static_cast<int>(i))
                    {
                        selectedItemIndex = -1;
                        selectedItem = nullptr;
                    }
                    StartCombatWithEnemy(boss, true);
                    return;
                }
                btnX += 66;
            }
            else if (item->type == ItemType::Resource)
            {
                // Check if this resource is edible (has healing properties, e.g. fish)
                auto res = std::dynamic_pointer_cast<Resource>(item);
                if (res && res->healAmount > 0)
                {
                    if (renderer.Button("Eat", btnX, btnY, 50, 22, itemFocusIdx++))
                    {
                        if (res->healAmount > 0) player->RestoreHealth(res->healAmount);
                        if (res->manaAmount > 0) player->RestoreMana(res->manaAmount);
                        inv.RemoveOneItem(i);
                        if (selectedItemIndex == static_cast<int>(i))
                        {
                            selectedItemIndex = -1;
                            selectedItem = nullptr;
                        }
                    }
                    btnX += 56;
                }
            }
            // Drop button (always available)
            if (renderer.Button("Drop", btnX, btnY, 38, 22, itemFocusIdx++))
            {
                inv.RemoveOneItem(i);
                if (selectedItemIndex == static_cast<int>(i))
                {
                    selectedItemIndex = -1;
                    selectedItem = nullptr;
                }
            }
        }

        // Page navigation footer (below item list)
        int pageNavY = startY + visibleCount * rowH + 6;
        if (invMaxPage > 0)
        {
            if (renderer.Button("< Prev", xLeft, pageNavY, 88, 24, itemFocusIdx++) && inventoryPage > 0)
            {
                inventoryPage--;
                inventoryFocusRow = inventoryPage * itemsPerPage;
            }
            if (renderer.Button("Next >", xLeft + 94, pageNavY, 88, 24, itemFocusIdx++) && inventoryPage < invMaxPage)
            {
                inventoryPage++;
                inventoryFocusRow = inventoryPage * itemsPerPage;
            }
            renderer.DrawText("Page " + std::to_string(inventoryPage + 1) + " / " + std::to_string(invMaxPage + 1),
                              xLeft + 188, pageNavY + 4, 12, CQColors::TextDim);
        }
    }

    // ---- Equipment comparison panel ----
    if (selectedItem)
    {
        int panelX = xLeft;
        int panelY = GRenderer::H - 250;
        int panelW = GRenderer::W - 140;
        int panelH = 170;

        renderer.DrawRect(panelX, panelY, panelW, panelH, CQColors::BgPanel);
        renderer.DrawRectLines(panelX, panelY, panelW, panelH, CQColors::BorderLight, 1);

        // Show selected item details
        Color selectedRarityColor = RarityColor(static_cast<Rarity>(selectedItem->rarity));
        std::string selectedSetName = GetSetNameFromItem(selectedItem);
        std::string selectedHeader = selectedItem->name;
        if (!selectedSetName.empty())
            selectedHeader += " [" + selectedSetName + "]";
        if (selectedItem->requiredLevel > 1)
            selectedHeader += " (Lv " + std::to_string(selectedItem->requiredLevel) + ")";
        renderer.DrawText("Selected: " + selectedHeader, panelX + 10, panelY + 10, 16, selectedRarityColor);

        // Get currently equipped item for comparison
        std::shared_ptr<Item> equippedItem = nullptr;
        std::string slotName;

        if (auto w = std::dynamic_pointer_cast<Weapon>(selectedItem))
        {
            equippedItem = eq.weapon;
            slotName = "Weapon";
        }
        else if (auto oh = std::dynamic_pointer_cast<Offhand>(selectedItem))
        {
            equippedItem = eq.offhand;
            slotName = "Offhand";
        }
        else if (auto a = std::dynamic_pointer_cast<Armor>(selectedItem))
        {
            switch (a->piece) {
                case ArmorPiece::Helmet: equippedItem = eq.helmet; slotName = "Helmet"; break;
                case ArmorPiece::Chest:  equippedItem = eq.chest;  slotName = "Chest"; break;
                case ArmorPiece::Gloves: equippedItem = eq.gloves; slotName = "Gloves"; break;
                case ArmorPiece::Pants:  equippedItem = eq.pants;  slotName = "Pants"; break;
                case ArmorPiece::Boots:  equippedItem = eq.boots;  slotName = "Boots"; break;
            }
        }
        else if (auto ac = std::dynamic_pointer_cast<Accessory>(selectedItem))
        {
            // Check which ring slot or amulet
            if (!eq.ring1) { equippedItem = nullptr; slotName = "Ring 1 (empty)"; }
            else if (!eq.ring2) { equippedItem = nullptr; slotName = "Ring 2 (empty)"; }
            else { equippedItem = eq.amulet; slotName = "Amulet"; }
        }

        // Display comparison
        int compX = panelX + 10;
        int compY = panelY + 35;

        if (equippedItem)
        {
            renderer.DrawText("Currently Equipped (" + slotName + "):", compX, compY, 14, CQColors::TextDim);
            compY += 20;

            // Show equipped item stats
            if (auto ew = std::dynamic_pointer_cast<Weapon>(equippedItem))
            {
                std::string info = equippedItem->name + " - DMG:" + std::to_string(ew->damage);
                if (ew->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(ew->element)) + ":" + std::to_string(ew->elementDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, RarityColor(static_cast<Rarity>(equippedItem->rarity)));
            }
            else if (auto eoh = std::dynamic_pointer_cast<Offhand>(equippedItem))
            {
                std::string info = equippedItem->name;
                if (eoh->defense > 0) info += " - DEF:" + std::to_string(eoh->defense);
                if (eoh->damageBonus > 0) info += " DMG:" + std::to_string(eoh->damageBonus);
                if (eoh->manaBonus > 0) info += " MP:" + std::to_string(eoh->manaBonus);
                if (eoh->arcaneDamage > 0) info += " [Arcane:" + std::to_string(eoh->arcaneDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, RarityColor(static_cast<Rarity>(equippedItem->rarity)));
            }
            else if (auto ea = std::dynamic_pointer_cast<Armor>(equippedItem))
            {
                std::string info = equippedItem->name + " - DEF:" + std::to_string(ea->defense);
                for (const auto& [elem, val] : ea->elementalResist)
                    info += " [" + std::string(ElementName(elem)) + "Res:" + std::to_string(val) + "]";
                renderer.DrawText(info, compX, compY, 14, RarityColor(static_cast<Rarity>(equippedItem->rarity)));
            }
            else if (auto eac = std::dynamic_pointer_cast<Accessory>(equippedItem))
            {
                std::string info = equippedItem->name + " - HP:" + std::to_string(eac->bonusHealth)
                    + " MP:" + std::to_string(eac->bonusMana);
                if (eac->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(eac->element)) + ":" + std::to_string(eac->elementDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, RarityColor(static_cast<Rarity>(equippedItem->rarity)));
            }
            compY += 20;

            // Show equipped item passive
            std::string eqPassive = GetPassiveText(equippedItem);
            if (!eqPassive.empty())
            {
                while (eqPassive.size() > 80) eqPassive.pop_back();
                renderer.DrawText("Passive: " + eqPassive, compX, compY, 12, CQColors::TextGreen);
                compY += 16;
            }

            // Show new item stats
            renderer.DrawText("New Item:", compX + 300, panelY + 35, 14, CQColors::TextDim);
            int newCompY = panelY + 55;
            if (auto nw = std::dynamic_pointer_cast<Weapon>(selectedItem))
            {
                std::string info = selectedItem->name + " - DMG:" + std::to_string(nw->damage);
                if (nw->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(nw->element)) + ":" + std::to_string(nw->elementDamage) + "]";
                renderer.DrawText(info, compX + 300, newCompY, 14, selectedRarityColor);
            }
            else if (auto noh = std::dynamic_pointer_cast<Offhand>(selectedItem))
            {
                std::string info = selectedItem->name;
                if (noh->defense > 0) info += " - DEF:" + std::to_string(noh->defense);
                if (noh->damageBonus > 0) info += " DMG:" + std::to_string(noh->damageBonus);
                if (noh->manaBonus > 0) info += " MP:" + std::to_string(noh->manaBonus);
                if (noh->arcaneDamage > 0) info += " [Arcane:" + std::to_string(noh->arcaneDamage) + "]";
                renderer.DrawText(info, compX + 300, newCompY, 14, selectedRarityColor);
            }
            else if (auto na = std::dynamic_pointer_cast<Armor>(selectedItem))
            {
                std::string info = selectedItem->name + " - DEF:" + std::to_string(na->defense);
                for (const auto& [elem, val] : na->elementalResist)
                    info += " [" + std::string(ElementName(elem)) + "Res:" + std::to_string(val) + "]";
                renderer.DrawText(info, compX + 300, newCompY, 14, selectedRarityColor);
            }
            else if (auto nac = std::dynamic_pointer_cast<Accessory>(selectedItem))
            {
                std::string info = selectedItem->name + " - HP:" + std::to_string(nac->bonusHealth)
                    + " MP:" + std::to_string(nac->bonusMana);
                if (nac->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(nac->element)) + ":" + std::to_string(nac->elementDamage) + "]";
                renderer.DrawText(info, compX + 300, newCompY, 14, selectedRarityColor);
            }
            newCompY += 20;

            // Show new item passive
            std::string newPassive = GetPassiveText(selectedItem);
            if (!newPassive.empty())
            {
                while (newPassive.size() > 80) newPassive.pop_back();
                renderer.DrawText("Passive: " + newPassive, compX + 300, newCompY, 12, CQColors::TextGreen);
            }
        }
        else
        {
            renderer.DrawText("No item currently equipped in " + slotName, compX, compY, 14, CQColors::TextDim);
            compY += 20;

            // Show new item stats
            if (auto nw = std::dynamic_pointer_cast<Weapon>(selectedItem))
            {
                std::string info = "New: " + selectedItem->name + " - DMG:" + std::to_string(nw->damage);
                if (nw->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(nw->element)) + ":" + std::to_string(nw->elementDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, selectedRarityColor);
            }
            else if (auto noh = std::dynamic_pointer_cast<Offhand>(selectedItem))
            {
                std::string info = "New: " + selectedItem->name;
                if (noh->defense > 0) info += " - DEF:" + std::to_string(noh->defense);
                if (noh->damageBonus > 0) info += " DMG:" + std::to_string(noh->damageBonus);
                if (noh->manaBonus > 0) info += " MP:" + std::to_string(noh->manaBonus);
                if (noh->arcaneDamage > 0) info += " [Arcane:" + std::to_string(noh->arcaneDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, selectedRarityColor);
            }
            else if (auto na = std::dynamic_pointer_cast<Armor>(selectedItem))
            {
                std::string info = "New: " + selectedItem->name + " - DEF:" + std::to_string(na->defense);
                for (const auto& [elem, val] : na->elementalResist)
                    info += " [" + std::string(ElementName(elem)) + "Res:" + std::to_string(val) + "]";
                renderer.DrawText(info, compX, compY, 14, selectedRarityColor);
            }
            else if (auto nac = std::dynamic_pointer_cast<Accessory>(selectedItem))
            {
                std::string info = "New: " + selectedItem->name + " - HP:" + std::to_string(nac->bonusHealth)
                    + " MP:" + std::to_string(nac->bonusMana);
                if (nac->element != ElementType::Physical)
                    info += " [" + std::string(ElementName(nac->element)) + ":" + std::to_string(nac->elementDamage) + "]";
                renderer.DrawText(info, compX, compY, 14, selectedRarityColor);
            }
            compY += 20;

            // Show new item passive
            std::string newPassive = GetPassiveText(selectedItem);
            if (!newPassive.empty())
            {
                while (newPassive.size() > 80) newPassive.pop_back();
                renderer.DrawText("Passive: " + newPassive, compX, compY, 12, CQColors::TextGreen);
            }
        }
    }

    // ---- Equipment panel (right side) ----
    y = 100;
    renderer.DrawText("Equipped:", xRight, y, 18, CQColors::TextGold);
    y += 24;

    // Count equipped items
    int equippedCount = 0;
    if (eq.weapon) equippedCount++;
    if (eq.offhand) equippedCount++;
    if (eq.helmet) equippedCount++;
    if (eq.chest) equippedCount++;
    if (eq.gloves) equippedCount++;
    if (eq.pants) equippedCount++;
    if (eq.boots) equippedCount++;
    if (eq.ring1) equippedCount++;
    if (eq.ring2) equippedCount++;
    if (eq.amulet) equippedCount++;

    // Calculate power level (rough estimate)
    int totalDef = eq.GetTotalDefense() + player->GetStats().defense;
    int totalDmg = eq.GetWeaponDamage();
    int totalHP = player->GetMaxHealth();
    int totalMP = player->GetMaxMana();
    int powerLevel = totalDef + totalDmg + (totalHP / 5) + (totalMP / 5);

    // Equipment summary header
    renderer.DrawText(std::to_string(equippedCount) + "/10 slots filled", xRight, y, 13, CQColors::TextDim);
    y += 16;
    renderer.DrawText("Power: " + std::to_string(powerLevel), xRight, y, 14, CQColors::TextGold);
    y += 18;

    // Stats summary line
    std::string statsSummary = "DEF:" + std::to_string(totalDef)
        + "  DMG:" + std::to_string(totalDmg)
        + "  HP:" + std::to_string(totalHP)
        + "  MP:" + std::to_string(totalMP);
    renderer.DrawText(statsSummary, xRight, y, 12, CQColors::TextLight);
    y += 20;

    // Separator line
    renderer.DrawRect(xRight, y, 380, 1, CQColors::BorderLight);
    y += 6;

    struct SlotBind { std::string name; std::shared_ptr<Item> item; };
    auto slots = std::vector<SlotBind>{
        {"weapon", eq.weapon}, {"offhand", eq.offhand},
        {"helmet", eq.helmet}, {"chest", eq.chest},
        {"gloves", eq.gloves}, {"pants", eq.pants}, {"boots", eq.boots},
        {"ring1", eq.ring1}, {"ring2", eq.ring2}, {"amulet", eq.amulet}
    };

    for (auto& s : slots)
    {
        std::string displayName = SlotDisplayName(s.name);
        Color slotColor = s.item ? RarityColor(static_cast<Rarity>(s.item->rarity)) : CQColors::TextDim;

        // Slot background box
        int slotBoxH = s.item && s.item->HasAnyPassive() ? 32 : 20;
        Color slotBg = s.item ? Color{40, 30, 20, 180} : Color{30, 20, 15, 120};
        renderer.DrawRect(xRight - 4, y - 3, 390, slotBoxH, slotBg);

        // Rarity border for equipped items
        if (s.item)
        {
            Color rarityBorder = RarityColor(static_cast<Rarity>(s.item->rarity));
            rarityBorder.a = 120;
            renderer.DrawRectLines(xRight - 4, y - 3, 390, slotBoxH, rarityBorder, 1);
        }

        // Icon + slot name + item name + key stat — all on one line
        if (s.item)
            DrawItemIcon(s.item->name, s.item->rarity, xRight, y - 1, 14);

        std::string line = displayName + ": ";
        if (s.item)
        {
            line += s.item->name;
            // Append key stat inline
            if (auto w = std::dynamic_pointer_cast<Weapon>(s.item))
            {
                line += " DMG:" + std::to_string(w->damage);
                if (w->element != ElementType::Physical)
                    line += " " + std::string(ElementName(w->element)) + ":" + std::to_string(w->elementDamage);
            }
            else if (auto oh = std::dynamic_pointer_cast<Offhand>(s.item))
            {
                if (oh->defense > 0) line += " DEF:" + std::to_string(oh->defense);
                if (oh->damageBonus > 0) line += " DMG:" + std::to_string(oh->damageBonus);
                if (oh->manaBonus > 0) line += " MP:" + std::to_string(oh->manaBonus);
                if (oh->arcaneDamage > 0) line += " [Arcane:" + std::to_string(oh->arcaneDamage) + "]";
            }
            else if (auto a = std::dynamic_pointer_cast<Armor>(s.item))
            {
                line += " DEF:" + std::to_string(a->defense);
                for (const auto& [elem, val] : a->elementalResist)
                    line += " " + std::string(ElementName(elem)) + ":" + std::to_string(val);
            }
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(s.item))
            {
                line += " HP:" + std::to_string(ac->bonusHealth) + " MP:" + std::to_string(ac->bonusMana);
                if (ac->element != ElementType::Physical)
                    line += " " + std::string(ElementName(ac->element)) + ":" + std::to_string(ac->elementDamage);
            }
        }
        else
        {
            line += "(empty)";
        }

        while (line.size() > 48) line.pop_back();
        renderer.DrawText(line, xRight + 18, y, 13, slotColor);

        // Unequip button
        if (s.item)
        {
            if (renderer.Button("X##" + s.name, xRight + 350, y - 2, 28, 18))
                player->UnequipItem(s.name);
        }
        y += 16;

        // Passive line (only if item has passives)
        if (s.item && s.item->HasAnyPassive())
        {
            std::string passive = GetPassiveText(s.item);
            if (!passive.empty())
            {
                while (passive.size() > 52) passive.pop_back();
                renderer.DrawText("  " + passive, xRight + 18, y, 11, CQColors::TextGreen);
                y += 14;
            }
        }
        y += 4;
    }

    // ---- Set Bonus Display ----
    y += 4;
    DrawSetBonusSection(renderer, eq, xRight, y, 350);

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, tabCount + totalActionButtons))
    {
        selectedItemIndex = -1;
        selectedItem = nullptr;
        currentState = GameState::Exploring;
    }
}

void Game::StateStats()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Character Stats");
    int y = 110;
    auto s = player->GetStats();
    std::string cn;
    switch (player->GetCharacterClass()) {
        case CharacterClass::Warrior:  cn = "Warrior";  break;
        case CharacterClass::Priest:   cn = "Priest";   break;
        case CharacterClass::Mage:     cn = "Mage";     break;
        case CharacterClass::Archer:   cn = "Archer";   break;
        case CharacterClass::Merchant: cn = "Merchant"; break;
    }
    std::vector<std::string> lines = {
        "Name: " + player->GetName() + "   Level: " + std::to_string(player->GetLevel()),
        "Class: " + cn,
        "Race: " + RaceDatabase::Get(player->GetRace()).name,
        "",
        "HP: " + std::to_string(player->GetCurrentHealth()) + "/" + std::to_string(player->GetMaxHealth())
            + "   MP: " + std::to_string(player->GetCurrentMana()) + "/" + std::to_string(player->GetMaxMana()),
        "Strength: " + std::to_string(s.strength) + "   Dexterity: " + std::to_string(s.dexterity),
        "Intelligence: " + std::to_string(s.intelligence) + "   Wisdom: " + std::to_string(s.wisdom),
        "Vitality: " + std::to_string(s.vitality) + "   Defense: " + std::to_string(s.defense),
        "",
        "XP: " + std::to_string(player->GetExperience()) + " / "
            + std::to_string(Character::CalculateRequiredXP(player->GetLevel()))
    };
    for (const auto& l : lines) { renderer.DrawText(l, 70, y, 18, CQColors::TextLight); y += 24; }
    if (religion.GetGod() != GodType::None)
    {
        y += 10;
        renderer.DrawText("Faith: " + religion.GetGodName()
            + " (Devotion " + std::to_string(religion.GetDevotionLevel()) + ")"
            + "  Donated: " + std::to_string(religion.GetTotalDonated()) + " gold",
            70, y, 16, CQColors::TextGold);
        y += 30;
    }
    y += 10;
    renderer.DrawText("Skills:", 70, y, 20, CQColors::TextGold);
    y += 28;
    const int skillCount = static_cast<int>(player->GetSkills().GetSkillCount());
    int skillMaxPage = (skillCount > SKILLS_PER_PAGE) ? ((skillCount - 1) / SKILLS_PER_PAGE) : 0;
    if (skillStatsPage < 0) skillStatsPage = 0;
    if (skillStatsPage > skillMaxPage) skillStatsPage = skillMaxPage;
    int skillStart = skillStatsPage * SKILLS_PER_PAGE;
    int skillEnd = std::min(skillStart + SKILLS_PER_PAGE, skillCount);
    int skillFocusIdx = 0;
    for (int i = skillStart; i < skillEnd; ++i)
    {
        auto sk = player->GetSkills().GetSkill(i);
        if (!sk) continue;
        int needed = Skill::GetXPToLevel(sk->level);
        std::string cdText = sk->currentCooldown > 0 ? " (CD:" + std::to_string(sk->currentCooldown) + ")" : "";
        renderer.DrawText(std::to_string(i + 1) + ". " + sk->name
            + " Lv." + std::to_string(sk->level)
            + " XP:" + std::to_string(sk->experience) + "/" + std::to_string(needed)
            + cdText
            + " Req:" + std::to_string(sk->requiredLevel),
            80, y, 15, CQColors::TextLight);
        y += 22;
    }
    if (skillMaxPage > 0)
    {
        int navY = GRenderer::H - 130;
        if (renderer.Button("< Prev", 70, navY, 100, 32, skillFocusIdx++) && skillStatsPage > 0) skillStatsPage--;
        if (renderer.Button("Next >", 180, navY, 100, 32, skillFocusIdx++) && skillStatsPage < skillMaxPage) skillStatsPage++;
        renderer.DrawText("Page " + std::to_string(skillStatsPage + 1) + " / " + std::to_string(skillMaxPage + 1),
                          290, navY + 6, 13, CQColors::TextDim);
    }
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, skillFocusIdx))
        currentState = GameState::Exploring;
}

void Game::StateJobs()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Jobs");
    int y = 110;
    auto& js = player->GetJobSystem();

    // Job-specific colors
    Color jobColors[] = {
        {180, 120, 60, 255},   // Mining - brown/copper
        {60, 160, 60, 255},    // Lumberjacking - green
        {60, 120, 200, 255},   // Fishing - blue
        {200, 140, 50, 255}    // Smithing - orange
    };

    // Draw each job with progress bar
    auto drawJob = [&](int yy, JobType jt, const char* label, Color barColor, int focusIdx) {
        auto& job = js.GetJob(jt);
        int required = Job::RequiredXP(job.level);
        float progress = (required > 0) ? static_cast<float>(job.experience) / static_cast<float>(required) : 0.0f;

        // Job name and level
        std::string header = std::string(label) + "  Lv." + std::to_string(job.level);
        if (job.jobPoints > 0)
            header += "  [" + std::to_string(job.jobPoints) + " pts]";
        renderer.DrawText(header, 70, yy, 16, CQColors::TextGold);
        yy += 20;

        // Specialization display
        if (job.HasSpecialization())
        {
            const auto& spec = job.GetSpecializationData();
            renderer.DrawText("[" + spec.name + "] " + spec.bonusDescription, 90, yy, 11, CQColors::TextGreen);
            yy += 16;
        }
        else if (job.CanChooseSpecialization())
        {
            renderer.DrawText("Specialization available! Choose below.", 90, yy, 11, CQColors::TextGold);
            yy += 16;
        }

        // XP progress bar
        int barX = 90;
        int barY = yy;
        int barW = 300;
        int barH = 16;

        // Bar background
        renderer.DrawRect(barX, barY, barW, barH, Color{30, 20, 15, 255});
        renderer.DrawRectLines(barX, barY, barW, barH, CQColors::BorderLight, 1);

        // Bar fill
        int fillW = static_cast<int>(barW * progress);
        if (fillW > 0)
        {
            // Gradient effect - lighter on top
            Color fillDark = barColor;
            fillDark.r = static_cast<unsigned char>(fillDark.r * 0.7f);
            fillDark.g = static_cast<unsigned char>(fillDark.g * 0.7f);
            fillDark.b = static_cast<unsigned char>(fillDark.b * 0.7f);
            renderer.DrawRect(barX + 1, barY + 1, fillW - 1, barH - 1, barColor);
        }

        // XP text on bar
        std::string xpText = std::to_string(job.experience) + " / " + std::to_string(required) + " XP";
        int textW = MeasureText(xpText.c_str(), 12);
        int textX = barX + (barW - textW) / 2;
        renderer.DrawText(xpText, textX, barY + 2, 12, CQColors::TextLight);

        // Level milestone indicators
        static const std::vector<int> milestones = {5, 10, 25, 50, 75, 100};
        bool isMilestone = std::find(milestones.begin(), milestones.end(), job.level) != milestones.end();
        if (isMilestone)
        {
            renderer.DrawText("Milestone!", barX + barW + 10, barY + 1, 11, CQColors::TextGreen);
        }

        // Fatigue display
        float fatiguePct = (static_cast<float>(job.fatigue) / Job::MAX_FATIGUE) * 100.0f;
        Color fatigueColor = (fatiguePct < 33.0f) ? CQColors::TextGreen :
                             (fatiguePct < 66.0f) ? CQColors::TextGold : CQColors::TextRed;
        std::string fatigueText = "Fatigue: " + std::to_string(static_cast<int>(fatiguePct)) + "%";
        renderer.DrawText(fatigueText, barX + barW + 10, barY + 16, 11, fatigueColor);

        // Show what's next
        std::string nextUnlock;
        if (job.level < 5) nextUnlock = "Unlocks at Lv.5: Rare resources";
        else if (job.level < 10) nextUnlock = "Unlocks at Lv.10: Apprentice rank";
        else if (job.level < 25) nextUnlock = "Unlocks at Lv.25: Journeyman rank";
        else if (job.level < 50) nextUnlock = "Unlocks at Lv.50: Expert rank";
        else if (job.level < 75) nextUnlock = "Unlocks at Lv.75: Master rank";
        else if (job.level < 100) nextUnlock = "Unlocks at Lv.100: Grandmaster rank";
        if (!nextUnlock.empty())
        {
            renderer.DrawText(nextUnlock, 90, barY + barH + 4, 11, CQColors::TextDim);
        }

        // Work button
        int wh = (player->GetCharacterClass() == CharacterClass::Merchant) ? 5 : 3;
        std::string btnLabel = "Work " + std::string(label) + " (" + std::to_string(wh) + "h)";
        if (renderer.Button(btnLabel, barX + barW + 10, barY - 2, 160, 20, focusIdx))
        {
            js.WorkJob(jt, wh, player->GetInventory(), &achievementSystem);

            // Track job achievements
            achievementSystem.UpdateProgress("job_hours_100", wh);
            achievementSystem.UpdateProgress("job_hours_500", wh);
            achievementSystem.UpdateProgress("job_hours_1000", wh);

            // Track job quest progress
            jobQuestSystem.UpdateProgress(JobQuestType::WorkHours, job.GetJobName(), wh);
            jobQuestSystem.UpdateProgress(JobQuestType::Collect, job.GetJobName(), wh);
        }

        return barY + barH + 22;
    };

    y = drawJob(y, JobType::Mining, "Mining", jobColors[0], 0);
    y += 8;
    y = drawJob(y, JobType::Lumberjacking, "Lumberjacking", jobColors[1], 1);
    y += 8;
    y = drawJob(y, JobType::Fishing, "Fishing", jobColors[2], 2);
    y += 8;
    y = drawJob(y, JobType::Smithing, "Smithing", jobColors[3], 3);
    y += 8;

    // Separator line
    renderer.DrawRect(70, y, GRenderer::W - 140, 1, CQColors::BorderLight);
    y += 12;

    // Show combat synergy summary
    std::string synergy = js.GetCombatSynergyDescription();
    if (synergy != "No job combat bonuses active")
    {
        renderer.DrawText("Job Combat Bonuses:", 70, y, 14, CQColors::TextGold);
        y += 18;
        renderer.DrawText(synergy, 90, y, 13, CQColors::TextGreen);
        y += 20;
    }

    // Specialization choice section
    {
        bool anySpecNeeded = false;
        auto checkSpec = [&](JobType jt, const char* label) {
            auto& job = js.GetJob(jt);
            if (job.CanChooseSpecialization()) anySpecNeeded = true;
        };
        checkSpec(JobType::Mining, "Mining");
        checkSpec(JobType::Lumberjacking, "Lumberjacking");
        checkSpec(JobType::Fishing, "Fishing");
        checkSpec(JobType::Smithing, "Smithing");

        if (anySpecNeeded)
        {
            renderer.DrawText("Specializations (choose at Lv.5):", 70, y, 14, CQColors::TextGold);
            y += 20;

            auto drawSpecChoice = [&](JobType jt, const char* jobName, Color barColor) {
                auto& job = js.GetJob(jt);
                if (!job.CanChooseSpecialization()) return;

                SpecializationType specA = SpecializationManager::GetSpecA(jt);
                SpecializationType specB = SpecializationManager::GetSpecB(jt);
                const auto& dataA = SpecializationManager::GetSpecialization(specA);
                const auto& dataB = SpecializationManager::GetSpecialization(specB);

                renderer.DrawText(std::string(jobName) + ":", 90, y, 13, barColor);
                y += 18;

                if (renderer.Button(dataA.name + ": " + dataA.bonusDescription, 100, y, 340, 22, 10))
                {
                    job.ChooseSpecialization(specA);
                }
                y += 26;

                if (renderer.Button(dataB.name + ": " + dataB.bonusDescription, 100, y, 340, 22, 11))
                {
                    job.ChooseSpecialization(specB);
                }
                y += 28;
            };

            drawSpecChoice(JobType::Mining, "Mining", jobColors[0]);
            drawSpecChoice(JobType::Lumberjacking, "Lumberjacking", jobColors[1]);
            drawSpecChoice(JobType::Fishing, "Fishing", jobColors[2]);
            drawSpecChoice(JobType::Smithing, "Smithing", jobColors[3]);
        }
    }

    // Quick work all button
    int wh = (player->GetCharacterClass() == CharacterClass::Merchant) ? 5 : 3;
    keyboardNav.SetFocusCount(8); // 4 jobs + Perks + Skill Tree + Upgrades + Back
    if (renderer.Button("Work All Jobs (" + std::to_string(wh) + "h each)", 70, y, 300, 36, 4))
    {
        js.WorkJob(JobType::Mining, wh, player->GetInventory(), &achievementSystem);
        js.WorkJob(JobType::Lumberjacking, wh, player->GetInventory(), &achievementSystem);
        js.WorkJob(JobType::Fishing, wh, player->GetInventory(), &achievementSystem);
        js.WorkJob(JobType::Smithing, wh, player->GetInventory(), &achievementSystem);

        // Track job achievements for all jobs
        int totalWh = wh * 4;
        achievementSystem.UpdateProgress("job_hours_100", totalWh);
        achievementSystem.UpdateProgress("job_hours_500", totalWh);
        achievementSystem.UpdateProgress("job_hours_1000", totalWh);
    }
    y += 46;
    if (renderer.Button("Job Perks", 70, y, 200, 36, 4))
    {
        selectedJobIdx = -1;
        currentState = GameState::JobPerks;
    }
    if (renderer.Button("Skill Tree", 290, y, 200, 36, 5))
    {
        skillTreeJobIdx = -1;
        currentState = GameState::JobSkillTree;
    }
    if (renderer.Button("Upgrades", 510, y, 200, 36, 6))
    {
        upgradeJobIdx = -1;
        currentState = GameState::JobUpgrades;
    }
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 7))
        currentState = GameState::Exploring;
}

void Game::StateJobPerks()
{
    if (!player) { currentState = GameState::Jobs; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    auto& js = player->GetJobSystem();

    // If no job selected, show job list
    if (selectedJobIdx < 0)
    {
        renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Job Perks — Choose a Job");
        int y = 110;
        renderer.DrawText("Select a job to view its perk tree:", 70, y, 16, CQColors::TextDim);
        y += 30;

        keyboardNav.SetFocusCount(5); // 4 jobs + Back
        JobType jobTypes[] = { JobType::Mining, JobType::Lumberjacking, JobType::Fishing, JobType::Smithing };
        const char* jobNames[] = { "Mining", "Lumberjacking", "Fishing", "Smithing" };

        for (int i = 0; i < 4; ++i)
        {
            auto& job = js.GetJob(jobTypes[i]);
            int unlocked = 0;
            for (const auto& p : job.perks) if (p.unlocked) unlocked++;
            std::string label = std::string(jobNames[i]) + "  Lv." + std::to_string(job.level)
                + "  Pts:" + std::to_string(job.jobPoints)
                + "  [" + std::to_string(unlocked) + "/" + std::to_string(job.perks.size()) + " perks]";

            Color cardBg = CQColors::SkillCardBg(player->GetCharacterClass());
            renderer.DrawRect(70, y, 600, 32, cardBg);

            if (renderer.Button(label, 70, y, 600, 32, i))
            {
                selectedJobIdx = i;
                jobPerkPage = 0;
            }
            y += 40;
        }

        if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 4))
            currentState = GameState::Jobs;
        return;
    }

    // Show perk tree for selected job
    JobType jobTypes[] = { JobType::Mining, JobType::Lumberjacking, JobType::Fishing, JobType::Smithing };
    const char* jobNames[] = { "Mining", "Lumberjacking", "Fishing", "Smithing" };
    auto& job = js.GetJob(jobTypes[selectedJobIdx]);

    std::string title = std::string(jobNames[selectedJobIdx]) + " Perks";
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, title);

    int y = 110;
    renderer.DrawText("Job Points: " + std::to_string(job.jobPoints), 70, y, 18, CQColors::TextGold);
    y += 10;
    renderer.DrawText("Job Level: " + std::to_string(job.level) + "  |  Click a perk to unlock", 70, y + 18, 14, CQColors::TextDim);
    y += 40;

    // Count unlockable perks on the current page for focus count
    const int totalPerks = static_cast<int>(job.perks.size());
    int perkMaxPage = (totalPerks > PERKS_PER_PAGE) ? ((totalPerks - 1) / PERKS_PER_PAGE) : 0;
    if (jobPerkPage < 0) jobPerkPage = 0;
    if (jobPerkPage > perkMaxPage) jobPerkPage = perkMaxPage;
    int perkStart = jobPerkPage * PERKS_PER_PAGE;
    int perkEnd = std::min(perkStart + PERKS_PER_PAGE, totalPerks);

    int unlockCount = 0;
    for (int u = perkStart; u < perkEnd; ++u)
        if (job.CanUnlockPerk(u))
            unlockCount++;
    int navExtra = (perkMaxPage > 0) ? 2 : 0;
    int detailFocusCount = unlockCount + navExtra + 2; // unlock + Prev/Next + Back-to-Jobs + Back
    keyboardNav.SetFocusCount(detailFocusCount);
    int detailFocusIdx = 0;

    for (int u = perkStart; u < perkEnd; ++u)
    {
        const auto& perk = job.perks[u];

        // Branch separator (only when this page actually shows index 3)
        if (u == 3)
        {
            y += 8;
            renderer.DrawText("--- Branch 2 ---", 90, y, 14, CQColors::TextDim);
            y += 20;
        }

        std::string status;
        Color statusColor;
        if (perk.unlocked)
        {
            status = "[UNLOCKED]";
            statusColor = CQColors::TextGreen;
        }
        else if (job.CanUnlockPerk(u))
        {
            status = "[UNLOCK]";
            statusColor = CQColors::TextGold;
        }
        else
        {
            status = "[Lv." + std::to_string(perk.requiredLevel) + "]";
            statusColor = CQColors::TextDim;
        }

        renderer.DrawText(status + "  " + perk.name, 90, y, 16, statusColor);
        y += 20;
        renderer.DrawText(perk.description, 110, y, 13, CQColors::TextDim);
        y += 20;

        if (!perk.unlocked && job.CanUnlockPerk(u))
        {
            if (renderer.Button("Unlock (1 PT)", 110, y, 160, 26, detailFocusIdx++))
                job.UnlockPerk(u);
            y += 30;
        }
        else
        {
            y += 6;
        }
        y += 4;
    }

    // Page nav footer (above the back buttons)
    if (perkMaxPage > 0)
    {
        int navY = GRenderer::H - 154;
        if (renderer.Button("< Prev", 70, navY, 100, 30, detailFocusIdx++) && jobPerkPage > 0) jobPerkPage--;
        if (renderer.Button("Next >", 180, navY, 100, 30, detailFocusIdx++) && jobPerkPage < perkMaxPage) jobPerkPage++;
        renderer.DrawText("Page " + std::to_string(jobPerkPage + 1) + " / " + std::to_string(perkMaxPage + 1),
                          290, navY + 6, 13, CQColors::TextDim);
    }

    if (renderer.Button("Back to Jobs", 70, GRenderer::H - 100, 160, 36, detailFocusIdx++))
    {
        selectedJobIdx = -1;
        jobPerkPage = 0;
        return;
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 100, 120, 36, detailFocusIdx++))
    {
        selectedJobIdx = -1;
        jobPerkPage = 0;
        currentState = GameState::Jobs;
    }
}

void Game::StateJobSkillTree()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    auto& js = player->GetJobSystem();
    auto& st = js.GetSkillTree();
    JobType jobTypes[] = { JobType::Mining, JobType::Lumberjacking, JobType::Fishing, JobType::Smithing };
    const char* jobNames[] = { "Mining", "Lumberjacking", "Fishing", "Smithing" };

    // If no job selected, show job list
    if (skillTreeJobIdx < 0)
    {
        renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Skill Tree — Choose a Job");
        int y = 110;
        renderer.DrawText("Select a job to view its skill tree (3 branches x 5 tiers each):", 70, y, 16, CQColors::TextDim);
        y += 30;

        keyboardNav.SetFocusCount(5);
        for (int i = 0; i < 4; ++i)
        {
            auto& job = js.GetJob(jobTypes[i]);
            int unlocked = 0;
            for (auto& s : st.GetSkills(jobTypes[i]))
                if (s.unlocked) unlocked++;
            std::string label = std::string(jobNames[i]) + "  Lv." + std::to_string(job.level)
                + "  Pts:" + std::to_string(job.skillPoints)
                + "  [" + std::to_string(unlocked) + "/15 skills]";

            renderer.DrawRect(70, y, 600, 32, CQColors::SkillCardBg(player->GetCharacterClass()));
            if (renderer.Button(label, 70, y, 600, 32, i))
            {
                skillTreeJobIdx = i;
                skillTreePage = 0;
            }
            y += 40;
        }
        if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 4))
            currentState = GameState::Jobs;
        return;
    }

    // Show skill tree for selected job
    JobType currentJobType = jobTypes[skillTreeJobIdx];
    auto& job = js.GetJob(currentJobType);
    const auto& skills = st.GetSkills(currentJobType);

    std::string title = std::string(jobNames[skillTreeJobIdx]) + " Skill Tree";
    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, title);

    int y = 85;
    renderer.DrawText("Skill Points: " + std::to_string(job.skillPoints), 70, y, 18, CQColors::TextGold);
    y += 10;
    renderer.DrawText("Branches: Efficiency (speed/fatigue) | Quality (quality/rare) | Luck (rare/mythical)", 70, y + 18, 13, CQColors::TextDim);
    y += 40;

    // Pagination
    int totalSkills = static_cast<int>(skills.size());
    if (totalSkills <= 0) { skillTreeJobIdx = -1; return; }
    int maxPage = (totalSkills - 1) / SKILL_TREE_PER_PAGE;
    if (skillTreePage < 0) skillTreePage = 0;
    if (skillTreePage > maxPage) skillTreePage = maxPage;
    int startIdx = skillTreePage * SKILL_TREE_PER_PAGE;
    int endIdx = std::min(startIdx + SKILL_TREE_PER_PAGE, totalSkills);

    // Count unlockable on this page for focus
    int unlockableCount = 0;
    for (int si = startIdx; si < endIdx; ++si)
        if (st.CanUnlockSkill(currentJobType, skills[si].branch, skills[si].tier, job.level, job.skillPoints))
            unlockableCount++;
    int navExtra = (maxPage > 0) ? 2 : 0;
    int focusCount = unlockableCount + navExtra + 2;
    keyboardNav.SetFocusCount(focusCount);
    int focusIdx = 0;

    const char* branchNames[] = { "Efficiency", "Quality", "Luck" };
    Color branchColors[] = {
        {100, 200, 100, 255},
        {100, 150, 255, 255},
        {255, 200, 100, 255}
    };

    for (int si = startIdx; si < endIdx; ++si)
    {
        const auto& sk = skills[si];

        // Branch label
        bool showBranchHeader = (si == startIdx || skills[si-1].branch != sk.branch);
        if (showBranchHeader)
        {
            y += 4;
            renderer.DrawText(("-- " + std::string(branchNames[static_cast<int>(sk.branch)]) + " --").c_str(),
                              90, y, 14, branchColors[static_cast<int>(sk.branch)]);
            y += 20;
        }

        std::string status;
        Color statusColor;
        if (sk.unlocked)
        {
            status = "[UNLOCKED]";
            statusColor = CQColors::TextGreen;
        }
        else if (st.CanUnlockSkill(currentJobType, sk.branch, sk.tier, job.level, job.skillPoints))
        {
            status = "[UNLOCK]";
            statusColor = CQColors::TextGold;
        }
        else
        {
            status = "[Lv." + std::to_string(sk.requiredLevel) + "]";
            statusColor = CQColors::TextDim;
        }

        renderer.DrawText(status + "  " + sk.name + " (Tier " + std::to_string(sk.tier) + ")", 90, y, 15, statusColor);
        y += 18;
        renderer.DrawText(sk.description, 110, y, 12, CQColors::TextDim);
        y += 16;

        if (!sk.unlocked && st.CanUnlockSkill(currentJobType, sk.branch, sk.tier, job.level, job.skillPoints))
        {
            if (renderer.Button("Unlock (1 PT)", 110, y, 160, 24, focusIdx++))
            {
                int& sp = job.skillPoints;
                st.UnlockSkill(currentJobType, sk.branch, sk.tier, sp);
            }
            y += 28;
        }
        else
        {
            y += 4;
        }
    }

    // Page nav
    if (maxPage > 0)
    {
        int navY = GRenderer::H - 110;
        if (renderer.Button("< Prev", 70, navY, 100, 30, focusIdx++) && skillTreePage > 0) skillTreePage--;
        if (renderer.Button("Next >", 180, navY, 100, 30, focusIdx++) && skillTreePage < maxPage) skillTreePage++;
        renderer.DrawText("Page " + std::to_string(skillTreePage + 1) + " / " + std::to_string(maxPage + 1),
                          290, navY + 6, 13, CQColors::TextDim);
    }

    if (renderer.Button("Back to Jobs", 70, GRenderer::H - 60, 160, 36, focusIdx++))
    {
        skillTreeJobIdx = -1;
        skillTreePage = 0;
        return;
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 60, 120, 36, focusIdx++))
    {
        skillTreeJobIdx = -1;
        skillTreePage = 0;
        currentState = GameState::Jobs;
    }
}

void Game::StateJobUpgrades()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    auto& js = player->GetJobSystem();
    auto& locUpgrades = js.GetLocationUpgrades();
    const auto& weather = js.GetEnvironment();
    JobType jobTypes[] = { JobType::Mining, JobType::Lumberjacking, JobType::Fishing, JobType::Smithing };
    const char* jobNames[] = { "Mining", "Lumberjacking", "Fishing", "Smithing" };
    Color jobColors[] = {
        {160, 120, 80, 255},
        {80, 180, 80, 255},
        {60, 120, 200, 255},
        {200, 120, 60, 255}
    };

    // Weather info bar at top
    renderer.DrawPanel(50, 40, GRenderer::W - 100, 50, "Current Conditions");
    renderer.DrawText(weather.GetWeatherDescription(), 70, 60, 14, CQColors::TextDim);

    // Show upgrade list
    renderer.DrawPanel(50, 100, GRenderer::W - 100, GRenderer::H - 150, "Location Upgrades");

    int y = 145;
    renderer.DrawText("Gold: " + std::to_string(player->GetInventory().GetGold()), 70, y, 16, CQColors::TextGold);
    y += 28;

    const auto& upgrades = locUpgrades.upgrades;
    keyboardNav.SetFocusCount(static_cast<int>(upgrades.size()) + 1); // upgrades + Back
    int focusIdx = 0;

    for (size_t i = 0; i < upgrades.size(); ++i)
    {
        const auto& u = upgrades[i];
        int cost = (u.level < u.maxLevel) ? locUpgrades.GetUpgradeCost(u.type) : 0;
        bool maxed = (u.level >= u.maxLevel);
        bool canAfford = !maxed && player->GetInventory().GetGold() >= cost;

        std::string label = u.name + "  Lv." + std::to_string(u.level) + "/" + std::to_string(u.maxLevel);
        if (maxed)
            label += " [MAX]";
        else
            label += "  Cost: " + std::to_string(cost) + "g";

        renderer.DrawRect(70, y, GRenderer::W - 200, 32, CQColors::SkillCardBg(player->GetCharacterClass()));

        Color textColor = maxed ? CQColors::TextGreen : (canAfford ? CQColors::TextGold : CQColors::TextDim);
        renderer.DrawText(label, 80, y + 6, 15, textColor);

        // Description
        renderer.DrawText(u.description, 80, y + 24, 11, CQColors::TextDim);

        // Bonus details
        std::string bonuses;
        if (u.speedBonusPerLevel > 0.0f)   bonuses += "Speed +" + std::to_string(static_cast<int>(u.speedBonusPerLevel * 100 * u.level)) + "% ";
        if (u.qualityBonusPerLevel > 0.0f) bonuses += "Quality +" + std::to_string(static_cast<int>(u.qualityBonusPerLevel * 100 * u.level)) + "% ";
        if (u.fatigueReductionPerLevel > 0.0f) bonuses += "Fatigue -" + std::to_string(static_cast<int>(u.fatigueReductionPerLevel * 100 * u.level)) + "% ";
        if (u.rareFindBonusPerLevel > 0.0f) bonuses += "Rare +" + std::to_string(static_cast<int>(u.rareFindBonusPerLevel * 100 * u.level)) + "% ";
        if (u.xpBonusPerLevel > 0.0f)       bonuses += "XP +" + std::to_string(static_cast<int>(u.xpBonusPerLevel * 100 * u.level)) + "% ";
        if (u.storageBonusPerLevel > 0)     bonuses += "Storage +" + std::to_string(u.storageBonusPerLevel * u.level) + " ";
        if (!bonuses.empty())
            renderer.DrawText("Current: " + bonuses, 80, y + 38, 11, CQColors::TextGreen);

        y += 52;

        // Upgrade button
        if (!maxed && canAfford)
        {
            if (renderer.Button("Upgrade (" + std::to_string(cost) + "g)", GRenderer::W - 260, y - 50, 180, 28, focusIdx++))
            {
                int gold = player->GetInventory().GetGold();
                if (locUpgrades.PerformUpgrade(u.type, gold))
                {
                    player->GetInventory().RemoveGold(cost);
                }
            }
        }
        else
        {
            focusIdx++;
        }
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 50, 120, 36, focusIdx++))
        currentState = GameState::Jobs;
}

void Game::StateSkillLoadout()
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (!tutorialSkillsOpened) { tutorialSkillsOpened = true; AddTutorialHint("Skills", "Equip skills in your loadout. Earn skill XP by using them in combat!"); }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    // Initialize editing state on first entry
    if (!loadoutEditActive)
    {
        loadoutEditCopy = player->GetSkillLoadout();
        loadoutAttackSkillEdit = player->GetAttackSkillIndex();
        loadoutEditActive = true;
    }

    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Skill Loadout — Select up to 4");
    int y = 110;
    renderer.DrawText("Click a skill to toggle it in your loadout (max 4):", 70, y, 16, CQColors::TextDim);
    y += 30;

    // Build list of non-Attack skill indices
    std::vector<int> skillIndices;
    for (size_t i = 0; i < player->GetSkills().GetSkillCount(); ++i)
    {
        auto sk = player->GetSkills().GetSkill(i);
        if (sk && !(i == 0 && sk->name == "Attack"))
            skillIndices.push_back(static_cast<int>(i));
    }

    const int skillsPerPage = 10;
    int totalSkills = static_cast<int>(skillIndices.size());
    int maxPage = (totalSkills > 0) ? ((totalSkills - 1) / skillsPerPage) : 0;
    if (skillLoadoutPage < 0) skillLoadoutPage = 0;
    if (skillLoadoutPage > maxPage) skillLoadoutPage = maxPage;

    int startIdx = skillLoadoutPage * skillsPerPage;
    int endIdx = std::min(startIdx + skillsPerPage, totalSkills);
    int visibleSkills = endIdx - startIdx;
    int loadoutFocusCount = visibleSkills * 2 + 2;
    if (maxPage > 0) loadoutFocusCount += 2;
    keyboardNav.SetFocusCount(loadoutFocusCount);

    int focusIdx = 0;
    for (int si = startIdx; si < endIdx; ++si)
    {
        int i = skillIndices[si];
        auto sk = player->GetSkills().GetSkill(i);
        if (!sk) continue;

        bool inLoadout = std::find(loadoutEditCopy.begin(), loadoutEditCopy.end(), i) != loadoutEditCopy.end();
        bool isAttackSkill = (i == loadoutAttackSkillEdit);
        std::string elemStr = (sk->element != ElementType::Physical) ? " [" + std::string(ElementName(sk->element)) + "]" : "";
        std::string label = sk->name + elemStr
            + " (MP:" + std::to_string(sk->manaCost)
            + " CD:" + std::to_string(sk->cooldown)
            + " | Lv." + std::to_string(sk->level) + ")";

        Color cardBg = CQColors::SkillCardBg(sk->characterClass);
        renderer.DrawRect(70, y, 750, 30, cardBg);

        if (renderer.Button(label, 70, y, 620, 30, focusIdx))
        {
            if (inLoadout)
            {
                auto it = std::find(loadoutEditCopy.begin(), loadoutEditCopy.end(), i);
                if (it != loadoutEditCopy.end()) loadoutEditCopy.erase(it);
            }
            else if (loadoutEditCopy.size() < static_cast<size_t>(player->GetMaxLoadoutSkills()))
            {
                loadoutEditCopy.push_back(i);
            }
        }
        focusIdx++;

        if (renderer.Button(isAttackSkill ? "Unset" : "Set ATK", 690, y, 55, 30, focusIdx))
        {
            loadoutAttackSkillEdit = isAttackSkill ? 0 : i;
        }

        if (inLoadout)
        {
            renderer.DrawRect(615, y, 70, 30, { 70, 55, 15, 255 });
            renderer.DrawText("EQUIP", 627, y + 7, 14, CQColors::GoldBright);
        }

        if (isAttackSkill)
        {
            renderer.DrawRect(690, y, 55, 30, { 140, 40, 40, 255 });
            renderer.DrawText("ATK", 698, y + 7, 14, WHITE);
        }
        renderer.DrawText("  " + sk->description, 85, y + 30, 14, CQColors::TextDim);
        std::string formula = sk->GetDamageFormula();
        renderer.DrawText("  Formula: " + formula, 85, y + 44, 13, CQColors::TextGold);
        int estDmg = sk->EstimateDamage(player->GetStats(), player->GetWeaponDamage(), player->GetElementalBonus(sk->element));
        if (sk->baseDamage > 0)
        {
            renderer.DrawText("  Est. Damage: ~" + std::to_string(estDmg), 85, y + 58, 13, CQColors::TextGreen);
            y += 74;
        }
        else
        {
            y += 60;
        }
    }

    // Pagination controls
    if (maxPage > 0)
    {
        int btnY = GRenderer::H - 110;
        if (skillLoadoutPage > 0 && renderer.Button("< Prev", 70, btnY, 100, 32, focusIdx++))
            skillLoadoutPage--;
        if (skillLoadoutPage < maxPage && renderer.Button("Next >", 180, btnY, 100, 32, focusIdx++))
            skillLoadoutPage++;
        renderer.DrawText("Page " + std::to_string(skillLoadoutPage + 1) + "/" + std::to_string(maxPage + 1),
                          renderer.CenterX(80) - 40, btnY + 8, 14, CQColors::TextDim);
    }

    int bottomY = GRenderer::H - 100;
    renderer.DrawText("Selected: " + std::to_string(loadoutEditCopy.size())
        + " / " + std::to_string(player->GetMaxLoadoutSkills()),
        70, bottomY - 50, 16, CQColors::TextGold);

    if (renderer.Button("Save", 70, bottomY - 20, 120, 36, focusIdx))
    {
        player->SetSkillLoadout(loadoutEditCopy);
        player->SetAttackSkillIndex(loadoutAttackSkillEdit);
        loadoutEditActive = false;
        currentState = GameState::Exploring;
    }
    focusIdx++;
    if (renderer.Button("Back", renderer.CenterX(120), bottomY - 20, 120, 36, focusIdx))
    {
        loadoutEditActive = false;
        currentState = GameState::Exploring;
    }
}

void Game::StateSkillUpgrade()
{
    if (!player) { currentState = GameState::Exploring; return; }

    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    auto& skills = player->GetSkills();

    // ---- Character Mastery view ----
    if (viewingCharacterMastery)
    {
        std::string title = "Character Mastery";
        renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, title);

        int y = 110;
        renderer.DrawText("Level: " + std::to_string(player->charMasteryLevel)
            + "  Points: " + std::to_string(player->charMasteryPoints)
            + "  XP: " + std::to_string(player->charMasteryXP) + "/" + std::to_string(player->GetCharMasteryXPToLevel()),
            70, y, 18, CQColors::TextGold);
        y += 10;
        renderer.DrawText("Spend mastery points to unlock class-themed stat bonuses.", 70, y + 18, 14, CQColors::TextDim);
        y += 42;

        const char* branchDescs[3][5] = {
            { "+5% Max HP", "+5% Max HP", "+5% Max HP", "+5% Max HP", "+5% Max HP" },
            { nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, nullptr, nullptr, nullptr, nullptr },
        };

        // Class-specific branch names and descriptions
        struct BranchInfo { const char* name; const char* desc; };
        BranchInfo offenseInfo, utilityInfo;
        switch (player->GetCharacterClass())
        {
            case CharacterClass::Warrior:
                offenseInfo = {"Might", "+5% Physical DMG per node"};
                utilityInfo = {"Fortitude", "+2% Damage Reduction per node"};
                break;
            case CharacterClass::Priest:
                offenseInfo = {"Divinity", "+5% Holy DMG per node"};
                utilityInfo = {"Grace", "+5% Healing Power per node"};
                break;
            case CharacterClass::Mage:
                offenseInfo = {"Arcana", "+5% Spell DMG per node"};
                utilityInfo = {"Channeling", "-3% Mana Cost per node"};
                break;
            case CharacterClass::Archer:
                offenseInfo = {"Precision", "+5% Ranged DMG per node"};
                utilityInfo = {"Swiftness", "+3% Dodge Chance per node"};
                break;
            case CharacterClass::Merchant:
                offenseInfo = {"Opulence", "+5% Gold Find per node"};
                utilityInfo = {"Endurance", "+3% XP Gain per node"};
                break;
            default:
                offenseInfo = {"Offense", "+5% DMG per node"};
                utilityInfo = {"Utility", "Class bonus per node"};
                break;
        }

        const char* branch2Desc[5];
        const char* branch3Desc[5];
        for (int i = 0; i < 5; ++i) { branch2Desc[i] = offenseInfo.desc; branch3Desc[i] = utilityInfo.desc; }

        int colW = (GRenderer::W - 180) / 3;
        const char* bNames[3] = { "Vitality", offenseInfo.name, utilityInfo.name };
        const char* bDescs[3][5];
        for (int i = 0; i < 5; ++i) { bDescs[0][i] = branchDescs[0][i]; bDescs[1][i] = branch2Desc[i]; bDescs[2][i] = branch3Desc[i]; }

        int focusCount = 2; // Back to Skills + Back
        for (int b = 0; b < 3; ++b)
            for (int n = 0; n < 5; ++n)
                if (player->CanUnlockCharMasteryNode(b, n))
                    focusCount++;
        keyboardNav.SetFocusCount(focusCount);
        int focusIdx = 0;

        for (int b = 0; b < 3; ++b)
        {
            int bx = 80 + b * colW;
            Color branchColor = (b == 0) ? CQColors::TextGreen : (b == 1) ? CQColors::TextFire : CQColors::TextArcane;
            renderer.DrawText(bNames[b], bx, y, 16, branchColor);
            for (int n = 0; n < 5; ++n)
            {
                int ny = y + 24 + n * 28;
                bool unlocked = player->charMasteryNodes[b][n];
                bool canUnlock = player->CanUnlockCharMasteryNode(b, n);
                bool locked = !unlocked && !canUnlock;
                std::string nodeText = "Lv" + std::to_string((n + 1) * 4) + ": " + bDescs[b][n];
                Color nodeColor = unlocked ? CQColors::TextGreen : (canUnlock ? CQColors::TextGold : CQColors::TextDim);
                std::string icon = unlocked ? "[*]" : (canUnlock ? "[o]" : "[ ]");
                renderer.DrawText(icon + " " + nodeText, bx, ny, 13, nodeColor);

                if (locked)
                {
                    int reqLvl = (n + 1) * 4;
                    renderer.DrawText("Req Mastery Lv" + std::to_string(reqLvl), bx + 20, ny + 14, 10, CQColors::TextDim);
                }

                if (canUnlock)
                {
                    if (renderer.Button("Unlock##cm" + std::to_string(b) + std::to_string(n),
                                        bx + colW - 140, ny, 120, 24, focusIdx++))
                    {
                        player->UnlockCharMasteryNode(b, n);
                    }
                }
            }
        }
        y += 24 + 5 * 28 + 20;

        // Effect summary
        int totalHpPct = player->GetCharMasteryBonusHP();
        int totalDef = player->GetCharMasteryBonusDEF();
        float totalDmg = player->GetCharMasteryDamageBonus();
        float totalDr = player->GetCharMasteryDamageReduction();
        float totalHeal = player->GetCharMasteryHealingBonus();
        float totalManaReduce = player->GetCharMasteryManaCostReduction();
        float totalDodge = player->GetCharMasteryDodgeChance();
        float totalGoldFind = player->GetCharMasteryGoldFind();
        float totalXpBonus = player->GetCharMasteryXPBonus();

        std::string summary;
        if (totalHpPct > 0) summary += "HP: +" + std::to_string(totalHpPct) + "%  ";
        if (totalDef > 0) summary += "DEF: +" + std::to_string(totalDef) + "  ";
        if (totalDmg > 0.0f) summary += "DMG: +" + std::to_string(static_cast<int>(totalDmg * 100)) + "%  ";
        if (totalDr > 0.0f) summary += "DR: +" + std::to_string(static_cast<int>(totalDr * 100)) + "%  ";
        if (totalHeal > 0.0f) summary += "Healing: +" + std::to_string(static_cast<int>(totalHeal * 100)) + "%  ";
        if (totalManaReduce > 0.0f) summary += "Mana Cost: -" + std::to_string(static_cast<int>(totalManaReduce * 100)) + "%  ";
        if (totalDodge > 0.0f) summary += "Dodge: +" + std::to_string(static_cast<int>(totalDodge * 100)) + "%  ";
        if (totalGoldFind > 0.0f) summary += "Gold Find: +" + std::to_string(static_cast<int>(totalGoldFind * 100)) + "%  ";
        if (totalXpBonus > 0.0f) summary += "XP Gain: +" + std::to_string(static_cast<int>(totalXpBonus * 100)) + "%  ";
        if (summary.empty()) summary = "No mastery bonuses unlocked yet.";
        renderer.DrawText(summary, 70, y, 14, CQColors::TextGold);
        y += 18;

        // Show overflow scaling info when all 15 nodes are unlocked
        if (player->AllCharMasteryNodesUnlocked())
        {
            renderer.DrawText("All nodes unlocked! Overflow: each mastery level grants:",
                              70, y, 13, CQColors::TextGreen);
            y += 16;
            renderer.DrawText("+1% HP  +1 DEF  +0.5% DMG  +0.3% DR  +class bonus",
                              80, y, 12, CQColors::TextDim);
            y += 16;
        }

        int backY = std::max(y + 30, GRenderer::H - 100);
        if (renderer.Button("Back to Skills", 70, backY, 160, 36, focusIdx++))
        {
            viewingCharacterMastery = false;
            return;
        }
        if (renderer.Button("Back", renderer.CenterX(120), backY, 120, 36, focusIdx++))
        {
            viewingCharacterMastery = false;
            currentState = GameState::Exploring;
        }

        return;
    }

    // ---- Mastery-only view: dedicated panel for a single skill's mastery tree ----
    if (viewingMasteryIdx >= 0 && viewingMasteryIdx < static_cast<int>(skills.GetSkillCount()))
    {
        auto sk = skills.GetSkill(viewingMasteryIdx);
        if (!sk || sk->level < 50)
        {
            viewingMasteryIdx = -1;
        }
        else
        {
            int totalPoints = 0;
            for (size_t i = 0; i < skills.GetSkillCount(); ++i)
            {
                auto s = skills.GetSkill(i);
                if (s) totalPoints += s->skillPoints;
            }

            std::string title = sk->name + " Mastery";
            renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, title);

            int y = 110;
            renderer.DrawText("Skill Points: " + std::to_string(totalPoints), 70, y, 18, CQColors::TextGold);
            y += 10;
            renderer.DrawText("Mastery Level: " + std::to_string(sk->masteryLevel)
                + "  Points: " + std::to_string(sk->masteryPoints)
                + "  XP: " + std::to_string(sk->masteryXP) + "/" + std::to_string(sk->GetMasteryXPToLevel()),
                70, y + 18, 14, CQColors::TextDim);
            y += 50;

            const char* branchNames[] = { "Damage", "Utility", "Special" };
            const char* branchDescs[3][5] = {
                { "+5% dmg per node", "+5% dmg per node", "+5% dmg per node", "+5% dmg per node", "+5% dmg per node" },
                { "-1 cooldown", "-1 cooldown", "-2 mana cost", "-1 cooldown", "-3 mana cost" },
                { "+10% skill XP", "+10% skill XP", "+10% skill XP", "+10% skill XP", "+10% skill XP" },
            };
            const char* branchRequirements[3][5] = {
                { "Requires Lv10 skill", "Requires Lv20 skill", "Requires Lv30 skill", "Requires Lv40 skill", "Requires Lv50 skill" },
                { "Requires Lv10 skill", "Requires Lv20 skill", "Requires Lv30 skill", "Requires Lv40 skill", "Requires Lv50 skill" },
                { "Requires Lv10 skill", "Requires Lv20 skill", "Requires Lv30 skill", "Requires Lv40 skill", "Requires Lv50 skill" },
            };

            int masteryFocusCount = 0;
            for (int b = 0; b < 3; ++b)
                for (int n = 0; n < 5; ++n)
                    if (sk->CanUnlockMasteryNode(b, n))
                        masteryFocusCount++;
            int focusCount = masteryFocusCount + 2; // + Back to Skills + Back
            keyboardNav.SetFocusCount(focusCount);
            int focusIdx = 0;

            int colW = (GRenderer::W - 180) / 3;
            for (int b = 0; b < 3; ++b)
            {
                int bx = 80 + b * colW;
                Color branchColor = (b == 0) ? CQColors::TextFire : (b == 1) ? CQColors::TextIce : CQColors::TextArcane;
                renderer.DrawText(branchNames[b], bx, y, 16, branchColor);
                for (int n = 0; n < 5; ++n)
                {
                    int ny = y + 24 + n * 28;
                    bool unlocked = sk->masteryNodes[b][n];
                    bool canUnlock = sk->CanUnlockMasteryNode(b, n);
                    bool locked = !unlocked && !canUnlock;
                    std::string nodeText = "Lv" + std::to_string((n + 1) * 10) + ": " + branchDescs[b][n];
                    Color nodeColor = unlocked ? CQColors::TextGreen : (canUnlock ? CQColors::TextGold : CQColors::TextDim);
                    std::string icon = unlocked ? "[*]" : (canUnlock ? "[o]" : "[ ]");
                    renderer.DrawText(icon + " " + nodeText, bx, ny, 13, nodeColor);

                    if (locked)
                        renderer.DrawText(branchRequirements[b][n], bx + 20, ny + 14, 10, CQColors::TextDim);

                    if (canUnlock)
                    {
                        if (renderer.Button("Unlock##m" + std::to_string(b) + std::to_string(n),
                                            bx + colW - 140, ny, 120, 24, focusIdx++))
                        {
                            sk->UnlockMasteryNode(b, n);
                        }
                    }
                }
            }
            y += 24 + 5 * 28 + 20;

            // Effect summary
            int totalDmgBonus = 0;
            for (int n = 0; n < 5; ++n) if (sk->masteryNodes[0][n]) totalDmgBonus += 5;
            int totalCdReduce = 0;
            for (int n = 0; n < 5; ++n) if (sk->masteryNodes[1][n]) totalCdReduce += (n == 1 || n == 3) ? 1 : (n == 2 ? 2 : (n == 4 ? 3 : 0));
            int totalMpReduce = 0;
            for (int n = 0; n < 5; ++n) if (sk->masteryNodes[1][n]) totalMpReduce += (n == 2 ? 2 : (n == 4 ? 3 : 0));
            int totalXpBonus = 0;
            for (int n = 0; n < 5; ++n) if (sk->masteryNodes[2][n]) totalXpBonus += 10;

            std::string summary;
            if (totalDmgBonus > 0) summary += "Damage: +" + std::to_string(totalDmgBonus) + "%  ";
            if (totalCdReduce > 0) summary += "Cooldown: -" + std::to_string(totalCdReduce) + "s  ";
            if (totalMpReduce > 0) summary += "Mana Cost: -" + std::to_string(totalMpReduce) + "  ";
            if (totalXpBonus > 0) summary += "Skill XP: +" + std::to_string(totalXpBonus) + "%  ";
            if (summary.empty()) summary = "No mastery bonuses unlocked yet.";
            renderer.DrawText(summary, 70, y, 14, CQColors::TextGold);

            // Back buttons
            int backY = std::max(y + 30, GRenderer::H - 100);
            if (renderer.Button("Back to Skills", 70, backY, 160, 36, focusIdx++))
            {
                viewingMasteryIdx = -1;
                return;
            }
            if (renderer.Button("Back", renderer.CenterX(120), backY, 120, 36, focusIdx++))
            {
                viewingMasteryIdx = -1;
                currentState = GameState::Exploring;
            }

            return;
        }
    }

    // ---- Detail view: upgrade tree for selected skill ----
    if (selectedSkillIdx >= 0 && selectedSkillIdx < static_cast<int>(skills.GetSkillCount()))
    {
        auto sk = skills.GetSkill(selectedSkillIdx);
        if (!sk || sk->upgrades.empty())
        {
            selectedSkillIdx = -1;
        }
        else
        {
            // Calculate total skill points
            int totalPoints = 0;
            for (size_t i = 0; i < skills.GetSkillCount(); ++i)
            {
                auto s = skills.GetSkill(i);
                if (s) totalPoints += s->skillPoints;
            }

            std::string elemStr = (sk->element != ElementType::Physical) ? " [" + std::string(ElementName(sk->element)) + "]" : "";
            std::string title = sk->name + elemStr + " Upgrades";
            renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, title);

            int y = 110;
            renderer.DrawText("Skill Points: " + std::to_string(totalPoints), 70, y, 18, CQColors::TextGold);
            y += 10;
            renderer.DrawText("Skill Level: " + std::to_string(sk->level), 70, y + 18, 14, CQColors::TextDim);
            y += 40;

            // Count unlockable upgrades for focus count
            int unlockCount = 0;
            for (size_t u = 0; u < sk->upgrades.size(); ++u)
                if (!sk->upgrades[u].unlocked && sk->CanUnlockUpgrade(static_cast<int>(u)))
                    unlockCount++;

            // Count mastery unlock buttons
            int masteryUnlockCount = 0;
            if (sk->level >= 50)
            {
                for (int b = 0; b < 3; ++b)
                    for (int n = 0; n < 5; ++n)
                        if (sk->CanUnlockMasteryNode(b, n))
                            masteryUnlockCount++;
            }
            int detailFocusCount = unlockCount + masteryUnlockCount + 2; // unlock buttons + Back to Skills + Back
            keyboardNav.SetFocusCount(detailFocusCount);
            int detailFocusIdx = 0;

            for (size_t u = 0; u < sk->upgrades.size(); ++u)
            {
                const auto& up = sk->upgrades[u];
                std::string status;
                Color statusColor;
                if (up.unlocked)
                {
                    status = "[UNLOCKED]";
                    statusColor = CQColors::TextGreen;
                }
                else if (sk->CanUnlockUpgrade(static_cast<int>(u)))
                {
                    status = "[UNLOCK]";
                    statusColor = CQColors::TextGold;
                }
                else
                {
                    status = "[LOCKED]";
                    statusColor = CQColors::TextDim;
                }

                // Upgrade name and description
                renderer.DrawText(status + "  " + up.name, 90, y, 16, statusColor);
                y += 20;
                renderer.DrawText(up.description, 110, y, 13, CQColors::TextDim);
                y += 20;

                // Unlock button
                if (!up.unlocked && sk->CanUnlockUpgrade(static_cast<int>(u)))
                {
                    if (renderer.Button("Unlock (" + std::to_string(up.tier) + " TP)", 110, y, 160, 26, detailFocusIdx++))
                        sk->UnlockUpgrade(static_cast<int>(u));
                    y += 30;
                }
                else
                {
                    y += 6;
                }
                y += 4;
            }

            // ---- Mastery Tree (shown for level 50 skills) ----
            if (sk->level >= 50)
            {
                y += 8;
                int my = y;
                int panelH = 160;
                int panelBottom = my + panelH;
                int maxPanelBottom = GRenderer::H - 60;
                if (panelBottom > maxPanelBottom) my = maxPanelBottom - panelH;
                if (my < y) my = y;
                renderer.DrawRect(60, my, GRenderer::W - 120, panelH, CQColors::BgPanel);
                renderer.DrawRectLines(60, my, GRenderer::W - 120, panelH, CQColors::Gold, 1);
                my += 8;

                renderer.DrawText("--- MASTERY ---  Level " + std::to_string(sk->masteryLevel)
                    + "  Points: " + std::to_string(sk->masteryPoints)
                    + "  XP: " + std::to_string(sk->masteryXP) + "/" + std::to_string(sk->GetMasteryXPToLevel()),
                    75, my, 14, CQColors::Gold);
                my += 22;

                const char* branchNames[] = { "Damage", "Utility", "Special" };
                const char* branchDescs[3][5] = {
                    { "+5% dmg", "+5% dmg", "+5% dmg", "+5% dmg", "+5% dmg" },
                    { "-1 CD", "-1 CD", "-2 MP", "-1 CD", "-3 MP" },
                    { "+10% XP", "+10% XP", "+10% XP", "+10% XP", "+10% XP" },
                };
                int colW = (GRenderer::W - 160) / 3;

                for (int b = 0; b < 3; ++b)
                {
                    int bx = 80 + b * colW;
                    renderer.DrawText(branchNames[b], bx, my, 14, CQColors::TextGold);
                    for (int n = 0; n < 5; ++n)
                    {
                        int ny = my + 22 + n * 24;
                        if (ny + 20 > my + panelH - 8) break;
                        bool unlocked = sk->masteryNodes[b][n];
                        bool canUnlock = sk->CanUnlockMasteryNode(b, n);
                        std::string nodeText = "Lv" + std::to_string((n + 1) * 2) + ": " + branchDescs[b][n];
                        Color nodeColor = unlocked ? CQColors::TextGreen : (canUnlock ? CQColors::TextGold : CQColors::TextDim);
                        renderer.DrawText((unlocked ? "[*] " : canUnlock ? "[o] " : "[ ] ") + nodeText, bx, ny, 12, nodeColor);

                        if (canUnlock)
                        {
                            if (renderer.Button("Unlock##" + std::to_string(b) + std::to_string(n),
                                                bx + 200, ny - 2, 60, 20, detailFocusIdx++))
                            {
                                sk->UnlockMasteryNode(b, n);
                            }
                        }
                    }
                }
                y = my + panelH + 8;
            }

            // Back buttons at the bottom
            int backY = std::max(y + 4, GRenderer::H - 100);
            if (renderer.Button("Back to Skills", 70, backY, 160, 36, detailFocusIdx++))
            {
                selectedSkillIdx = -1;
                return;
            }

            if (renderer.Button("Back", renderer.CenterX(120), backY, 120, 36, detailFocusIdx++))
            {
                selectedSkillIdx = -1;
                currentState = GameState::Exploring;
            }

            return;
        }
    }

    // ---- Overview: list of all skills ----
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Skill Upgrades");
    int y = 110;

    int totalPoints = 0;
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto s = skills.GetSkill(i);
        if (s) totalPoints += s->skillPoints;
    }

    renderer.DrawText("Skill Points: " + std::to_string(totalPoints), 70, y, 18, CQColors::TextGold);
    y += 10;
    renderer.DrawText("Click a skill to view its upgrade tree", 70, y + 18, 14, CQColors::TextDim);
    y += 42;

    // Count non-Attack skills for pagination
    const int skillsPerPage = 12;
    std::vector<int> skillIndices;
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto sk = skills.GetSkill(i);
        if (!sk) continue;
        if (i == 0 && sk->name == "Attack") continue;
        skillIndices.push_back(static_cast<int>(i));
    }
    int totalSkills = static_cast<int>(skillIndices.size());
    int maxSkillPage = (totalSkills > skillsPerPage) ? ((totalSkills - 1) / skillsPerPage) : 0;
    if (skillOverviewPage < 0) skillOverviewPage = 0;
    if (skillOverviewPage > maxSkillPage) skillOverviewPage = maxSkillPage;

    int startIdx = skillOverviewPage * skillsPerPage;
    int endIdx = std::min(startIdx + skillsPerPage, totalSkills);
    int visibleSkills = endIdx - startIdx;
    int focusCount = visibleSkills + 1; // +1 for Back
    if (maxSkillPage > 0) focusCount += 2; // +2 for Prev/Next
    if (player->GetLevel() >= Character::MAX_LEVEL) focusCount += 1; // +1 for Character Mastery button
    keyboardNav.SetFocusCount(focusCount);
    int focusIdx = 0;

    // Character Mastery button (visible at level 50+)
    if (player->GetLevel() >= Character::MAX_LEVEL)
    {
        std::string cmLabel = "Character Mastery  Lv" + std::to_string(player->charMasteryLevel)
            + "  Pts:" + std::to_string(player->charMasteryPoints);
        if (renderer.Button(cmLabel, 70, y, 360, 32, focusIdx++))
        {
            viewingCharacterMastery = true;
            return;
        }
        y += 38;
    }

    for (int si = startIdx; si < endIdx; ++si)
    {
        int i = skillIndices[si];
        auto sk = skills.GetSkill(i);
        if (!sk) continue;

        std::string elemStr = (sk->element != ElementType::Physical) ? " [" + std::string(ElementName(sk->element)) + "]" : "";

        // Count unlocked upgrades
        int unlocked = 0;
        for (const auto& up : sk->upgrades)
            if (up.unlocked) unlocked++;
        int total = static_cast<int>(sk->upgrades.size());

        std::string label = sk->name + elemStr
            + "  Lv." + std::to_string(sk->level)
            + "  Pts:" + std::to_string(sk->skillPoints);
        if (total > 0)
            label += "  [" + std::to_string(unlocked) + "/" + std::to_string(total) + " upgrades]";
        if (sk->masteryLevel > 0)
            label += "  M" + std::to_string(sk->masteryLevel);

        Color nameColor = (sk->skillPoints > 0) ? CQColors::TextGold : CQColors::TextLight;

        // Class-colored tinted background for skill card
        Color cardBg = CQColors::SkillCardBg(sk->characterClass);
        int cardW = (sk->level >= 50) ? 530 : 650;
        renderer.DrawRect(70, y, cardW, 32, cardBg);

        if (renderer.Button(label, 70, y, cardW, 32, focusIdx++))
            selectedSkillIdx = static_cast<int>(i);

        if (sk->level >= 50)
        {
            std::string masteryLabel = "Mastery";
            if (sk->masteryLevel > 0)
                masteryLabel += " Lv" + std::to_string(sk->masteryLevel);
            if (renderer.Button(masteryLabel, 70 + cardW + 10, y, 110, 32, focusIdx++))
            {
                viewingMasteryIdx = static_cast<int>(i);
                return;
            }
        }

        y += 38;
    }

    // Pagination controls
    if (maxSkillPage > 0)
    {
        int btnY = GRenderer::H - 100;
        if (skillOverviewPage > 0 && renderer.Button("< Prev", 70, btnY, 100, 32, focusIdx++))
            skillOverviewPage--;
        if (skillOverviewPage < maxSkillPage && renderer.Button("Next >", 180, btnY, 100, 32, focusIdx++))
            skillOverviewPage++;
        renderer.DrawText("Page " + std::to_string(skillOverviewPage + 1) + "/" + std::to_string(maxSkillPage + 1),
                          renderer.CenterX(80) - 40, btnY + 8, 14, CQColors::TextDim);
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 100, 120, 36, focusIdx++))
        currentState = GameState::Exploring;
}

void Game::StateCraft()
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (!tutorialCraftingOpened) { tutorialCraftingOpened = true; AddTutorialHint("Crafting", "Combine resources to forge weapons and armor. Higher job levels unlock recipes!"); }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Crafting");
    int y = 110;
    const auto& recipes = crafting.GetRecipes();

    const int recipesPerPage = 12;
    craftMaxPage = static_cast<int>((recipes.size() + recipesPerPage - 1) / recipesPerPage) - 1;
    if (craftPage < 0) craftPage = 0;
    if (craftPage > craftMaxPage) craftPage = craftMaxPage;

    int startIdx = craftPage * recipesPerPage;
    int endIdx = std::min(startIdx + recipesPerPage, static_cast<int>(recipes.size()));

    // Count craftable recipes on this page for focus count
    int craftableOnPage = 0;
    for (int i = startIdx; i < endIdx; ++i)
        if (crafting.CanCraft(i, player->GetInventory())) craftableOnPage++;
    int craftFocusCount = craftableOnPage + 1; // +1 for Back
    if (craftMaxPage > 0) craftFocusCount += 2; // +2 for Prev/Next
    keyboardNav.SetFocusCount(craftFocusCount);
    int craftFocusIdx = 0;

    int col = 0;
    for (int i = startIdx; i < endIdx; ++i)
    {
        const auto& r = recipes[i];
        bool can = crafting.CanCraft(i, player->GetInventory());
        int cx = 70 + col * 310;
        int cy = y;
        std::string n = r.name;
        if (!can) n += " [Missing]";
        renderer.DrawText(n, cx, cy, 16, can ? CQColors::TextLight : CQColors::TextDim);
        cy += 20;
        std::string reqs;
        for (size_t j = 0; j < r.requirements.size(); ++j)
        {
            if (j > 0) reqs += ", ";
            int have = crafting.CountResources(r.requirements[j].resourceName, player->GetInventory());
            reqs += r.requirements[j].resourceName + " x" + std::to_string(r.requirements[j].quantity)
                + " (have:" + std::to_string(have) + ")";
        }
        int maxReqW = 280;
        while (!reqs.empty() && MeasureText(reqs.c_str(), 12) > maxReqW)
            reqs.pop_back();
        renderer.DrawText(reqs, cx + 5, cy, 12, CQColors::TextDim);
        cy += 18;
        if (can && renderer.Button("Craft", cx + 50, cy, 100, 28, craftFocusIdx++))
        {
            auto item = crafting.Craft(i, player->GetInventory());
            if (item)
            {
                wiki.MarkItemDiscovered(item->name);
                jobQuestSystem.UpdateProgress(JobQuestType::Craft, item->name);
                renderer.DrawText("Crafted: " + item->name + "!", cx, cy + 34, 14, CQColors::TextGreen);
            }
        }
        col++;
        if (col >= 2) { col = 0; y += 90; }
    }

    if (craftMaxPage > 0)
    {
        int btnY = GRenderer::H - 80;
        if (craftPage > 0 && renderer.Button("< Prev", 70, btnY, 100, 36, craftFocusIdx++))
            craftPage--;
        if (craftPage < craftMaxPage && renderer.Button("Next >", 180, btnY, 100, 36, craftFocusIdx++))
            craftPage++;
        renderer.DrawText("Page " + std::to_string(craftPage + 1) + "/" + std::to_string(craftMaxPage + 1),
                           renderer.CenterX(80) - 40, btnY + 10, 14, CQColors::TextDim);
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, craftFocusIdx++))
        currentState = GameState::Exploring;
}

void Game::StateReligion()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Shrine of Devotion");
    int y = 110;
    if (religion.GetGod() == GodType::None)
    {
        keyboardNav.SetFocusCount(5);
        renderer.DrawText("Choose a god to devote yourself to:", 70, y, 20, CQColors::TextGold);
        y += 36;
        struct GC { GodType g; const char* n; const char* d; };
        GC gods[4] = {
            { GodType::Shaim,  "Shaim",  "God of purity and rebirth. (+HP & Defense)" },
            { GodType::Karosh, "Karosh", "God of destruction. (+Attack, -Defense)" },
            { GodType::Amala,  "Amala",  "Iron god of craftsmanship. (+DEF & Crafting)" },
            { GodType::Tordo,  "Tordo",  "God of strength and hate. (+Power at a cost)" },
        };
        for (int i = 0; i < 4; ++i)
        {
            if (renderer.Button(gods[i].n, 70, y, 150, 36, i))
                religion.SetGod(gods[i].g);
            renderer.DrawText(gods[i].d, 230, y + 8, 14, CQColors::TextLight);
            y += 44;
        }
        if (renderer.Button("Leave Shrine", renderer.CenterX(160), y + 20, 160, 40, 4))
            currentState = GameState::Exploring;
    }
    else
    {
        keyboardNav.SetFocusCount(5);
        renderer.DrawText("Faith: " + religion.GetGodName(), 70, y, 22, CQColors::TextGold);
        y += 28;
        renderer.DrawText(religion.GetGodDescription(), 70, y, 16, CQColors::TextLight);
        y += 24;
        renderer.DrawText("Devotion Level: " + std::to_string(religion.GetDevotionLevel()) + "/10", 70, y, 16, CQColors::TextLight);
        y += 24;
        int nc = religion.GetNextDevotionCost();
        renderer.DrawText("Gold Donated: " + std::to_string(religion.GetTotalDonated()) + "/" + std::to_string(nc), 70, y, 16, CQColors::TextDim);
        y += 30;

        // Prayer section
        if (religion.CanPray())
        {
            if (renderer.Button("Pray (" + std::to_string(religion.GetPrayerCooldown()) + " left today)", 70, y, 320, 36, 0))
            {
                std::string result = religion.Pray(player);
                AddCombatLog("[Prayer] " + result);
            }
        }
        else
        {
            renderer.DrawText("No prayers remaining today", 70, y, 16, CQColors::TextDim);
        }
        y += 44;

        // Donate
        if (renderer.Button("Donate Gold (" + std::to_string(player->GetInventory().GetGold()) + " available)", 70, y, 320, 40, 1))
        {
            int donate = std::min(nc - religion.GetTotalDonated(), player->GetInventory().GetGold());
            if (donate > 0 && player->GetInventory().RemoveGold(donate))
                if (religion.Donate(donate))
                {
                    std::string msg = religion.ApplyDevotionBonus(player);
                    if (!msg.empty()) AddCombatLog(msg);
                }
        }
        y += 50;

        // God Quest
        const auto& quest = religion.GetActiveQuest();
        renderer.DrawText("--- God Quest ---", 70, y, 16, CQColors::TextGold);
        y += 20;
        if (religion.IsQuestComplete())
        {
            renderer.DrawText("Quest Complete! Reward: +" + std::to_string(quest.rewardDevotion) + " Devotion", 90, y, 14, CQColors::TextGreen);
            y += 18;
            if (renderer.Button("Collect Reward", 90, y, 160, 26, 2))
                religion.CompleteQuest();
            y += 30;
        }
        else
        {
            renderer.DrawText(quest.description, 90, y, 14, CQColors::TextLight);
            y += 18;
            renderer.DrawText("Progress: " + std::to_string(quest.currentCount) + "/" + std::to_string(quest.targetCount), 90, y, 14, CQColors::TextDim);
            y += 24;
        }

        // God Abilities
        auto ab1 = religion.GetGodAbility1();
        auto ab2 = religion.GetGodAbility2();
        if (religion.HasAbility1() || religion.HasAbility2())
        {
            renderer.DrawText("--- God Abilities ---", 70, y, 16, CQColors::TextGold);
            y += 20;
            if (religion.HasAbility1())
            {
                renderer.DrawText(ab1.name + ": " + ab1.description, 90, y, 14, CQColors::TextGreen);
                y += 18;
            }
            else
            {
                renderer.DrawText("[Devotion " + std::to_string(ab1.requiredDevotion) + "] " + ab1.name, 90, y, 14, CQColors::TextDim);
                y += 18;
            }
            if (religion.HasAbility2())
            {
                renderer.DrawText(ab2.name + ": " + ab2.description, 90, y, 14, CQColors::TextGreen);
                y += 18;
            }
            else
            {
                renderer.DrawText("[Devotion " + std::to_string(ab2.requiredDevotion) + "] " + ab2.name, 90, y, 14, CQColors::TextDim);
                y += 18;
            }
        }

        y += 10;
        if (renderer.Button("Abandon Faith", 70, y, 160, 36, 3)) religion.SetGod(GodType::None);
        y += 46;
        if (renderer.Button("Leave Shrine", renderer.CenterX(160), y + 10, 160, 40, 4))
            currentState = GameState::Exploring;
    }
}

void Game::StateShop()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 50, GRenderer::W - 100, GRenderer::H - 110, "Shop");

    auto& inv = player->GetInventory();

    if (shopItems.empty())
    {
        int diff = areas[currentAreaIndex].difficulty;
        int reqLv = std::max(1, (diff - 1) * 5 + 1);
        auto addShop = [&](std::shared_ptr<Item> item) { if (item) { item->requiredLevel = reqLv; shopItems.push_back(item); } };
        addShop(LootTable::CreatePotion(diff));
        addShop(LootTable::CreatePotion(diff));
        addShop(LootTable::CreatePotion(diff));
        addShop(LootTable::CreateCommonWeapon(diff));
        addShop(LootTable::CreateCommonArmor(diff));
        if (RNG::Next(2) == 0)
            addShop(LootTable::CreateRareWeapon(diff));
        else
            addShop(LootTable::CreateRareArmor(diff));
    }

    int xLeft = 70;
    int xRight = GRenderer::W / 2 + 40;
    int y = 100;

    // Gold
    renderer.DrawText("Gold: " + std::to_string(inv.GetGold()), xLeft, y, 20, CQColors::TextGold);
    y += 30;

    // Shop items (left side)
    renderer.DrawText("- For Sale -", xLeft, y, 16, CQColors::TextDim);
    y += 24;
    int shopBuyCount = static_cast<int>(shopItems.size());
    int shopSellTotal = static_cast<int>(inv.GetItemCount());
    int shopSellVisible = std::min(SHOP_SELL_PER_PAGE, shopSellTotal);
    int shopSellMaxPage = (shopSellTotal > SHOP_SELL_PER_PAGE) ? ((shopSellTotal - 1) / SHOP_SELL_PER_PAGE) : 0;
    int shopSellNav = (shopSellMaxPage > 0) ? 2 : 0;
    int shopSellCount = shopSellVisible + shopSellNav;
    keyboardNav.SetFocusCount(shopBuyCount + shopSellCount + 3 + 1); // +3 quick-sell + Leave
    int shopFocusIdx = 0;
    for (size_t i = 0; i < shopItems.size(); ++i)
    {
        auto& item = shopItems[i];
        if (!item) continue;
        int iy = y + static_cast<int>(i) * 30;
        int price = item->sellValue * 2;

        std::string line = item->name + "  [" + std::to_string(price) + "g]";
        DrawItemIcon(item->name, item->rarity, xLeft, iy - 2, 20);
        int maxShopW = 190;
        while (!line.empty() && MeasureText(line.c_str(), 14) > maxShopW)
            line.pop_back();
        renderer.DrawText(line, xLeft + 24, iy, 14, RarityColor(static_cast<Rarity>(item->rarity)));

        // Tooltip on hover
        if (renderer.IsMouseInRect(xLeft - 2, iy - 4, 24, 24))
        {
            std::string tip = item->name;
            tip += "\nRarity: " + std::string(RarityName(static_cast<Rarity>(item->rarity)));
            if (item->requiredLevel > 1)
                tip += "\nReq. Level: " + std::to_string(item->requiredLevel);
            tip += "\nPrice: " + std::to_string(price) + "g";
            if (auto w = std::dynamic_pointer_cast<Weapon>(item))
            {
                tip += "\nDamage: " + std::to_string(w->damage);
                tip += "\nWeapon: " + std::string(WeaponTypeName(w->weaponType));
                if (w->element != ElementType::Physical)
                    tip += "\n" + std::string(ElementName(w->element)) + " Damage: " + std::to_string(w->elementDamage);
            }
            else if (auto a = std::dynamic_pointer_cast<Armor>(item))
            {
                tip += "\nDefense: " + std::to_string(a->defense);
                for (const auto& [elem, val] : a->elementalResist)
                    if (val > 0) tip += "\n" + std::string(ElementName(elem)) + " Resist: " + std::to_string(val);
            }
            else if (auto con = std::dynamic_pointer_cast<Consumable>(item))
            {
                tip += "\n" + con->GetDescription();
            }
            renderer.DrawTooltip(tip, static_cast<int>(GetMousePosition().x), static_cast<int>(GetMousePosition().y));
        }

        if (renderer.Button("Buy##" + std::to_string(i), xLeft + 220, iy, 50, 22, shopFocusIdx++))
        {
            if (inv.GetGold() >= price)
            {
                auto bought = item->Clone();
                bought->count = 1;
                if (inv.AddItem(bought))
                {
                    inv.RemoveGold(price);
                    wiki.MarkItemDiscovered(bought->name);
                    achievementSystem.UpdateProgress("social_shop_50");
                    achievementSystem.UpdateProgress("social_shop_200");
                    // Remove non-consumable items from stock (potions stay infinite)
                    if (item->type != ItemType::Consumable)
                    {
                        shopItems.erase(shopItems.begin() + i);
                    }
                }
            }
        }
    }

    // Player inventory (right side) — sell mode
    y = 100;
    renderer.DrawText("- Your Items -", xRight, y, 16, CQColors::TextDim);
    y += 24;

    // Quick-sell buttons
    auto quickSell = [&](int maxRarity, const char* label)
    {
        int totalGold = 0;
        int sold = 0;
        for (size_t si = 0; si < inv.GetItemCount(); )
        {
            auto item = inv.GetItem(si);
            if (item && item->rarity <= maxRarity)
            {
                totalGold += item->sellValue * item->count;
                inv.AddGold(item->sellValue * item->count);
                inv.RemoveItem(si);
                sold++;
            }
            else
            {
                si++;
            }
        }
        if (sold > 0)
        {
            achievementSystem.UpdateProgress("social_sell_100", sold);
            achievementSystem.UpdateProgress("social_sell_500", sold);
            achievementSystem.UpdateProgress("social_sell_1000", sold);
            int newTotal = static_cast<int>(inv.GetItemCount());
            int newMaxPage = (newTotal > SHOP_SELL_PER_PAGE) ? ((newTotal - 1) / SHOP_SELL_PER_PAGE) : 0;
            if (shopSellPage > newMaxPage) shopSellPage = newMaxPage;
        }
    };
    int qsy = y;
    if (renderer.Button("Sell ≤ Rare", xRight, qsy, 120, 22, shopFocusIdx++))
        quickSell(static_cast<int>(Rarity::Rare), "Sell ≤ Rare");
    if (renderer.Button("Sell ≤ Uncommon", xRight + 126, qsy, 140, 22, shopFocusIdx++))
        quickSell(static_cast<int>(Rarity::Uncommon), "Sell ≤ Uncommon");
    if (renderer.Button("Sell ≤ Common", xRight + 272, qsy, 130, 22, shopFocusIdx++))
        quickSell(static_cast<int>(Rarity::Common), "Sell ≤ Common");
    y += 28;

    if (inv.GetItemCount() == 0)
    {
        renderer.DrawText("No items to sell.", xRight, y, 14, CQColors::TextDim);
    }
    else
    {
        const int sellTotal = static_cast<int>(inv.GetItemCount());
        int sellMaxPage = (sellTotal > SHOP_SELL_PER_PAGE) ? ((sellTotal - 1) / SHOP_SELL_PER_PAGE) : 0;
        if (shopSellPage < 0) shopSellPage = 0;
        if (shopSellPage > sellMaxPage) shopSellPage = sellMaxPage;
        int sellStart = shopSellPage * SHOP_SELL_PER_PAGE;
        int sellEnd = std::min(sellStart + SHOP_SELL_PER_PAGE, sellTotal);
        for (int idx = sellStart; idx < sellEnd; ++idx)
        {
            size_t i = static_cast<size_t>(idx);
            auto item = inv.GetItem(i);
            if (!item) continue;
            int iy = y + (idx - sellStart) * 30;
            std::string line = item->name;
            if (item->count > 1) line += " x" + std::to_string(item->count);
            line += "  [" + std::to_string(item->sellValue) + "g]";
            DrawItemIcon(item->name, item->rarity, xRight, iy - 2, 20);
            int maxSellW = 190;
            while (!line.empty() && MeasureText(line.c_str(), 14) > maxSellW)
                line.pop_back();
            renderer.DrawText(line, xRight + 24, iy, 14, RarityColor(static_cast<Rarity>(item->rarity)));
            if (renderer.Button("Sell##" + std::to_string(i), xRight + 220, iy, 50, 22, shopFocusIdx++))
            {
                inv.AddGold(item->sellValue);
                inv.RemoveOneItem(i);
                achievementSystem.UpdateProgress("social_sell_100");
                achievementSystem.UpdateProgress("social_sell_500");
                achievementSystem.UpdateProgress("social_sell_1000");
                // Re-clamp page in case list shrank below current page
                int newTotal = static_cast<int>(inv.GetItemCount());
                int newMaxPage = (newTotal > SHOP_SELL_PER_PAGE) ? ((newTotal - 1) / SHOP_SELL_PER_PAGE) : 0;
                if (shopSellPage > newMaxPage) shopSellPage = newMaxPage;
            }
        }

        // Sell-side page nav (below item list)
        if (sellMaxPage > 0)
        {
            int navY = y + (sellEnd - sellStart) * 30 + 4;
            if (renderer.Button("< Prev", xRight, navY, 88, 22, shopFocusIdx++) && shopSellPage > 0) shopSellPage--;
            if (renderer.Button("Next >", xRight + 94, navY, 88, 22, shopFocusIdx++) && shopSellPage < sellMaxPage) shopSellPage++;
            renderer.DrawText("Page " + std::to_string(shopSellPage + 1) + " / " + std::to_string(sellMaxPage + 1),
                              xRight + 188, navY + 4, 12, CQColors::TextDim);
        }
    }

    if (renderer.Button("Leave Shop", renderer.CenterX(160), GRenderer::H - 80, 160, 40, shopFocusIdx++))
    {
        shopItems.clear();
        currentState = GameState::Exploring;
    }
}

void Game::StateAreaSelect()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    // Title
    renderer.DrawPanel(100, 30, GRenderer::W - 200, 50, "World Map of Eluna");

    // Node positions — 5 rows (area 11 = Forbidden Citadel)
    struct NodePos { float x, y; };
    std::vector<NodePos> positions(areas.size());

    // Row 0: nodes 0, 1, 2
    positions[0] = { 200, 160 };
    positions[1] = { 512, 160 };
    positions[2] = { 824, 160 };
    // Row 1: nodes 3, 4
    positions[3] = { 356, 300 };
    positions[4] = { 668, 300 };
    // Row 2: nodes 5, 6
    positions[5] = { 356, 440 };
    positions[6] = { 668, 440 };
    // Row 3: nodes 7, 8, 9
    positions[7] = { 200, 580 };
    positions[8] = { 512, 580 };
    positions[9] = { 824, 580 };
    // Row 4: node 10 (Forbidden Citadel)
    if (static_cast<int>(areas.size()) > 10)
        positions[10] = { 512, 700 };

    float nodeRadius = 28.0f;
    int currentNode = currentAreaIndex;

    // Determine reachable areas (connected to current)
    std::vector<bool> reachable(areas.size(), false);
    reachable[currentNode] = true;
    for (int conn : areas[currentNode].connections)
        reachable[conn] = true;

    // Mouse position for hover
    Vector2 mousePos = GetMousePosition();
    int hoveredNode = -1;
    for (int i = 0; i < static_cast<int>(areas.size()); ++i)
    {
        float dx = mousePos.x - positions[i].x;
        float dy = mousePos.y - positions[i].y;
        if (dx * dx + dy * dy <= nodeRadius * nodeRadius)
            hoveredNode = i;
    }

    // Draw lines first (behind nodes)
    for (int i = 0; i < static_cast<int>(areas.size()); ++i)
    {
        for (int conn : areas[i].connections)
        {
            if (conn > i) // draw each line once
            {
                bool bothReachable = reachable[i] && reachable[conn];
                Color lineColor = bothReachable ? CQColors::GoldDim : CQColors::BtnBg;
                int thick = bothReachable ? 2 : 1;
                renderer.DrawLine(
                    static_cast<int>(positions[i].x), static_cast<int>(positions[i].y),
                    static_cast<int>(positions[conn].x), static_cast<int>(positions[conn].y),
                    lineColor, thick);
            }
        }
    }

    // Draw nodes
    keyboardNav.SetFocusCount(static_cast<int>(areas.size()) + 1); // +1 for Back
    for (int i = 0; i < static_cast<int>(areas.size()); ++i)
    {
        int cx = static_cast<int>(positions[i].x);
        int cy = static_cast<int>(positions[i].y);

        Color fillColor;
        Color outlineColor;
        int outlineThick = 2;

        if (i == currentNode)
        {
            fillColor = CQColors::GoldBright;
            outlineColor = CQColors::Gold;
            outlineThick = 3;
        }
        else if (reachable[i])
        {
            // Citadel is locked until Chronos is defeated
            if (i == 10 && !chronosDefeated)
            {
                fillColor = {80, 30, 30, 255};
                outlineColor = CQColors::TextDim;
            }
            else
            {
                fillColor = CQColors::TextGreen;
                outlineColor = CQColors::GoldDim;
            }
        }
        else
        {
            fillColor = CQColors::BtnBg;
            outlineColor = CQColors::TextDim;
        }

        // Hovered highlight
        if (i == hoveredNode && i != currentNode)
        {
            outlineColor = CQColors::FocusBorder;
            outlineThick = 3;
        }

        // Clickable check
        if (reachable[i] && i != currentNode)
        {
            // Gate Forbidden Citadel behind chronosDefeated
            if (i == 10 && !chronosDefeated) continue;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && i == hoveredNode)
            {
                currentAreaIndex = i;
                religion.ResetPrayers();
                player->GetJobSystem().ReduceAllFatigue(10);

                int areasVisited = currentAreaIndex + 1;
                achievementSystem.SetProgress("explore_3_areas", areasVisited);
                achievementSystem.SetProgress("explore_6_areas", areasVisited);
                achievementSystem.SetProgress("explore_all_areas", areasVisited);
            }
        }

        // Draw node
        renderer.DrawCircle(cx, cy, nodeRadius, fillColor);
        renderer.DrawCircleLines(cx, cy, nodeRadius, outlineColor, outlineThick);

        // Area name below node
        renderer.DrawCenteredText(areas[i].name, cy + static_cast<int>(nodeRadius) + 4, 12,
                                  reachable[i] ? CQColors::TextLight : CQColors::TextDim);

        // Difficulty below name
        std::string diffText = "D:" + std::to_string(areas[i].difficulty);
        renderer.DrawCenteredText(diffText, cy + static_cast<int>(nodeRadius) + 18, 11,
                                  reachable[i] ? CQColors::TextGold : CQColors::TextDim);
    }

    // Tooltip for hovered node
    if (hoveredNode >= 0 && hoveredNode < static_cast<int>(areas.size()))
    {
        int tx = static_cast<int>(positions[hoveredNode].x) - 140;
        int ty = static_cast<int>(positions[hoveredNode].y) + static_cast<int>(nodeRadius) + 34;
        if (ty > GRenderer::H - 80) ty = static_cast<int>(positions[hoveredNode].y) - 80;
        renderer.DrawTooltip(areas[hoveredNode].description, tx, ty, 280);
    }

    // Back button
    int backY = GRenderer::H - 60;
    keyboardNav.SetFocusCount(static_cast<int>(areas.size()) + 1);
    if (renderer.Button("Back", renderer.CenterX(120), backY, 120, 40, static_cast<int>(areas.size())))
        currentState = GameState::Exploring;
}

void Game::StateQuestLog()
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (!tutorialQuestsChecked) { tutorialQuestsChecked = true; AddTutorialHint("Quests", "Complete quests for bonus XP and gold. Check objectives carefully!"); }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(60, 50, GRenderer::W - 120, GRenderer::H - 110, "Quest Board");

    auto& qm = player->GetQuestManager();
    qm.UpdateGatherQuests(player->GetInventory());

    const char* tabs[] = { "Available", "In Progress", "Completed" };
    int tx = 100;
    for (int t = 0; t < 3; ++t)
    {
        if (renderer.Button(tabs[t], tx, 100, 120, 30, t))
        {
            questTab = t;
            questPage = 0;
        }
        tx += 130;
    }

    const int PAGE_SIZE = 7;
    int startY = 145;
    int rowH = 60;

    // First pass: count visible quests for this tab
    int totalCount = 0;
    for (size_t i = 0; i < qm.GetQuestCount(); ++i)
    {
        Quest* q = qm.GetQuest(i);
        if (!q) continue;
        if (questTab == 0 && q->status == QuestStatus::NotStarted) totalCount++;
        else if (questTab == 1 && q->status == QuestStatus::InProgress) totalCount++;
        else if (questTab == 2 && q->status == QuestStatus::Completed) totalCount++;
    }

    questMaxPage = (totalCount > PAGE_SIZE) ? ((totalCount - 1) / PAGE_SIZE) : 0;
    if (questPage > questMaxPage) questPage = questMaxPage;
    if (questPage < 0) questPage = 0;

    // Count claimable quests on current page for focus
    int claimableOnPage = 0;
    {
        int tmpSkip = questPage * PAGE_SIZE;
        int tmpDrawn = 0;
        for (size_t i = 0; i < qm.GetQuestCount(); ++i)
        {
            Quest* q = qm.GetQuest(i);
            if (!q) continue;
            bool show = false;
            if (questTab == 0 && q->status == QuestStatus::NotStarted) show = true;
            else if (questTab == 1 && q->status == QuestStatus::InProgress) show = true;
            else if (questTab == 2 && q->status == QuestStatus::Completed) show = true;
            if (!show) continue;
            if (tmpSkip > 0) { tmpSkip--; continue; }
            if (tmpDrawn >= PAGE_SIZE) break;
            tmpDrawn++;
            if (questTab == 2 && q->status == QuestStatus::Completed && !q->rewarded)
                claimableOnPage++;
        }
    }
    int questFocusCount = 3 + claimableOnPage + 1; // tabs + claims + Back
    if (questMaxPage > 0) questFocusCount += 2; // Prev/Next
    keyboardNav.SetFocusCount(questFocusCount);
    int questFocusIdx = 3; // first 3 are tabs

    int skipCount = questPage * PAGE_SIZE;
    int drawnCount = 0;
    bool any = false;

    for (size_t i = 0; i < qm.GetQuestCount(); ++i)
    {
        Quest* q = qm.GetQuest(i);
        if (!q) continue;

        bool show = false;
        if (questTab == 0 && q->status == QuestStatus::NotStarted) show = true;
        else if (questTab == 1 && q->status == QuestStatus::InProgress) show = true;
        else if (questTab == 2 && q->status == QuestStatus::Completed) show = true;

        if (!show) continue;
        any = true;

        if (skipCount > 0) { skipCount--; continue; }
        if (drawnCount >= PAGE_SIZE) break;

        int iy = startY + drawnCount * rowH;
        drawnCount++;

        renderer.DrawText(q->title, 80, iy, 16, CQColors::TextGold);
        renderer.DrawText(q->description, 80, iy + 18, 13, CQColors::TextDim);

        if (q->type == QuestType::Collect && q->status == QuestStatus::InProgress)
            renderer.DrawText(std::to_string(q->currentCount) + "/" + std::to_string(q->targetCount)
                              + " " + q->gatherItemName, 80, iy + 34, 12, CQColors::TextLight);
        else if (q->targetCount > 0 && q->status == QuestStatus::InProgress)
            renderer.DrawText(std::to_string(q->currentCount) + "/" + std::to_string(q->targetCount)
                              + " " + q->targetName, 80, iy + 34, 12, CQColors::TextLight);

        if (questTab == 2 && q->status == QuestStatus::Completed && !q->rewarded)
        {
            if (renderer.Button("Claim##" + std::to_string(i), GRenderer::W - 260, iy, 80, 24, questFocusIdx++))
            {
                int xpReward = q->rewardXP;
                // Apply achievement quest XP bonus
                float achQuestXPBonus = achievementSystem.GetQuestXPBonusByTier();
                if (achQuestXPBonus > 0.0f)
                {
                    int achBonus = static_cast<int>(xpReward * achQuestXPBonus);
                    if (achBonus > 0)
                        xpReward += achBonus;
                }
                player->GainXP(xpReward);
                player->GetInventory().AddGold(q->rewardGold);
                q->rewarded = true;

                // Show quest reward notification
                questRewardNotifications.emplace_back(q->title, xpReward, q->rewardGold);

                // Track quest completion achievements
                achievementSystem.UpdateProgress("quest_first");
                achievementSystem.UpdateProgress("quest_5");
                achievementSystem.UpdateProgress("quest_10");
                achievementSystem.UpdateProgress("quest_25");
                achievementSystem.UpdateProgress("quest_50");
                achievementSystem.UpdateProgress("quest_100");
                if (q->type == QuestType::Kill)
                {
                    achievementSystem.UpdateProgress("quest_kill_10");
                    achievementSystem.UpdateProgress("quest_kill_25");
                    achievementSystem.UpdateProgress("quest_kill_50");
                }
                else if (q->type == QuestType::Collect)
                {
                    achievementSystem.UpdateProgress("quest_gather_10");
                    achievementSystem.UpdateProgress("quest_gather_25");
                    achievementSystem.UpdateProgress("quest_gather_50");
                }
                else if (q->type == QuestType::Explore)
                {
                    achievementSystem.UpdateProgress("quest_explore_5");
                    achievementSystem.UpdateProgress("quest_explore_10");
                    achievementSystem.UpdateProgress("quest_explore_15");
                }
            }
        }
        else if (questTab == 0 && q->status == QuestStatus::NotStarted)
        {
            renderer.DrawText("Visit NPC to accept", GRenderer::W - 260, iy + 4, 12, CQColors::TextDim);
        }
    }

    if (!any)
        renderer.DrawText(questTab == 0 ? "No quests available. Visit NPCs to find quests."
                         : questTab == 1 ? "No active quests."
                         : "No completed quests.",
                         80, startY, 16, CQColors::TextDim);

    // Pagination
    int navY = GRenderer::H - 80;
    if (questPage > 0)
    {
        if (renderer.Button("< Prev", renderer.CenterX(250) - 130, navY, 100, 32, questFocusIdx++))
            questPage--;
    }
    if (questPage < questMaxPage)
    {
        if (renderer.Button("Next >", renderer.CenterX(250) + 30, navY, 100, 32, questFocusIdx++))
            questPage++;
    }
    if (questMaxPage > 0)
    {
        std::string pageStr = "Page " + std::to_string(questPage + 1) + "/" + std::to_string(questMaxPage + 1);
        renderer.DrawText(pageStr, renderer.CenterX(250) - 30, navY + 8, 14, CQColors::TextDim);
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 130, 120, 40, questFocusIdx++))
    {
        questTab = 0;
        questPage = 0;
        currentState = GameState::Exploring;
    }
}

// ============================================================
//  COMBAT
// ============================================================

void Game::StateCombat()
{
    if (!player || !currentEnemy)
    {
        currentState = GameState::Exploring;
        return;
    }

    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    // Draw the new photo-based battle screen
    bool isBoss = false;
    auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
    if (mon && mon->IsBoss())
        isBoss = true;

    int playerXP = player->GetExperience();
    int playerMaxXP = player->GetLevel() >= Character::MAX_LEVEL ? 0 : Character::CalculateRequiredXP(player->GetLevel());
    FactionID areaFaction = reputationSystem.GetFactionByArea(currentAreaIndex);
    int repValue = reputationSystem.GetRepIntoCurrentRank(areaFaction);
    int repMax = reputationSystem.GetRepForCurrentRankBracket(areaFaction);
    std::string repLabel = reputationSystem.GetRankTitle(areaFaction) + " Rep";

    // Compute mastery data for combat display
    std::string masterySkillName;
    int masteryXP = 0, masteryMaxXP = 0, masteryLevel = 0;
    auto& combatLoadout = player->GetSkillLoadout();
    auto getMasteryData = [&](int skillIdx) -> bool {
        auto msk = player->GetSkills().GetSkill(skillIdx);
        if (msk && msk->level >= 50) {
            masterySkillName = msk->name;
            masteryXP = msk->masteryXP;
            masteryMaxXP = msk->GetMasteryXPToLevel();
            masteryLevel = msk->masteryLevel;
            return true;
        }
        return false;
    };
    bool foundMastery = false;
    if (lastUsedSkillIdx >= 0)
        foundMastery = getMasteryData(lastUsedSkillIdx);
    if (!foundMastery) {
        for (int idx : combatLoadout) {
            if (getMasteryData(idx)) { foundMastery = true; break; }
        }
    }
    if (!foundMastery && lastUsedSkillIdx >= 0) {
        auto msk = player->GetSkills().GetSkill(lastUsedSkillIdx);
        if (msk && msk->masteryLevel > 0) {
            masterySkillName = msk->name;
            masteryLevel = msk->masteryLevel;
        }
    }

    BattleRenderer::DrawBattleScreen(renderer, *currentEnemy, *player, combatLog,
                                       combatPhase, enemyFlashTimer, isBoss, "",
                                       &petManager, player->GetLevel(),
                                       playerXP, playerMaxXP,
                                       repValue, repMax, repLabel,
                                       masterySkillName, masteryXP, masteryMaxXP, masteryLevel,
                                       player->charMasteryXP,
                                       player->GetCharMasteryXPToLevel(),
                                       player->charMasteryLevel);

    // Update flash timer after rendering
    if (enemyFlashTimer > 0)
    {
        enemyFlashTimer -= GetFrameTime();
        if (enemyFlashTimer < 0) enemyFlashTimer = 0;
    }

    // BattleLayout for positioning UI elements
    BattleLayout layout;
    layout.Calculate(GRenderer::W, GRenderer::H);

    // Encounter info (dungeon room)
    if (isDungeonBossFight || !roomQueue.empty())
    {
        std::string roomInfo = "Dungeon Room " + std::to_string(currentDungeonRoom + 1);
        if (isDungeonBossFight)
            roomInfo += " [BOSS]";
        else
            roomInfo += " (Enemy " + std::to_string(currentRoomEnemyIndex + 1)
                + "/" + std::to_string(roomQueue.size()) + ")";
        renderer.DrawText(roomInfo, 30, layout.playerBarY - 16, 14, CQColors::TextGold);
    }

    // Actions - draw on top of the battle screen
    if (combatPhase == CombatPhase::PlayerTurn)
    {
        int btnSize = layout.abilitySize;
        int btnSpacing = layout.abilitySpacing;
        
        // Build button list: Attack, loadout skills, Defend, Items, Flee, god abilities
        struct BtnInfo { std::string label; Color color; };
        std::vector<BtnInfo> btns;
        btns.push_back({"Attack", RED});

        const auto& loadout = player->GetSkillLoadout();
        for (size_t j = 0; j < loadout.size(); ++j)
        {
            int idx = loadout[j];
            auto sk = player->GetSkills().GetSkill(idx);
            if (!sk) continue;
            std::string sLabel = sk->name;
            if (sLabel.size() > 10) sLabel = sLabel.substr(0, 10);
            Color sColor = {100, 150, 255, 255};
            btns.push_back({sLabel, sColor});
        }

        btns.push_back({"Defend", {100, 200, 100, 255}});
        btns.push_back({"Items", {255, 200, 50, 255}});
        btns.push_back({"Flee", {180, 180, 180, 255}});

        int godBtnCount = 0;
        if (religion.CanUseGodAbility1()) godBtnCount++;
        if (religion.CanUseGodAbility2()) godBtnCount++;
        int totalButtons = static_cast<int>(btns.size()) + godBtnCount;
        
        int totalW = totalButtons * btnSize + (totalButtons - 1) * btnSpacing;
        int startX = (GRenderer::W - totalW) / 2;
        int btnY = layout.abilityY;

        keyboardNav.SetFocusCount(totalButtons);

        int focusIdx = 0;
        int loadoutOffset = 1 + static_cast<int>(loadout.size());
        for (size_t b = 0; b < btns.size(); ++b)
        {
            int bx = startX + static_cast<int>(b) * (btnSize + btnSpacing);
            if (renderer.Button(btns[b].label, bx, btnY, btnSize, btnSize, focusIdx++, 14))
            {
                if (b == 0)
                    DoPlayerAttack(CombatAction::Attack, player->GetAttackSkillIndex());
                else if (b < loadoutOffset && b > 0)
                {
                    lastUsedSkillIdx = loadout[b - 1];
                    DoPlayerAttack(CombatAction::UseSkill, loadout[b - 1]);
                }
                else if (btns[b].label == "Defend")
                    DoPlayerAttack(CombatAction::Defend);
                else if (btns[b].label == "Items")
                    combatPhase = CombatPhase::ItemSelect;
                else if (btns[b].label == "Flee")
                {
                    if (isDungeonBossFight)
                    {
                        AddCombatLog("There is no escape from a boss!");
                    }
                    else
                    {
                        int chance = std::clamp(50 + 5 * (player->GetLevel() - currentEnemy->GetLevel()), 10, 95);
                        if (RNG::Percent() < chance)
                        {
                            leveledUpThisCombat = false;
                            AddCombatLog("You fled from combat!");
                            if (!roomQueue.empty()) roomQueue.clear();
                            currentEnemy.reset();
                            currentState = GameState::Exploring;
    }
    else if (repQuestTab == 1)
    {
                            AddCombatLog("Couldn't escape! (" + std::to_string(chance) + "% chance)");
                            combatPhase = CombatPhase::EnemyTurn;
                            enemyActionTime = renderer.GetTime();
                        }
                    }
                }
            }
        }
        
        // God ability buttons
        if (religion.CanUseGodAbility1())
        {
            auto ab1 = religion.GetGodAbility1();
            if (renderer.Button(ab1.name, startX + static_cast<int>(btns.size()) * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++, 14))
                DoPlayerGodAbility(1);
        }
        if (religion.CanUseGodAbility2())
        {
            auto ab2 = religion.GetGodAbility2();
            if (renderer.Button(ab2.name, startX + (static_cast<int>(btns.size()) + (godBtnCount > 1 ? 1 : 0)) * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++, 14))
                DoPlayerGodAbility(2);
        }
    }
    else if (combatPhase == CombatPhase::ItemSelect)
    {
        int panelX = 30;
        int panelY = layout.abilityY - 10;
        int panelW = GRenderer::W - 60;
        int panelH = 200;
        renderer.DrawRect(panelX, panelY, panelW, panelH, {15, 15, 20, 230});
        renderer.DrawRectLines(panelX, panelY, panelW, panelH, {80, 80, 100, 200});

        renderer.DrawText("Select Item:", panelX + 10, panelY + 8, 16, CQColors::TextGold);
        int iy = panelY + 30;
        bool found = false;
        int itemFocusCount = 0;
        for (size_t j = 0; j < player->GetInventory().GetItemCount(); ++j)
        {
            auto item = player->GetInventory().GetItem(j);
            if (!item) continue;
            if (item->type == ItemType::Consumable)
                itemFocusCount++;
            else if (item->type == ItemType::Resource)
            {
                auto res = std::dynamic_pointer_cast<Resource>(item);
                if (res && res->healAmount > 0)
                    itemFocusCount++;
            }
        }
        itemFocusCount++;
        keyboardNav.SetFocusCount(itemFocusCount);
        int focusIdx = 0;
        for (size_t j = 0; j < player->GetInventory().GetItemCount() && iy < panelY + panelH - 30; ++j)
        {
            auto item = player->GetInventory().GetItem(j);
            if (!item) continue;
            bool usable = false;
            std::string label;
            if (item->type == ItemType::Consumable)
            {
                usable = true;
                label = item->name + " (x" + std::to_string(item->count) + ")";
            }
            else if (item->type == ItemType::Resource)
            {
                auto res = std::dynamic_pointer_cast<Resource>(item);
                if (res && res->healAmount > 0)
                {
                    usable = true;
                    label = item->name + " (x" + std::to_string(item->count) + ") [+" + std::to_string(res->healAmount) + " HP]";
                }
            }
            if (!usable) continue;
            found = true;
            if (renderer.Button(label, panelX + 10, iy, 350, 26, focusIdx))
            {
                if (item->type == ItemType::Consumable)
                {
                    DoPlayerUseItem(static_cast<int>(j));
                }
                else if (item->type == ItemType::Resource)
                {
                    auto res = std::dynamic_pointer_cast<Resource>(item);
                    if (res)
                    {
                        if (res->healAmount > 0) player->RestoreHealth(res->healAmount);
                        if (res->manaAmount > 0) player->RestoreMana(res->manaAmount);
                        player->GetInventory().RemoveOneItem(j);
                        std::string msg = "You eat " + res->name + "!";
                        if (res->healAmount > 0) msg += " (+" + std::to_string(res->healAmount) + " HP)";
                        if (res->manaAmount > 0) msg += " (+" + std::to_string(res->manaAmount) + " MP)";
                        AddCombatLog(msg);
                        combatPhase = CombatPhase::EnemyTurn;
                        enemyActionTime = renderer.GetTime();
                    }
                }
            }
            iy += 30;
            focusIdx++;
        }
        if (!found)
        {
            renderer.DrawText("No usable items!", panelX + 10, iy, 14, CQColors::TextDim);
            iy += 22;
        }
        if (renderer.Button("Cancel", panelX + panelW - 110, panelY + 8, 100, 26, focusIdx))
            combatPhase = CombatPhase::PlayerTurn;
    }
    else if (combatPhase == CombatPhase::EnemyTurn)
    {
        if (renderer.GetTime() - enemyActionTime > 0.8)
            DoEnemyTurn();
    }
    else if (combatPhase == CombatPhase::Victory)
    {
        if (leveledUpThisCombat)
        {
            renderer.DrawRect(renderer.CenterX(400), 350, 400, 60, CQColors::Gold);
            renderer.DrawRectLines(renderer.CenterX(400), 350, 400, 60, CQColors::TextLight, 3);
            renderer.DrawCenteredText("LEVEL UP!", 380, 40, CQColors::BgDark);
        }

        // Check if we're in a dungeon with more enemies
        if (!roomQueue.empty() && currentRoomEnemyIndex + 1 < static_cast<int>(roomQueue.size()))
        {
            keyboardNav.SetFocusCount(1);
            if (renderer.Button("Continue", renderer.CenterX(160), 600, 160, 44, 0))
            {
                leveledUpThisCombat = false;
                // Rest between enemies
                int healHP = player->GetMaxHealth() * 30 / 100;
                int healMP = player->GetMaxMana() * 30 / 100;
                player->RestoreHealth(healHP);
                player->RestoreMana(healMP);
                AddCombatLog(inDungeonEncounter
                    ? "You rest and recover between rooms."
                    : "You catch your breath between enemies.");
                currentRoomEnemyIndex++;
                StartCombatWithEnemy(roomQueue[currentRoomEnemyIndex], false);
            }
        }
        else if (isDungeonBossFight)
        {
            keyboardNav.SetFocusCount(1);
            if (renderer.Button("Continue", renderer.CenterX(160), 600, 160, 44, 0))
            {
                leveledUpThisCombat = false;
                // Dungeon done
                dungeonCompleted = true;
                currentEnemy.reset();
                ClearLog();
                currentState = GameState::DungeonComplete;
            }
        }
        else if (inDungeonEncounter)
        {
            keyboardNav.SetFocusCount(1);
            if (renderer.Button("Continue", renderer.CenterX(160), 600, 160, 44, 0))
            {
                leveledUpThisCombat = false;
                currentEnemy.reset();
                ClearLog();
                roomQueue.clear();
                const auto& dungeon = areas[currentAreaIndex].dungeons[currentDungeonIndex];
                if (currentDungeonRoom + 1 < static_cast<int>(dungeon.rooms.size()))
                {
                    currentDungeonRoom++;
                    {
                        const auto& rooms = dungeon.rooms;
                        dungeonRoomEnemyCount = rooms[currentDungeonRoom].minEnemies
                            + RNG::Next(rooms[currentDungeonRoom].maxEnemies
                                       - rooms[currentDungeonRoom].minEnemies + 1);
                    }
                    currentState = GameState::DungeonExplore;
                }
                else
                {
                    dungeonCompleted = true;
                    currentState = GameState::DungeonComplete;
                }
            }
        }
        else if (currentAreaIndex == 10 && citadelSelectedBoss >= 0)
        {
            // Citadel boss defeated — return to boss select
            keyboardNav.SetFocusCount(1);
            if (renderer.Button("Return to Citadel", renderer.CenterX(160), 600, 200, 44, 0))
            {
                leveledUpThisCombat = false;
                citadelSelectedBoss = -1;
                currentEnemy.reset();
                ClearLog();
                currentState = GameState::CitadelBossSelect;
            }
        }
        else
        {
            // Area encounter — offer choice to continue or return
            keyboardNav.SetFocusCount(2);
            int btnY = 550;
            if (renderer.Button("Keep Fighting", renderer.CenterX(160), btnY, 160, 44, 0))
            {
                leveledUpThisCombat = false;
                ClearLog();
                roomQueue.clear();
                auto monsters = areas[currentAreaIndex].spawnMonsters();
                if (!monsters.empty())
                {
                    for (auto& m : monsters)
                        m->GetSkills().AddSkill(std::make_shared<CommonAttack>());
                    isDungeonBossFight = false;
                    inDungeonEncounter = false;
                    StartCombatWithEnemy(monsters[0], false);
                }
                else
                {
                    currentEnemy.reset();
                    currentState = GameState::Exploring;
                }
            }
            else if (renderer.Button("Return to Camp", renderer.CenterX(160), btnY + 50, 160, 44, 1))
            {
                leveledUpThisCombat = false;
                currentEnemy.reset();
                ClearLog();
                roomQueue.clear();
                currentState = GameState::Exploring;
            }
        }
    }
    else if (combatPhase == CombatPhase::Defeat)
    {
        renderer.DrawCenteredText("You were defeated...", 400, 36, CQColors::TextRed);
        if (deathPenaltyApplied)
        {
            std::string penaltyMsg = "Lost " + std::to_string(deathGoldLost) + " gold and "
                + std::to_string(deathXpLost) + " XP. Respawning at camp...";
            renderer.DrawCenteredText(penaltyMsg, 400, 60, CQColors::TextDim);
        }
        keyboardNav.SetFocusCount(1);
        if (renderer.Button("Continue", renderer.CenterX(160), 500, 160, 44, 0))
        {
            leveledUpThisCombat = false;
            if (!roomQueue.empty()) roomQueue.clear();
            currentEnemy.reset();
            ClearLog();
            // Respawn at current area with 50% HP/MP
            player->SetCurrentHealth(player->GetMaxHealth() / 2);
            player->SetCurrentMana(player->GetMaxMana() / 2);
            currentState = GameState::Exploring;
            deathPenaltyApplied = false;
        }
    }
}

void Game::DoPlayerAttack(CombatAction action, int skillIdx)
{
    if (!player || !currentEnemy) return;

    int enemyHpBefore = currentEnemy->GetCurrentHealth();
    int playerHpBefore = player->GetCurrentHealth();
    int playerMpBefore = player->GetCurrentMana();

    std::string result;
    if (action == CombatAction::Defend)
        result = combatSystem->ExecuteTurn(player, currentEnemy, CombatAction::Defend);
    else
        result = combatSystem->ExecuteTurn(player, currentEnemy, action, skillIdx);
    AddCombatLog(result);

    // Floating damage text on enemy
    int enemyDmg = enemyHpBefore - currentEnemy->GetCurrentHealth();
    if (enemyDmg > 0)
    {
        enemyFlashTimer = 0.15f;
        bool isCrit = (result.find("CRITICAL") != std::string::npos);
        Color dmgColor = isCrit ? YELLOW : RED;
        int fontSize = isCrit ? 24 : 18;
        AddFloatingText("-" + std::to_string(enemyDmg), 680.0f, 130.0f, dmgColor, fontSize);
        if (isCrit)
            AddCriticalParticles(680.0f, 140.0f);
        else
            AddParticleBurst(680.0f, 140.0f, RED, 6);
    }

    // Floating heal text on player
    int playerHeal = player->GetCurrentHealth() - playerHpBefore;
    if (playerHeal > 0)
    {
        AddFloatingText("+" + std::to_string(playerHeal), 340.0f, 130.0f, GREEN, 18);
        AddHealParticles(340.0f, 140.0f);
    }

    int manaGain = player->GetCurrentMana() - playerMpBefore;
    if (manaGain > 0)
        AddFloatingText("+" + std::to_string(manaGain) + " MP", 340.0f, 150.0f, BLUE, 14);

    // Floating damage text on player (from reflect or thorns)
    int playerDmg = playerHpBefore - player->GetCurrentHealth();
    if (playerDmg > 0 && action != CombatAction::Defend)
        AddFloatingText("-" + std::to_string(playerDmg), 340.0f, 150.0f, RED, 14);

    if (!currentEnemy->IsAlive())
    {
        AddCombatLog(currentEnemy->GetName() + " has been defeated!");
        AddFloatingText("DEFEATED!", 680.0f, 110.0f, GOLD, 28);
        AddParticleBurst(680.0f, 140.0f, GOLD, 20);
        AddParticleBurst(GRenderer::W - 134.0f, 52.0f, GOLD, 15);
        auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
        if (mon)
            ProcessVictory(mon);
        combatPhase = CombatPhase::Victory;
    }
    else
    {
        // Pet auto-attack (if equipped and enemy alive)
        const Pet* equippedPet = petManager.GetEquippedPet();
        if (equippedPet)
        {
            int petDmgBefore = currentEnemy->GetCurrentHealth();
            int petDamage = petManager.CalculatePetDamage(player->GetLevel());
            currentEnemy->TakeDamage(petDamage, petManager.GetPetElement());
            int petActualDmg = petDmgBefore - currentEnemy->GetCurrentHealth();
            std::string petMsg = petManager.ExecutePetAttack(player->GetLevel(), currentEnemy->GetName());
            AddCombatLog(petMsg);
            if (petActualDmg > 0)
            {
                AddFloatingText("-" + std::to_string(petActualDmg), 680.0f, 170.0f, PURPLE, 16);
                AddParticleBurst(680.0f, 180.0f, PURPLE, 5);
            }

            // Pet special ability proc
            PetAbilityResult abilityResult = petManager.TryProcPetAbility(player->GetLevel());
            if (abilityResult.procced && currentEnemy->IsAlive())
            {
                currentEnemy->ApplyEffect(abilityResult.effect, abilityResult.duration,
                                          abilityResult.potency, abilityResult.abilityName);
                AddCombatLog(abilityResult.message);
                AddFloatingText(abilityResult.abilityName, 680.0f, 150.0f, RED, 14);
                AddParticleBurst(680.0f, 160.0f, RED, 8);
            }

            if (!currentEnemy->IsAlive())
            {
                AddCombatLog(currentEnemy->GetName() + " has been defeated by your pet!");
                AddFloatingText("DEFEATED!", 680.0f, 110.0f, GOLD, 28);
                AddParticleBurst(680.0f, 140.0f, GOLD, 20);
                auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
                if (mon)
                    ProcessVictory(mon);
                combatPhase = CombatPhase::Victory;
                return;
            }
        }

        combatPhase = CombatPhase::EnemyTurn;
        enemyActionTime = renderer.GetTime();
    }
}

void Game::DoPlayerUseItem(int inventoryIndex)
{
    if (!player || !currentEnemy) return;
    auto& inv = player->GetInventory();
    if (inventoryIndex < 0 || inventoryIndex >= static_cast<int>(inv.GetItemCount())) return;
    auto item = inv.GetItem(inventoryIndex);
    if (!item || item->type != ItemType::Consumable) return;

    auto con = std::dynamic_pointer_cast<Consumable>(item);
    if (!con) return;

    int hpBefore = player->GetCurrentHealth();
    int mpBefore = player->GetCurrentMana();
    con->Use(*player);
    inv.RemoveOneItem(inventoryIndex);

    int hpGained = player->GetCurrentHealth() - hpBefore;
    int mpGained = player->GetCurrentMana() - mpBefore;
    std::string msg = "You use " + con->name + "!";
    if (hpGained > 0) msg += " (+" + std::to_string(hpGained) + " HP)";
    if (mpGained > 0) msg += " (+" + std::to_string(mpGained) + " MP)";

    // Floating heal text
    if (hpGained > 0)
    {
        AddFloatingText("+" + std::to_string(hpGained), 340.0f, 130.0f, GREEN, 20);
        AddHealParticles(340.0f, 140.0f);
    }
    if (mpGained > 0)
        AddFloatingText("+" + std::to_string(mpGained) + " MP", 340.0f, 150.0f, BLUE, 16);

    std::string result = combatSystem->ExecuteTurn(player, currentEnemy, CombatAction::UseItem);
    AddCombatLog(msg);
    AddCombatLog(result);

    if (!currentEnemy->IsAlive())
    {
        AddCombatLog(currentEnemy->GetName() + " has been defeated!");
        auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
        if (mon)
            ProcessVictory(mon);
        combatPhase = CombatPhase::Victory;
    }
    else
    {
        // Pet auto-attack after item use
        const Pet* equippedPet = petManager.GetEquippedPet();
        if (equippedPet)
        {
            int petDmgBefore = currentEnemy->GetCurrentHealth();
            int petDamage = petManager.CalculatePetDamage(player->GetLevel());
            currentEnemy->TakeDamage(petDamage, petManager.GetPetElement());
            int petActualDmg = petDmgBefore - currentEnemy->GetCurrentHealth();
            std::string petMsg = petManager.ExecutePetAttack(player->GetLevel(), currentEnemy->GetName());
            AddCombatLog(petMsg);
            if (petActualDmg > 0)
            {
                AddFloatingText("-" + std::to_string(petActualDmg), 680.0f, 170.0f, PURPLE, 16);
                AddParticleBurst(680.0f, 180.0f, PURPLE, 5);
            }

            // Pet special ability proc
            PetAbilityResult abilityResult = petManager.TryProcPetAbility(player->GetLevel());
            if (abilityResult.procced && currentEnemy->IsAlive())
            {
                currentEnemy->ApplyEffect(abilityResult.effect, abilityResult.duration,
                                          abilityResult.potency, abilityResult.abilityName);
                AddCombatLog(abilityResult.message);
                AddFloatingText(abilityResult.abilityName, 680.0f, 150.0f, RED, 14);
                AddParticleBurst(680.0f, 160.0f, RED, 8);
            }

            // Pet skill tree elemental procs (burn/freeze/stun/poison)
            if (currentEnemy->IsAlive())
            {
                auto treeProcs = petManager.TryTreeElementalProcs(player->GetLevel());
                for (auto& tp : treeProcs)
                {
                    if (!currentEnemy->IsAlive()) break;
                    currentEnemy->ApplyEffect(tp.effect, tp.duration,
                                              tp.potency, tp.abilityName);
                    AddCombatLog(tp.message);
                    Color elemCol = PURPLE;
                    if (tp.effect == EffectType::Burn)    elemCol = {255, 100, 50, 255};
                    if (tp.effect == EffectType::Freeze)  elemCol = {100, 200, 255, 255};
                    if (tp.effect == EffectType::Poison)  elemCol = {100, 220, 80, 255};
                    if (tp.effect == EffectType::Stun)    elemCol = {255, 255, 100, 255};
                    AddFloatingText(tp.abilityName, 680.0f, 130.0f, elemCol, 12);
                    AddParticleBurst(680.0f, 140.0f, elemCol, 5);
                }
            }

            if (!currentEnemy->IsAlive())
            {
                AddCombatLog(currentEnemy->GetName() + " has been defeated by your pet!");
                auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
                if (mon)
                    ProcessVictory(mon);
                combatPhase = CombatPhase::Victory;
                return;
            }
        }

        combatPhase = CombatPhase::EnemyTurn;
        enemyActionTime = renderer.GetTime();
    }
}

void Game::DoPlayerGodAbility(int abilityIndex)
{
    if (!player || !currentEnemy) return;

    int playerHpBefore = player->GetCurrentHealth();
    std::string result;

    if (abilityIndex == 1 && religion.CanUseGodAbility1())
        result = religion.ExecuteGodAbility1(player);
    else if (abilityIndex == 2 && religion.CanUseGodAbility2())
        result = religion.ExecuteGodAbility2(player);

    if (result.empty()) return;

    AddCombatLog(result);

    // Floating heal text on player
    int playerHeal = player->GetCurrentHealth() - playerHpBefore;
    if (playerHeal > 0)
    {
        AddFloatingText("+" + std::to_string(playerHeal), 340.0f, 130.0f, GREEN, 18);
        AddHealParticles(340.0f, 140.0f);
    }

    if (!currentEnemy->IsAlive())
    {
        AddCombatLog(currentEnemy->GetName() + " has been defeated!");
        auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
        if (mon)
            ProcessVictory(mon);
        combatPhase = CombatPhase::Victory;
        return;
    }

    // Pet auto-attack after god ability
    const Pet* equippedPet = petManager.GetEquippedPet();
    if (equippedPet)
    {
        int petDmgBefore = currentEnemy->GetCurrentHealth();
        int petDamage = petManager.CalculatePetDamage(player->GetLevel());
        currentEnemy->TakeDamage(petDamage, petManager.GetPetElement());
        int petActualDmg = petDmgBefore - currentEnemy->GetCurrentHealth();
        std::string petMsg = petManager.ExecutePetAttack(player->GetLevel(), currentEnemy->GetName());
        AddCombatLog(petMsg);
        if (petActualDmg > 0)
        {
            AddFloatingText("-" + std::to_string(petActualDmg), 680.0f, 170.0f, PURPLE, 16);
            AddParticleBurst(680.0f, 180.0f, PURPLE, 5);
        }

            // Pet special ability proc
            PetAbilityResult abilityResult = petManager.TryProcPetAbility(player->GetLevel());
            if (abilityResult.procced && currentEnemy->IsAlive())
            {
                currentEnemy->ApplyEffect(abilityResult.effect, abilityResult.duration,
                                          abilityResult.potency, abilityResult.abilityName);
                AddCombatLog(abilityResult.message);
                AddFloatingText(abilityResult.abilityName, 680.0f, 150.0f, RED, 14);
                AddParticleBurst(680.0f, 160.0f, RED, 8);
            }

            // Pet skill tree elemental procs (burn/freeze/stun/poison)
            if (currentEnemy->IsAlive())
            {
                auto treeProcs = petManager.TryTreeElementalProcs(player->GetLevel());
                for (auto& tp : treeProcs)
                {
                    if (!currentEnemy->IsAlive()) break;
                    currentEnemy->ApplyEffect(tp.effect, tp.duration,
                                              tp.potency, tp.abilityName);
                    AddCombatLog(tp.message);
                    Color elemCol = PURPLE;
                    if (tp.effect == EffectType::Burn)    elemCol = {255, 100, 50, 255};
                    if (tp.effect == EffectType::Freeze)  elemCol = {100, 200, 255, 255};
                    if (tp.effect == EffectType::Poison)  elemCol = {100, 220, 80, 255};
                    if (tp.effect == EffectType::Stun)    elemCol = {255, 255, 100, 255};
                    AddFloatingText(tp.abilityName, 680.0f, 130.0f, elemCol, 12);
                    AddParticleBurst(680.0f, 140.0f, elemCol, 5);
                }
            }

            if (!currentEnemy->IsAlive())
        {
            AddCombatLog(currentEnemy->GetName() + " has been defeated by your pet!");
            auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
        if (mon)
            ProcessVictory(mon);
        lastUsedSkillIdx = -1;
        combatPhase = CombatPhase::Victory;
            return;
        }
    }

    combatPhase = CombatPhase::EnemyTurn;
    enemyActionTime = renderer.GetTime();
}

void Game::DoEnemyTurn()
{
    if (!currentEnemy || !player) return;
    
    int playerHpBefore = player->GetCurrentHealth();
    int enemyHpBefore = currentEnemy->GetCurrentHealth();

    auto monster = std::dynamic_pointer_cast<Monster>(currentEnemy);
    EnemyAIType aiType = monster ? monster->GetAIType() : EnemyAIType::Balanced;
    
    CombatAction ea = CombatAction::Attack;
    int es = 0;
    
    int skillCount = currentEnemy->GetSkills().GetSkillCount();
    bool hasSkills = skillCount > 1;
    int healthPercent = (currentEnemy->GetCurrentHealth() * 100) / currentEnemy->GetMaxHealth();
    
    switch (aiType)
    {
        case EnemyAIType::Aggressive:
            // Always attack, but use skills when available for more damage
            if (hasSkills && RNG::Next(3) == 0)
            {
                ea = CombatAction::UseSkill;
                es = 1 + RNG::Next(skillCount - 1);
            }
            break;
            
        case EnemyAIType::Defensive:
            // Defends often (50%), attacks otherwise
            if (RNG::Next(2) == 0)
                ea = CombatAction::Defend;
            else if (hasSkills && RNG::Next(2) == 0)
            {
                ea = CombatAction::UseSkill;
                es = 1 + RNG::Next(skillCount - 1);
            }
            break;
            
        case EnemyAIType::Caster:
            // Prefers skills, only attacks when out of mana
            if (hasSkills && currentEnemy->GetCurrentMana() > 10)
            {
                ea = CombatAction::UseSkill;
                es = 1 + RNG::Next(skillCount - 1);
            }
            break;
            
        case EnemyAIType::Berserker:
            // Attacks when healthy, uses skills when low HP
            if (healthPercent < 50 && hasSkills)
            {
                ea = CombatAction::UseSkill;
                es = 1 + RNG::Next(skillCount - 1);
            }
            break;
            
        case EnemyAIType::Boss:
            // Smart behavior: heals/defends when low, uses skills when possible
            if (healthPercent < 30 && RNG::Next(2) == 0)
                ea = CombatAction::Defend;
            else if (hasSkills)
            {
                ea = CombatAction::UseSkill;
                es = 1 + RNG::Next(skillCount - 1);
            }
            break;
            
        case EnemyAIType::Balanced:
        default:
            // Original random behavior
            {
                int ec = RNG::Next(3);
                switch (ec)
                {
                    case 0: ea = CombatAction::Attack; break;
                    case 1:
                        if (hasSkills)
                        { ea = CombatAction::UseSkill; es = 1 + RNG::Next(skillCount - 1); }
                        break;
                    case 2: ea = CombatAction::Defend; break;
                }
            }
            break;
    }
    
    std::string r = combatSystem->ExecuteTurn(currentEnemy, player, ea, es);
    AddCombatLog(r);

    // Floating damage text on player from enemy attack
    int playerDmg = playerHpBefore - player->GetCurrentHealth();
    if (playerDmg > 0)
    {
        bool crit = (r.find("CRITICAL") != std::string::npos);
        Color dmgColor = crit ? YELLOW : RED;
        int fontSize = crit ? 24 : 18;
        AddFloatingText("-" + std::to_string(playerDmg), 340.0f, 130.0f, dmgColor, fontSize);
        AddParticleBurst(340.0f, 140.0f, RED, crit ? 12 : 6);
    }

    // Floating heal from enemy lifesteal or similar
    int enemyHeal = currentEnemy->GetCurrentHealth() - enemyHpBefore;
    if (enemyHeal > 0)
        AddFloatingText("+" + std::to_string(enemyHeal), 680.0f, 150.0f, GREEN, 14);

    if (!currentEnemy->IsAlive())
    {
        AddCombatLog(currentEnemy->GetName() + " has been defeated by poison!");
        AddFloatingText("DEFEATED!", 680.0f, 110.0f, GOLD, 28);
        AddParticleBurst(680.0f, 140.0f, GOLD, 20);
        auto mon = std::dynamic_pointer_cast<Monster>(currentEnemy);
        if (mon)
            ProcessVictory(mon);
        combatPhase = CombatPhase::Victory;
    }
    else if (!player->IsAlive())
    {
        AddCombatLog("You have been defeated...");
        // Apply death penalty: 10% gold + 10% XP (never de-level)
        int goldLost = player->GetInventory().GetGold() / 10;
        int xpLost = player->GetExperience() / 10;
        player->GetInventory().RemoveGold(goldLost);
        player->SetExperience(std::max(0, player->GetExperience() - xpLost));
        deathGoldLost = goldLost;
        deathXpLost = xpLost;
        deathPenaltyApplied = true;
        lastUsedSkillIdx = -1;
        combatPhase = CombatPhase::Defeat;
    }
    else
        combatPhase = CombatPhase::PlayerTurn;
}

void Game::AddCombatLog(const std::string& msg)
{
    combatLog.push_back(msg);
}

void Game::ClearLog()
{
    combatLog.clear();
}

void Game::ProcessVictory(std::shared_ptr<Monster> enemy)
{
    if (!player || !enemy) return;

    int xp = enemy->GetExperienceReward();
    int gold = enemy->GetGoldReward();

    // Gold find passive
    int goldFindPct = Passives::GetGoldFindPercent(player->GetEquipment());
    if (goldFindPct > 0)
    {
        int bonus = gold * goldFindPct / 100;
        gold += bonus;
        AddCombatLog("Gold Find: +" + std::to_string(bonus) + " gold!");
    }

    // Character mastery gold find
    float charMasteryGold = player->GetCharMasteryGoldFind();
    if (charMasteryGold > 0.0f)
    {
        int cmGBonus = static_cast<int>(gold * charMasteryGold);
        if (cmGBonus > 0)
        {
            gold += cmGBonus;
            AddCombatLog("[Char Mastery] Gold Find: +" + std::to_string(cmGBonus) + " gold!");
        }
    }

    if (player->GetCharacterClass() == CharacterClass::Merchant)
    {
        int bonus = gold / 2;
        gold += bonus;
        AddCombatLog("Merchant bonus: +" + std::to_string(bonus) + " gold!");
    }

    // Achievement gold bonus
    {
        AchievementReward achR = achievementSystem.GetTotalRewards();
        if (achR.goldBonus > 0)
        {
            int achGold = gold * achR.goldBonus / 1000;
            if (achGold > 0)
            {
                gold += achGold;
                AddCombatLog("[Achievement] Gold Bonus: +" + std::to_string(achGold) + " gold!");
            }
        }
    }

    // XP boost passive
    int xpBoostPct = Passives::GetExpBoostPercent(player->GetEquipment());
    if (xpBoostPct > 0)
    {
        int bonus = xp * xpBoostPct / 100;
        xp += bonus;
        AddCombatLog("XP Boost: +" + std::to_string(bonus) + " XP!");
    }

    // Character mastery XP bonus
    float charMasteryXpBonus = player->GetCharMasteryXPBonus();
    if (charMasteryXpBonus > 0.0f)
    {
        int cmBonus = static_cast<int>(xp * charMasteryXpBonus);
        if (cmBonus > 0)
        {
            xp += cmBonus;
            AddCombatLog("[Char Mastery] XP Gain: +" + std::to_string(cmBonus) + " XP!");
        }
    }

    // Achievement XP bonus
    AchievementReward achRewards = achievementSystem.GetTotalRewards();
    if (achRewards.questXPBonus > 0.0f)
    {
        int achBonus = static_cast<int>(xp * achRewards.questXPBonus);
        if (achBonus > 0)
        {
            xp += achBonus;
            AddCombatLog("[Achievement] XP Bonus: +" + std::to_string(achBonus) + " XP!");
        }
    }

    // Pet passive bonuses
    {
        float petXpBonus = petManager.GetXPBonus();
        if (petXpBonus > 0.0f)
        {
            int petBonus = static_cast<int>(xp * petXpBonus);
            if (petBonus > 0)
            {
                xp += petBonus;
                AddCombatLog("[Pet] XP Bonus: +" + std::to_string(petBonus) + " XP!");
            }
        }
        float petGoldBonus = petManager.GetGoldFind();
        if (petGoldBonus > 0.0f)
        {
            int petGBonus = static_cast<int>(gold * petGoldBonus);
            if (petGBonus > 0)
            {
                gold += petGBonus;
                AddCombatLog("[Pet] Gold Find: +" + std::to_string(petGBonus) + " gold!");
            }
        }
    }

    int prevLevel = player->GetLevel();
    player->GainXP(xp);
    player->GetInventory().AddGold(gold);
    AddCombatLog("Gained " + std::to_string(xp) + " XP and " + std::to_string(gold) + " gold!");

    // Pet XP — pets receive 50% of the player's XP reward
    {
        int petXp = xp / 2;
        if (petXp > 0)
        {
            petManager.GainPetXP(petXp);
            if (petManager.HasEvolved())
            {
                Pet* eqPet = petManager.GetEquippedPet();
                if (eqPet)
                {
                    std::string evoMsg = "*** PET EVOLUTION! " + eqPet->GetCurrentName()
                                       + " has " + eqPet->GetEvolutionLabel() + "! ***";
                    AddCombatLog(evoMsg);
                    petManager.AddNotification(eqPet->GetCurrentName(),
                        eqPet->GetEvolutionLabel() + "! New ability: " + eqPet->GetAbilityName() + "!");
                }
                petManager.ClearEvolveFlag();
            }
            if (petManager.HasLeveledUp())
            {
                Pet* eqPet = petManager.GetEquippedPet();
                if (eqPet)
                {
                    AddCombatLog("*** PET LEVEL UP! " + eqPet->GetCurrentName() + " is now level "
                                 + std::to_string(eqPet->level) + "! ***");
                    petManager.AddNotification(eqPet->GetCurrentName(),
                        "Level up! Now level " + std::to_string(eqPet->level) + "!");
                }
                petManager.ClearLevelUpFlag();
            }
            // Pet passives scale with level/evolution — refresh them on the player
            ApplyPetPassivesToPlayer();
        }
    }

    // Pet heal on kill
    int petHealOnKill = petManager.GetHealOnKill();
    if (petHealOnKill > 0)
    {
        int hpBefore = player->GetCurrentHealth();
        player->RestoreHealth(petHealOnKill);
        int healed = player->GetCurrentHealth() - hpBefore;
        if (healed > 0)
            AddCombatLog("[Pet] Healed " + std::to_string(healed) + " HP on kill!");
    }

    if (player->GetLevel() > prevLevel)
    {
        leveledUpThisCombat = true;
        AddCombatLog("*** LEVEL UP! You are now level " + std::to_string(player->GetLevel()) + "! ***");
        achievementSystem.UpdateProgress("level_10", 0);
        achievementSystem.UpdateProgress("level_25", 0);
        achievementSystem.UpdateProgress("level_50", 0);
    }

    // Gold accumulation achievement
    int totalGold = player->GetInventory().GetGold();
    achievementSystem.SetProgress("gold_1000", totalGold);
    achievementSystem.SetProgress("gold_10000", totalGold);
    achievementSystem.SetProgress("gold_100000", totalGold);

    if (religion.GetGod() != GodType::None && religion.GetDevotionLevel() > 0)
    {
        std::string msg = religion.ApplyDevotionBonus(player);
        if (!msg.empty()) AddCombatLog(msg);
    }

    // God quest progress
    if (religion.GetGod() != GodType::None && !religion.IsQuestComplete())
    {
        if (religion.ProgressQuest(enemy->GetName()))
            AddCombatLog("[God Quest] Quest complete! Visit the Shrine to claim your reward.");
    }

    player->GetQuestManager().UpdateKillQuests(enemy->GetName());
    player->GetQuestManager().UpdateGatherQuests(player->GetInventory());

    // Reputation gains from kills
    FactionID killFaction = reputationSystem.GetFactionByArea(currentAreaIndex);
    reputationSystem.AddKillReputation(killFaction, enemy->IsBoss());
    reputationSystem.UpdateRepeatableQuestProgress(enemy->GetName());

    // Check for Legend-rank faction pets
    for (int fi = 0; fi < reputationSystem.GetFactionCount(); ++fi)
    {
        auto fID = static_cast<FactionID>(fi);
        if (reputationSystem.GetRank(fID) == RepRank::Legend)
        {
            std::string petID = petManager.GetFactionLegendPetID(fID);
            if (!petID.empty())
            {
                Pet* p = petManager.GetPet(petID);
                if (p && !p->obtained)
                {
                    petManager.ObtainPet(petID); wiki.MarkPetObtained(petID);
                    AddCombatLog("*** FACTION PET UNLOCKED *** " + p->name + " joins your side!");
                    petManager.AddNotification(p->name, "Legend rank reward!");
                }
            }
        }
    }

    wiki.MarkEnemyDefeated(enemy->GetName());

    // Achievement tracking — combat kills
    achievementSystem.UpdateProgress("combat_first_kill");
    achievementSystem.UpdateProgress("combat_kills_10");
    achievementSystem.UpdateProgress("combat_kills_50");
    achievementSystem.UpdateProgress("combat_kills_100");
    achievementSystem.UpdateProgress("combat_kills_500");
    achievementSystem.UpdateProgress("combat_kills_1000");
    if (enemy->IsBoss())
    {
        achievementSystem.UpdateProgress("combat_boss_1");
        achievementSystem.UpdateProgress("combat_boss_5");
        achievementSystem.UpdateProgress("combat_boss_10");
    }

    // Notify about quest completion (rewards claimed at Quest Board)
    for (size_t i = 0; i < player->GetQuestManager().GetQuestCount(); ++i)
    {
        Quest* q = player->GetQuestManager().GetQuest(i);
        if (q && q->status == QuestStatus::Completed && !q->rewarded)
            AddCombatLog("Quest completed: " + q->title + "! Visit the Quest Board to claim your reward.");
    }

    // Loot — Legendary class gear from Primordial One, else standard loot
    if (enemy->GetName() == "The Primordial One")
    {
        auto loot = LootTable::GenerateLegendaryClassLoot(player->GetCharacterClass());
        for (auto& item : loot)
        {
            player->GetInventory().AddItem(item);
            wiki.MarkItemDiscovered(item->name);
            std::string prefix = (item->rarity >= 5) ? "*** LEGENDARY *** " : (item->rarity >= 4) ? "*** EPIC *** " : "";
            AddCombatLog(prefix + "Legendary Loot: " + item->name);
        }
    }
    else
    {
        int diff = 1 + (player->GetLevel() - 1) / 5;
        int dropTier = enemy->GetDropTier();
        auto loot = enemy->IsBoss()
            ? LootTable::GenerateBossLoot(diff, dropTier, -1, player->GetCharacterClass())
            : LootTable::GenerateLoot(diff, dropTier, -1, player->GetCharacterClass());
        for (auto& item : loot)
        {
            if (player->GetInventory().AddItem(item))
            {
                wiki.MarkItemDiscovered(item->name);
                jobQuestSystem.UpdateProgress(JobQuestType::Collect, item->name);
                if (item->rarity >= 3)
                    jobQuestSystem.UpdateProgress(JobQuestType::QualityFind, item->name);
                if (item->rarity >= 5)
                    jobQuestSystem.UpdateProgress(JobQuestType::FindRare, item->name);
                AddCombatLog("Loot: " + item->name);
            }
            else
            {
                AddCombatLog("Inventory full! Lost: " + item->name);
            }
        }

        // Unique item drop — bosses 30%, normal enemies 3%
        {
            int uniqueChance = enemy->IsBoss() ? 30 : 3;
            if (RNG::Percent() < uniqueChance)
            {
                auto unique = LootTable::GenerateUniqueDrop(enemy->GetName(), diff, enemy->IsBoss(), player->GetCharacterClass());
                if (unique)
                {
                    player->GetInventory().AddItem(unique);
                    wiki.MarkItemDiscovered(unique->name);
                    std::string prefix = (unique->rarity >= 5) ? "*** LEGENDARY DROP *** " : "*** EPIC DROP *** ";
                    AddCombatLog(prefix + unique->name + "!");
                }
            }
        }

        // Special drops from this enemy type
        for (const auto& sd : enemy->GetSpecialDrops())
        {
            if (RNG::Percent() < sd.chance)
            {
                int qty = sd.minQty + (RNG::Next(sd.maxQty - sd.minQty + 1));
                auto sumItem = SummoningRegistry::Create(sd.itemName);
                std::shared_ptr<Item> drop;
                if (sumItem)
                {
                    drop = sumItem;
                }
                else
                {
                    auto res = std::make_shared<Resource>(sd.itemName, dropTier, dropTier * 5);
                    res->count = qty;
                    drop = res;
                }
                if (drop->type == ItemType::Summoning)
                    drop->count = 1;
                player->GetInventory().AddItem(drop);
                wiki.MarkItemDiscovered(sd.itemName);
                std::string prefix = (sumItem) ? "*** ESSENCE DROP *** " : "";
                AddCombatLog(prefix + "Special Drop: " + sd.itemName + " x" + std::to_string(drop->count));
            }
        }

        // Boss pet drop
        if (enemy->IsBoss())
        {
            std::string droppedPetID = petManager.RollBossDropPet(player->GetLevel(), enemy->GetName());
            if (!droppedPetID.empty())
            {
                petManager.ObtainPet(droppedPetID); wiki.MarkPetObtained(droppedPetID);
                const Pet* dropped = petManager.GetPet(droppedPetID);
                if (dropped)
                {
                    AddCombatLog("*** PET DROP *** " + dropped->name + " joins your side!");
                    petManager.AddNotification(dropped->name, "New companion obtained!");
                }
            }
        }

        // Endgame unlock — first Chronos kill
        if (!chronosDefeated && enemy->GetName() == "Chronos, the Time Ender")
        {
            chronosDefeated = true;
            AddCombatLog("*** ENDGAME UNLOCKED *** The Forbidden Citadel awaits!");
            achNotifications.emplace_back("Endgame Unlocked!", "New area and systems available.");
        }

        // Citadel boss kill tracking
        if (currentAreaIndex == 10 && citadelSelectedBoss >= 0 && citadelSelectedBoss < 10)
        {
            citadelBossKillCounts[citadelSelectedBoss]++;
        }
    }
}

// ============================================================
//  ENCOUNTERS
// ============================================================

void Game::TrackEnemyDefeat(const std::string& enemyName)
{
    wiki.MarkEnemyDefeated(enemyName);
}

void Game::StartAreaEncounter()
{
    if (!player || !player->IsAlive()) return;
    if (!tutorialExplored) { tutorialExplored = true; AddTutorialHint("Exploration", "Fight monsters to earn XP and loot. Use skills wisely!"); }
    auto monsters = areas[currentAreaIndex].spawnMonsters();
    if (monsters.empty()) { return; }
    for (auto& m : monsters)
        m->GetSkills().AddSkill(std::make_shared<CommonAttack>());
    roomQueue.clear();
    isDungeonBossFight = false;
    inDungeonEncounter = false;
    StartCombatWithEnemy(monsters[0], false);
}

void Game::StartCombatWithEnemy(std::shared_ptr<Monster> enemy, bool isBoss)
{
    if (!player || !enemy) return;
    if (!tutorialCombatEntered) { tutorialCombatEntered = true; AddTutorialHint("Combat", "Pick a skill or attack. Defend to reduce incoming damage."); }

    religion.ResetGodAbilityCooldowns();
    if (!combatSystem->StartCombat(player, enemy)) { currentState = GameState::Exploring; return; }
    lastUsedSkillIdx = -1;

    // Apply job combat synergy bonuses AFTER StartCombat() which resets tempDefenseBonus/attackBonus
    auto& js = player->GetJobSystem();
    int jobDef = js.GetTotalCombatDefense();
    int jobDmg = js.GetTotalCombatDamage();
    int jobHp = js.GetTotalCombatHealth();
    if (jobDef > 0) player->IncreaseTempDefense(jobDef);
    if (jobDmg > 0) player->SetAttackBonus(player->GetAttackBonus() + player->GetWeaponDamage() * jobDmg / 100);
    if (jobHp > 0) player->SetCurrentHealth(std::min(player->GetCurrentHealth() + jobHp, player->GetMaxHealth()));

    // Apply achievement reward bonuses AFTER StartCombat()
    AchievementReward achRewards = achievementSystem.GetTotalRewards();
    if (achRewards.statBonusATK > 0)
        player->SetAttackBonus(player->GetAttackBonus() + achRewards.statBonusATK);
    if (achRewards.statBonusDEF > 0)
        player->IncreaseTempDefense(achRewards.statBonusDEF);
    if (achRewards.statBonusHP > 0)
        player->SetCurrentHealth(std::min(player->GetCurrentHealth() + achRewards.statBonusHP, player->GetMaxHealth()));

    // Apply tier-based combat bonuses from achievements AFTER StartCombat()
    int achTierATK = achievementSystem.GetCombatATKBonusByTier();
    int achTierDEF = achievementSystem.GetCombatDEFBonusByTier();
    if (achTierATK > 0)
        player->SetAttackBonus(player->GetAttackBonus() + achTierATK);
    if (achTierDEF > 0)
        player->IncreaseTempDefense(achTierDEF);

    currentEnemy = enemy;
    combatPhase = CombatPhase::PlayerTurn;
    if (isBoss)
        AddCombatLog("The " + enemy->GetName() + " appears! Prepare for a tough fight!");
    else if (!roomQueue.empty())
        AddCombatLog("A " + enemy->GetName() + " appears! (Room " + std::to_string(currentDungeonRoom + 1) + ")");
    else
        AddCombatLog("A wild " + enemy->GetName() + " appears!");
    if (jobDef > 0 || jobDmg > 0 || jobHp > 0)
        AddCombatLog("[Jobs] " + js.GetCombatSynergyDescription());
    currentState = GameState::InCombat;
    renderer.StartTransition(0.2f);
}

void Game::SaveGamePrompt()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(150, 50, GRenderer::W - 300, GRenderer::H - 100, "Save Game — Choose Slot");
    auto slots = saveManager.ListSlots();
    int sy = 110;
    keyboardNav.SetFocusCount(SaveGameManager::SLOT_COUNT + 1); // +1 for Cancel
    for (int i = 0; i < SaveGameManager::SLOT_COUNT; ++i)
    {
        bool focused = keyboardNav.IsFocused(i);
        bool hover = renderer.IsMouseInRect(200, sy, GRenderer::W - 400, 60);
        Color bg = (hover || focused) ? CQColors::BtnHover : CQColors::BgPanel;
        renderer.DrawRect(200, sy, GRenderer::W - 400, 60, bg);
        renderer.DrawRectLines(200, sy, GRenderer::W - 400, 60, slots[i].occupied ? CQColors::Gold : CQColors::BorderLight, 1);
        if (focused)
            renderer.DrawFocusHighlight(200, sy, GRenderer::W - 400, 60);
        renderer.DrawText("Slot " + std::to_string(i + 1), 215, sy + 5, 18, CQColors::TextGold);
        if (slots[i].occupied)
        {
            std::string cn;
            switch (slots[i].characterClass) {
                case CharacterClass::Warrior:  cn = "Warrior";  break;
                case CharacterClass::Priest:   cn = "Priest";   break;
                case CharacterClass::Mage:     cn = "Mage";     break;
                case CharacterClass::Archer:   cn = "Archer";   break;
                case CharacterClass::Merchant: cn = "Merchant"; break;
            }
            renderer.DrawText(slots[i].playerName + " - Lv." + std::to_string(slots[i].level) + " " + cn,
                              215, sy + 30, 14, CQColors::TextLight);
        }
        else
        {
            renderer.DrawText("Empty", 215, sy + 30, 14, CQColors::TextDim);
        }
        if ((hover && renderer.IsMouseClickedOn(200, sy, GRenderer::W - 400, 60))
            || (focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))))
        {
            if (slots[i].occupied)
            {
                if (saveConfirmSlot == i + 1)
                {
                    SaveToSlot(i + 1);
                    saveConfirmSlot = -1;
                    return;
                }
                else
                {
                    saveConfirmSlot = i + 1;
                }
            }
            else
            {
                SaveToSlot(i + 1);
                saveConfirmSlot = -1;
                return;
            }
        }
        if (saveConfirmSlot == i + 1)
        {
            renderer.DrawText("Click again to OVERWRITE", 215 + 300, sy + 30, 12, CQColors::TextRed);
        }
        sy += 68;
    }
    if (renderer.Button("Cancel", renderer.CenterX(120), sy + 10, 120, 40, SaveGameManager::SLOT_COUNT))
    {
        saveConfirmSlot = -1;
        currentState = GameState::Exploring;
    }
}

void Game::LoadGamePrompt()
{
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(150, 50, GRenderer::W - 300, GRenderer::H - 100, "Load Game — Choose Slot");
    auto slots = saveManager.ListSlots();
    int sy = 110;
    keyboardNav.SetFocusCount(SaveGameManager::SLOT_COUNT + 1); // +1 for Back
    for (int i = 0; i < SaveGameManager::SLOT_COUNT; ++i)
    {
        bool focused = keyboardNav.IsFocused(i);
        bool hover = renderer.IsMouseInRect(200, sy, GRenderer::W - 400, 60);
        Color bg = (hover || focused) ? CQColors::BtnHover : CQColors::BgPanel;
        renderer.DrawRect(200, sy, GRenderer::W - 400, 60, bg);
        renderer.DrawRectLines(200, sy, GRenderer::W - 400, 60, slots[i].occupied ? CQColors::Gold : CQColors::BorderLight, 1);
        if (focused)
            renderer.DrawFocusHighlight(200, sy, GRenderer::W - 400, 60);
        renderer.DrawText("Slot " + std::to_string(i + 1), 215, sy + 5, 18, CQColors::TextGold);
        if (slots[i].occupied)
        {
            std::string cn;
            switch (slots[i].characterClass) {
                case CharacterClass::Warrior:  cn = "Warrior";  break;
                case CharacterClass::Priest:   cn = "Priest";   break;
                case CharacterClass::Mage:     cn = "Mage";     break;
                case CharacterClass::Archer:   cn = "Archer";   break;
                case CharacterClass::Merchant: cn = "Merchant"; break;
            }
            renderer.DrawText(slots[i].playerName + " - Lv." + std::to_string(slots[i].level) + " " + cn,
                              215, sy + 30, 14, CQColors::TextLight);
            if ((hover && renderer.IsMouseClickedOn(200, sy, GRenderer::W - 400, 60))
                || (focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))))
            {
                LoadFromSlot(i + 1);
                return;
            }
        }
        else
        {
            renderer.DrawText("Empty", 215, sy + 30, 14, CQColors::TextDim);
        }
        sy += 68;
    }
    if (renderer.Button("Back", renderer.CenterX(120), sy + 10, 120, 40, SaveGameManager::SLOT_COUNT))
        currentState = GameState::MainMenu;
}

void Game::SaveToSlot(int slot)
{
    if (!player) { currentState = GameState::Exploring; return; }
    if (saveManager.SaveGame(player, slot, currentAreaIndex, religion, achievementSystem, reputationSystem, petManager, wiki, chronosDefeated, citadelBossKillCounts, legendaryRecipesUnlocked))
        renderer.DrawCenteredText("Game saved to Slot " + std::to_string(slot) + "!", 350, 24, CQColors::TextGreen);
    currentState = GameState::Exploring;
}

void Game::LoadFromSlot(int slot)
{
    bool loadedChronos = false;
    auto loaded = saveManager.LoadGame(slot, currentAreaIndex, religion, achievementSystem, reputationSystem, petManager, wiki, loadedChronos, citadelBossKillCounts, legendaryRecipesUnlocked);
    if (loaded)
    {
        player = loaded;
        chronosDefeated = loadedChronos;
        if (currentAreaIndex < 0 || currentAreaIndex >= static_cast<int>(areas.size()))
            currentAreaIndex = 0;
        selectedSkillIdx = -1;
        viewingMasteryIdx = -1;
        selectedAchievementCategory = -1;
        if (player->GetQuestManager().GetQuestCount() == 0)
            player->GetQuestManager().InitializeDefaultQuests();
        ClearLog();
        currentState = GameState::Exploring;
        ApplyPetPassivesToPlayer();
        for (const auto& p : petManager.GetPets())
        {
            if (p.obtained)
            {
                wiki.MarkPetObtained(p.id);
                wiki.MarkPetObtainedByName(p.name);
            }
        }
        // Mark all carried equipment as discovered
        for (size_t i = 0; i < player->GetInventory().GetItemCount(); ++i)
            if (auto item = player->GetInventory().GetItem(i))
                wiki.MarkItemDiscovered(item->name);
        // Mark equipped items
        auto& eq = player->GetEquipment();
        if (eq.weapon) wiki.MarkItemDiscovered(eq.weapon->name);
        if (eq.offhand) wiki.MarkItemDiscovered(eq.offhand->name);
        if (eq.helmet) wiki.MarkItemDiscovered(eq.helmet->name);
        if (eq.chest) wiki.MarkItemDiscovered(eq.chest->name);
        if (eq.gloves) wiki.MarkItemDiscovered(eq.gloves->name);
        if (eq.pants) wiki.MarkItemDiscovered(eq.pants->name);
        if (eq.boots) wiki.MarkItemDiscovered(eq.boots->name);
        if (eq.ring1) wiki.MarkItemDiscovered(eq.ring1->name);
        if (eq.ring2) wiki.MarkItemDiscovered(eq.ring2->name);
        if (eq.amulet) wiki.MarkItemDiscovered(eq.amulet->name);
    }
}

void Game::ApplyPetPassivesToPlayer()
{
    if (!player) return;
    // Cursor max-HP upward if the pet bonus increased so the player benefits
    // immediately (otherwise they'd have to heal up to the new max).
    int oldMaxHP = player->GetMaxHealth();
    int oldMaxMP = player->GetMaxMana();

    player->SetPetBonuses(
        petManager.GetHealthBonus(),
        petManager.GetManaBonus(),
        petManager.GetDamageBonus(),
        petManager.GetDefenseBonus(),
        petManager.GetCritChance(),
        petManager.GetCritDamage());

    int newMaxHP = player->GetMaxHealth();
    int newMaxMP = player->GetMaxMana();
    if (newMaxHP > oldMaxHP)
        player->SetCurrentHealth(player->GetCurrentHealth() + (newMaxHP - oldMaxHP));
    if (newMaxMP > oldMaxMP)
        player->SetCurrentMana(player->GetCurrentMana() + (newMaxMP - oldMaxMP));
    if (player->GetCurrentHealth() > newMaxHP) player->SetCurrentHealth(newMaxHP);
    if (player->GetCurrentMana() > newMaxMP)   player->SetCurrentMana(newMaxMP);
}

// ============================================================
//  WIKI / CODEX
// ============================================================

void Game::StateWiki()
{
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    keyboardNav.SetFocusCount(1);
    wiki.SetPlayer(player.get());
    wiki.Draw(renderer);
    if (renderer.Button("Back", renderer.CenterX(100), GRenderer::H - 48, 100, 36, 0))
        currentState = GameState::Exploring;
}

// ============================================================
//  NPC DIALOGUE
// ============================================================

void Game::StartNPCDialogue(const NPC& npc)
{
    currentNPC = npc;
    npcDialoguePhase = 0;
    currentState = GameState::NPCDialogue;
}

void Game::StateNPCDialogue()
{
    if (!player) { currentState = GameState::Exploring; return; }

    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(100, 80, GRenderer::W - 200, GRenderer::H - 160, currentNPC.GetName());

    int y = 140;
    std::string dialogue;

    switch (npcDialoguePhase)
    {
        case 0: dialogue = currentNPC.GetDialogue().greeting; break;
        case 1: dialogue = currentNPC.GetDialogue().questOffer; break;
        case 2: dialogue = currentNPC.GetDialogue().questComplete; break;
        case 3: dialogue = currentNPC.GetDialogue().farewell; break;
        default: dialogue = currentNPC.GetDialogue().farewell; break;
    }

    renderer.DrawText(dialogue, 130, y, 18, CQColors::TextLight);
    y += 60;

    auto& qm = player->GetQuestManager();
    const auto& linkedQuests = currentNPC.GetLinkedQuestIndices();

    // Count action buttons for focus
    int actionCount = 0;
    for (int idx : linkedQuests)
    {
        Quest* quest = qm.GetQuest(idx);
        if (!quest) continue;
        if (quest->status == QuestStatus::NotStarted) actionCount++;
        else if (quest->status == QuestStatus::Completed && !quest->rewarded) actionCount++;
    }
    actionCount++; // Goodbye
    keyboardNav.SetFocusCount(actionCount);
    int focusIdx = 0;

    for (int idx : linkedQuests)
    {
        Quest* quest = qm.GetQuest(idx);
        if (!quest) continue;

        if (quest->status == QuestStatus::NotStarted)
        {
            if (renderer.Button("Accept: " + quest->title, 130, y, 320, 36, focusIdx))
            {
                quest->status = QuestStatus::InProgress;
            }
            y += 46;
            focusIdx++;
        }
        else if (quest->status == QuestStatus::InProgress)
        {
            std::string progressText;
            if (quest->type == QuestType::Collect)
                progressText = quest->title + " (" + std::to_string(quest->currentCount) + "/" + std::to_string(quest->targetCount) + " " + quest->gatherItemName + ")";
            else
                progressText = quest->title + " (" + std::to_string(quest->currentCount) + "/" + std::to_string(quest->targetCount) + " " + quest->targetName + ")";

            renderer.DrawText(progressText, 130, y, 14, CQColors::TextLight);
            y += 24;
        }
        else if (quest->status == QuestStatus::Completed && !quest->rewarded)
        {
            if (renderer.Button("Claim: " + quest->title, 130, y, 320, 36, focusIdx))
            {
                int xpReward = quest->rewardXP;
                float achQuestXPBonus = achievementSystem.GetQuestXPBonusByTier();
                if (achQuestXPBonus > 0.0f)
                {
                    int achBonus = static_cast<int>(xpReward * achQuestXPBonus);
                    if (achBonus > 0)
                        xpReward += achBonus;
                }
                player->GainXP(xpReward);
                player->GetInventory().AddGold(quest->rewardGold);
                quest->rewarded = true;

                // Show quest reward notification
                questRewardNotifications.emplace_back(quest->title, xpReward, quest->rewardGold);

                // Reputation from quest completion
                FactionID qFaction = reputationSystem.GetFactionByArea(currentAreaIndex);
                reputationSystem.OnQuestCompleted(qFaction, 10 + quest->rewardXP / 10);

                // Check for Legend-rank faction pet
                if (reputationSystem.GetRank(qFaction) == RepRank::Legend)
                {
                    std::string petID = petManager.GetFactionLegendPetID(qFaction);
                    if (!petID.empty())
                    {
                        Pet* p = petManager.GetPet(petID);
                        if (p && !p->obtained)
                        {
                            petManager.ObtainPet(petID); wiki.MarkPetObtained(petID);
                            questRewardNotifications.emplace_back("Pet Unlocked: " + p->name, 0, 0);
                            petManager.AddNotification(p->name, "Legend rank reward!");
                        }
                    }
                }

                // Track quest completion achievements
                achievementSystem.UpdateProgress("quest_first");
                achievementSystem.UpdateProgress("quest_5");
                achievementSystem.UpdateProgress("quest_10");
                achievementSystem.UpdateProgress("quest_25");
                achievementSystem.UpdateProgress("quest_50");
                achievementSystem.UpdateProgress("quest_100");
                if (quest->type == QuestType::Kill)
                {
                    achievementSystem.UpdateProgress("quest_kill_10");
                    achievementSystem.UpdateProgress("quest_kill_25");
                    achievementSystem.UpdateProgress("quest_kill_50");
                }
                else if (quest->type == QuestType::Collect)
                {
                    achievementSystem.UpdateProgress("quest_gather_10");
                    achievementSystem.UpdateProgress("quest_gather_25");
                    achievementSystem.UpdateProgress("quest_gather_50");
                }
                else if (quest->type == QuestType::Explore)
                {
                    achievementSystem.UpdateProgress("quest_explore_5");
                    achievementSystem.UpdateProgress("quest_explore_10");
                    achievementSystem.UpdateProgress("quest_explore_15");
                }
            }
            y += 46;
            focusIdx++;
        }
    }

    if (renderer.Button("Goodbye", 130, y, 120, 36, focusIdx))
    {
        currentState = GameState::Exploring;
    }
}

void Game::StateAchievements()
{
    int y = 100;
    int focusIdx = 0;

    // Title
    renderer.DrawText("Achievements", 130, y, 28, CQColors::Gold);
    y += 40;

    // Completion stats
    int total = achievementSystem.GetTotalAchievements();
    int unlocked = achievementSystem.GetTotalUnlocked();
    float pct = achievementSystem.GetCompletionPercentage();

    renderer.DrawText("Progress: " + std::to_string(unlocked) + " / " + std::to_string(total) +
                      " (" + std::to_string(static_cast<int>(pct)) + "%)", 130, y, 14, CQColors::TextLight);
    y += 20;

    // Progress bar
    int barW = 400;
    int barH = 12;
    int barX = 130;
    renderer.DrawRect(barX, y, barW, barH, CQColors::BgDark);
    if (total > 0)
    {
        int fillW = static_cast<int>(barW * pct / 100.0f);
        if (fillW > 0)
            renderer.DrawRect(barX, y, fillW, barH, CQColors::Gold);
    }
    y += 24;

    // Category tabs
    static const char* categoryNames[] = { "All", "Job", "Combat", "Quest", "Exploration", "Progression", "Social" };
    static const AchievementCategory categoryValues[] = {
        AchievementCategory::Job, AchievementCategory::Job, AchievementCategory::Combat,
        AchievementCategory::Quest, AchievementCategory::Exploration,
        AchievementCategory::Progression, AchievementCategory::Social
    };

    int tabX = 130;
    for (int i = 0; i < 7; ++i)
    {
        bool selected = (i == 0 && selectedAchievementCategory == -1) || (i > 0 && selectedAchievementCategory == static_cast<int>(categoryValues[i]));
        Color tabColor = selected ? CQColors::Gold : CQColors::TextDim;

        if (renderer.Button(categoryNames[i], tabX, y, 80, 28, focusIdx))
        {
            selectedAchievementCategory = (i == 0) ? -1 : static_cast<int>(categoryValues[i]);
            achievementsPage = 0;
        }
        tabX += 88;
    }
    y += 40;

    // Achievement list — pre-filter by category, then paginate the filtered set
    std::vector<AchievementProgress> allProgress = achievementSystem.GetAllProgress();
    std::vector<const AchievementProgress*> filtered;
    for (const auto& prog : allProgress)
    {
        const AchievementDefinition* def = achievementSystem.GetDefinition(prog.achievementId);
        if (!def) continue;
        if (selectedAchievementCategory != -1 && static_cast<int>(def->category) != selectedAchievementCategory)
            continue;
        filtered.push_back(&prog);
    }
    int achTotal = static_cast<int>(filtered.size());
    int achMaxPage = (achTotal > ACHIEVEMENTS_PER_PAGE) ? ((achTotal - 1) / ACHIEVEMENTS_PER_PAGE) : 0;
    if (achievementsPage < 0) achievementsPage = 0;
    if (achievementsPage > achMaxPage) achievementsPage = achMaxPage;
    int achStart = achievementsPage * ACHIEVEMENTS_PER_PAGE;
    int achEnd = std::min(achStart + ACHIEVEMENTS_PER_PAGE, achTotal);

    for (int ai = achStart; ai < achEnd; ++ai)
    {
        const auto& prog = *filtered[ai];
        const AchievementDefinition* def = achievementSystem.GetDefinition(prog.achievementId);
        if (!def) continue;

        // Draw achievement box
        Color bgColor = prog.unlocked ? CQColors::BgPanel : CQColors::BgDark;
        if (prog.recentlyUnlocked)
            bgColor = (Color){60, 50, 20, 255}; // golden highlight
        renderer.DrawRect(130, y, 500, 40, bgColor);

        // Recently unlocked pulse border
        if (prog.recentlyUnlocked)
        {
            float pulse = 0.5f + 0.5f * sinf(static_cast<float>(GetTime()) * 6.0f);
            Color pulseColor = {255, 215, 0, static_cast<unsigned char>(180 * pulse)};
            renderer.DrawRectLines(130, y, 500, 40, pulseColor, 2);
        }

        // Tier indicator
        Color tierColor;
        switch (def->tier)
        {
            case AchievementTier::Bronze: tierColor = (Color){180, 120, 60, 255}; break;
            case AchievementTier::Silver: tierColor = CQColors::TextLight; break;
            case AchievementTier::Gold: tierColor = CQColors::Gold; break;
            default: tierColor = CQColors::TextDim; break;
        }
        renderer.DrawRect(132, y + 2, 4, 36, tierColor);

        // Achievement name
        Color nameColor = prog.unlocked ? CQColors::TextGold : CQColors::TextDim;
        renderer.DrawText(def->name, 144, y + 4, 14, nameColor);

        // Description
        renderer.DrawText(def->description, 144, y + 22, 11, CQColors::TextDim);

        // Progress bar (right side)
        int pBarW = 120;
        int pBarH = 8;
        int pBarX = 500;
        int pBarY = y + 16;
        renderer.DrawRect(pBarX, pBarY, pBarW, pBarH, CQColors::BgDark);
        if (def->targetValue > 0)
        {
            float progress = static_cast<float>(prog.currentValue) / static_cast<float>(def->targetValue);
            if (progress > 1.0f) progress = 1.0f;
            int fillW = static_cast<int>(pBarW * progress);
            if (fillW > 0)
                renderer.DrawRect(pBarX, pBarY, fillW, pBarH, tierColor);
        }

        // Progress text
        std::string progressText = std::to_string(prog.currentValue) + "/" + std::to_string(def->targetValue);
        renderer.DrawText(progressText, pBarX + pBarW + 8, pBarY - 2, 10, CQColors::TextLight);

        // Status
        if (prog.unlocked)
        {
            renderer.DrawText("UNLOCKED", 640, y + 12, 12, CQColors::TextGreen);
        }

        y += 46;
    }

    // Achievement page nav footer
    if (achMaxPage > 0)
    {
        int navY = y + 4;
        if (renderer.Button("< Prev", 130, navY, 100, 28, focusIdx++) && achievementsPage > 0) achievementsPage--;
        if (renderer.Button("Next >", 240, navY, 100, 28, focusIdx++) && achievementsPage < achMaxPage) achievementsPage++;
        renderer.DrawText("Page " + std::to_string(achievementsPage + 1) + " / " + std::to_string(achMaxPage + 1),
                          350, navY + 6, 13, CQColors::TextDim);
        y = navY + 36;
    }

    // Summary at bottom
    y += 10;
    AchievementReward totalRewards = achievementSystem.GetTotalRewards();
    if (totalRewards.goldBonus > 0 || totalRewards.statBonusHP > 0 || totalRewards.jobXPBonus > 0.0f)
    {
        renderer.DrawText("Total Rewards:", 130, y, 14, CQColors::TextGold);
        y += 20;
        if (totalRewards.goldBonus > 0)
            renderer.DrawText("Gold: +" + std::to_string(totalRewards.goldBonus), 150, y, 12, CQColors::TextLight);
        if (totalRewards.statBonusHP > 0)
            renderer.DrawText("HP: +" + std::to_string(totalRewards.statBonusHP), 280, y, 12, CQColors::TextLight);
        if (totalRewards.statBonusATK > 0)
            renderer.DrawText("ATK: +" + std::to_string(totalRewards.statBonusATK), 380, y, 12, CQColors::TextLight);
        if (totalRewards.statBonusDEF > 0)
            renderer.DrawText("DEF: +" + std::to_string(totalRewards.statBonusDEF), 480, y, 12, CQColors::TextLight);
        y += 18;
        if (totalRewards.jobXPBonus > 0.0f)
            renderer.DrawText("Job XP: +" + std::to_string(static_cast<int>(totalRewards.jobXPBonus * 100)) + "%", 150, y, 12, CQColors::TextLight);
        if (totalRewards.questXPBonus > 0.0f)
            renderer.DrawText("Quest XP: +" + std::to_string(static_cast<int>(totalRewards.questXPBonus * 100)) + "%", 350, y, 12, CQColors::TextLight);
        y += 18;

        // Tier-based bonuses
        float tierJobXP = achievementSystem.GetJobXPBonusByTier();
        float tierQuestXP = achievementSystem.GetQuestXPBonusByTier();
        int tierATK = achievementSystem.GetCombatATKBonusByTier();
        int tierDEF = achievementSystem.GetCombatDEFBonusByTier();
        if (tierJobXP > 0.0f || tierQuestXP > 0.0f || tierATK > 0 || tierDEF > 0)
        {
            renderer.DrawText("Tier Bonuses:", 130, y, 12, CQColors::TextGold);
            y += 16;
            if (tierJobXP > 0.0f)
                renderer.DrawText("Job XP: +" + std::to_string(static_cast<int>(tierJobXP * 100)) + "%", 150, y, 11, CQColors::TextLight);
            if (tierQuestXP > 0.0f)
                renderer.DrawText("Quest XP: +" + std::to_string(static_cast<int>(tierQuestXP * 100)) + "%", 350, y, 11, CQColors::TextLight);
            y += 14;
            if (tierATK > 0)
                renderer.DrawText("ATK: +" + std::to_string(tierATK), 150, y, 11, CQColors::TextLight);
            if (tierDEF > 0)
                renderer.DrawText("DEF: +" + std::to_string(tierDEF), 350, y, 11, CQColors::TextLight);
            y += 18;
        }

        // Unlockables
        renderer.DrawText("Unlockables:", 130, y, 12, CQColors::TextGold);
        y += 16;
        if (achievementSystem.IsNewGamePlusUnlocked())
            renderer.DrawText("[UNLOCKED] New Game+", 150, y, 11, CQColors::TextGreen);
        else
            renderer.DrawText("[LOCKED] Complete all quest achievements", 150, y, 11, CQColors::TextDim);
        y += 14;
        if (achievementSystem.IsBossRushUnlocked())
            renderer.DrawText("[UNLOCKED] Boss Rush Mode", 150, y, 11, CQColors::TextGreen);
        else
            renderer.DrawText("[LOCKED] Complete all combat achievements", 150, y, 11, CQColors::TextDim);
        y += 14;
        if (achievementSystem.IsMasterClassUnlocked())
            renderer.DrawText("[UNLOCKED] Master Class Tree", 150, y, 11, CQColors::TextGreen);
        else
            renderer.DrawText("[LOCKED] Complete all job achievements", 150, y, 11, CQColors::TextDim);
        y += 14;
        if (achievementSystem.IsDevCommentaryUnlocked())
            renderer.DrawText("[UNLOCKED] Developer Commentary", 150, y, 11, CQColors::TextGreen);
        else
            renderer.DrawText("[LOCKED] Unlock all achievements", 150, y, 11, CQColors::TextDim);
    }

    // Back button — place below content or at bottom of screen, whichever is lower
    int backY = std::max(y + 20, GRenderer::H - 60);
    if (renderer.Button("Back", 130, backY, 120, 36, focusIdx))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

// ============================================================
//  REPUTATION SYSTEM
// ============================================================

void Game::DrawReputationNotifications()
{
    float dt = GetFrameTime();
    float yOffset = 60.0f;

    for (int i = static_cast<int>(reputationSystem.notifications.size()) - 1; i >= 0; --i)
    {
        auto& n = reputationSystem.notifications[i];
        if (n.life <= 0.0f) continue;

        float alpha = 1.0f;
        if (n.life < 0.5f) alpha = n.life / 0.5f;

        int panelW = 280;
        int panelH = 48;
        int px = GRenderer::W - panelW - 10;
        int py = static_cast<int>(yOffset);

        float slideProgress = 1.0f;
        if (n.life > n.maxLife - 0.3f)
            slideProgress = (n.maxLife - n.life) / 0.3f;
        px = GRenderer::W - static_cast<int>((panelW + 10) * slideProgress);

        Color bg = {20, 25, 40, static_cast<unsigned char>(220 * alpha)};
        Color border = {80, 140, 220, static_cast<unsigned char>(255 * alpha)};
        Color repColor = {100, 200, 255, static_cast<unsigned char>(255 * alpha)};
        Color srcColor = {180, 180, 170, static_cast<unsigned char>(255 * alpha)};

        DrawRectangle(px, py, panelW, panelH, bg);
        DrawRectangleLines(px, py, panelW, panelH, border);

        const auto& fData = reputationSystem.GetFactionData(n.faction);
        std::string repLine = "+" + std::to_string(n.amount) + " " + fData.name + " Rep";
        DrawText(repLine.c_str(), px + 10, py + 6, 13, repColor);
        DrawText(n.source.c_str(), px + 10, py + 26, 11, srcColor);

        yOffset += panelH + 4.0f;
    }
}

void Game::StateReputation()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    // Faction selection list
    if (selectedFactionIdx < 0)
    {
        renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Faction Reputation");
        int y = 110;

        renderer.DrawText("Total Reputation Earned: " + std::to_string(reputationSystem.GetTotalReputationEarned()),
                          70, y, 14, CQColors::TextGold);
        y += 24;

        // Faction colors — all 20 factions
        Color factionColors[] = {
            {200, 180, 100, 255},  // Guardians - gold
            {60, 160, 60, 255},    // Shadow Wardens - green
            {60, 120, 200, 255},   // Tide Callers - blue
            {200, 80, 40, 255},    // Dragon Scholars - red
            {160, 120, 200, 255},  // Highland Clans - purple
            {100, 60, 140, 255},   // Void Seekers - deep purple
            {220, 200, 100, 255},  // Celestial Order - bright gold
            {80, 40, 120, 255},    // Void Exarchs - dark purple
            {40, 160, 200, 255},   // Arcane Conclave - cyan
            {100, 200, 160, 255},  // Chronos Wardens - teal
            // Forbidden Citadel bosses
            {180, 60, 60, 255},    // Abyssal Sentinel - crimson
            {120, 40, 180, 255},   // Void Empress - violet
            {220, 100, 20, 255},   // Infernal Colossus - orange
            {100, 180, 220, 255},  // Glacial Wraith - ice blue
            {180, 180, 40, 255},   // Storm Arbiter - yellow
            {60, 140, 60, 255},    // Plague Sovereign - sickly green
            {240, 220, 140, 255},  // Holy Arbiter - holy gold
            {160, 80, 200, 255},   // Chrono Overlord - purple
            {40, 20, 80, 255},     // Void Monarch - void dark
            {200, 200, 200, 255},  // The Unbroken - silver
        };

        int totalFactions = reputationSystem.GetFactionCount();
        int totalPages = (totalFactions + FACTIONS_PER_PAGE - 1) / FACTIONS_PER_PAGE;
        if (totalPages < 1) totalPages = 1;
        if (repFactionPage >= totalPages) repFactionPage = totalPages - 1;
        if (repFactionPage < 0) repFactionPage = 0;

        int startIdx = repFactionPage * FACTIONS_PER_PAGE;
        int endIdx = std::min(startIdx + FACTIONS_PER_PAGE, totalFactions);

        int focusCount = (endIdx - startIdx) + 1; // factions + back button
        keyboardNav.SetFocusCount(focusCount);

        for (int i = startIdx; i < endIdx; ++i)
        {
            auto fid = static_cast<FactionID>(i);
            const auto& fData = reputationSystem.GetFactionData(fid);
            Color col = factionColors[i % 20];

            // Faction card
            Color cardBg = {25, 25, 35, 255};
            renderer.DrawRect(70, y, GRenderer::W - 160, 50, cardBg);
            renderer.DrawRectLines(70, y, GRenderer::W - 160, 50, col);

            // Faction name and rank
            std::string header = fData.name + "  [" + reputationSystem.GetRankName(fData.rank) + "]";
            renderer.DrawText(header, 80, y + 4, 16, col);

            // Rep bar
            int barX = 80;
            int barY = y + 24;
            int barW = 350;
            int barH = 14;
            renderer.DrawRect(barX, barY, barW, barH, Color{20, 20, 25, 255});

            RepRank currentRank = fData.rank;
            int currentRep = fData.reputation;
            int nextThreshold = reputationSystem.GetRankThreshold(currentRank);
            if (currentRank != RepRank::Legend)
            {
                int nextRankThreshold = reputationSystem.GetRankThreshold(
                    static_cast<RepRank>(static_cast<int>(currentRank) + 1));
                float progress = 0.0f;
                if (nextRankThreshold > nextThreshold)
                    progress = static_cast<float>(currentRep - nextThreshold)
                             / static_cast<float>(nextRankThreshold - nextThreshold);
                int fillW = static_cast<int>(barW * std::min(progress, 1.0f));
                if (fillW > 0)
                    renderer.DrawRect(barX + 1, barY + 1, fillW - 1, barH - 1, col);

                std::string repText = std::to_string(currentRep) + " / " + std::to_string(nextRankThreshold);
                int textW = MeasureText(repText.c_str(), 11);
                renderer.DrawText(repText, barX + (barW - textW) / 2, barY + 1, 11, CQColors::TextLight);
            }
            else
            {
                renderer.DrawRect(barX + 1, barY + 1, barW - 1, barH - 1, col);
                int textW = MeasureText("MAX", 11);
                renderer.DrawText("MAX", barX + (barW - textW) / 2, barY + 1, 11, CQColors::TextLight);
            }

            // Next rank hint
            if (currentRank != RepRank::Legend)
            {
                int toNext = reputationSystem.GetRepToNextRank(fid);
                renderer.DrawText("Next rank in " + std::to_string(toNext) + " rep",
                                  barX + barW + 10, barY + 1, 11, CQColors::TextDim);
            }

            // Area name
            renderer.DrawText(fData.areaName, GRenderer::W - 180, y + 4, 12, CQColors::TextDim);

            if (renderer.Button("", 70, y, GRenderer::W - 160, 50, i))
            {
                selectedFactionIdx = i;
                repQuestPage = 0;
            }

            y += 58;
        }

        // Faction list pagination
        if (totalPages > 1)
        {
            y += 4;
            if (renderer.Button("< Prev", 90, y, 100, 28, focusCount++))
            {
                if (repFactionPage > 0) repFactionPage--;
            }
            std::string pageText = "Page " + std::to_string(repFactionPage + 1) + " / " + std::to_string(totalPages);
            renderer.DrawText(pageText, GRenderer::W / 2 - 60, y + 6, 14, CQColors::TextGold);
            if (renderer.Button("Next >", 90 + 100 + 200, y, 100, 28, focusCount++))
            {
                if (repFactionPage < totalPages - 1) repFactionPage++;
            }
            y += 36;
        }

        if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, focusCount))
        {
            currentState = GameState::Exploring;
            renderer.StartTransition();
        }
        return;
    }

    // Faction detail view
    auto fid = static_cast<FactionID>(selectedFactionIdx);
    const auto& fData = reputationSystem.GetFactionData(fid);

    Color factionColors[] = {
        {200, 180, 100, 255}, {60, 160, 60, 255}, {60, 120, 200, 255}, {200, 80, 40, 255},
        {160, 120, 200, 255}, {100, 60, 140, 255}, {220, 200, 100, 255}, {80, 40, 120, 255},
        {40, 160, 200, 255}, {100, 200, 160, 255},
        // Forbidden Citadel bosses
        {180, 60, 60, 255}, {120, 40, 180, 255}, {220, 100, 20, 255}, {100, 180, 220, 255},
        {180, 180, 40, 255}, {60, 140, 60, 255}, {240, 220, 140, 255}, {160, 80, 200, 255},
        {40, 20, 80, 255}, {200, 200, 200, 255},
    };
    Color col = factionColors[selectedFactionIdx % 20];

    std::string title = fData.name + " — " + reputationSystem.GetRankName(fData.rank);
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, title);

    int y = 110;
    renderer.DrawText(fData.description, 70, y, 13, CQColors::TextDim);
    y += 20;
    renderer.DrawText("Area: " + fData.areaName + "  |  Reputation: " + std::to_string(fData.reputation),
                      70, y, 14, col);
    y += 24;

    // Tabs: Overview | Repeatable Quests | Shop
    int tabFocusBase = 0;
    if (renderer.Button("Overview", 70, y, 160, 28, tabFocusBase))
    {
        repQuestTab = 0;
        repQuestPage = 0;
        repVendorPage = 0;
    }
    if (renderer.Button("Repeatable Quests", 240, y, 200, 28, tabFocusBase + 1))
    {
        repQuestTab = 1;
        repQuestPage = 0;
        repVendorPage = 0;
    }
    if (renderer.Button("Shop", 450, y, 120, 28, tabFocusBase + 2))
    {
        repQuestTab = 2;
        repQuestPage = 0;
        repVendorPage = 0;
    }
    y += 38;

    int focusIdx = 2;

    if (repQuestTab == 0)
    {
        // Rewards overview
        renderer.DrawText("Rewards by Rank:", 70, y, 16, CQColors::TextGold);
        y += 22;

        for (const auto& reward : fData.rewards)
        {
            bool unlocked = static_cast<int>(fData.rank) >= static_cast<int>(reward.requiredRank);
            Color statusCol = unlocked ? CQColors::TextGreen : CQColors::TextDim;
            std::string status = unlocked ? "[UNLOCKED]" : "[LOCKED]";

            renderer.DrawText(status + "  " + reputationSystem.GetRankName(reward.requiredRank) + ": " + reward.name,
                              90, y, 14, statusCol);
            y += 18;
            renderer.DrawText(reward.description, 110, y, 12, CQColors::TextDim);
            y += 20;
        }

        // Shop discount
        float discount = reputationSystem.GetShopDiscount(fid);
        float xpBonus = reputationSystem.GetXPBonus(fid);
        float combatBonus = reputationSystem.GetCombatStatBonus(fid);

        y += 8;
        renderer.DrawText("Active Bonuses:", 70, y, 14, CQColors::TextGold);
        y += 20;
        if (discount > 0.0f)
            renderer.DrawText("Shop Discount: " + std::to_string(static_cast<int>(discount * 100)) + "%",
                              90, y, 13, CQColors::TextGreen);
        else
            renderer.DrawText("Shop Discount: None", 90, y, 13, CQColors::TextDim);
        y += 18;
        if (xpBonus > 0.0f)
            renderer.DrawText("XP Bonus: " + std::to_string(static_cast<int>(xpBonus * 100)) + "%",
                              90, y, 13, CQColors::TextGreen);
        else
            renderer.DrawText("XP Bonus: None", 90, y, 13, CQColors::TextDim);
        y += 18;
        if (combatBonus > 0.0f)
            renderer.DrawText("Combat Stats: +" + std::to_string(static_cast<int>(combatBonus * 100)) + "%",
                              90, y, 13, CQColors::TextGreen);
        else
            renderer.DrawText("Combat Stats: None", 90, y, 13, CQColors::TextDim);
    }
    else if (repQuestTab == 1)
    {
        // Repeatable quests
        const auto& quests = reputationSystem.GetRepeatableQuests(fid);
        renderer.DrawText("Complete quests to earn reputation and rewards!", 70, y, 13, CQColors::TextDim);
        y += 22;

        int totalQuests = static_cast<int>(quests.size());
        int totalPages = (totalQuests + REPEATABLE_QUESTS_PER_PAGE - 1) / REPEATABLE_QUESTS_PER_PAGE;
        if (totalPages < 1) totalPages = 1;
        if (repQuestPage >= totalPages) repQuestPage = totalPages - 1;
        if (repQuestPage < 0) repQuestPage = 0;

        int startIdx = repQuestPage * REPEATABLE_QUESTS_PER_PAGE;
        int endIdx = std::min(startIdx + REPEATABLE_QUESTS_PER_PAGE, totalQuests);

        for (int i = startIdx; i < endIdx; ++i)
        {
            const auto& q = quests[i];
            bool canAccept = !q.active && !q.completed && static_cast<int>(fData.rank) >= q.requiredRank;
            bool isActive = q.active && !q.completed;
            bool isDone = q.completed;

            Color qCol = isDone ? CQColors::TextGreen : (isActive ? CQColors::TextGold : CQColors::TextLight);
            std::string status = isDone ? "[COMPLETE]" : (isActive ? "[IN PROGRESS]" : "");
            std::string rankReq = " (Requires " + reputationSystem.GetRankName(static_cast<RepRank>(q.requiredRank)) + ")";

            renderer.DrawText(status + " " + q.title + rankReq, 90, y, 14, qCol);
            y += 18;
            renderer.DrawText(q.description, 110, y, 12, CQColors::TextDim);
            y += 16;
            renderer.DrawText("Reward: " + std::to_string(q.reputationReward) + " Rep, "
                              + std::to_string(q.goldReward) + " Gold, "
                              + std::to_string(q.xpReward) + " XP",
                              110, y, 12, CQColors::TextGold);
            y += 16;

            if (isActive)
            {
                std::string progText = "Progress: " + std::to_string(q.currentCount) + "/" + std::to_string(q.targetCount);
                renderer.DrawText(progText, 110, y, 13, CQColors::TextGold);
                y += 18;
            }

            if (canAccept && renderer.Button("Accept##rq" + std::to_string(i), 110, y, 120, 24, focusIdx++))
            {
                reputationSystem.AcceptRepeatableQuest(fid, i);
            }
            else if (isDone && renderer.Button("Claim##rq" + std::to_string(i), 110, y, 120, 24, focusIdx++))
            {
                int repReward = q.reputationReward;
                int goldReward = q.goldReward;
                int xpReward = q.xpReward;

                reputationSystem.ClaimRepeatableQuestReward(fid, i);
                player->GainXP(xpReward);
                player->GetInventory().AddGold(goldReward);

                questRewardNotifications.emplace_back(q.title, xpReward, goldReward);

                // Check for Legend-rank faction pet
                if (reputationSystem.GetRank(fid) == RepRank::Legend)
                {
                    std::string petID = petManager.GetFactionLegendPetID(fid);
                    if (!petID.empty())
                    {
                        Pet* p = petManager.GetPet(petID);
                        if (p && !p->obtained)
                        {
                            petManager.ObtainPet(petID); wiki.MarkPetObtained(petID);
                            questRewardNotifications.emplace_back("Pet Unlocked: " + p->name, 0, 0);
                            petManager.AddNotification(p->name, "Legend rank reward!");
                        }
                    }
                }
            }
            y += 32;
        }

        // Pagination controls
        if (totalPages > 1)
        {
            y += 4;
            if (renderer.Button("< Prev", 90, y, 100, 28, focusIdx++))
            {
                if (repQuestPage > 0) repQuestPage--;
            }
            std::string pageText = "Page " + std::to_string(repQuestPage + 1) + " / " + std::to_string(totalPages);
            renderer.DrawText(pageText, GRenderer::W / 2 - 60, y + 6, 14, CQColors::TextGold);
            if (renderer.Button("Next >", 90 + 100 + 200, y, 100, 28, focusIdx++))
            {
                if (repQuestPage < totalPages - 1) repQuestPage++;
            }
            y += 36;
        }
    }
    else if (repQuestTab == 2)
    {
        const auto& stock = reputationSystem.GetVendorStock(fid);
        renderer.DrawText("Faction Shop — Spend your gold on exclusive gear!", 70, y, 13, CQColors::TextDim);
        y += 10;
        renderer.DrawText("Gold: " + std::to_string(player->GetInventory().GetGold()), 70, y, 14, CQColors::TextGold);
        y += 22;

        int totalItems = static_cast<int>(stock.size());
        int totalPages = (totalItems + VENDOR_ITEMS_PER_PAGE - 1) / VENDOR_ITEMS_PER_PAGE;
        if (totalPages < 1) totalPages = 1;
        if (repVendorPage >= totalPages) repVendorPage = totalPages - 1;
        if (repVendorPage < 0) repVendorPage = 0;

        int startIdx = repVendorPage * VENDOR_ITEMS_PER_PAGE;
        int endIdx = std::min(startIdx + VENDOR_ITEMS_PER_PAGE, totalItems);

        for (int i = startIdx; i < endIdx; ++i)
        {
            const auto& item = stock[i];
            bool rankMet = static_cast<int>(fData.rank) >= static_cast<int>(item.requiredRank);
            bool canAfford = player->GetInventory().GetGold() >= item.goldCost;
            bool canBuy = rankMet && canAfford;

            Color itemCol = canBuy ? CQColors::TextLight : CQColors::TextDim;

            std::string rankLabel = "[" + reputationSystem.GetRankName(item.requiredRank) + "]";
            renderer.DrawText(item.name + "  " + rankLabel, 90, y, 14, itemCol);
            y += 16;
            renderer.DrawText(item.description, 110, y, 12, CQColors::TextDim);
            y += 16;
            std::string priceStr = std::to_string(item.goldCost) + " Gold";
            Color priceCol = canAfford ? CQColors::TextGold : CQColors::TextRed;
            renderer.DrawText(priceStr, 110, y, 13, priceCol);
            y += 18;

            if (!rankMet)
            {
                std::string lockText = "Requires " + reputationSystem.GetRankName(item.requiredRank);
                renderer.DrawText(lockText, 110, y, 12, CQColors::TextRed);
                y += 16;
            }

            if (canBuy && renderer.Button("Buy##vi" + std::to_string(i), 110, y, 100, 24, focusIdx++))
            {
                if (reputationSystem.CanBuyVendorItem(fid, i, player->GetInventory().GetGold()))
                {
                    player->GetInventory().RemoveGold(item.goldCost);
                    auto newItem = item.createItem();
                    if (newItem)
                    {
                        player->GetInventory().AddItem(newItem);
                    }
                }
            }
            y += 30;
        }

        if (totalPages > 1)
        {
            y += 4;
            if (renderer.Button("< Prev", 90, y, 100, 28, focusIdx++))
            {
                if (repVendorPage > 0) repVendorPage--;
            }
            std::string pageText = "Page " + std::to_string(repVendorPage + 1) + " / " + std::to_string(totalPages);
            renderer.DrawText(pageText, GRenderer::W / 2 - 60, y + 6, 14, CQColors::TextGold);
            if (renderer.Button("Next >", 90 + 100 + 200, y, 100, 28, focusIdx++))
            {
                if (repVendorPage < totalPages - 1) repVendorPage++;
            }
            y += 36;
        }
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, focusIdx))
    {
        selectedFactionIdx = -1;
        repQuestTab = 0;
        repQuestPage = 0;
    }
}

// ============================================================
//  PET NOTIFICATIONS
// ============================================================

void Game::DrawPetNotifications()
{
    float yOffset = 100.0f;
    for (size_t i = 0; i < petManager.notifications.size(); ++i)
    {
        auto& n = petManager.notifications[i];
        if (n.life <= 0.0f) continue;

        float alpha = 1.0f;
        if (n.life < 0.5f) alpha = n.life / 0.5f;

        int panelW = 280;
        int panelH = 48;
        int px = GRenderer::W - panelW - 10;
        int py = static_cast<int>(yOffset);

        float slideProgress = 1.0f;
        if (n.life > n.maxLife - 0.3f)
            slideProgress = (n.maxLife - n.life) / 0.3f;
        px = GRenderer::W - static_cast<int>((panelW + 10) * slideProgress);

        Color bg = {25, 15, 35, static_cast<unsigned char>(220 * alpha)};
        Color border = {180, 100, 220, static_cast<unsigned char>(255 * alpha)};
        Color nameColor = {200, 160, 255, static_cast<unsigned char>(255 * alpha)};
        Color msgColor = {180, 180, 170, static_cast<unsigned char>(255 * alpha)};

        DrawRectangle(px, py, panelW, panelH, bg);
        DrawRectangleLines(px, py, panelW, panelH, border);

        DrawText(n.petName.c_str(), px + 10, py + 6, 13, nameColor);
        DrawText(n.message.c_str(), px + 10, py + 26, 11, msgColor);

        yOffset += panelH + 4.0f;
    }
}

// ============================================================
//  PET STATE
// ============================================================

void Game::StatePets()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Pet Gallery");
    int y = 110;

    // Collection summary
    const auto& allPets = petManager.GetPets();
    int ownedCount = 0, evolvedCount = 0, ascendedCount = 0, mythicCount = 0;
    for (const auto& p : allPets)
    {
        if (p.obtained) ownedCount++;
        if (p.obtained && p.evolutionTier >= 1) evolvedCount++;
        if (p.obtained && p.evolutionTier >= 2) ascendedCount++;
        if (p.obtained && p.evolutionTier >= 3) mythicCount++;
    }

    renderer.DrawText("Obtained: " + std::to_string(ownedCount) + " / " + std::to_string(allPets.size())
                      + "   Evolved: " + std::to_string(evolvedCount)
                      + "   Ascended: " + std::to_string(ascendedCount)
                      + "   Mythic: " + std::to_string(mythicCount),
                      70, y, 13, CQColors::TextDim);
    y += 20;

    // Equipped pet summary
    const Pet* equipped = petManager.GetEquippedPet();
    if (equipped)
    {
        renderer.DrawText("Equipped: " + equipped->GetCurrentName()
                          + "  Lv:" + std::to_string(equipped->level)
                          + "  [" + equipped->GetEvolutionLabel() + "]",
                          70, y, 14, CQColors::TextGold);
        y += 18;
    }
    else
    {
        renderer.DrawText("No pet equipped", 70, y, 14, CQColors::TextDim);
        y += 18;
    }
    y += 4;

    // Pet list — paginated (fights off-screen overflow for 20 pets)
    const int totalPets = static_cast<int>(allPets.size());
    int petMaxPage = (totalPets > PETS_PER_PAGE) ? ((totalPets - 1) / PETS_PER_PAGE) : 0;
    if (petListPage < 0) petListPage = 0;
    if (petListPage > petMaxPage) petListPage = petMaxPage;
    int petStart = petListPage * PETS_PER_PAGE;
    int petEnd = std::min(petStart + PETS_PER_PAGE, totalPets);

    int focusIdx = 0;
    for (int pi = petStart; pi < petEnd; ++pi)
    {
        const auto& p = allPets[pi];
        Color elemCol = CQColors::TextLight;
        switch (p.element)
        {
            case ElementType::Fire:      elemCol = {255, 100, 50, 255}; break;
            case ElementType::Ice:       elemCol = {100, 200, 255, 255}; break;
            case ElementType::Lightning: elemCol = {255, 255, 100, 255}; break;
            case ElementType::Arcane:    elemCol = {200, 120, 255, 255}; break;
            case ElementType::Poison:    elemCol = {100, 220, 80, 255}; break;
            case ElementType::Holy:      elemCol = {255, 240, 200, 255}; break;
            default:                     elemCol = {200, 200, 200, 255}; break;
        }

        bool isEquipped = p.equipped;
        Color cardBg = isEquipped ? Color{40, 30, 60, 255}
                      : p.obtained ? Color{25, 25, 35, 255}
                      : Color{18, 18, 22, 255};
        renderer.DrawRect(70, y, GRenderer::W - 160, 48, cardBg);

        // Border color: gold if equipped, element color if obtained, dim if unobtained
        Color borderCol = isEquipped ? CQColors::Gold
                        : p.obtained ? elemCol
                        : Color{60, 60, 70, 255};
        if (p.obtained && p.evolutionTier == 1)
            borderCol = {100, 255, 180, 255};  // green glow for evolved
        else if (p.obtained && p.evolutionTier == 2)
            borderCol = {255, 215, 0, 255};    // gold glow for ascended
        else if (p.obtained && p.evolutionTier == 3)
            borderCol = {255, 100, 255, 255};  // purple glow for mythic
        if (p.obtained && p.rarity == PetRarity::Legendary)
            borderCol = {255, 180, 50, 255};   // legendary orange-gold
        renderer.DrawRectLines(70, y, GRenderer::W - 160, 48, borderCol);

        if (p.obtained)
        {
            // Show full info
            std::string nameStr = p.GetCurrentName();
            if (isEquipped) nameStr += " [EQUIPPED]";
            if (p.evolutionTier > 0)
                nameStr += "  (" + p.GetEvolutionLabel() + ")";
            if (p.rarity == PetRarity::Legendary)
                nameStr += "  [LEGENDARY]";
            Color nameCol = elemCol;
            if (p.rarity == PetRarity::Legendary)
                nameCol = {255, 180, 50, 255};
            renderer.DrawText(nameStr, 80, y + 3, 14, nameCol);

            std::string statsLine = std::string(ElementName(p.element))
                                  + "  ATK:" + std::to_string(p.GetScaledAttack())
                                  + "  Lv:" + std::to_string(p.level)
                                  + " / " + std::to_string(Pet::MAX_PET_LEVEL);
            if (p.level >= Pet::SKILL_TREE_UNLOCK_LEVEL && p.skillPoints > 0)
                statsLine += "  SP:" + std::to_string(p.skillPoints);
            renderer.DrawText(statsLine, 80, y + 18, 11, CQColors::TextDim);

            // XP bar
            if (p.level < Pet::MAX_PET_LEVEL)
            {
                int reqXP = Pet::CalculateRequiredXP(p.level);
                float progress = static_cast<float>(p.experience) / static_cast<float>(reqXP);
                int barX = 80;
                int barW = 150;
                int barH = 8;
                renderer.DrawRect(barX, y + 32, barW, barH, Color{20, 20, 25, 255});
                int fillW = static_cast<int>(barW * std::min(progress, 1.0f));
                if (fillW > 0)
                    renderer.DrawRect(barX + 1, y + 33, fillW - 1, barH - 1, CQColors::TextGold);
            }
            else if (p.skillPoints > 0)
            {
                int reqXP = Pet::CalculateRequiredXP(p.level);
                float progress = (reqXP > 0) ? static_cast<float>(p.experience) / static_cast<float>(reqXP) : 0.0f;
                int barX = 80;
                int barW = 150;
                int barH = 8;
                renderer.DrawRect(barX, y + 32, barW, barH, Color{20, 20, 25, 255});
                int fillW = static_cast<int>(barW * std::min(progress, 1.0f));
                if (fillW > 0)
                    renderer.DrawRect(barX + 1, y + 33, fillW - 1, barH - 1, CQColors::TextGreen);
            }

            // Ability info
            if (p.evolutionTier > 0 && p.specialAbility != EffectType::None)
            {
                renderer.DrawText(p.GetAbilityName(), 340, y + 18, 11, CQColors::TextGreen);
            }

            // Source
            std::string sourceLabel = (p.source == PetSource::FactionLegend) ? "Faction"
                                    : (p.source == PetSource::CitadelDrop) ? "Citadel" : "Boss";
            renderer.DrawText(sourceLabel, GRenderer::W - 250, y + 3, 10, CQColors::TextDim);

            // Equip/Unequip button
            if (!isEquipped)
            {
                if (renderer.Button("Equip##pet" + p.id, GRenderer::W - 240, y + 10, 80, 28, focusIdx++))
                {
                    petManager.EquipPet(p.id);
                    ApplyPetPassivesToPlayer();
                }
            }
            else
            {
                if (renderer.Button("Unequip##pet" + p.id, GRenderer::W - 240, y + 10, 80, 28, focusIdx++))
                {
                    petManager.UnequipAll();
                    ApplyPetPassivesToPlayer();
                }
            }
            // Details button
            if (renderer.Button("Info##pet" + p.id, GRenderer::W - 150, y + 10, 80, 28, focusIdx++))
            {
                viewingPetIdx = pi;
                currentState = GameState::PetDetail;
                renderer.StartTransition();
            }
        }
        else
        {
            // Unobtained — show silhouette
            renderer.DrawText("???", 80, y + 3, 14, Color{80, 80, 90, 255});
            std::string hint = std::string(ElementName(p.element));
            if (p.source == PetSource::FactionLegend)
                hint += "  (Faction Legend reward)";
            else if (p.source == PetSource::CitadelDrop)
                hint += "  (Citadel boss: " + p.bossName + ")";
            else
                hint += "  (Boss drop: " + p.bossName + ")";
            renderer.DrawText(hint, 80, y + 18, 11, Color{60, 60, 70, 255});
        }

        y += 56;
    }

    // Page navigation footer
    int navY = GRenderer::H - 130;
    if (petMaxPage > 0)
    {
        if (renderer.Button("< Prev", 70, navY, 100, 32, focusIdx++))
            if (petListPage > 0) petListPage--;
        if (renderer.Button("Next >", 180, navY, 100, 32, focusIdx++))
            if (petListPage < petMaxPage) petListPage++;
        renderer.DrawText("Page " + std::to_string(petListPage + 1) + " / " + std::to_string(petMaxPage + 1),
                          290, navY + 6, 13, CQColors::TextDim);
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, focusIdx))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

// ---- StateJobQuests ----
void Game::StateJobQuests()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, "Job Quests");
    int y = 90;
    int focusIdx = 0;

    // Generate daily quests if not done today
    jobQuestSystem.GenerateDailyQuests(player->GetJobSystem().GetJobs());

    // Tabs: Active | Available | Completed
    if (renderer.Button("Active", 70, y, 100, 28, focusIdx++))
        jobQuestPage = 0;
    if (renderer.Button("Available", 180, y, 100, 28, focusIdx++))
        jobQuestPage = 1;
    if (renderer.Button("Completed", 290, y, 100, 28, focusIdx++))
        jobQuestPage = 2;
    y += 36;

    renderer.DrawText("Completed total: " + std::to_string(jobQuestSystem.GetTotalQuestsCompleted()),
                      70, y, 12, CQColors::TextDim);
    y += 18;

    std::vector<JobQuest> quests;
    if (jobQuestPage == 0) quests = jobQuestSystem.GetActiveQuests();
    else if (jobQuestPage == 1) quests = jobQuestSystem.GetAvailableQuests();
    else quests = jobQuestSystem.GetCompletedQuests();

    if (quests.empty())
    {
        renderer.DrawText("No quests in this category.", 70, y, 14, CQColors::TextDim);
        y += 20;
    }

    int startIdx = 0;
    int endIdx = std::min(static_cast<int>(quests.size()), startIdx + JOB_QUESTS_PER_PAGE);
    for (int qi = startIdx; qi < endIdx; ++qi)
    {
        const auto& q = quests[qi];
        Color statusCol = CQColors::TextDim;
        std::string statusStr;
        switch (q.status)
        {
            case JobQuestStatus::Available:  statusStr = "Open";    statusCol = CQColors::TextLight; break;
            case JobQuestStatus::Active:     statusStr = "Active";  statusCol = CQColors::TextGold;   break;
            case JobQuestStatus::Completed:  statusStr = "Done";    statusCol = CQColors::TextGreen;  break;
            case JobQuestStatus::Failed:     statusStr = "Failed";  statusCol = CQColors::TextRed;    break;
        }

        renderer.DrawText("[" + statusStr + "] " + q.title, 80, y, 14, statusCol);
        y += 18;
        renderer.DrawText(q.description, 90, y, 12, CQColors::TextDim);
        y += 16;

        if (q.status == JobQuestStatus::Active)
        {
            std::string progressStr = "Progress: " + std::to_string(q.currentCount) + " / " + std::to_string(q.targetCount);
            renderer.DrawText(progressStr, 90, y, 12, CQColors::TextLight);
            y += 16;
        }

        renderer.DrawText("Rewards: " + std::to_string(q.rewardXP) + " XP, "
                          + std::to_string(q.rewardGold) + " Gold, "
                          + std::to_string(q.rewardJobPoints) + " JP",
                          90, y, 11, CQColors::TextGreen);
        y += 16;

        if (q.status == JobQuestStatus::Available)
        {
            if (renderer.Button("Accept", 460, y - 16, 80, 24, focusIdx++))
                jobQuestSystem.AcceptQuest(q.id);
        }
        else if (q.status == JobQuestStatus::Active && q.currentCount >= q.targetCount)
        {
            if (renderer.Button("Claim", 460, y - 16, 80, 24, focusIdx++))
                jobQuestSystem.CompleteQuest(q.id);
        }
        y += 12;
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 70, 120, 36, focusIdx++))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

// ---- StateResourceChain ----
void Game::StateResourceChain()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, "Chain Crafting");
    int y = 90;
    int focusIdx = 0;

    int jobLevel = 1;
    auto& jobs = player->GetJobSystem().GetJobs();
    if (!jobs.empty())
        jobLevel = jobs[0].level;

    renderer.DrawText("Job Level: " + std::to_string(jobLevel), 70, y, 13, CQColors::TextDim);
    y += 20;

    auto recipes = resourceChain.GetAvailableRecipes(jobLevel);

    if (recipes.empty())
    {
        renderer.DrawText("No chain recipes available yet. Level up your jobs!", 70, y, 14, CQColors::TextDim);
        y += 20;
    }

    int totalPages = (static_cast<int>(recipes.size()) > CHAIN_PER_PAGE)
        ? ((static_cast<int>(recipes.size()) - 1) / CHAIN_PER_PAGE + 1) : 1;
    if (chainPage < 0) chainPage = 0;
    if (chainPage >= totalPages) chainPage = totalPages - 1;

    int startIdx = chainPage * CHAIN_PER_PAGE;
    int endIdx = std::min(startIdx + CHAIN_PER_PAGE, static_cast<int>(recipes.size()));

    for (int ri = startIdx; ri < endIdx; ++ri)
    {
        const auto& recipe = recipes[ri];
        bool canCraft = resourceChain.CanCraft(recipe, player->GetInventory(), jobLevel);

        renderer.DrawText(recipe.name + " (Tier " + std::to_string(recipe.resultTier) + ")",
                          80, y, 14, canCraft ? CQColors::TextGold : CQColors::TextDim);
        y += 18;
        renderer.DrawText(recipe.description, 90, y, 12, CQColors::TextDim);
        y += 16;

        std::string ingStr = "Needs: " + recipe.ingredient1 + " x" + std::to_string(recipe.ingredient1Count);
        if (!recipe.ingredient2.empty())
            ingStr += " + " + recipe.ingredient2 + " x" + std::to_string(recipe.ingredient2Count);
        renderer.DrawText(ingStr, 90, y, 12, canCraft ? CQColors::TextLight : CQColors::TextRed);
        y += 16;
        renderer.DrawText("Result: " + recipe.resultName + "  (+" + std::to_string(recipe.bonusXP) + " XP)",
                          90, y, 12, CQColors::TextGreen);
        y += 18;

        if (canCraft)
        {
            if (renderer.Button("Craft", 460, y - 18, 80, 24, focusIdx++))
            {
                auto result = resourceChain.Craft(recipe, player->GetInventory(), jobLevel);
                if (result)
                {
                    player->GetInventory().AddItem(result);
                    jobQuestSystem.UpdateProgress(JobQuestType::ChainCraft, recipe.name);
                }
            }
        }
        y += 8;
    }

    // Pagination
    if (totalPages > 1)
    {
        if (chainPage > 0 && renderer.Button("< Prev", 200, GRenderer::H - 110, 80, 28, focusIdx++))
            chainPage--;
        renderer.DrawText(std::to_string(chainPage + 1) + " / " + std::to_string(totalPages),
                          290, GRenderer::H - 107, 14, CQColors::TextDim);
        if (chainPage < totalPages - 1 && renderer.Button("Next >", 340, GRenderer::H - 110, 80, 28, focusIdx++))
            chainPage++;
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 70, 120, 36, focusIdx++))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

// ---- StateMasterClass ----
void Game::StateMasterClass()
{
    // Redirect to Evolution state where Master Class is shown
    currentState = GameState::Evolution;
    renderer.StartTransition();
}

// ---- StateEscort ----
void Game::StateEscort()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, "Escort Mission");
    int y = 90;
    int focusIdx = 0;

    const Quest* escortQuest = nullptr;
    for (size_t i = 0; i < player->GetQuestManager().GetQuestCount(); ++i)
    {
        Quest* q = player->GetQuestManager().GetQuest(i);
        if (q && q->type == QuestType::Escort && q->status == QuestStatus::InProgress)
        {
            escortQuest = q;
            break;
        }
    }

    if (!escortQuest)
    {
        renderer.DrawText("No active escort mission.", 70, y, 14, CQColors::TextDim);
        y += 20;
        renderer.DrawText("Find an NPC who needs protection on the road!", 70, y, 13, CQColors::TextLight);
    }
    else
    {
        renderer.DrawText(escortQuest->title, 70, y, 18, CQColors::TextGold);
        y += 24;
        renderer.DrawText(escortQuest->description, 70, y, 13, CQColors::TextLight);
        y += 20;

        std::string progressStr = "Enemies defeated: "
            + std::to_string(escortQuest->currentCount) + " / "
            + std::to_string(escortQuest->targetCount);
        renderer.DrawText(progressStr, 70, y, 14, CQColors::TextLight);
        y += 20;

        if (escortQuest->currentCount >= escortQuest->targetCount)
        {
            renderer.DrawText("Escort complete! Return to the quest giver.", 70, y, 14, CQColors::TextGreen);
            y += 20;
        }
        else
        {
            renderer.DrawText("Keep fighting to protect your charge!", 70, y, 14, CQColors::TextDim);
            y += 20;
        }
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 70, 120, 36, focusIdx++))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

void Game::StateEvolution()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Class Evolution");
    int y = 120;
    int focusIdx = 0;

    std::string className;
    switch (player->GetCharacterClass())
    {
        case CharacterClass::Warrior:  className = "Warrior";  break;
        case CharacterClass::Priest:   className = "Priest";   break;
        case CharacterClass::Mage:     className = "Mage";     break;
        case CharacterClass::Archer:   className = "Archer";   break;
        case CharacterClass::Merchant: className = "Merchant"; break;
        default: className = "Unknown"; break;
    }

    if (player->HasClassEvolved())
    {
        renderer.DrawText("You have already evolved!", 70, y, 20, CQColors::TextGold);
        y += 30;
        renderer.DrawText("Class: " + className + " " + player->GetEvolvedClassName(),
                          70, y, 16, CQColors::TextLight);
        y += 22;
        renderer.DrawText("Loadout skills: " + std::to_string(player->GetMaxLoadoutSkills()),
                          70, y, 14, CQColors::TextDim);
        y += 20;

        std::string passive;
        switch (player->GetCharacterClass())
        {
            case CharacterClass::Warrior:  passive = "Hero's Resolve — 10% damage reduction"; break;
            case CharacterClass::Priest:   passive = "Sage's Grace — 15% healing bonus"; break;
            case CharacterClass::Mage:     passive = "Archmage's Insight — 15% mana cost reduction"; break;
            case CharacterClass::Archer:   passive = "Ranger's Precision — 10% critical hit bonus"; break;
            case CharacterClass::Merchant: passive = "Tycoon's Fortune — 25% gold find bonus"; break;
            default: passive = "Unknown"; break;
        }
        renderer.DrawText("Passive: " + passive, 70, y, 14, CQColors::TextGreen);

        y += 30;
        if (player->HasMastered())
        {
            renderer.DrawText("MASTER CLASS: " + player->GetMasterClassName(), 70, y, 20, CQColors::TextGold);
            y += 22;
            renderer.DrawText("+15% all damage, +10% DR, +20% max HP", 70, y, 14, CQColors::TextGreen);
        }
        else
        {
            renderer.DrawText("Master Class (endgame advancement)", 70, y, 16, CQColors::TextLight);
            y += 20;
            renderer.DrawText("New class: " + className + " " + player->GetMasterClassName(),
                              70, y, 14, CQColors::TextGold);
            y += 20;
            renderer.DrawText("Benefits: +15% all damage, +10% DR, +20% max HP",
                              70, y, 13, CQColors::TextDim);
            y += 24;

            bool reqMastery = true;
            for (int b = 0; b < Player::CHAR_MASTERY_BRANCHES && reqMastery; ++b)
                for (int n = 0; n < Player::CHAR_MASTERY_NODES_PER_BRANCH && reqMastery; ++n)
                    if (!player->charMasteryNodes[b][n]) reqMastery = false;

            renderer.DrawText(reqMastery ? "[x]" : "[ ]", 80, y, 14, reqMastery ? CQColors::TextGreen : CQColors::TextDim);
            renderer.DrawText("All 15 Character Mastery nodes unlocked",
                              110, y, 14, reqMastery ? CQColors::TextLight : CQColors::TextDim);
            y += 20;

            if (reqMastery)
            {
                if (renderer.Button("MASTER CLASS", renderer.CenterX(200), y, 200, 44, focusIdx++))
                {
                    player->MasterClass();
                }
            }
            else
            {
                renderer.DrawText("Requirements not met", 70, y, 14, CQColors::TextDim);
            }
        }
    }
    else
    {
        renderer.DrawText("Evolve your class to unlock new power!", 70, y, 16, CQColors::TextLight);
        y += 24;
        renderer.DrawText("New class: " + className + " " + player->GetEvolvedClassName(),
                          70, y, 16, CQColors::TextGold);
        y += 22;
        renderer.DrawText("Benefits: +20% base stats, 6 loadout skill slots, class passive ability",
                          70, y, 13, CQColors::TextDim);
        y += 28;

        renderer.DrawText("Requirements:", 70, y, 16, CQColors::TextLight);
        y += 22;

        // Requirement 1: Level 50+
        bool reqLevel = player->GetLevel() >= 50;
        renderer.DrawText(reqLevel ? "[x]" : "[ ]", 80, y, 14, reqLevel ? CQColors::TextGreen : CQColors::TextDim);
        renderer.DrawText("Reach Level 50 (current: " + std::to_string(player->GetLevel()) + ")",
                          110, y, 14, reqLevel ? CQColors::TextLight : CQColors::TextDim);
        y += 20;

        // Requirement 2: Chronos defeated
        bool reqChronos = chronosDefeated;
        renderer.DrawText(reqChronos ? "[x]" : "[ ]", 80, y, 14, reqChronos ? CQColors::TextGreen : CQColors::TextDim);
        renderer.DrawText("Defeat Chronos, the Time Ender",
                          110, y, 14, reqChronos ? CQColors::TextLight : CQColors::TextDim);
        y += 20;

        // Requirement 3: Devotion 7+
        bool reqDevotion = religion.GetDevotionLevel() >= 7;
        renderer.DrawText(reqDevotion ? "[x]" : "[ ]", 80, y, 14, reqDevotion ? CQColors::TextGreen : CQColors::TextDim);
        renderer.DrawText("Devotion Rank 7+ (current: " + std::to_string(religion.GetDevotionLevel()) + ")",
                          110, y, 14, reqDevotion ? CQColors::TextLight : CQColors::TextDim);
        y += 20;

        // Requirement 4: 3 specific boss essences in inventory
        auto& inv = player->GetInventory();
        std::string essenceNames[] = {"Sentinel's Essence", "Empress's Essence", "Colossus Essence"};
        int essenceCounts[3] = {0, 0, 0};
        for (size_t i = 0; i < inv.GetItemCount(); ++i)
        {
            auto item = inv.GetItem(i);
            if (!item) continue;
            for (int e = 0; e < 3; ++e)
                if (item->name == essenceNames[e])
                    essenceCounts[e] += item->count;
        }
        bool reqEssences = essenceCounts[0] > 0 && essenceCounts[1] > 0 && essenceCounts[2] > 0;
        renderer.DrawText(reqEssences ? "[x]" : "[ ]", 80, y, 14, reqEssences ? CQColors::TextGreen : CQColors::TextDim);
        std::string essenceStr = "Gather 3 Boss Essences (";
        for (int e = 0; e < 3; ++e)
        {
            if (e > 0) essenceStr += ", ";
            std::string shortName = essenceNames[e];
            auto pos = shortName.find(" Essence");
            if (pos != std::string::npos) shortName = shortName.substr(0, pos);
            essenceStr += shortName + ": " + std::to_string(essenceCounts[e]);
        }
        essenceStr += ")";
        renderer.DrawText(essenceStr, 110, y, 14, reqEssences ? CQColors::TextLight : CQColors::TextDim);
        y += 30;

        bool allMet = reqLevel && reqChronos && reqDevotion && reqEssences;

        if (allMet)
        {
            if (renderer.Button("EVOLVE", renderer.CenterX(200), y, 200, 44, focusIdx++))
            {
                player->EvolveClass();
            }
        }
        else
        {
            renderer.DrawText("Requirements not met", 70, y, 14, CQColors::TextDim);
        }
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, focusIdx++))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}

void Game::StatePetDetail()
{
    if (!player || viewingPetIdx < 0) { currentState = GameState::Pets; return; }
    auto& allPets = petManager.GetPets();
    if (viewingPetIdx >= static_cast<int>(allPets.size())) { currentState = GameState::Pets; return; }

    Pet& pet = allPets[viewingPetIdx];
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    renderer.DrawPanel(50, 40, GRenderer::W - 100, GRenderer::H - 80, pet.GetCurrentName());
    int y = 80;
    int focusIdx = 0;

    Color elemCol = CQColors::TextLight;
    switch (pet.element)
    {
        case ElementType::Fire:      elemCol = {255, 100, 50, 255}; break;
        case ElementType::Ice:       elemCol = {100, 200, 255, 255}; break;
        case ElementType::Lightning: elemCol = {255, 255, 100, 255}; break;
        case ElementType::Arcane:    elemCol = {200, 120, 255, 255}; break;
        case ElementType::Poison:    elemCol = {100, 220, 80, 255}; break;
        case ElementType::Holy:      elemCol = {255, 240, 200, 255}; break;
        default:                     elemCol = {200, 200, 200, 255}; break;
    }

    // Header info
    renderer.DrawText(std::string(ElementName(pet.element)) + " Pet  |  " + PetRarityName(pet.rarity)
                      + "  |  " + pet.GetEvolutionLabel(),
                      70, y, 14, elemCol);
    y += 20;
    renderer.DrawText(pet.description, 70, y, 12, CQColors::TextDim);
    y += 22;

    if (pet.source == PetSource::FactionLegend)
        renderer.DrawText("Source: Faction Legend", 70, y, 11, CQColors::TextDim);
    else if (pet.source == PetSource::CitadelDrop)
        renderer.DrawText("Source: Citadel Boss — " + pet.bossName, 70, y, 11, CQColors::TextDim);
    else
        renderer.DrawText("Source: Boss Drop — " + pet.bossName, 70, y, 11, CQColors::TextDim);
    y += 20;

    // Level and XP
    if (pet.level >= Pet::SKILL_TREE_UNLOCK_LEVEL)
    {
        renderer.DrawText("Level: " + std::to_string(pet.level) + " (MAX)   Skill Points: " + std::to_string(pet.skillPoints),
                          70, y, 14, CQColors::TextGold);
    }
    else
    {
        renderer.DrawText("Level: " + std::to_string(pet.level) + " / " + std::to_string(Pet::MAX_PET_LEVEL),
                          70, y, 14, CQColors::TextLight);
        int reqXP = Pet::CalculateRequiredXP(pet.level);
        float progress = (reqXP > 0) ? static_cast<float>(pet.experience) / static_cast<float>(reqXP) : 0.0f;
        int barX = 70;
        int barW = 200;
        int barH = 8;
        y += 18;
        renderer.DrawRect(barX, y, barW, barH, Color{20, 20, 25, 255});
        int fillW = static_cast<int>(barW * std::min(progress, 1.0f));
        if (fillW > 0)
            renderer.DrawRect(barX + 1, y + 1, fillW - 1, barH - 2, CQColors::TextGold);
        renderer.DrawText(std::to_string(pet.experience) + " / " + std::to_string(reqXP) + " XP",
                          barX + barW + 8, y - 1, 11, CQColors::TextDim);
    }
    y += 20;

    // Stats
    renderer.DrawText("--- Stats ---", 70, y, 13, CQColors::TextLight);
    y += 16;

    auto drawStat = [&](const std::string& label, const std::string& value, Color col = CQColors::TextLight) {
        renderer.DrawText(label + ": " + value, 80, y, 12, col);
        y += 15;
    };

    drawStat("Attack", std::to_string(pet.GetScaledAttack()));
    drawStat("Damage Bonus", "+" + std::to_string(static_cast<int>(pet.GetScaledDamageBonus() * 100)) + "%");
    drawStat("Defense Bonus", "+" + std::to_string(static_cast<int>(pet.GetScaledDefenseBonus() * 100)) + "%");
    drawStat("Crit Chance", "+" + std::to_string(static_cast<int>(pet.GetScaledCritChance() * 100)) + "%");
    drawStat("Crit Damage", "+" + std::to_string(static_cast<int>(pet.GetScaledCritDamage() * 100)) + "%");
    drawStat("HP Bonus", "+" + std::to_string(pet.GetScaledHealthBonus()));
    drawStat("MP Bonus", "+" + std::to_string(pet.GetScaledManaBonus()));
    drawStat("Heal On Kill", "+" + std::to_string(pet.GetScaledHealOnKill()));
    drawStat("XP Bonus", "+" + std::to_string(static_cast<int>(pet.GetScaledXPBonus() * 100)) + "%");
    drawStat("Gold Find", "+" + std::to_string(static_cast<int>(pet.GetScaledGoldFind() * 100)) + "%");
    y += 4;

    // Special ability
    renderer.DrawText("--- Special Ability ---", 70, y, 13, CQColors::TextLight);
    y += 16;
    if (pet.evolutionTier > 0 && pet.specialAbility != EffectType::None)
    {
        drawStat(pet.GetAbilityName(), pet.GetAbilityDescription(), CQColors::TextGreen);
    }
    else
    {
        renderer.DrawText("Unlocks at evolution (Lv." + std::to_string(Pet::EVOLVE_LEVEL) + ")",
                          80, y, 12, CQColors::TextDim);
        y += 15;
    }
    y += 6;

    // Skill tree (only at level 100+)
    renderer.DrawText("--- Skill Tree ---", 70, y, 13, CQColors::TextLight);
    y += 16;

    if (pet.level < Pet::SKILL_TREE_UNLOCK_LEVEL)
    {
        renderer.DrawText("Unlocks at Level " + std::to_string(Pet::SKILL_TREE_UNLOCK_LEVEL),
                          80, y, 12, CQColors::TextDim);
        y += 15;
    }
    else
    {
        for (int b = 0; b < Pet::SKILL_TREE_BRANCHES; ++b)
        {
            Color branchCol = (b == 0) ? CQColors::TextGreen : (b == 1) ? Color{100, 180, 255, 255} : CQColors::TextGold;
            renderer.DrawText(Pet::GetBranchName(pet.element, b), 80, y, 13, branchCol);
            y += 16;

            for (int n = 0; n < Pet::SKILL_TREE_NODES_PER_BRANCH; ++n)
            {
                std::string nodeName = Pet::GetSkillNodeName(pet.element, b, n);
                std::string nodeDesc = Pet::GetSkillNodeDesc(pet.element, b, n);
                bool unlocked = pet.skillTreeNodes[b][n];

                if (unlocked)
                {
                    renderer.DrawText("[x] " + nodeName + " — " + nodeDesc, 90, y, 11, CQColors::TextGreen);
                    y += 14;
                }
                else if (pet.CanSpendSkillPoint(b, n))
                {
                    if (renderer.Button("[ ] " + nodeName + " (" + nodeDesc + ")##petSkill" + pet.id + std::to_string(b) + std::to_string(n),
                                        90, y, 500, 18, focusIdx++))
                    {
                        pet.SpendSkillPoint(b, n);
                    }
                    y += 18;
                }
                else
                {
                    renderer.DrawText("[ ] " + nodeName + " — locked", 90, y, 11, CQColors::TextDim);
                    y += 14;
                }
            }
            y += 4;
        }
    }

    keyboardNav.SetFocusCount(focusIdx + 1);
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 60, 120, 36, focusIdx++))
    {
        currentState = GameState::Pets;
        renderer.StartTransition();
    }
}
