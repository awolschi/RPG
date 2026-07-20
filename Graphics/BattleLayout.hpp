#ifndef BATTLE_LAYOUT_HPP
#define BATTLE_LAYOUT_HPP

struct BattleLayout
{
    int photoY = 0;
    int photoH = 460;
    int creatureX = 0;
    int creatureY = 0;
    int creatureW = 300;
    int creatureH = 300;

    int nameY = 470;
    int hpTextY = 500;
    int messageY = 530;

    int uiY = 460;
    int abilityY = 600;
    int abilitySize = 80;
    int abilitySpacing = 20;
    int playerBarY = 700;

    void Calculate(int screenW, int screenH);
};

#endif
