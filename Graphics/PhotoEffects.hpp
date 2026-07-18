#ifndef PHOTO_EFFECTS_HPP
#define PHOTO_EFFECTS_HPP

#include "raylib.h"
#include <string>

class TextureManager;

namespace PhotoEffects
{
    void DrawVignette(int screenW, int screenH, float strength = 0.7f);
    Texture2D Desaturate(Texture2D source, float amount = 0.5f);
    void ApplyContrast(Image& img, float factor = 1.2f);
    Texture2D GetOrCreateDesaturated(TextureManager& tm, const std::string& path, float amount = 0.5f);
}

#endif
