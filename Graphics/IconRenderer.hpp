#ifndef ICONRENDERER_HPP
#define ICONRENDERER_HPP

#include <string>
#include "raylib.h"
#include "../Items/Item.hpp"

Color GetRarityColor(int rarity);
Color RarityColor(Rarity r);
void DrawItemIcon(const std::string& itemName, int rarity, int x, int y, int size);
void DrawEnemyIcon(const std::string& enemyName, int x, int y, int size);
void DrawStatusIcon(const std::string& statusName, int x, int y, int size);

#endif
