#include "Game.hpp"
#include "KeyboardNav.hpp"
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
#include "../World/Enemies/Enemies.hpp"
#include "../World/NPCs/NPC.hpp"
#include <cstdlib>
#include <ctime>
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
      selectedJobIdx(-1),
      inventoryTab(0),
      enemyFlashTimer(0),
      previousState(GameState::MainMenu)
{
    srand(static_cast<unsigned>(time(nullptr)));
    UniqueItemRegistry::Initialize();
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
            1, std::vector<Dungeon>{ farm });
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
            2, std::vector<Dungeon>{ hollow });
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
            3, std::vector<Dungeon>{ grotto });
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
            4, std::vector<Dungeon>{ summit });
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
            5, std::vector<Dungeon>{ citadel });
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
            6, std::vector<Dungeon>{ abyss });
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
            7, std::vector<Dungeon>{ ascent });
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
            8, std::vector<Dungeon>{ endless });
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
            9, std::vector<Dungeon>{ sanctum });
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
            10, std::vector<Dungeon>{ chronos });
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
                case GameState::SavePrompt:
                    currentState = GameState::Exploring;
                    break;
                case GameState::JobPerks:
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
            case GameState::Wiki:              StateWiki(); break;
            case GameState::NPCDialogue:        StateNPCDialogue(); break;
            case GameState::Achievements:       StateAchievements(); break;
            case GameState::Exit:              break;
            default:                           currentState = GameState::MainMenu;
        }

        DrawFloatingTexts();
        DrawParticles();
        CheckAchievementNotifications();
        DrawAchievementNotifications();
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
            achNotifications.emplace_back(def->name, def->description);
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
    if (!areas[currentAreaIndex].dungeons.empty())
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
        "Shop", "Rest at Inn", "Codex", "Achievements", "Save Game", "Main Menu"
    };

    int bx = GRenderer::W - 240;
    int by = 110;
    keyboardNav.SetFocusCount(static_cast<int>(labels.size()) + extraButtons);
    for (size_t i = 0; i < labels.size(); ++i)
    {
        if (renderer.Button(labels[i], bx, by + static_cast<int>(i) * 44, 220, 38, static_cast<int>(i)))
        {
            if (i == 0) StartAreaEncounter();
            else if (i == 10) { currentState = GameState::Shop; renderer.StartTransition(); }
            else if (i == 11) {
                player->RestoreHealth(player->GetMaxHealth());
                player->RestoreMana(player->GetMaxMana());
                player->ResetTempDefense();
            }
            else if (i == 12) { currentState = GameState::Wiki; renderer.StartTransition(); }
            else if (i == 13) { currentState = GameState::Achievements; renderer.StartTransition(); }
            else if (i == 14) currentState = GameState::SavePrompt;
            else if (i == 15) {
                if (player->IsAlive()) currentState = GameState::MainMenu;
                else { player.reset(); currentState = GameState::MainMenu; }
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
                    + rand() % (rooms[0].maxEnemies - rooms[0].minEnemies + 1);
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
            renderer.DrawText("The Primordial One — Requires Level 50", 70, by, 22, CQColors::TextRed);
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
                    int idx = rand() % pool.size();
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
                player->GainXP(q->rewardXP);
                player->GetInventory().AddGold(q->rewardGold);
                q->rewarded = true;
            }
        }
        questsChecked = true;
    }

    renderer.DrawPanel(200, 100, GRenderer::W - 400, 400, "Dungeon Complete!");

    renderer.DrawCenteredText(dungeon.name + " conquered!", 200, 28, CQColors::TextGold);
    renderer.DrawCenteredText("All enemies cleared. The dungeon is safe once more.",
                              250, 18, CQColors::TextLight);

    if (renderer.Button("Return", renderer.CenterX(160), 420, 160, 44))
    {
        questsChecked = false;
        dungeonCompleted = false;
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
            selectedItemIndex = -1;
            selectedItem = nullptr;
        }
    }
    y += 34;

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
            case 1: show = (item->type == ItemType::Weapon); break;
            case 2: show = (item->type == ItemType::Armor); break;
            case 3: show = (item->type == ItemType::Accessory); break;
            case 4: show = (item->type == ItemType::Consumable); break;
            case 5: show = (item->type == ItemType::Resource || item->type == ItemType::QuestItem); break;
        }
        if (show) filteredIndices.push_back(i);
    }

    int itemCount = static_cast<int>(filteredIndices.size());

    // Arrow key navigation for item list
    if (itemCount > 0 && inventoryFocusRow >= itemCount)
        inventoryFocusRow = itemCount - 1;
    if (inventoryFocusRow < 0 && itemCount > 0) inventoryFocusRow = 0;
    if (IsKeyPressed(KEY_DOWN) && inventoryFocusRow < itemCount - 1)
        inventoryFocusRow++;
    if (IsKeyPressed(KEY_UP) && inventoryFocusRow > 0)
        inventoryFocusRow--;
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
    }

    // Pre-compute action button count per visible item for focus indexing
    int maxVisibleItems = 15;
    int visibleCount = std::min(itemCount, maxVisibleItems);
    std::vector<int> actionCountPerItem(visibleCount, 1); // every item has at least "D"
    for (int idx = 0; idx < visibleCount; ++idx)
    {
        size_t i = filteredIndices[idx];
        auto item = inv.GetItem(i);
        if (!item) continue;
        if (item->type == ItemType::Weapon || item->type == ItemType::Armor || item->type == ItemType::Accessory)
        {
            actionCountPerItem[idx] = 1; // D
            if (player->CanEquip(item)) actionCountPerItem[idx]++; // Eq
            actionCountPerItem[idx]++; // ?
        }
        else if (item->type == ItemType::Consumable)
        {
            actionCountPerItem[idx] = 2; // Use + D
        }
        else if (item->type == ItemType::Resource)
        {
            auto res = std::dynamic_pointer_cast<Resource>(item);
            actionCountPerItem[idx] = (res && res->healAmount > 0) ? 2 : 1; // Eat+D or D
        }
    }
    int totalActionButtons = 0;
    for (int c : actionCountPerItem) totalActionButtons += c;
    keyboardNav.SetFocusCount(tabCount + totalActionButtons + 1); // tabs + actions + Back

    renderer.DrawText("Items: " + std::to_string(itemCount), xLeft, y, 16, CQColors::TextDim);
    y += 26;

    // ---- Inventory items (left side) ----
    if (itemCount == 0)
    {
        renderer.DrawText("No items in this category.", xLeft, y, 18, CQColors::TextLight);
        y += 24;
    }
    else
    {
        int startY = y;
        int itemFocusIdx = tabCount; // action buttons start after tabs
        for (int idx = 0; idx < visibleCount; ++idx)
        {
            size_t i = filteredIndices[idx];
            auto item = inv.GetItem(i);
            if (!item) continue;
            int iy = startY + idx * 30;

            // Highlight keyboard-focused item
            if (idx == inventoryFocusRow)
            {
                renderer.DrawRect(xLeft - 4, iy - 4, 360, 26, Color{40, 40, 70, 100});
                keyboardNav.DrawFocusRect(xLeft - 4, iy - 4, 360, 26, CQColors::TextDim);
            }

            // Check if this item is selected for comparison
            bool isSelected = (static_cast<int>(i) == selectedItemIndex && item == selectedItem);

            std::string line = item->name;
            std::string setName = GetSetNameFromItem(item);
            if (!setName.empty())
                line += " [" + setName + "]";
            if (item->count > 1)
                line += " x" + std::to_string(item->count);
            if (auto w = std::dynamic_pointer_cast<Weapon>(item))
            {
                line += " [" + std::string(WeaponTypeName(w->weaponType)) + "] DMG:" + std::to_string(w->damage);
                if (w->element != ElementType::Physical)
                    line += " " + std::string(ElementName(w->element)) + ":" + std::to_string(w->elementDamage);
            }
            else if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
            {
                line += " [" + std::string(OffhandTypeName(oh->offhandType)) + "]";
                if (oh->defense > 0) line += " DEF:" + std::to_string(oh->defense);
                if (oh->manaBonus > 0) line += " MP:" + std::to_string(oh->manaBonus);
                if (oh->arcaneDamage > 0) line += " [Arcane:" + std::to_string(oh->arcaneDamage) + "]";
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
                line += " [" + pn + "] DEF:" + std::to_string(a->defense);
                for (const auto& [elem, val] : a->elementalResist)
                    line += " " + std::string(ElementName(elem)) + "Res:" + std::to_string(val);
            }
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
                line += " [Acc] HP:" + std::to_string(ac->bonusHealth)
                    + " MP:" + std::to_string(ac->bonusMana);
            else if (auto con = std::dynamic_pointer_cast<Consumable>(item))
                line += " [" + con->GetDescription() + "]";
            DrawItemIcon(item->name, item->rarity, xLeft, iy - 2, 20);

            // Rarity glow effect for Epic/Legendary items
            if (item->rarity >= static_cast<int>(Rarity::Epic))
            {
                unsigned char glowAlpha = static_cast<unsigned char>(
                    60 + 40.0 * std::sin(renderer.GetTime() * 3.0));
                Color glowColor = RarityColor(static_cast<Rarity>(item->rarity));
                glowColor.a = glowAlpha;
                renderer.DrawRect(xLeft - 2, iy - 4, 24, 24, glowColor);
            }

            Color textColor = isSelected ? CQColors::TextGold : RarityColor(static_cast<Rarity>(item->rarity));
            int maxLineW = 240;
            while (!line.empty() && MeasureText(line.c_str(), 14) > maxLineW)
                line.pop_back();
            renderer.DrawText(line, xLeft + 24, iy, 14, textColor);

            // Tooltip on hover over icon
            if (renderer.IsMouseInRect(xLeft - 2, iy - 4, 24, 24))
            {
                std::string tip = item->name;
                tip += "\nRarity: " + std::string(RarityName(static_cast<Rarity>(item->rarity)));
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
                    if (oh->manaBonus > 0) tip += "\n+" + std::to_string(oh->manaBonus) + " MP";
                    if (oh->arcaneDamage > 0) tip += "\nArcane: " + std::to_string(oh->arcaneDamage);
                }
                else if (auto con = std::dynamic_pointer_cast<Consumable>(item))
                {
                    tip += "\n" + con->GetDescription();
                }
                else if (auto res = std::dynamic_pointer_cast<Resource>(item))
                {
                    if (res->healAmount > 0) tip += "\nHeals: " + std::to_string(res->healAmount) + " HP";
                    if (res->manaAmount > 0) tip += "\nRestores: " + std::to_string(res->manaAmount) + " MP";
                }
                if (item->count > 1) tip += "\nQuantity: " + std::to_string(item->count);
                renderer.DrawTooltip(tip, static_cast<int>(GetMousePosition().x), static_cast<int>(GetMousePosition().y));
            }

            // Action buttons
            int btnX = xLeft + 300;
            if (item->type == ItemType::Weapon || item->type == ItemType::Armor || item->type == ItemType::Accessory)
            {
                bool canEquip = player->CanEquip(item);
                if (canEquip)
                {
                    if (renderer.Button("Eq", btnX, iy, 34, 22, itemFocusIdx++))
                        player->EquipItem(item);
                }
                else
                {
                    renderer.DrawText("Eq", btnX, iy, 12, CQColors::TextDim);
                }
                btnX += 38;
                // Compare button
                if (renderer.Button("?", btnX, iy, 22, 22, itemFocusIdx++))
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
                if (con && renderer.Button("Use", btnX, iy, 40, 22, itemFocusIdx++))
                {
                    con->Use(*player);
                    inv.RemoveOneItem(i);
                    if (selectedItemIndex == static_cast<int>(i))
                    {
                        selectedItemIndex = -1;
                        selectedItem = nullptr;
                    }
                }
                btnX += 44;
            }
            else if (item->type == ItemType::Resource)
            {
                // Check if this resource is edible (has healing properties, e.g. fish)
                auto res = std::dynamic_pointer_cast<Resource>(item);
                if (res && res->healAmount > 0)
                {
                    if (renderer.Button("Eat", btnX, iy, 40, 22, itemFocusIdx++))
                    {
                        int hpBefore = player->GetCurrentHealth();
                        int mpBefore = player->GetCurrentMana();
                        if (res->healAmount > 0) player->RestoreHealth(res->healAmount);
                        if (res->manaAmount > 0) player->RestoreMana(res->manaAmount);
                        inv.RemoveOneItem(i);
                        if (selectedItemIndex == static_cast<int>(i))
                        {
                            selectedItemIndex = -1;
                            selectedItem = nullptr;
                        }
                    }
                    btnX += 44;
                }
            }
            if (renderer.Button("D", btnX, iy, 28, 22, itemFocusIdx++))
            {
                inv.RemoveOneItem(i);
                if (selectedItemIndex == static_cast<int>(i))
                {
                    selectedItemIndex = -1;
                    selectedItem = nullptr;
                }
            }
        }
        if (itemCount > maxVisibleItems)
        {
            int remaining = itemCount - maxVisibleItems;
            renderer.DrawText("... and " + std::to_string(remaining) + " more items",
                xLeft, startY + visibleCount * 30 + 4, 13, CQColors::TextDim);
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
    for (size_t i = 0; i < player->GetSkills().GetSkillCount(); ++i)
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
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 0))
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
    keyboardNav.SetFocusCount(6); // 4 jobs + Perks + Back
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
    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 80, 120, 40, 5))
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
                selectedJobIdx = i;
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

    // Count unlockable perks for focus count
    int unlockCount = 0;
    for (size_t u = 0; u < job.perks.size(); ++u)
        if (job.CanUnlockPerk(static_cast<int>(u)))
            unlockCount++;
    int detailFocusCount = unlockCount + 2; // unlock buttons + Back to Jobs + Back
    keyboardNav.SetFocusCount(detailFocusCount);
    int detailFocusIdx = 0;

    for (size_t u = 0; u < job.perks.size(); ++u)
    {
        const auto& perk = job.perks[u];

        // Branch separator
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
        else if (job.CanUnlockPerk(static_cast<int>(u)))
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

        if (!perk.unlocked && job.CanUnlockPerk(static_cast<int>(u)))
        {
            if (renderer.Button("Unlock (1 PT)", 110, y, 160, 26, detailFocusIdx++))
                job.UnlockPerk(static_cast<int>(u));
            y += 30;
        }
        else
        {
            y += 6;
        }
        y += 4;
    }

    if (renderer.Button("Back to Jobs", 70, GRenderer::H - 100, 160, 36, detailFocusIdx++))
    {
        selectedJobIdx = -1;
        return;
    }

    if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 100, 120, 36, detailFocusIdx++))
    {
        selectedJobIdx = -1;
        currentState = GameState::Jobs;
    }
}

void Game::StateSkillLoadout()
{
    if (!player) { currentState = GameState::Exploring; return; }
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    renderer.DrawPanel(50, 60, GRenderer::W - 100, GRenderer::H - 120, "Skill Loadout — Select up to 4");
    int y = 110;
    renderer.DrawText("Click a skill to toggle it in your loadout (max 4):", 70, y, 16, CQColors::TextDim);
    y += 30;

    auto& loadout = player->GetSkillLoadout();
    auto loadoutCopy = loadout;

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
    int loadoutFocusCount = visibleSkills + 2; // skills + Save + Back
    if (maxPage > 0) loadoutFocusCount += 2; // Prev/Next
    keyboardNav.SetFocusCount(loadoutFocusCount);

    int focusIdx = 0;
    for (int si = startIdx; si < endIdx; ++si)
    {
        int i = skillIndices[si];
        auto sk = player->GetSkills().GetSkill(i);
        if (!sk) continue;

        bool inLoadout = player->IsInLoadout(i);
        std::string elemStr = (sk->element != ElementType::Physical) ? " [" + std::string(ElementName(sk->element)) + "]" : "";
        std::string label = sk->name + elemStr
            + " (MP:" + std::to_string(sk->manaCost)
            + " CD:" + std::to_string(sk->cooldown)
            + " | Lv." + std::to_string(sk->level) + ")";

        // Class-colored tinted background for skill card
        Color cardBg = CQColors::SkillCardBg(sk->characterClass);
        renderer.DrawRect(70, y, 600, 30, cardBg);

        if (inLoadout)
        {
            // Golden equipped badge
            renderer.DrawRect(675, y, 95, 30, { 70, 55, 15, 255 });
            renderer.DrawText("EQUIPPED", 680, y + 7, 14, CQColors::GoldBright);
        }

        if (renderer.Button(label, 70, y, 600, 30, focusIdx))
        {
            if (inLoadout)
            {
                auto it = std::find(loadoutCopy.begin(), loadoutCopy.end(), i);
                if (it != loadoutCopy.end()) loadoutCopy.erase(it);
            }
            else if (loadoutCopy.size() < Player::MAX_LOADOUT_SKILLS)
            {
                loadoutCopy.push_back(i);
            }
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
        focusIdx++;
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
    renderer.DrawText("Selected: " + std::to_string(loadoutCopy.size())
        + " / " + std::to_string(Player::MAX_LOADOUT_SKILLS),
        70, bottomY - 50, 16, CQColors::TextGold);

    if (renderer.Button("Save", 70, bottomY - 20, 120, 36, focusIdx))
    {
        player->SetSkillLoadout(loadoutCopy);
        currentState = GameState::Exploring;
    }
    focusIdx++;
    if (renderer.Button("Back", renderer.CenterX(120), bottomY - 20, 120, 36, focusIdx))
        currentState = GameState::Exploring;
}

void Game::StateSkillUpgrade()
{
    if (!player) { currentState = GameState::Exploring; return; }

    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());

    static int selectedSkillIdx = -1;

    auto& skills = player->GetSkills();

    // Calculate total skill points
    int totalPoints = 0;
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto sk = skills.GetSkill(i);
        if (sk) totalPoints += sk->skillPoints;
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
            int detailFocusCount = unlockCount + 2; // unlock buttons + Back to Skills + Back
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

            if (renderer.Button("Back to Skills", 70, GRenderer::H - 100, 160, 36, detailFocusIdx++))
            {
                selectedSkillIdx = -1;
                return;
            }

            if (renderer.Button("Back", renderer.CenterX(120), GRenderer::H - 100, 120, 36, detailFocusIdx++))
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
    keyboardNav.SetFocusCount(focusCount);
    int focusIdx = 0;

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

        Color nameColor = (sk->skillPoints > 0) ? CQColors::TextGold : CQColors::TextLight;

        // Class-colored tinted background for skill card
        Color cardBg = CQColors::SkillCardBg(sk->characterClass);
        renderer.DrawRect(70, y, 650, 32, cardBg);

        if (renderer.Button(label, 70, y, 650, 32, focusIdx++))
            selectedSkillIdx = static_cast<int>(i);

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
                renderer.DrawText("Crafted: " + item->name + "!", cx, cy + 34, 14, CQColors::TextGreen);
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
                if (religion.Donate(donate)) religion.ApplyDevotionBonus(player);
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
        shopItems.push_back(LootTable::CreatePotion(areas[currentAreaIndex].difficulty));
        shopItems.push_back(LootTable::CreatePotion(areas[currentAreaIndex].difficulty));
        shopItems.push_back(LootTable::CreatePotion(areas[currentAreaIndex].difficulty));
        shopItems.push_back(LootTable::CreateCommonWeapon(areas[currentAreaIndex].difficulty));
        shopItems.push_back(LootTable::CreateCommonArmor(areas[currentAreaIndex].difficulty));
        if (rand() % 2 == 0)
            shopItems.push_back(LootTable::CreateRareWeapon(areas[currentAreaIndex].difficulty));
        else
            shopItems.push_back(LootTable::CreateRareArmor(areas[currentAreaIndex].difficulty));
    }

    int xLeft = 70;
    int xRight = GRenderer::W / 2 + 40;
    int y = 100;

    // Gold
    renderer.DrawText("Gold: " + std::to_string(inv.GetGold()), xLeft, y, 20, CQColors::TextGold);
    y += 30;

    // Shop items (left side)
    renderer.DrawText("— For Sale —", xLeft, y, 16, CQColors::TextDim);
    y += 24;
    int shopBuyCount = static_cast<int>(shopItems.size());
    int shopSellCount = inv.GetItemCount();
    keyboardNav.SetFocusCount(shopBuyCount + shopSellCount + 1); // +1 for Leave
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
                    inv.RemoveGold(price);
            }
        }
    }

    // Player inventory (right side) — sell mode
    y = 100;
    renderer.DrawText("— Your Items —", xRight, y, 16, CQColors::TextDim);
    y += 24;
    if (inv.GetItemCount() == 0)
    {
        renderer.DrawText("No items to sell.", xRight, y, 14, CQColors::TextDim);
    }
    else
    {
        for (size_t i = 0; i < inv.GetItemCount(); ++i)
        {
            auto item = inv.GetItem(i);
            if (!item) continue;
            int iy = y + static_cast<int>(i) * 30;
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
            }
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
    renderer.DrawPanel(100, 60, GRenderer::W - 200, GRenderer::H - 130, "Travel Across Eluna");
    int y = 110;
    keyboardNav.SetFocusCount(static_cast<int>(areas.size()) + 1); // +1 for Back
    for (size_t i = 0; i < areas.size(); ++i)
    {
        std::string lbl = areas[i].name
            + " (D:" + std::to_string(areas[i].difficulty) + ")";
        if (renderer.Button(lbl, 130, y, 280, 36, static_cast<int>(i)))
        {
            currentAreaIndex = static_cast<int>(i);
            religion.ResetPrayers();

            // Track exploration achievements
            int areasVisited = 0;
            for (size_t a = 0; a <= static_cast<size_t>(currentAreaIndex); ++a)
                areasVisited++;
            achievementSystem.SetProgress("explore_3_areas", areasVisited);
            achievementSystem.SetProgress("explore_6_areas", areasVisited);
            achievementSystem.SetProgress("explore_all_areas", areasVisited);
        }
        renderer.DrawText(areas[i].description, 430, y + 8, 14, CQColors::TextLight);
        if (!areas[i].dungeons.empty())
        {
            int cnt = 0;
            for (const auto& d : areas[i].dungeons) cnt += d.rooms.size();
            renderer.DrawText(std::to_string(areas[i].dungeons.size()) + " dungeon(s), "
                              + std::to_string(cnt) + " rooms total",
                              430, y + 26, 12, CQColors::TextDim);
        }
        y += 44;
    }
    if (renderer.Button("Back", renderer.CenterX(120), y + 20, 120, 40, static_cast<int>(areas.size())))
        currentState = GameState::Exploring;
}

void Game::StateQuestLog()
{
    if (!player) { currentState = GameState::Exploring; return; }
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

    BattleRenderer::DrawBattleScreen(renderer, *currentEnemy, *player, combatLog,
                                      combatPhase, enemyFlashTimer, isBoss);

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
        renderer.DrawText(roomInfo, 30, layout.playerBarY - 20, 14, CQColors::TextGold);
    }

    // Actions - draw on top of the battle screen
    if (combatPhase == CombatPhase::PlayerTurn)
    {
        int btnSize = layout.abilitySize;
        int btnSpacing = layout.abilitySpacing;
        
        // Count available buttons (5 base + god abilities)
        int godBtnCount = 0;
        if (religion.CanUseGodAbility1()) godBtnCount++;
        if (religion.CanUseGodAbility2()) godBtnCount++;
        int totalButtons = 5 + godBtnCount;
        
        int totalW = totalButtons * btnSize + (totalButtons - 1) * btnSpacing;
        int startX = (GRenderer::W - totalW) / 2;
        int btnY = layout.abilityY;

        keyboardNav.SetFocusCount(totalButtons);

        const char* labels[] = {"Attack", "Skills", "Defend", "Items", "Flee"};
        int focusIdx = 0;
        if (renderer.Button(labels[0], startX, btnY, btnSize, btnSize, focusIdx++))
            DoPlayerAttack(CombatAction::Attack);
        if (renderer.Button(labels[1], startX + (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
            combatPhase = CombatPhase::SkillSelect;
        if (renderer.Button(labels[2], startX + 2 * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
            DoPlayerAttack(CombatAction::Defend);
        if (renderer.Button(labels[3], startX + 3 * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
            combatPhase = CombatPhase::ItemSelect;
        if (renderer.Button(labels[4], startX + 4 * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
        {
            leveledUpThisCombat = false;
            AddCombatLog("You fled from combat!");
            if (!roomQueue.empty()) roomQueue.clear();
            currentEnemy.reset();
            currentState = isDungeonBossFight ? GameState::DungeonExplore : GameState::Exploring;
        }
        
        // God ability buttons
        if (religion.CanUseGodAbility1())
        {
            auto ab1 = religion.GetGodAbility1();
            if (renderer.Button(ab1.name, startX + 5 * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
                DoPlayerGodAbility(1);
        }
        if (religion.CanUseGodAbility2())
        {
            auto ab2 = religion.GetGodAbility2();
            if (renderer.Button(ab2.name, startX + (godBtnCount > 1 ? 6 : 5) * (btnSize + btnSpacing), btnY, btnSize, btnSize, focusIdx++))
                DoPlayerGodAbility(2);
        }
    }
    else if (combatPhase == CombatPhase::SkillSelect)
    {
        int panelX = 30;
        int panelY = layout.abilityY - 10;
        int panelW = GRenderer::W - 60;
        int panelH = 200;
        renderer.DrawRect(panelX, panelY, panelW, panelH, {15, 15, 20, 230});
        renderer.DrawRectLines(panelX, panelY, panelW, panelH, {80, 80, 100, 200});

        renderer.DrawText("Select Skill:", panelX + 10, panelY + 8, 16, CQColors::TextGold);
        int sky = panelY + 30;
        const auto& loadout = player->GetSkillLoadout();
        int skillFocusCount = 0;
        for (size_t j = 0; j < loadout.size(); ++j)
        {
            int idx = loadout[j];
            auto sk = player->GetSkills().GetSkill(idx);
            if (sk) skillFocusCount++;
        }
        skillFocusCount++;
        keyboardNav.SetFocusCount(skillFocusCount);
        int focusIdx = 0;
        for (size_t j = 0; j < loadout.size(); ++j)
        {
            int idx = loadout[j];
            auto sk = player->GetSkills().GetSkill(idx);
            if (!sk) continue;
            if (renderer.Button(sk->name + " [" + ElementName(sk->element) + "] (" + std::to_string(sk->manaCost) + "MP)", panelX + 10, sky, 350, 26, focusIdx))
            {
                DoPlayerAttack(CombatAction::UseSkill, idx);
            }
            sky += 30;
            focusIdx++;
        }
        if (renderer.Button("Cancel", panelX + panelW - 110, panelY + 8, 100, 26, focusIdx))
            combatPhase = CombatPhase::PlayerTurn;
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

        keyboardNav.SetFocusCount(1);
        if (renderer.Button("Continue", renderer.CenterX(160), 600, 160, 44, 0))
        {
            leveledUpThisCombat = false;

            // Check if we're in a dungeon with more enemies
            if (!roomQueue.empty() && currentRoomEnemyIndex + 1 < static_cast<int>(roomQueue.size()))
            {
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
            else if (isDungeonBossFight)
            {
                // Dungeon done
                dungeonCompleted = true;
                currentEnemy.reset();
                ClearLog();
                currentState = GameState::DungeonComplete;
            }
            else if (inDungeonEncounter)
            {
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
                            + rand() % (rooms[currentDungeonRoom].maxEnemies
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
            else
            {
                // Area encounter — chain into next random fight (unlimited)
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
        }
    }
    else if (combatPhase == CombatPhase::Defeat)
    {
        renderer.DrawCenteredText("You were defeated...", 400, 36, CQColors::TextRed);
        keyboardNav.SetFocusCount(1);
        if (renderer.Button("Continue", renderer.CenterX(160), 500, 160, 44, 0))
        {
            leveledUpThisCombat = false;
            if (!roomQueue.empty()) roomQueue.clear();
            player.reset();
            currentEnemy.reset();
            ClearLog();
            currentState = GameState::MainMenu;
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
            if (hasSkills && rand() % 3 == 0)
            {
                ea = CombatAction::UseSkill;
                es = 1 + rand() % (skillCount - 1);
            }
            break;
            
        case EnemyAIType::Defensive:
            // Defends often (50%), attacks otherwise
            if (rand() % 2 == 0)
                ea = CombatAction::Defend;
            else if (hasSkills && rand() % 2 == 0)
            {
                ea = CombatAction::UseSkill;
                es = 1 + rand() % (skillCount - 1);
            }
            break;
            
        case EnemyAIType::Caster:
            // Prefers skills, only attacks when out of mana
            if (hasSkills && currentEnemy->GetCurrentMana() > 10)
            {
                ea = CombatAction::UseSkill;
                es = 1 + rand() % (skillCount - 1);
            }
            break;
            
        case EnemyAIType::Berserker:
            // Attacks when healthy, uses skills when low HP
            if (healthPercent < 50 && hasSkills)
            {
                ea = CombatAction::UseSkill;
                es = 1 + rand() % (skillCount - 1);
            }
            break;
            
        case EnemyAIType::Boss:
            // Smart behavior: heals/defends when low, uses skills when possible
            if (healthPercent < 30 && rand() % 2 == 0)
                ea = CombatAction::Defend;
            else if (hasSkills)
            {
                ea = CombatAction::UseSkill;
                es = 1 + rand() % (skillCount - 1);
            }
            break;
            
        case EnemyAIType::Balanced:
        default:
            // Original random behavior
            {
                int ec = rand() % 3;
                switch (ec)
                {
                    case 0: ea = CombatAction::Attack; break;
                    case 1:
                        if (hasSkills)
                        { ea = CombatAction::UseSkill; es = 1 + rand() % (skillCount - 1); }
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

    if (!player->IsAlive())
    { AddCombatLog("You have been defeated..."); combatPhase = CombatPhase::Defeat; }
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

    int prevLevel = player->GetLevel();
    player->GainXP(xp);
    player->GetInventory().AddGold(gold);
    AddCombatLog("Gained " + std::to_string(xp) + " XP and " + std::to_string(gold) + " gold!");

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
        religion.ApplyDevotionBonus(player);

    // God quest progress
    if (religion.GetGod() != GodType::None && !religion.IsQuestComplete())
    {
        if (religion.ProgressQuest(enemy->GetName()))
            AddCombatLog("[God Quest] Quest complete! Visit the Shrine to claim your reward.");
    }

    player->GetQuestManager().UpdateKillQuests(enemy->GetName());
    player->GetQuestManager().UpdateGatherQuests(player->GetInventory());

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
            std::string prefix = (item->rarity >= 5) ? "*** LEGENDARY *** " : (item->rarity >= 4) ? "*** EPIC *** " : "";
            AddCombatLog(prefix + "Legendary Loot: " + item->name);
        }
    }
    else
    {
        int diff = 1 + (player->GetLevel() - 1) / 5;
        int dropTier = enemy->GetDropTier();
        auto loot = enemy->IsBoss()
            ? LootTable::GenerateBossLoot(diff, dropTier)
            : LootTable::GenerateLoot(diff, dropTier);
        for (auto& item : loot)
        {
            player->GetInventory().AddItem(item);
            AddCombatLog("Loot: " + item->name);
        }

        // Unique item drop — bosses 30%, normal enemies 3%
        {
            int uniqueChance = enemy->IsBoss() ? 30 : 3;
            if (rand() % 100 < uniqueChance)
            {
                auto unique = LootTable::GenerateUniqueDrop(enemy->GetName(), diff, enemy->IsBoss());
                if (unique)
                {
                    player->GetInventory().AddItem(unique);
                    std::string prefix = (unique->rarity >= 5) ? "*** LEGENDARY DROP *** " : "*** EPIC DROP *** ";
                    AddCombatLog(prefix + unique->name + "!");
                }
            }
        }

        // Special drops from this enemy type
        for (const auto& sd : enemy->GetSpecialDrops())
        {
            if (rand() % 100 < sd.chance)
            {
                int qty = sd.minQty + (rand() % (sd.maxQty - sd.minQty + 1));
                auto drop = std::make_shared<Resource>(sd.itemName, dropTier, dropTier * 5);
                drop->count = qty;
                player->GetInventory().AddItem(drop);
                AddCombatLog("Special Drop: " + sd.itemName + " x" + std::to_string(qty));
            }
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

    // Apply job combat synergy bonuses
    auto& js = player->GetJobSystem();
    int jobDef = js.GetTotalCombatDefense();
    int jobDmg = js.GetTotalCombatDamage();
    int jobHp = js.GetTotalCombatHealth();
    if (jobDef > 0) player->IncreaseTempDefense(jobDef);
    if (jobDmg > 0) player->SetAttackBonus(player->GetAttackBonus() + player->GetWeaponDamage() * jobDmg / 100);
    if (jobHp > 0) player->SetCurrentHealth(std::min(player->GetCurrentHealth() + jobHp, player->GetMaxHealth()));

    // Apply achievement reward bonuses
    AchievementReward achRewards = achievementSystem.GetTotalRewards();
    if (achRewards.statBonusATK > 0)
        player->SetAttackBonus(player->GetAttackBonus() + achRewards.statBonusATK);
    if (achRewards.statBonusDEF > 0)
        player->IncreaseTempDefense(achRewards.statBonusDEF);
    if (achRewards.statBonusHP > 0)
        player->SetCurrentHealth(std::min(player->GetCurrentHealth() + achRewards.statBonusHP, player->GetMaxHealth()));

    // Apply tier-based combat bonuses from achievements
    int achTierATK = achievementSystem.GetCombatATKBonusByTier();
    int achTierDEF = achievementSystem.GetCombatDEFBonusByTier();
    if (achTierATK > 0)
        player->SetAttackBonus(player->GetAttackBonus() + achTierATK);
    if (achTierDEF > 0)
        player->IncreaseTempDefense(achTierDEF);

    religion.ResetGodAbilityCooldowns();
    if (!combatSystem->StartCombat(player, enemy)) { currentState = GameState::Exploring; return; }
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
            renderer.DrawText(slots[i].playerName + " — Lv." + std::to_string(slots[i].level) + " " + cn,
                              215, sy + 30, 14, CQColors::TextLight);
        }
        else
        {
            renderer.DrawText("Empty", 215, sy + 30, 14, CQColors::TextDim);
        }
        if ((hover && renderer.IsMouseClickedOn(200, sy, GRenderer::W - 400, 60))
            || (focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))))
        {
            SaveToSlot(i + 1);
            return;
        }
        sy += 68;
    }
    if (renderer.Button("Cancel", renderer.CenterX(120), sy + 10, 120, 40, SaveGameManager::SLOT_COUNT))
        currentState = GameState::Exploring;
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
            renderer.DrawText(slots[i].playerName + " — Lv." + std::to_string(slots[i].level) + " " + cn,
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
    if (saveManager.SaveGame(player, slot, currentAreaIndex, religion, achievementSystem))
        renderer.DrawCenteredText("Game saved to Slot " + std::to_string(slot) + "!", 350, 24, CQColors::TextGreen);
    currentState = GameState::Exploring;
}

void Game::LoadFromSlot(int slot)
{
    static bool init = true;
    auto loaded = saveManager.LoadGame(slot, currentAreaIndex, religion, achievementSystem);
    if (loaded)
    {
        player = loaded;
        if (player->GetQuestManager().GetQuestCount() == 0)
            player->GetQuestManager().InitializeDefaultQuests();
        ClearLog();
        init = true;
        currentState = GameState::Exploring;
    }
}

// ============================================================
//  WIKI / CODEX
// ============================================================

void Game::StateWiki()
{
    keyboardNav.Update();
    renderer.SetCurrentFocus(keyboardNav.GetFocus());
    keyboardNav.SetFocusCount(1);
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
                player->GainXP(quest->rewardXP);
                player->GetInventory().AddGold(quest->rewardGold);
                quest->rewarded = true;
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
    static int selectedCategory = -1; // -1 = All
    static const char* categoryNames[] = { "All", "Job", "Combat", "Quest", "Exploration", "Progression", "Social" };
    static const AchievementCategory categoryValues[] = {
        AchievementCategory::Job, AchievementCategory::Job, AchievementCategory::Combat,
        AchievementCategory::Quest, AchievementCategory::Exploration,
        AchievementCategory::Progression, AchievementCategory::Social
    };

    int tabX = 130;
    for (int i = 0; i < 7; ++i)
    {
        bool selected = (i == 0 && selectedCategory == -1) || (i > 0 && selectedCategory == static_cast<int>(categoryValues[i]));
        Color tabColor = selected ? CQColors::Gold : CQColors::TextDim;

        if (renderer.Button(categoryNames[i], tabX, y, 80, 28, focusIdx))
        {
            selectedCategory = (i == 0) ? -1 : static_cast<int>(categoryValues[i]);
        }
        tabX += 88;
    }
    y += 40;

    // Achievement list
    std::vector<AchievementProgress> allProgress = achievementSystem.GetAllProgress();
    int displayCount = 0;
    int maxDisplay = 12;

    for (const auto& prog : allProgress)
    {
        if (displayCount >= maxDisplay) break;

        const AchievementDefinition* def = achievementSystem.GetDefinition(prog.achievementId);
        if (!def) continue;

        // Filter by category
        if (selectedCategory != -1 && static_cast<int>(def->category) != selectedCategory)
            continue;

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
        displayCount++;
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

    // Back button
    y = GRenderer::H - 60;
    if (renderer.Button("Back", 130, y, 120, 36, focusIdx))
    {
        currentState = GameState::Exploring;
        renderer.StartTransition();
    }
}
