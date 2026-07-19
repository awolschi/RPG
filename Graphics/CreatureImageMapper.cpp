#include "CreatureImageMapper.hpp"
#include "TextureManager.hpp"
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>

static std::string AssetPath(const std::string& relative)
{
    std::string appDir = GetApplicationDirectory();
    return appDir + "../" + relative;
}

static std::string ToLower(const std::string& s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

static std::string StripPrefixes(const std::string& name)
{
    static const std::vector<std::string> prefixes = {
        "greater ", "lesser ", "elder ", "ancient ", "shadow ",
        "dire ", "feral ", "corrupted ", "cursed ", "forsaken ",
        "fallen ", "exalted ", "primordial ", "cosmic ", "star ",
        "mana ", "crystal ", "arcane ", "void ", "bone ",
        "stone ", "fire ", "earth ", "light ", "sea ",
        "time ", "chrono ", "temporal ", "nether ",
        "the ", "farm ", "aran ", "astral "
    };
    std::string lower = ToLower(name);
    for (const auto& prefix : prefixes)
    {
        if (lower.substr(0, prefix.size()) == prefix)
        {
            return name.substr(prefix.size());
        }
    }
    return name;
}

static std::string ExtractKeyword(const std::string& name)
{
    std::string stripped = StripPrefixes(name);
    std::string lower = ToLower(stripped);
    return lower;
}

static const std::vector<std::string> knownCreatures = {
    "wolf", "spider", "slime", "rat",
    "chicken", "boar", "treant", "wraith", "pirate",
    "serpent", "crab", "siren", "orc", "harpy",
    "golem", "gryphon", "phantom", "demon", "knight",
    "lich", "phoenix", "seraphim",
    "construct", "beast", "horror",
    "bandit", "dark elf", "shadow", "kraken",
    "overseer", "warlord", "lord", "chronos",
    "guardian", "wisp", "spellblade", "weaver",
    "primeordeal", "warrior"
};

static std::string FindBestMatch(const std::string& normalized)
{
    for (const auto& creature : knownCreatures)
    {
        if (normalized.find(creature) != std::string::npos)
            return creature;
    }
    return "";
}

Texture2D CreatureImageMapper::GetCreatureTexture(TextureManager& tm, const std::string& enemyName)
{
    std::string normalized = ExtractKeyword(enemyName);

    std::string directPath = AssetPath("assets/creatures/" + normalized + ".jpeg");
    if (FileExists(directPath.c_str()))
        return tm.Load(directPath);

    directPath = AssetPath("assets/creatures/" + normalized + ".png");
    if (FileExists(directPath.c_str()))
        return tm.Load(directPath);

    std::string match = FindBestMatch(normalized);
    if (!match.empty())
    {
        std::string matchNoSpaces = match;
        matchNoSpaces.erase(std::remove(matchNoSpaces.begin(), matchNoSpaces.end(), ' '), matchNoSpaces.end());

        std::string matchedPath = AssetPath("assets/creatures/" + matchNoSpaces + ".jpeg");
        if (FileExists(matchedPath.c_str()))
            return tm.Load(matchedPath);

        matchedPath = AssetPath("assets/creatures/" + matchNoSpaces + ".png");
        if (FileExists(matchedPath.c_str()))
            return tm.Load(matchedPath);

        matchedPath = AssetPath("assets/creatures/" + match + ".jpeg");
        if (FileExists(matchedPath.c_str()))
            return tm.Load(matchedPath);

        matchedPath = AssetPath("assets/creatures/" + match + ".png");
        if (FileExists(matchedPath.c_str()))
            return tm.Load(matchedPath);
    }

    return {};
}

Texture2D CreatureImageMapper::GetBackgroundTexture(TextureManager& tm, const std::string& areaName)
{
    std::string normalized = ToLower(areaName);

    std::string path = AssetPath("assets/backgrounds/" + normalized + ".jpeg");
    if (FileExists(path.c_str()))
        return tm.Load(path);

    path = AssetPath("assets/backgrounds/" + normalized + ".png");
    if (FileExists(path.c_str()))
        return tm.Load(path);

    std::string match = FindBestMatch(normalized);
    if (!match.empty())
    {
        path = AssetPath("assets/backgrounds/" + match + ".jpeg");
        if (FileExists(path.c_str()))
            return tm.Load(path);

        path = AssetPath("assets/backgrounds/" + match + ".png");
        if (FileExists(path.c_str()))
            return tm.Load(path);
    }

    return {};
}
