#ifndef ICONRENDERER_HPP
#define ICONRENDERER_HPP

#include <string>
#include "raylib.h"

Color GetRarityColor(int rarity);
void DrawItemIcon(const std::string& itemName, int rarity, int x, int y, int size);
void DrawEnemyIcon(const std::string& enemyName, int x, int y, int size);

#endif
