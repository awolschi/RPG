#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "raylib.h"

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    Texture2D Load(const std::string& path);
    Texture2D Get(const std::string& path);
    bool Has(const std::string& path) const;
    void UnloadAll();

private:
    std::unordered_map<std::string, Texture2D> cache;
};

#endif
