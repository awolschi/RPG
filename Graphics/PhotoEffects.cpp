#include "PhotoEffects.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <cstdio>

void PhotoEffects::DrawVignette(int screenW, int screenH, float strength)
{
    int bottomFadeH = static_cast<int>(screenH * 0.45f);
    int bottomStart = screenH - bottomFadeH;

    for (int i = 0; i < bottomFadeH; i += 2)
    {
        float t = static_cast<float>(i) / static_cast<float>(bottomFadeH);
        unsigned char alpha = static_cast<unsigned char>(255.0f * t * strength);
        DrawRectangle(0, bottomStart + i, screenW, 2, {0, 0, 0, alpha});
    }

    int edgeFadeW = static_cast<int>(screenW * 0.15f);
    for (int i = 0; i < edgeFadeW; i += 2)
    {
        float t = static_cast<float>(i) / static_cast<float>(edgeFadeW);
        unsigned char alpha = static_cast<unsigned char>(180.0f * t * strength);
        DrawRectangle(i, 0, 2, screenH, {0, 0, 0, alpha});
        DrawRectangle(screenW - i - 2, 0, 2, screenH, {0, 0, 0, alpha});
    }

    int topFadeH = static_cast<int>(screenH * 0.08f);
    for (int i = 0; i < topFadeH; i += 2)
    {
        float t = static_cast<float>(i) / static_cast<float>(topFadeH);
        unsigned char alpha = static_cast<unsigned char>(120.0f * t * strength);
        DrawRectangle(0, i, screenW, 2, {0, 0, 0, alpha});
    }
}

Texture2D PhotoEffects::Desaturate(Texture2D source, float amount)
{
    if (source.id == 0) return {};

    Image img = LoadImageFromTexture(source);
    if (img.data == nullptr) return source;

    unsigned char* pixels = static_cast<unsigned char*>(img.data);
    int pixelCount = img.width * img.height;

    for (int i = 0; i < pixelCount; ++i)
    {
        int offset = i * 4;
        unsigned char r = pixels[offset];
        unsigned char g = pixels[offset + 1];
        unsigned char b = pixels[offset + 2];

        float gray = 0.299f * r + 0.587f * g + 0.114f * b;
        float newR = r + (gray - r) * amount;
        float newG = g + (gray - g) * amount;
        float newB = b + (gray - b) * amount;

        pixels[offset]     = static_cast<unsigned char>(std::max(0.0f, std::min(255.0f, newR)));
        pixels[offset + 1] = static_cast<unsigned char>(std::max(0.0f, std::min(255.0f, newG)));
        pixels[offset + 2] = static_cast<unsigned char>(std::max(0.0f, std::min(255.0f, newB)));
    }

    Texture2D result = LoadTextureFromImage(img);
    UnloadImage(img);
    return result;
}

void PhotoEffects::ApplyContrast(Image& img, float factor)
{
    if (img.data == nullptr) return;

    unsigned char* pixels = static_cast<unsigned char*>(img.data);
    int pixelCount = img.width * img.height;

    for (int i = 0; i < pixelCount; ++i)
    {
        int offset = i * 4;
        for (int c = 0; c < 3; ++c)
        {
            float val = static_cast<float>(pixels[offset + c]) / 255.0f;
            val = (val - 0.5f) * factor + 0.5f;
            val = std::max(0.0f, std::min(1.0f, val));
            pixels[offset + c] = static_cast<unsigned char>(val * 255.0f);
        }
    }
}

Texture2D PhotoEffects::GetOrCreateDesaturated(TextureManager& tm, const std::string& path, float amount)
{
    std::string cacheKey = path + "_desat_" + std::to_string(static_cast<int>(amount * 100));
    if (tm.Has(cacheKey))
        return tm.Get(cacheKey);

    Texture2D original = tm.Get(path);
    if (original.id == 0) return {};

    Texture2D desat = Desaturate(original, amount);
    return desat;
}
