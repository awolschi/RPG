#ifndef WIKI_HPP
#define WIKI_HPP

#include <string>
#include <vector>
#include "../Graphics/GRenderer.hpp"
#include "../World/Areas/Area.hpp"
#include "../Characters/Classes/Classes.hpp"

enum class WikiTab
{
    Equipment,
    Skills,
    Enemies,
    Resources,
    Crafting,
    Areas,
    COUNT
};

class Wiki
{
public:
    Wiki();

    void Draw(GRenderer& renderer);
    void SetAreas(const std::vector<Area>& areaList);

private:
    WikiTab currentTab;
    int page;
    int maxPage;
    const std::vector<Area>* areas;

    std::string searchQuery;
    char searchBuf[64];

    struct WikiEntry
    {
        std::string name;
        std::string category;
        std::string info;
        std::string source;
        int rarity;
        std::string lootInfo;
    };

    std::vector<WikiEntry> equipEntries;
    std::vector<WikiEntry> skillEntries;
    std::vector<WikiEntry> enemyEntries;
    std::vector<WikiEntry> resourceEntries;
    std::vector<WikiEntry> craftEntries;
    std::vector<WikiEntry> areaEntries;

    void BuildEquipmentDatabase();
    void BuildSkillDatabase();
    void BuildEnemyDatabase();
    void BuildResourceDatabase();
    void BuildCraftingDatabase();
    void BuildAreaDatabase();

    void DrawTabPage(GRenderer& renderer, std::vector<WikiEntry>& entries, const std::string& title);
    void DrawTabBar(GRenderer& renderer);
};

#endif
