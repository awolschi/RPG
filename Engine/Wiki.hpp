#ifndef WIKI_HPP
#define WIKI_HPP

#include <set>
#include <string>
#include <vector>
#include "../Graphics/GRenderer.hpp"
#include "../World/Areas/Area.hpp"
#include "../Characters/Classes/Classes.hpp"

class Player;

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
    Pets,
    COUNT
};

class Wiki
{
public:
    Wiki();

    void Draw(GRenderer& renderer);
    void SetAreas(const std::vector<Area>& areaList);
    void SetPlayer(Player* p) { player = p; }
    void MarkEnemyDefeated(const std::string& name);
    bool IsEnemyDefeated(const std::string& name) const;
    void MarkPetObtained(const std::string& id);
    bool IsPetObtained(const std::string& id) const;
    void MarkPetObtainedByName(const std::string& name);
    void MarkItemDiscovered(const std::string& name);
    bool IsItemDiscovered(const std::string& name) const;

    // Save / Load discovery state
    std::string Serialize() const;
    void Deserialize(const std::string& data);

private:
    WikiTab currentTab;
    int page;
    int maxPage;
    const std::vector<Area>* areas;
    Player* player = nullptr;

    std::string searchQuery;
    char searchBuf[64];

    // Enemy detail view
    int selectedEnemyIdx;  // index into filtered list, -1 = none
    bool showEnemyDetail;

    // Pet detail view
    int selectedPetIdx;
    bool showPetDetail;

    // Keyboard navigation state
    int focusedEntry;      // which entry row is keyboard-focused (-1 = none)
    int wikiFocusPhase;    // 0=tab bar, 1=search, 2=entries, 3=page nav, 4=detail back

    std::set<std::string> defeatedEnemies;
    std::set<std::string> obtainedPets;       // keyed by pet display name
    std::set<std::string> discoveredItems;   // keyed by item name

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
    std::vector<WikiEntry> petEntries;

    void BuildEquipmentDatabase();
    void BuildSkillDatabase();
    void BuildEnemyDatabase();
    void BuildResourceDatabase();
    void BuildCraftingDatabase();
    void BuildAreaDatabase();
    void BuildSetDatabase();
    void BuildUniqueDatabase();
    void BuildPetDatabase();

    void DrawTabPage(GRenderer& renderer, std::vector<WikiEntry>& entries, const std::string& title);
    void DrawTabBar(GRenderer& renderer);
};

#endif
