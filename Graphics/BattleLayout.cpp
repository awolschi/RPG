#include "BattleLayout.hpp"

void BattleLayout::Calculate(int screenW, int screenH)
{
    photoY = 0;
    photoH = static_cast<int>(screenH * 0.60f);

    creatureW = static_cast<int>(screenW * 0.40f);
    creatureH = creatureW;
    creatureX = (screenW - creatureW) / 2;
    creatureY = photoH - creatureH - 30;

    uiY = photoH;
    nameY = photoH + 10;
    hpTextY = nameY + 28;
    messageY = hpTextY + 22;

    int totalAbilityW = 4 * abilitySize + 3 * abilitySpacing;
    int abilityStartX = (screenW - totalAbilityW) / 2;
    abilityY = uiY + 70;

    playerBarY = screenH - 50;
}
