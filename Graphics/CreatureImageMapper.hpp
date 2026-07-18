#ifndef CREATURE_IMAGE_MAPPER_HPP
#define CREATURE_IMAGE_MAPPER_HPP

#include "raylib.h"
#include <string>

class TextureManager;

namespace CreatureImageMapper
{
    Texture2D GetCreatureTexture(TextureManager& tm, const std::string& enemyName);
    Texture2D GetBackgroundTexture(TextureManager& tm, const std::string& areaName);
}

#endif
