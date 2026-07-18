#ifndef WIKI_HPP
#define WIKI_HPP

#include <set>
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
    Sets,
    Uniques,
    COUNT
};

class Wiki
{
public:
    Wiki();

    void Draw(GRenderer& renderer);
    void SetAreas(const std::vector<Area>& areaList);
    void MarkEnemyDefeated(const std::string& name);
    bool IsEnemyDefeated(const std::string& name) const;

private:
    WikiTab currentTab;
    int page;
    int maxPage;
    const std::vector<Area>* areas;

    std::string searchQuery;
    char searchBuf[64];

    // Enemy detail view
    int selectedEnemyIdx;  // index into filtered list, -1 = none
    bool showEnemyDetail;

    // Keyboard navigation state
    int focusedEntry;      // which entry row is keyboard-focused (-1 = none)
    int wikiFocusPhase;    // 0=tab bar, 1=search, 2=entries, 3=page nav, 4=detail back

    std::set<std::string> defeatedEnemies;

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
    std::vector<WikiEntry> setEntries;
    std::vector<WikiEntry> uniqueEntries;

    void BuildEquipmentDatabase();
    void BuildSkillDatabase();
    void BuildEnemyDatabase();
    void BuildResourceDatabase();
    void BuildCraftingDatabase();
    void BuildAreaDatabase();
    void BuildSetDatabase();
    void BuildUniqueDatabase();

    void DrawTabPage(GRenderer& renderer, std::vector<WikiEntry>& entries, const std::string& title);
    void DrawTabBar(GRenderer& renderer);
};

#endif
