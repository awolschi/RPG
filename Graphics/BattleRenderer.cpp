#include "BattleRenderer.hpp"
#include "PhotoEffects.hpp"
#include "CreatureImageMapper.hpp"
#include "IconRenderer.hpp"
#include "Colors.hpp"
#include "../Characters/Character.hpp"
#include "../Characters/Monster.hpp"
#include "../Engine/Game.hpp"
#include "../Factions/Pet.hpp"
#include "../Skills/Skill.hpp"
#include <cmath>

static void DrawCreatureFallback(GRenderer& renderer, const std::string& name, int x, int y, int size)
{
    DrawEnemyIcon(name, x, y, size);
}

void BattleRenderer::DrawBattleScreen(GRenderer& renderer,
                                       Character& enemy,
                                       Character& player,
                                       const std::vector<std::string>& combatLog,
                                       CombatPhase phase,
                                       float enemyFlashTimer,
                                       bool isBoss,
                                       const std::string& battleMessage,
                                       const PetManager* petManager,
                                       int playerLevel,
                                       int playerXP,
                                       int playerMaxXP,
                                       int reputationValue,
                                       int reputationMax,
                                       const std::string& repLabel,
                                       const std::string& masterySkillName,
                                       int masteryXP,
                                       int masteryMaxXP,
                                       int masteryLevel,
                                       int charMasteryXP,
                                       int charMasteryMaxXP,
                                       int charMasteryLevel)
{
    BattleLayout layout;
    layout.Calculate(GRenderer::W, GRenderer::H);

    renderer.Clear(BLACK);

    Texture2D bgTex = CreatureImageMapper::GetBackgroundTexture(
        renderer.textureManager, "Felrona");

    if (bgTex.id != 0)
    {
        renderer.DrawTextureFit(bgTex, 0, 0, GRenderer::W, layout.photoH);
    }
    else
    {
        Color bgGradTop = {40, 50, 65, 255};
        Color bgGradBot = {20, 25, 35, 255};
        for (int y = 0; y < layout.photoH; y += 4)
        {
            float t = static_cast<float>(y) / static_cast<float>(layout.photoH);
            Color c = {
                static_cast<unsigned char>(bgGradTop.r + (bgGradBot.r - bgGradTop.r) * t),
                static_cast<unsigned char>(bgGradTop.g + (bgGradBot.g - bgGradTop.g) * t),
                static_cast<unsigned char>(bgGradTop.b + (bgGradBot.b - bgGradTop.b) * t),
                255
            };
            DrawRectangle(0, y, GRenderer::W, 4, c);
        }
    }

    Texture2D creatureTex = CreatureImageMapper::GetCreatureTexture(
        renderer.textureManager, enemy.GetName());

    int creatureDrawX = layout.creatureX;
    int creatureDrawY = layout.creatureY;

    if (creatureTex.id != 0)
    {
        float bob = std::sin(static_cast<float>(::GetTime()) * 1.5f) * 4.0f;
        creatureDrawY += static_cast<int>(bob);
        renderer.DrawTextureFit(creatureTex, creatureDrawX, creatureDrawY,
                                layout.creatureW, layout.creatureH);
    }
    else
    {
        DrawCreatureFallback(renderer, enemy.GetName(),
                             creatureDrawX, creatureDrawY, layout.creatureW);
    }

    if (enemyFlashTimer > 0)
    {
        unsigned char flashAlpha = static_cast<unsigned char>(180.0f * (enemyFlashTimer / 0.15f));
        DrawRectangle(creatureDrawX, creatureDrawY, layout.creatureW, layout.creatureH,
                       {255, 255, 255, flashAlpha});
    }

    PhotoEffects::DrawVignette(GRenderer::W, GRenderer::H, 0.8f);

    DrawRectangle(0, layout.uiY, GRenderer::W, GRenderer::H - layout.uiY, {0, 0, 0, 240});

    std::string enemyTitle = enemy.GetName();
    auto mon = dynamic_cast<Monster*>(&enemy);
    if (mon && mon->IsBoss())
        enemyTitle += " [BOSS]";

    renderer.DrawCenteredTextFont(enemyTitle, layout.nameY, 32, WHITE, renderer.GetSerifFont());

    int hpBarW = 220;
    int hpBarX = (GRenderer::W - hpBarW) / 2;
    renderer.DrawBarLabeled(enemy.GetCurrentHealth(), enemy.GetMaxHealth(),
                            hpBarX, layout.hpTextY, hpBarW, 18,
                            CQColors::HpFg, CQColors::HpBg, "HP");

    if (!battleMessage.empty())
    {
        renderer.DrawCenteredText(battleMessage, layout.messageY, 18, WHITE);
    }

    int logX = 30;
    int logY = layout.messageY + 30;
    int logW = GRenderer::W - 60;
    int logH = layout.abilityY - logY - 10;

    DrawRectangle(logX, logY, logW, logH, {10, 10, 15, 180});
    DrawRectangleLines(logX, logY, logW, logH, {80, 80, 100, 100});

    int lty = logY + 8;
    int si = static_cast<int>(combatLog.size()) > 5 ? static_cast<int>(combatLog.size()) - 5 : 0;
    for (int i = si; i < static_cast<int>(combatLog.size()); ++i)
    {
        Color tc = CQColors::TextLight;
        if (combatLog[i].find("LEGENDARY") != std::string::npos)
            tc = RarityColor(Rarity::Legendary);
        else if (combatLog[i].find("EPIC") != std::string::npos)
            tc = RarityColor(Rarity::Epic);
        else if (combatLog[i].find("uses") != std::string::npos) tc = CQColors::TextGold;
        else if (combatLog[i].find("attack") != std::string::npos) tc = CQColors::TextRed;
        else if (combatLog[i].find("defensive") != std::string::npos) tc = CQColors::TextGreen;

        std::string msg = combatLog[i];
        int maxMsgW = logW - 20;
        while (!msg.empty() && MeasureText(msg.c_str(), 14) > maxMsgW)
            msg.pop_back();
        DrawText(msg.c_str(), logX + 10, lty, 14, tc);
        lty += 22;
    }

    DrawPlayerBar(renderer, player.GetName(),
                  player.GetCurrentHealth(), player.GetMaxHealth(),
                  player.GetCurrentMana(), player.GetMaxMana(),
                  layout.playerBarY,
                  playerXP, playerMaxXP,
                  masterySkillName, masteryXP, masteryMaxXP, masteryLevel,
                  charMasteryXP, charMasteryMaxXP, charMasteryLevel);

    // Show equipped pet info (inside player bar area, right side)
    int petInfoY = layout.playerBarY;
    if (petManager)
    {
        const Pet* pet = petManager->GetEquippedPet();
        if (pet)
        {
            int petInfoX = GRenderer::W - 250;

            Color elemCol = CQColors::TextDim;
            switch (pet->element)
            {
                case ElementType::Fire:      elemCol = {255, 100, 50, 255}; break;
                case ElementType::Ice:       elemCol = {100, 200, 255, 255}; break;
                case ElementType::Lightning: elemCol = {255, 255, 100, 255}; break;
                case ElementType::Arcane:    elemCol = {200, 120, 255, 255}; break;
                case ElementType::Poison:    elemCol = {100, 220, 80, 255}; break;
                case ElementType::Holy:      elemCol = {255, 240, 200, 255}; break;
                default:                     elemCol = {200, 200, 200, 255}; break;
            }

            DrawText(pet->GetCurrentName().c_str(), petInfoX, petInfoY + 6, 13, elemCol);

            int petDmg = petManager->CalculatePetDamage(playerLevel);
            std::string dmgText = "ATK:" + std::to_string(petDmg);
            DrawText(dmgText.c_str(), petInfoX + 130, petInfoY + 6, 12, CQColors::TextDim);

            // Pet XP bar
            int petLvl = pet->level;
            if (petLvl < Pet::MAX_PET_LEVEL)
            {
                int petXpBarX = petInfoX;
                int petXpBarY = petInfoY + 22;
                int petXpBarW = 200;
                int petXpCurrent = pet->experience;
                int petXpMax = Pet::CalculateRequiredXP(petLvl);
                if (petXpMax > 0)
                    renderer.DrawBarLabeled(petXpCurrent, petXpMax, petXpBarX, petXpBarY, petXpBarW, 12,
                                            CQColors::XpFg, CQColors::XpBg, "Lv." + std::to_string(petLvl));
            }
            else
            {
                DrawText(("Lv." + std::to_string(petLvl) + " MAX").c_str(), petInfoX, petInfoY + 22, 10, CQColors::TextGold);
            }
        }
    }

    // Faction reputation bar (inside player bar area, bottom)
    if (reputationMax > 0)
    {
        int repBarX = 30;
        int repBarY = layout.playerBarY + 64;
        int repBarW = GRenderer::W - 60;
        renderer.DrawBarLabeled(reputationValue, reputationMax, repBarX, repBarY, repBarW, 14,
                                CQColors::RepFg, CQColors::RepBg, repLabel);
    }

    if (phase == CombatPhase::PlayerTurn)
    {
        // Action buttons are drawn interactively by Game::StateCombat
    }
}

void BattleRenderer::DrawAbilityButton(GRenderer& renderer,
                                        const std::string& label,
                                        int x, int y, int size,
                                        bool hovered, bool pressed,
                                        Color iconColor)
{
    Color bgTop = pressed ? Color{30, 30, 35, 255} : Color{45, 45, 50, 255};
    Color bgBot = pressed ? Color{20, 20, 25, 255} : Color{35, 35, 40, 255};

    for (int i = 0; i < size; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(size);
        Color c = {
            static_cast<unsigned char>(bgTop.r + (bgBot.r - bgTop.r) * t),
            static_cast<unsigned char>(bgTop.g + (bgBot.g - bgTop.g) * t),
            static_cast<unsigned char>(bgTop.b + (bgBot.b - bgTop.b) * t),
            255
        };
        DrawRectangle(x, y + i, size, 1, c);
    }

    Color borderColor = hovered ? Color{255, 120, 0, 255} : Color{180, 90, 0, 200};
    DrawRectangleLines(x, y, size, size, borderColor);

    if (hovered)
    {
        DrawRectangleLines(x - 1, y - 1, size + 2, size + 2, {255, 120, 0, 80});
    }

    int iconSize = size / 3;
    int iconX = x + (size - iconSize) / 2;
    int iconY = y + (size - iconSize) / 2 - 8;
    DrawRectangle(iconX, iconY, iconSize, iconSize, iconColor);
    Color highlight = {255, 255, 255, 40};
    DrawRectangle(iconX, iconY, iconSize, iconSize / 2, highlight);

    int tw = MeasureText(label.c_str(), 14);
    DrawText(label.c_str(), x + (size - tw) / 2, y + size - 22, 14, WHITE);
}

void BattleRenderer::DrawPlayerBar(GRenderer& renderer,
                                    const std::string& name,
                                    int currentHP, int maxHP,
                                    int currentMP, int maxMP,
                                    int y,
                                    int currentXP,
                                    int maxXP,
                                    const std::string& masterySkillName,
                                    int masteryXP,
                                    int masteryMaxXP,
                                    int masteryLevel,
                                    int charMasteryXP,
                                    int charMasteryMaxXP,
                                    int charMasteryLevel)
{
    int barX = 30;
    int barW = GRenderer::W - 60;
    int barH = 76;

    DrawRectangle(barX, y, barW, barH, {15, 15, 20, 240});
    DrawRectangleLines(barX, y, barW, barH, {60, 60, 80, 200});

    DrawText(name.c_str(), barX + 10, y + 4, 16, CQColors::TextGold);

    int hpBarX = barX + 10;
    int hpBarY = y + 24;
    int hpBarW = 160;
    renderer.DrawBarLabeled(currentHP, maxHP, hpBarX, hpBarY, hpBarW, 16,
                            CQColors::HpFg, CQColors::HpBg, "HP");

    int mpBarX = hpBarX + hpBarW + 10;
    renderer.DrawBarLabeled(currentMP, maxMP, mpBarX, hpBarY, hpBarW, 16,
                            CQColors::ManaFg, CQColors::ManaBg, "MP");

    // XP bar
    int xpBarX = barX + 10;
    int xpBarY = y + 44;
    int xpBarW = barW - 20;
    if (maxXP > 0)
        renderer.DrawBarLabeled(currentXP, maxXP, xpBarX, xpBarY, xpBarW, 14,
                                CQColors::XpFg, CQColors::XpBg, "XP");
    else
        renderer.DrawText("MAX LEVEL", xpBarX, xpBarY, 12, CQColors::TextGold);

    // Mastery bar
    int masteryBarY = y + 60;
    if (masteryMaxXP > 0)
        renderer.DrawBarLabeled(masteryXP, masteryMaxXP, barX + 10, masteryBarY, barW - 20, 12,
                                CQColors::Gold, CQColors::GoldDim, "Mastery Lv" + std::to_string(masteryLevel) + " - " + masterySkillName);
    else if (masteryLevel > 0)
        DrawText(("Mastery Lv" + std::to_string(masteryLevel) + " - " + masterySkillName).c_str(), barX + 10, masteryBarY, 12, CQColors::TextGold);

    // Character Mastery bar
    if (charMasteryMaxXP > 0)
        renderer.DrawBarLabeled(charMasteryXP, charMasteryMaxXP, barX + 10, masteryBarY + 14, barW - 20, 12,
                                {180, 140, 220, 255}, {60, 40, 80, 255}, "Char Mastery Lv" + std::to_string(charMasteryLevel));
    else if (charMasteryLevel > 0)
        DrawText(("Char Mastery Lv" + std::to_string(charMasteryLevel)).c_str(), barX + 10, masteryBarY + 14, 12, {180, 140, 220, 255});
}
