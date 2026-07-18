#include "TextureManager.hpp"
#include "raylib.h"
#include <cstdio>

TextureManager::~TextureManager()
{
    UnloadAll();
}

Texture2D TextureManager::Load(const std::string& path)
{
    auto it = cache.find(path);
    if (it != cache.end())
        return it->second;

    if (!FileExists(path.c_str()))
    {
        printf("[TextureManager] File not found: %s\n", path.c_str());
        return {};
    }

    Texture2D tex = LoadTexture(path.c_str());
    if (tex.id == 0)
    {
        printf("[TextureManager] Failed to load: %s\n", path.c_str());
        return {};
    }

    cache[path] = tex;
    return tex;
}

Texture2D TextureManager::Get(const std::string& path)
{
    auto it = cache.find(path);
    if (it != cache.end())
        return it->second;

    return Load(path);
}

bool TextureManager::Has(const std::string& path) const
{
    return cache.find(path) != cache.end();
}

void TextureManager::UnloadAll()
{
    for (auto& [key, tex] : cache)
    {
        if (tex.id != 0)
            UnloadTexture(tex);
    }
    cache.clear();
}
