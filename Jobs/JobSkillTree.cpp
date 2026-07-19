#include "JobSkillTree.hpp"
#include <sstream>
#include <algorithm>

// Helper: name, desc, branch, tier, reqLevel, speed, fatigue, quality, rareFind, doubleChance, xp, sell, autoPickup, perfect, legendary, ultra, mythical
static JobSkill MS(const char* n, const char* d, SkillBranch br, int t, int lv,
    float sp=0, float fa=0, float qu=0, float rf=0, float dc=0, float xp=0, float sv=0,
    bool ap=false, bool pq=false, bool lq=false, bool ur=false, bool mi=false)
{
    JobSkill s;
    s.name = n; s.description = d; s.branch = br; s.tier = t; s.requiredLevel = lv;
    s.unlocked = false;
    s.speedBonus = sp; s.fatigueReduction = fa; s.qualityBonus = qu; s.rareFindBonus = rf;
    s.doubleChanceBonus = dc; s.xpBonus = xp; s.sellValueBonus = sv;
    s.autoPickup = ap; s.perfectQuality = pq; s.legendaryQuality = lq;
    s.ultraRare = ur; s.mythicalItems = mi;
    return s;
}

JobSkillTree::JobSkillTree()
{
    InitializeMiningSkills();
    InitializeLumberjackingSkills();
    InitializeFishingSkills();
    InitializeSmithingSkills();
}

void JobSkillTree::InitializeSkills(JobType) {}

// ---- Mining Skill Tree ----
void JobSkillTree::InitializeMiningSkills()
{
    miningSkills.clear();
    using B = SkillBranch;
    //                                    name                          desc                            br                  t  lv   sp     fa     qu     rf     dc     xp  sv  ap     pq     lq     ur     mi
    miningSkills.push_back(MS("Quick Strikes",        "Mining speed +10%",                    B::Efficiency,      1,  3,  0.10f));
    miningSkills.push_back(MS("Iron Stamina",         "Fatigue gain reduced by 20%",          B::Efficiency,      2, 10,  0,    0.20f));
    miningSkills.push_back(MS("Auto-Pickup",          "Resources collected automatically",     B::Efficiency,      3, 25,  0,    0,     0,     0,     0,     0,  0,  true));
    miningSkills.push_back(MS("Double Strike",        "15% chance for double resources",       B::Efficiency,      4, 50,  0,    0,     0,     0,     0.15f));
    miningSkills.push_back(MS("Triple Strike",        "5% chance for triple resources",        B::Efficiency,      5, 75,  0,    0,     0,     0,     0.05f));

    miningSkills.push_back(MS("Keen Eye",             "Quality chance +10%",                   B::Quality,         1,  5,  0,    0,     0.10f));
    miningSkills.push_back(MS("Careful Extraction",   "Fine resources more common",            B::Quality,         2, 15,  0,    0,     0.15f));
    miningSkills.push_back(MS("Master Touch",         "5% chance for Masterwork quality",      B::Quality,         3, 35,  0,    0,     0.05f));
    miningSkills.push_back(MS("Perfect Extraction",   "Guaranteed Fine or better quality",     B::Quality,         4, 60,  0,    0,     0.20f, 0,     0,     0,  0,  false, true));
    miningSkills.push_back(MS("Legendary Vein",       "Unique legendary resources can appear", B::Quality,         5, 90,  0,    0,     0.25f, 0,     0,     0,  0,  false, true));

    miningSkills.push_back(MS("Lucky Prospector",     "Rare finds +10%",                       B::Luck,            1,  8,  0,    0,     0,     0.10f));
    miningSkills.push_back(MS("Prospector's Intuition","Rare finds +20%",                      B::Luck,            2, 20,  0,    0,     0,     0.20f));
    miningSkills.push_back(MS("Deep Vein Sense",      "Rare finds +30%",                       B::Luck,            3, 40,  0,    0,     0,     0.30f));
    miningSkills.push_back(MS("Ultra-Rare Discovery", "Ultra-rare resources can appear",        B::Luck,            4, 65,  0,    0,     0,     0.20f, 0,     0,  0,  false, false, false, true));
    miningSkills.push_back(MS("Mythical Finds",       "Mythical resources can appear",          B::Luck,            5, 85,  0,    0,     0,     0.30f, 0,     0,  0,  false, false, false, false, true));
}

// ---- Lumberjacking Skill Tree ----
void JobSkillTree::InitializeLumberjackingSkills()
{
    lumberjackingSkills.clear();
    using B = SkillBranch;
    lumberjackingSkills.push_back(MS("Swift Cuts",           "Chopping speed +10%",                  B::Efficiency, 1,  3,  0.10f));
    lumberjackingSkills.push_back(MS("Woodland Endurance",   "Fatigue gain reduced by 20%",          B::Efficiency, 2, 10,  0,    0.20f));
    lumberjackingSkills.push_back(MS("Automatic Hauling",    "Logs collected automatically",          B::Efficiency, 3, 25,  0,    0,     0,     0,     0,     0,  0,  true));
    lumberjackingSkills.push_back(MS("Timber Burst",        "15% chance for double logs",            B::Efficiency, 4, 50,  0,    0,     0,     0,     0.15f));
    lumberjackingSkills.push_back(MS("Forest's Gift",       "5% chance for triple logs",             B::Efficiency, 5, 75,  0,    0,     0,     0,     0.05f));

    lumberjackingSkills.push_back(MS("Grain Reader",        "Quality chance +10%",                   B::Quality,    1,  5,  0,    0,     0.10f));
    lumberjackingSkills.push_back(MS("Selective Harvest",    "Fine wood more common",                 B::Quality,    2, 15,  0,    0,     0.15f));
    lumberjackingSkills.push_back(MS("Master Lumberjack",   "5% chance for Masterwork quality",      B::Quality,    3, 35,  0,    0,     0.05f));
    lumberjackingSkills.push_back(MS("Perfect Cut",         "Guaranteed Fine or better quality",     B::Quality,    4, 60,  0,    0,     0.20f, 0,     0,     0,  0,  false, true));
    lumberjackingSkills.push_back(MS("Ancient Timber",      "Unique legendary wood can appear",      B::Quality,    5, 90,  0,    0,     0.25f, 0,     0,     0,  0,  false, true));

    lumberjackingSkills.push_back(MS("Forest Wanderer",     "Rare finds +10%",                       B::Luck,       1,  8,  0,    0,     0,     0.10f));
    lumberjackingSkills.push_back(MS("Wild Instinct",       "Rare finds +20%",                       B::Luck,       2, 20,  0,    0,     0,     0.20f));
    lumberjackingSkills.push_back(MS("Ancient Growth Sense","Rare finds +30%",                       B::Luck,       3, 40,  0,    0,     0,     0.30f));
    lumberjackingSkills.push_back(MS("Ultra-Rare Timber",   "Ultra-rare wood can appear",             B::Luck,       4, 65,  0,    0,     0,     0.20f, 0,     0,  0,  false, false, false, true));
    lumberjackingSkills.push_back(MS("World Tree Fragment", "Mythical wood can appear",                B::Luck,       5, 85,  0,    0,     0,     0.30f, 0,     0,  0,  false, false, false, false, true));
}

// ---- Fishing Skill Tree ----
void JobSkillTree::InitializeFishingSkills()
{
    fishingSkills.clear();
    using B = SkillBranch;
    fishingSkills.push_back(MS("Quick Cast",          "Fishing speed +10%",                     B::Efficiency, 1,  3,  0.10f));
    fishingSkills.push_back(MS("Sea Legs",            "Fatigue gain reduced by 20%",            B::Efficiency, 2, 10,  0,    0.20f));
    fishingSkills.push_back(MS("Auto-Reel",           "Fish caught automatically",              B::Efficiency, 3, 25,  0,    0,     0,     0,     0,     0,  0,  true));
    fishingSkills.push_back(MS("Double Hook",         "15% chance for double catch",            B::Efficiency, 4, 50,  0,    0,     0,     0,     0.15f));
    fishingSkills.push_back(MS("Net Master",          "5% chance for triple catch",             B::Efficiency, 5, 75,  0,    0,     0,     0,     0.05f));

    fishingSkills.push_back(MS("Fresh Eyes",          "Quality chance +10%",                    B::Quality,    1,  5,  0,    0,     0.10f));
    fishingSkills.push_back(MS("Selective Catch",     "Fine fish more common",                  B::Quality,    2, 15,  0,    0,     0.15f));
    fishingSkills.push_back(MS("Master Angler",       "5% chance for Masterwork quality",       B::Quality,    3, 35,  0,    0,     0.05f));
    fishingSkills.push_back(MS("Perfect Specimen",    "Guaranteed Fine or better quality",      B::Quality,    4, 60,  0,    0,     0.20f, 0,     0,     0,  0,  false, true));
    fishingSkills.push_back(MS("Leviathan's Prize",   "Unique legendary fish can appear",       B::Quality,    5, 90,  0,    0,     0.25f, 0,     0,     0,  0,  false, true));

    fishingSkills.push_back(MS("Lucky Fisher",        "Rare finds +10%",                        B::Luck,       1,  8,  0,    0,     0,     0.10f));
    fishingSkills.push_back(MS("Deep Water Sense",    "Rare finds +20%",                        B::Luck,       2, 20,  0,    0,     0,     0.20f));
    fishingSkills.push_back(MS("Ocean Whisperer",     "Rare finds +30%",                        B::Luck,       3, 40,  0,    0,     0,     0.30f));
    fishingSkills.push_back(MS("Ultra-Rare Catch",    "Ultra-rare fish can appear",              B::Luck,       4, 65,  0,    0,     0,     0.20f, 0,     0,  0,  false, false, false, true));
    fishingSkills.push_back(MS("Mythical Catch",      "Mythical fish can appear",                B::Luck,       5, 85,  0,    0,     0,     0.30f, 0,     0,  0,  false, false, false, false, true));
}

// ---- Smithing Skill Tree ----
void JobSkillTree::InitializeSmithingSkills()
{
    smithingSkills.clear();
    using B = SkillBranch;
    smithingSkills.push_back(MS("Quick Forge",         "Smithing speed +10%",                     B::Efficiency, 1,  3,  0.10f));
    smithingSkills.push_back(MS("Steady Hands",        "Fatigue gain reduced by 20%",             B::Efficiency, 2, 10,  0,    0.20f));
    smithingSkills.push_back(MS("Auto-Craft",          "Items crafted automatically",             B::Efficiency, 3, 25,  0,    0,     0,     0,     0,     0,  0,  true));
    smithingSkills.push_back(MS("Double Output",       "15% chance for double crafted items",     B::Efficiency, 4, 50,  0,    0,     0,     0,     0.15f));
    smithingSkills.push_back(MS("Triple Output",       "5% chance for triple crafted items",      B::Efficiency, 5, 75,  0,    0,     0,     0,     0.05f));

    smithingSkills.push_back(MS("Sharp Eye",           "Quality chance +10%",                     B::Quality,    1,  5,  0,    0,     0.10f));
    smithingSkills.push_back(MS("Fine Tuning",         "Fine items more common",                  B::Quality,    2, 15,  0,    0,     0.15f));
    smithingSkills.push_back(MS("Master Smith",        "5% chance for Masterwork quality",        B::Quality,    3, 35,  0,    0,     0.05f));
    smithingSkills.push_back(MS("Perfect Temper",      "Guaranteed Fine or better quality",       B::Quality,    4, 60,  0,    0,     0.20f, 0,     0,     0,  0,  false, true));
    smithingSkills.push_back(MS("Legendary Forge",     "Unique legendary items can appear",       B::Quality,    5, 90,  0,    0,     0.25f, 0,     0,     0,  0,  false, true));

    smithingSkills.push_back(MS("Fortune's Hammer",    "Rare finds +10%",                         B::Luck,       1,  8,  0,    0,     0,     0.10f));
    smithingSkills.push_back(MS("Metal Intuition",     "Rare finds +20%",                         B::Luck,       2, 20,  0,    0,     0,     0.20f));
    smithingSkills.push_back(MS("Deep Forge Sense",    "Rare finds +30%",                         B::Luck,       3, 40,  0,    0,     0,     0.30f));
    smithingSkills.push_back(MS("Ultra-Rare Materials","Ultra-rare materials can appear",           B::Luck,       4, 65,  0,    0,     0,     0.20f, 0,     0,  0,  false, false, false, true));
    smithingSkills.push_back(MS("Mythril Discovery",   "Mythical materials can appear",             B::Luck,       5, 85,  0,    0,     0,     0.30f, 0,     0,  0,  false, false, false, false, true));
}

// ---- Query Methods ----

const JobSkill& JobSkillTree::GetSkill(JobType jobType, SkillBranch branch, int tier) const
{
    const auto& skills = GetSkills(jobType);
    for (const auto& s : skills)
        if (s.branch == branch && s.tier == tier) return s;
    return skills[0];
}

bool JobSkillTree::CanUnlockSkill(JobType jobType, SkillBranch branch, int tier, int jobLevel, int skillPoints) const
{
    if (skillPoints <= 0) return false;
    const auto& skills = GetSkills(jobType);
    for (const auto& s : skills)
    {
        if (s.branch == branch && s.tier == tier)
        {
            if (s.unlocked || jobLevel < s.requiredLevel) return false;
            if (tier > 1)
            {
                for (const auto& prev : skills)
                    if (prev.branch == branch && prev.tier == tier - 1 && !prev.unlocked)
                        return false;
            }
            return true;
        }
    }
    return false;
}

bool JobSkillTree::UnlockSkill(JobType jobType, SkillBranch branch, int tier, int& skillPoints)
{
    if (skillPoints <= 0) return false;
    auto& skills = GetSkillsRef(jobType);
    for (auto& s : skills)
    {
        if (s.branch == branch && s.tier == tier)
        {
            if (s.unlocked) return false;
            s.unlocked = true;
            skillPoints--;
            return true;
        }
    }
    return false;
}

float JobSkillTree::GetSpeedBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.speedBonus; return t; }
float JobSkillTree::GetFatigueReduction(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.fatigueReduction; return std::min(t,0.80f); }
float JobSkillTree::GetQualityBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.qualityBonus; return t; }
float JobSkillTree::GetRareFindBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.rareFindBonus; return t; }
float JobSkillTree::GetDoubleChanceBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.doubleChanceBonus; return t; }
float JobSkillTree::GetXPBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.xpBonus; return t; }
float JobSkillTree::GetSellValueBonus(JobType j) const { float t=0; for (auto& s : GetSkills(j)) if (s.unlocked) t+=s.sellValueBonus; return t; }

bool JobSkillTree::HasAutoPickup(JobType j) const { for (auto& s : GetSkills(j)) if (s.unlocked && s.autoPickup) return true; return false; }
bool JobSkillTree::HasPerfectQuality(JobType j) const { for (auto& s : GetSkills(j)) if (s.unlocked && s.perfectQuality) return true; return false; }
bool JobSkillTree::HasLegendaryQuality(JobType j) const { for (auto& s : GetSkills(j)) if (s.unlocked && s.legendaryQuality) return true; return false; }
bool JobSkillTree::HasUltraRare(JobType j) const { for (auto& s : GetSkills(j)) if (s.unlocked && s.ultraRare) return true; return false; }
bool JobSkillTree::HasMythicalItems(JobType j) const { for (auto& s : GetSkills(j)) if (s.unlocked && s.mythicalItems) return true; return false; }

const std::vector<JobSkill>& JobSkillTree::GetSkills(JobType j) const { return GetSkillsRef(j); }

std::vector<JobSkill>& JobSkillTree::GetSkillsRef(JobType j)
{
    switch (j) {
        case JobType::Mining:        return miningSkills;
        case JobType::Lumberjacking: return lumberjackingSkills;
        case JobType::Fishing:       return fishingSkills;
        case JobType::Smithing:      return smithingSkills;
    }
    return miningSkills;
}

const std::vector<JobSkill>& JobSkillTree::GetSkillsRef(JobType j) const
{
    switch (j) {
        case JobType::Mining:        return miningSkills;
        case JobType::Lumberjacking: return lumberjackingSkills;
        case JobType::Fishing:       return fishingSkills;
        case JobType::Smithing:      return smithingSkills;
    }
    return miningSkills;
}

// ---- Serialization ----

std::string JobSkillTree::Serialize() const
{
    std::ostringstream ss;
    auto ser = [&](const std::vector<JobSkill>& sk) {
        for (size_t i = 0; i < sk.size(); ++i) { if (i>0) ss << ","; ss << (sk[i].unlocked?"1":"0"); }
    };
    ser(miningSkills); ss << ";";
    ser(lumberjackingSkills); ss << ";";
    ser(fishingSkills); ss << ";";
    ser(smithingSkills);
    return ss.str();
}

void JobSkillTree::Deserialize(const std::string& data)
{
    std::istringstream ss(data);
    std::string jobStr;
    int ji = 0;
    while (std::getline(ss, jobStr, ';') && ji < 4)
    {
        auto& sk = GetSkillsRef(static_cast<JobType>(ji));
        std::istringstream ps(jobStr);
        std::string s; int idx = 0;
        while (std::getline(ps, s, ',') && idx < (int)sk.size()) { sk[idx].unlocked = (s=="1"); idx++; }
        ji++;
    }
}
