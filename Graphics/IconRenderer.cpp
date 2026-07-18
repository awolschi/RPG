#include "IconRenderer.hpp"
#include <cctype>
#include <cmath>

// ============================================================
//  COLOR PALETTE
// ============================================================

Color GetRarityColor(int rarity)
{
    return RarityColor(static_cast<Rarity>(rarity));
}

Color RarityColor(Rarity r)
{
    switch (r)
    {
        case Rarity::Common:    return {255, 255, 255, 255};
        case Rarity::Uncommon:  return {80, 220, 80, 255};
        case Rarity::Rare:      return {80, 150, 255, 255};
        case Rarity::Epic:      return {180, 80, 255, 255};
        case Rarity::Legendary: return {255, 165, 0, 255};
        default: return {200, 200, 200, 255};
    }
}

// ============================================================
//  HELPERS
// ============================================================

static Color Dim(Color c, float f)
{
    return {static_cast<unsigned char>(c.r * f),
            static_cast<unsigned char>(c.g * f),
            static_cast<unsigned char>(c.b * f),
            c.a};
}

static Vector2 V2(int x, int y)
{
    return {static_cast<float>(x), static_cast<float>(y)};
}

static bool NameContains(const std::string& name, const std::string& keyword)
{
    std::string upperName = name;
    std::string upperKey = keyword;
    for (auto& c : upperName) c = static_cast<char>(toupper(c));
    for (auto& c : upperKey) c = static_cast<char>(toupper(c));
    return upperName.find(upperKey) != std::string::npos;
}

// ============================================================
//  ITEM SHAPES  (all drawn at 24×24 scale)
// ============================================================

enum class ItemShape {
    Sword, Axe, Staff, Dagger, Bow, Mace,
    Helmet, ChestArmor, Gloves, Boots, Pants,
    Ring, Amulet,
    Crystal, Log, Fish, Ingot,
    Potion, Star,
    DefaultWeapon, DefaultArmor, DefaultAcc, DefaultResource
};

static ItemShape DetectItemShape(const std::string& name)
{
    if (NameContains(name, "Sword") || NameContains(name, "Blade") || NameContains(name, "Greatsword")
        || NameContains(name, "Cutlass") || NameContains(name, "Rapier") || NameContains(name, "Falchion")
        || NameContains(name, "Scimitar"))
        return ItemShape::Sword;
    if (NameContains(name, "Axe") || NameContains(name, "Hatchet"))
        return ItemShape::Axe;
    if (NameContains(name, "Staff") || NameContains(name, "Scepter") || NameContains(name, "Rod"))
        return ItemShape::Staff;
    if (NameContains(name, "Dagger") || NameContains(name, "Knife") || NameContains(name, "Shiv"))
        return ItemShape::Dagger;
    if (NameContains(name, "Bow") || NameContains(name, "Arrows") || NameContains(name, "Longbow"))
        return ItemShape::Bow;
    if (NameContains(name, "Mace") || NameContains(name, "Hammer") || NameContains(name, "Maul")
        || NameContains(name, "Flail"))
        return ItemShape::Mace;
    if (NameContains(name, "Helm") || NameContains(name, "Cap") || NameContains(name, "Crown")
        || NameContains(name, "Hood"))
        return ItemShape::Helmet;
    if (NameContains(name, "Robe") || NameContains(name, "Chest") || NameContains(name, "Chestplate")
        || NameContains(name, "Armor") || NameContains(name, "Plate") || NameContains(name, "Vest")
        || NameContains(name, "Tunic") || NameContains(name, "Tabard"))
        return ItemShape::ChestArmor;
    if (NameContains(name, "Glove") || NameContains(name, "Gauntlet") || NameContains(name, "Bracer"))
        return ItemShape::Gloves;
    if (NameContains(name, "Boot") || NameContains(name, "Shoe") || NameContains(name, "Sandals")
        || NameContains(name, "Treads"))
        return ItemShape::Boots;
    if (NameContains(name, "Pant") || NameContains(name, "Legging") || NameContains(name, "Legplate")
        || NameContains(name, "Greave") || NameContains(name, "Leggings"))
        return ItemShape::Pants;
    if (NameContains(name, "Ring") || NameContains(name, "Band"))
        return ItemShape::Ring;
    if (NameContains(name, "Amulet") || NameContains(name, "Pendant") || NameContains(name, "Seal")
        || NameContains(name, "Cloak") || NameContains(name, "Orb") || NameContains(name, "Might")
        || NameContains(name, "Blessing") || NameContains(name, "Pendant") || NameContains(name, "Necklace"))
        return ItemShape::Amulet;
    if (NameContains(name, "Ore") || NameContains(name, "Crystal") || NameContains(name, "Gem")
        || NameContains(name, "Stone") || NameContains(name, "Shard"))
        return ItemShape::Crystal;
    if (NameContains(name, "Wood") || NameContains(name, "Log") || NameContains(name, "Branch")
        || NameContains(name, "Timber"))
        return ItemShape::Log;
    if (NameContains(name, "Fish") || NameContains(name, "Salmon") || NameContains(name, "Trout"))
        return ItemShape::Fish;
    if (NameContains(name, "Ingot") || NameContains(name, "Bar"))
        return ItemShape::Ingot;
    if (NameContains(name, "Potion") || NameContains(name, "Elixir") || NameContains(name, "Vial")
        || NameContains(name, "Philter"))
        return ItemShape::Potion;
    if (NameContains(name, "Quest") || NameContains(name, "Artifact") || NameContains(name, "Relic"))
        return ItemShape::Star;

    // Generic fallbacks by keyword hints
    if (NameContains(name, "Sword") || NameContains(name, "Axe") || NameContains(name, "Blade")
        || NameContains(name, "Strike") || NameContains(name, "Flame") || NameContains(name, "Frost"))
        return ItemShape::DefaultWeapon;
    if (NameContains(name, "Hide") || NameContains(name, "Scale") || NameContains(name, "Skin"))
        return ItemShape::DefaultArmor;
    if (NameContains(name, "Silk") || NameContains(name, "Cloth"))
        return ItemShape::DefaultArmor;

    return ItemShape::DefaultWeapon;
}

static void DrawShape_Sword(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    int hb = s / 2 - 1;
    int hh = hb - 2;
    DrawTriangle(V2(cx, cy - hh), V2(cx - 4, cy), V2(cx + 4, cy), c);
    DrawRectangle(cx - 1, cy, 3, hb - 1, Dim(c, 0.6f));
    DrawCircle(cx, y + s - 2, 2, c);
}

static void DrawShape_Axe(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawTriangle(V2(cx - 5, cy - 3), V2(cx + 3, cy - 7), V2(cx + 4, cy + 2), c);
    DrawRectangle(cx - 1, cy + 1, 2, s / 2 - 2, Dim(c, 0.6f));
}

static void DrawShape_Staff(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawRectangle(cx - 1, y + 2, 2, s - 5, Dim(c, 0.6f));
    DrawCircle(cx, y + 3, 4, c);
    DrawCircle(cx, y + 3, 2, Dim(c, 1.5f));
}

static void DrawShape_Dagger(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawTriangle(V2(cx, y + 2), V2(cx - 3, y + s / 2), V2(cx + 3, y + s / 2), c);
    DrawRectangle(cx - 1, y + s / 2, 2, s / 2 - 2, Dim(c, 0.6f));
}

static void DrawShape_Bow(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int r = s / 2 - 1;
    DrawRectangle(cx - 1, y + 2, 2, s - 4, c);
    DrawLine(cx - r + 2, y + 3, cx + r - 2, y + s - 3, Dim(c, 0.5f));
}

static void DrawShape_Mace(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawRectangle(cx - 1, y + s / 2, 2, s / 2 - 2, Dim(c, 0.6f));
    DrawCircle(cx, y + s / 4 + 1, s / 4, c);
}

static void DrawShape_Helmet(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawEllipse(cx, y + s / 2, s / 2 - 1, s / 2 - 1, c);
    DrawRectangle(cx - s / 4, y + s / 2 - 1, s / 2, 2, Dim(c, 0.5f));
}

static void DrawShape_Chest(Color c, int x, int y, int s)
{
    DrawRectangle(x + 3, y + 3, s - 6, s - 8, c);
    DrawLine(x + s / 2, y + 3, x + s / 2, y + s - 5, Dim(c, 0.5f));
}

static void DrawShape_Gloves(Color c, int x, int y, int s)
{
    DrawCircle(x + s / 4 + 1, y + s / 4 + 1, s / 5, c);
    DrawCircle(x + 3 * s / 4 - 1, y + s / 4 + 1, s / 5, c);
    DrawRectangle(x + s / 4 - 1, y + s / 3, s / 2, s / 2, c);
}

static void DrawShape_Boots(Color c, int x, int y, int s)
{
    DrawRectangle(x + s / 4, y + 2, s / 3, s - 6, c);
    DrawRectangle(x + s / 4, y + s - 5, s / 2, 4, c);
}

static void DrawShape_Pants(Color c, int x, int y, int s)
{
    int mid = x + s / 2;
    DrawRectangle(mid - 4, y + 2, 8, s / 2 - 2, c);
    DrawRectangle(mid - 5, y + s / 2, 4, s / 2 - 3, c);
    DrawRectangle(mid + 1, y + s / 2, 4, s / 2 - 3, c);
}

static void DrawShape_Ring(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    int r = s / 2 - 2;
    DrawRing(V2(cx, cy), static_cast<float>(r - 2), static_cast<float>(r), 0, 360, 16, c);
    DrawCircle(cx, cy, r / 2, c);
}

static void DrawShape_Amulet(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawPoly(V2(cx, cy), 4, s / 2 - 2, 45, c);
    DrawLine(x + 2, y + 2, cx, cy - s / 4, Dim(c, 0.6f));
}

static void DrawShape_Crystal(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawPoly(V2(cx, cy), 4, s / 2 - 2, 0, c);
    DrawPoly(V2(cx, cy), 4, s / 4, 0, Dim(c, 1.3f));
}

static void DrawShape_Log(Color c, int x, int y, int s)
{
    DrawRectangle(x + 3, y + s / 4, s - 6, s / 2, c);
    for (int i = 0; i < 3; ++i)
        DrawLine(x + 4, y + s / 4 + 3 + i * 4, x + s - 4, y + s / 4 + 3 + i * 4, Dim(c, 0.6f));
}

static void DrawShape_Fish(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx - 2, cy, s / 3, s / 4, c);
    DrawTriangle(V2(cx + s / 3 + 1, cy), V2(cx + s / 3 - 3, cy - s / 4 - 1), V2(cx + s / 3 - 3, cy + s / 4 + 1), c);
    DrawCircle(cx - s / 5, cy - 1, 2, Dim(c, 0.5f));
}

static void DrawShape_Ingot(Color c, int x, int y, int s)
{
    DrawTriangle(V2(x + 3, y + s - 3), V2(x + s / 4, y + 3), V2(x + 3 * s / 4, y + 3), c);
    DrawTriangle(V2(x + s - 3, y + s - 3), V2(x + s / 4, y + 3), V2(x + 3 * s / 4, y + 3), c);
    DrawRectangle(x + s / 4, y + 3, s / 2, s / 2 - 6, c);
}

static void DrawShape_Potion(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawRectangle(cx - 2, y + 1, 4, 4, Dim(c, 0.6f));
    DrawRectangle(x + s / 4, y + 5, s / 2, s * 2 / 3, c);
    DrawTriangle(V2(x + s / 4, y + 5), V2(cx, y + s / 2), V2(x + 3 * s / 4, y + 5), Dim(c, 1.2f));
}

static void DrawShape_Star(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawPoly(V2(cx, cy), 5, s / 2 - 1, 0, c);
    DrawPoly(V2(cx, cy), 5, s / 4, 180, Dim(c, 1.3f));
}

// ============================================================
//  PUBLIC: DRAW ITEM ICON
// ============================================================

void DrawItemIcon(const std::string& itemName, int rarity, int x, int y, int size)
{
    Color c = GetRarityColor(rarity);
    ItemShape shape = DetectItemShape(itemName);

    switch (shape)
    {
        case ItemShape::Sword:        DrawShape_Sword(c, x, y, size); break;
        case ItemShape::Axe:          DrawShape_Axe(c, x, y, size); break;
        case ItemShape::Staff:        DrawShape_Staff(c, x, y, size); break;
        case ItemShape::Dagger:       DrawShape_Dagger(c, x, y, size); break;
        case ItemShape::Bow:          DrawShape_Bow(c, x, y, size); break;
        case ItemShape::Mace:         DrawShape_Mace(c, x, y, size); break;
        case ItemShape::Helmet:       DrawShape_Helmet(c, x, y, size); break;
        case ItemShape::ChestArmor:   DrawShape_Chest(c, x, y, size); break;
        case ItemShape::Gloves:       DrawShape_Gloves(c, x, y, size); break;
        case ItemShape::Boots:        DrawShape_Boots(c, x, y, size); break;
        case ItemShape::Pants:        DrawShape_Pants(c, x, y, size); break;
        case ItemShape::Ring:         DrawShape_Ring(c, x, y, size); break;
        case ItemShape::Amulet:       DrawShape_Amulet(c, x, y, size); break;
        case ItemShape::Crystal:      DrawShape_Crystal(c, x, y, size); break;
        case ItemShape::Log:          DrawShape_Log(c, x, y, size); break;
        case ItemShape::Fish:         DrawShape_Fish(c, x, y, size); break;
        case ItemShape::Ingot:        DrawShape_Ingot(c, x, y, size); break;
        case ItemShape::Potion:       DrawShape_Potion(c, x, y, size); break;
        case ItemShape::Star:         DrawShape_Star(c, x, y, size); break;
        default:                      DrawShape_Sword(c, x, y, size); break;
    }

    // Rarity-colored frame around the icon
    Color rc = GetRarityColor(rarity);
    DrawRectangleLines(x - 1, y - 1, size + 2, size + 2, rc);

    // Animated sparkle effect for Legendary items (rarity 5)
    if (rarity == 5)
    {
        float t = static_cast<float>(::GetTime());
        for (int i = 0; i < 3; ++i)
        {
            float phase = t * 3.0f + i * 2.094f; // 120 degrees apart
            float alpha = (std::sin(phase) + 1.0f) * 0.5f;
            if (alpha < 0.3f) continue;
            unsigned char sa = static_cast<unsigned char>(255.0f * alpha);
            Color sparkColor = {255, 230, 100, sa};
            // Star-shaped sparkle at different positions around the icon
            int sx = x + static_cast<int>(size * (0.2f + 0.6f * std::sin(t * 1.5f + i * 2.0f)));
            int sy = y + static_cast<int>(size * (0.2f + 0.6f * std::cos(t * 1.8f + i * 1.5f)));
            DrawLine(sx - 2, sy, sx + 2, sy, sparkColor);
            DrawLine(sx, sy - 2, sx, sy + 2, sparkColor);
            DrawLine(sx - 1, sy - 1, sx + 1, sy + 1, sparkColor);
            DrawLine(sx + 1, sy - 1, sx - 1, sy + 1, sparkColor);
        }
    }
}

// ============================================================
//  ENEMY PORTRAITS  (48×48 scale)
// ============================================================

enum class EnemyShape {
    Blob, Beast, Humanoid, Undead, Elemental, Dragon, Eldritch, Angel, Flying,
    UndeadArmy, Insectoid, Plant, Default
};

static EnemyShape DetectEnemyShape(const std::string& name)
{
    if (NameContains(name, "Slime") || NameContains(name, "Gelatinous") || NameContains(name, "Blob"))
        return EnemyShape::Blob;
    if (NameContains(name, "Skeleton") || NameContains(name, "Bone"))
        return EnemyShape::UndeadArmy;
    if (NameContains(name, "Spider") || NameContains(name, "Scorpion") || NameContains(name, "Insect")
        || NameContains(name, "Wasp") || NameContains(name, "Beetle"))
        return EnemyShape::Insectoid;
    if (NameContains(name, "Treant") || NameContains(name, "Ent") || NameContains(name, "Thorn")
        || NameContains(name, "Vine") || NameContains(name, "Dryad") || NameContains(name, "Briar")
        || NameContains(name, "Root") || NameContains(name, "Bramble"))
        return EnemyShape::Plant;
    if (NameContains(name, "Rat") || NameContains(name, "Wolf") || NameContains(name, "Bear")
        || NameContains(name, "Boar") || NameContains(name, "Viper") || NameContains(name, "Serpent"))
        return EnemyShape::Beast;
    if (NameContains(name, "Bandit") || NameContains(name, "Goblin") || NameContains(name, "Orc")
        || NameContains(name, "Knight") || NameContains(name, "Mage") || NameContains(name, "Guardian")
        || NameContains(name, "Thug") || NameContains(name, "Cultist") || NameContains(name, "Archer")
        || NameContains(name, "Warlock") || NameContains(name, "Assassin") || NameContains(name, "Warrior"))
        return EnemyShape::Humanoid;
    if (NameContains(name, "Zombie") || NameContains(name, "Ghost")
        || NameContains(name, "Wraith") || NameContains(name, "Lich") || NameContains(name, "Soul")
        || NameContains(name, "Banshee") || NameContains(name, "Ghoul"))
        return EnemyShape::Undead;
    if (NameContains(name, "Elemental") || NameContains(name, "Golem") || NameContains(name, "Construct"))
        return EnemyShape::Elemental;
    if (NameContains(name, "Dragon") || NameContains(name, "Drake") || NameContains(name, "Wyrm")
        || NameContains(name, "Wyvern"))
        return EnemyShape::Dragon;
    if (NameContains(name, "Void") || NameContains(name, "Cosmic") || NameContains(name, "Star")
        || NameContains(name, "Primordial") || NameContains(name, "Horror") || NameContains(name, "Walker")
        || NameContains(name, "Beast"))
        return EnemyShape::Eldritch;
    if (NameContains(name, "Celestial") || NameContains(name, "Seraph") || NameContains(name, "Angel")
        || NameContains(name, "Light"))
        return EnemyShape::Angel;
    if (NameContains(name, "Bat") || NameContains(name, "Bird") || NameContains(name, "Harpy")
        || NameContains(name, "Roc"))
        return EnemyShape::Flying;
    return EnemyShape::Default;
}

static Color EnemyColor(const std::string& name)
{
    if (NameContains(name, "BOSS") || NameContains(name, "Primordial") || NameContains(name, "Council")
        || NameContains(name, "King") || NameContains(name, "Lord"))
        return {255, 50, 50, 255};
    if (NameContains(name, "Void") || NameContains(name, "Cosmic") || NameContains(name, "Star"))
        return {180, 50, 200, 255};
    if (NameContains(name, "Celestial") || NameContains(name, "Seraph") || NameContains(name, "Light"))
        return {255, 215, 50, 255};
    if (NameContains(name, "Dragon") || NameContains(name, "Drake"))
        return {200, 80, 30, 255};
    if (NameContains(name, "Slime"))
        return {80, 200, 80, 255};
    if (NameContains(name, "Elemental"))
        return {80, 150, 255, 255};
    if (NameContains(name, "Skeleton") || NameContains(name, "Zombie") || NameContains(name, "Ghost"))
        return {180, 180, 200, 255};
    return {200, 160, 120, 255};
}

static void DrawEnemyShape_Blob(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx, cy + 2, s / 2 - 2, s / 3, c);
    DrawCircle(cx - 6, cy - 2, 4, Dim(c, 1.3f));
    DrawCircle(cx + 6, cy - 2, 4, Dim(c, 1.3f));
    DrawCircle(cx - 6, cy - 2, 2, Dim(c, 0.3f));
    DrawCircle(cx + 6, cy - 2, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Beast(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx, cy + 3, s / 2 - 1, s / 3, c);
    DrawCircle(cx - 8, cy - 3, 5, c);
    DrawCircle(cx + 8, cy - 3, 5, c);
    DrawCircle(cx - 8, cy - 3, 2, Dim(c, 0.3f));
    DrawCircle(cx + 8, cy - 3, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Humanoid(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    DrawCircle(cx, y + 8, 7, c);
    DrawRectangle(cx - 6, y + 14, 12, 16, c);
    DrawRectangle(cx - 10, y + 18, 5, 4, Dim(c, 0.7f));
    DrawRectangle(cx + 5, y + 18, 5, 4, Dim(c, 0.7f));
    DrawRectangle(cx - 4, y + 28, 4, 8, Dim(c, 0.7f));
    DrawRectangle(cx, y + 28, 4, 8, Dim(c, 0.7f));
    DrawCircle(cx - 2, y + 6, 2, Dim(c, 0.3f));
    DrawCircle(cx + 2, y + 6, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Undead(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawCircle(cx, cy, s / 2 - 2, c);
    DrawCircle(cx - 6, cy - 2, 3, Dim(c, 0.3f));
    DrawCircle(cx + 6, cy - 2, 3, Dim(c, 0.3f));
    DrawRectangle(cx - 4, cy + 5, 8, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Elemental(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawPoly(V2(cx, cy + 3), 4, s / 2 - 2, 0, c);
    DrawPoly(V2(cx, cy + 3), 4, s / 4, 0, Dim(c, 1.4f));
    DrawCircle(cx, cy + 2, 3, Dim(c, 1.5f));
}

static void DrawEnemyShape_Dragon(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawTriangle(V2(cx, cy - s / 3), V2(cx - s / 2 + 2, cy + s / 4), V2(cx + s / 2 - 2, cy + s / 4), c);
    DrawCircle(cx, cy + 2, 5, Dim(c, 0.5f));
    DrawTriangle(V2(cx - s / 4, cy - s / 3), V2(cx - s / 4 - 5, cy - s / 2), V2(cx - s / 4 + 2, cy - s / 3), Dim(c, 1.2f));
    DrawTriangle(V2(cx + s / 4, cy - s / 3), V2(cx + s / 4 + 5, cy - s / 2), V2(cx + s / 4 - 2, cy - s / 3), Dim(c, 1.2f));
}

static void DrawEnemyShape_Eldritch(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx, cy, s / 2 - 1, s / 2 - 1, c);
    DrawCircle(cx, cy, s / 4, Dim(c, 0.3f));
    DrawCircle(cx, cy, s / 6, c);
    DrawCircle(cx - 5, cy - 3, 2, Dim(c, 0.3f));
    DrawCircle(cx + 5, cy - 3, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Angel(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawCircle(cx, cy - 4, 7, c);
    DrawTriangle(V2(cx - 12, cy), V2(cx - 4, cy - 12), V2(cx, cy + 4), Dim(c, 0.7f));
    DrawTriangle(V2(cx + 12, cy), V2(cx + 4, cy - 12), V2(cx, cy + 4), Dim(c, 0.7f));
    DrawRectangle(cx - 5, cy + 2, 10, 14, Dim(c, 0.8f));
}

static void DrawEnemyShape_Flying(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawTriangle(V2(cx, y + 4), V2(cx - s / 2 + 2, cy + s / 4), V2(cx, cy), Dim(c, 0.6f));
    DrawTriangle(V2(cx, y + 4), V2(cx + s / 2 - 2, cy + s / 4), V2(cx, cy), Dim(c, 0.6f));
    DrawCircle(cx, cy + 2, 4, c);
    DrawCircle(cx - 1, cy + 1, 1, Dim(c, 0.3f));
}

static void DrawEnemyShape_UndeadArmy(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawCircle(cx - 8, cy - 4, 5, c);
    DrawCircle(cx - 8, cy - 4, 2, Dim(c, 0.3f));
    DrawCircle(cx - 10, cy - 6, 1, Dim(c, 0.3f));
    DrawCircle(cx - 6, cy - 6, 1, Dim(c, 0.3f));
    DrawRectangle(cx - 10, cy, 5, 3, Dim(c, 0.3f));
    DrawCircle(cx + 8, cy - 4, 5, Dim(c, 0.9f));
    DrawCircle(cx + 8, cy - 4, 2, Dim(c, 0.3f));
    DrawCircle(cx + 6, cy - 6, 1, Dim(c, 0.3f));
    DrawCircle(cx + 10, cy - 6, 1, Dim(c, 0.3f));
    DrawRectangle(cx + 6, cy, 5, 3, Dim(c, 0.3f));
    DrawCircle(cx, cy + 6, 6, Dim(c, 0.8f));
    DrawCircle(cx, cy + 6, 3, Dim(c, 0.3f));
    DrawRectangle(cx - 2, cy + 10, 4, 2, Dim(c, 0.3f));
}

static void DrawEnemyShape_Insectoid(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx, cy, s / 3, s / 4, c);
    DrawCircle(cx, cy - s / 4, s / 5, Dim(c, 1.2f));
    DrawCircle(cx - 1, cy - s / 4 - 1, 1, Dim(c, 0.3f));
    DrawCircle(cx + 2, cy - s / 4 - 1, 1, Dim(c, 0.3f));
    DrawLine(cx - s / 3, cy - 2, cx - s / 2, cy - 6, Dim(c, 0.7f));
    DrawLine(cx - s / 3, cy + 2, cx - s / 2, cy + 6, Dim(c, 0.7f));
    DrawLine(cx + s / 3, cy - 2, cx + s / 2, cy - 6, Dim(c, 0.7f));
    DrawLine(cx + s / 3, cy + 2, cx + s / 2, cy + 6, Dim(c, 0.7f));
    DrawLine(cx - s / 4, cy - s / 5, cx - s / 3, cy - s / 3, Dim(c, 0.6f));
    DrawLine(cx + s / 4, cy - s / 5, cx + s / 3, cy - s / 3, Dim(c, 0.6f));
}

static void DrawEnemyShape_Plant(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawRectangle(cx - 3, cy + 2, 6, s / 2 - 2, Dim(c, 0.5f));
    DrawTriangle(V2(cx, cy - s / 3), V2(cx - s / 3, cy + 4), V2(cx + s / 3, cy + 4), c);
    DrawTriangle(V2(cx - s / 5, cy - s / 5), V2(cx - s / 3 - 4, cy - 2), V2(cx - s / 6, cy - 2), Dim(c, 0.8f));
    DrawTriangle(V2(cx + s / 5, cy - s / 5), V2(cx + s / 3 + 4, cy - 2), V2(cx + s / 6, cy - 2), Dim(c, 0.8f));
    DrawCircle(cx - s / 6, cy - s / 4 + 2, 2, Dim(c, 1.3f));
    DrawCircle(cx + s / 6, cy - s / 4 + 2, 2, Dim(c, 1.3f));
}

static void DrawEnemyShape_Default(Color c, int x, int y, int s)
{
    int cx = x + s / 2;
    int cy = y + s / 2;
    DrawEllipse(cx, cy, s / 2 - 1, s / 3, Dim(c, 0.7f));
    DrawCircle(cx, cy - s / 6, s / 3, c);
    DrawCircle(cx - 4, cy - s / 6 - 2, 2, Dim(c, 0.3f));
    DrawCircle(cx + 4, cy - s / 6 - 2, 2, Dim(c, 0.3f));
    DrawRectangle(cx - 3, cy - s / 6 + 3, 6, 2, Dim(c, 0.3f));
}

// ============================================================
//  PUBLIC: DRAW ENEMY ICON
// ============================================================

static bool IsBossEnemy(const std::string& name)
{
    return NameContains(name, "BOSS") || NameContains(name, "King") || NameContains(name, "Lord")
        || NameContains(name, "Overseer") || NameContains(name, "Council")
        || NameContains(name, "Primordial") || NameContains(name, "Construct")
        || NameContains(name, "Chronos") || NameContains(name, "Dragon");
}

void DrawEnemyIcon(const std::string& enemyName, int x, int y, int size)
{
    // Subtle floating animation
    float bob = std::sin(static_cast<float>(::GetTime()) * 2.0f) * 2.0f;
    int drawY = y + static_cast<int>(bob);

    Color c = EnemyColor(enemyName);
    EnemyShape shape = DetectEnemyShape(enemyName);

    switch (shape)
    {
        case EnemyShape::Blob:       DrawEnemyShape_Blob(c, x, drawY, size); break;
        case EnemyShape::Beast:      DrawEnemyShape_Beast(c, x, drawY, size); break;
        case EnemyShape::Humanoid:   DrawEnemyShape_Humanoid(c, x, drawY, size); break;
        case EnemyShape::Undead:     DrawEnemyShape_Undead(c, x, drawY, size); break;
        case EnemyShape::Elemental:  DrawEnemyShape_Elemental(c, x, drawY, size); break;
        case EnemyShape::Dragon:     DrawEnemyShape_Dragon(c, x, drawY, size); break;
        case EnemyShape::Eldritch:   DrawEnemyShape_Eldritch(c, x, drawY, size); break;
        case EnemyShape::Angel:      DrawEnemyShape_Angel(c, x, drawY, size); break;
        case EnemyShape::Flying:     DrawEnemyShape_Flying(c, x, drawY, size); break;
        case EnemyShape::UndeadArmy: DrawEnemyShape_UndeadArmy(c, x, drawY, size); break;
        case EnemyShape::Insectoid:  DrawEnemyShape_Insectoid(c, x, drawY, size); break;
        case EnemyShape::Plant:      DrawEnemyShape_Plant(c, x, drawY, size); break;
        default:                     DrawEnemyShape_Default(c, x, drawY, size); break;
    }

    // Boss pulsing glowing outline
    if (IsBossEnemy(enemyName))
    {
        float t = static_cast<float>(::GetTime());
        unsigned char goldAlpha = static_cast<unsigned char>(120.0f + 100.0f * std::sin(t * 4.0f));
        Color bossGlow = {255, 215, 0, goldAlpha};
        DrawRectangleLines(x - 2, drawY - 2, size + 4, size + 4, bossGlow);
        Color bossGlowOuter = {255, 200, 0, static_cast<unsigned char>(goldAlpha / 2)};
        DrawRectangleLines(x - 4, drawY - 4, size + 8, size + 8, bossGlowOuter);
    }
}

// ============================================================
//  PUBLIC: DRAW STATUS ICON
// ============================================================

void DrawStatusIcon(const std::string& statusName, int x, int y, int size)
{
    int cx = x + size / 2;
    int cy = y + size / 2;

    if (NameContains(statusName, "Poison"))
    {
        Color gc = {50, 200, 50, 255};
        float t = static_cast<float>(::GetTime());
        DrawCircle(cx, cy, size / 2 - 2, {30, 120, 30, 200});
        DrawCircle(cx - 3, cy - 2 + static_cast<int>(std::sin(t * 3.0f) * 2), 3, gc);
        DrawCircle(cx + 2, cy + 1 + static_cast<int>(std::cos(t * 4.0f) * 2), 2, {80, 220, 80, 200});
        DrawCircle(cx, cy - 4 + static_cast<int>(std::sin(t * 5.0f + 1.0f) * 2), 2, gc);
    }
    else if (NameContains(statusName, "Burn") || NameContains(statusName, "Fire"))
    {
        Color fc = {255, 120, 30, 255};
        Color fh = {255, 200, 50, 200};
        DrawTriangle(V2(cx, cy - size / 2 + 2), V2(cx - size / 3, cy + size / 3),
                     V2(cx + size / 3, cy + size / 3), fc);
        DrawTriangle(V2(cx, cy - size / 4), V2(cx - size / 6, cy + size / 4),
                     V2(cx + size / 6, cy + size / 4), fh);
    }
    else if (NameContains(statusName, "Stun"))
    {
        Color sc = {255, 255, 50, 255};
        DrawTriangle(V2(cx, cy - size / 2 + 2), V2(cx + 4, cy - 2), V2(cx - 2, cy + 2), sc);
        DrawTriangle(V2(cx - 2, cy + 2), V2(cx + 2, cy - 2), V2(cx - 4, cy + size / 2 - 2), sc);
    }
    else if (NameContains(statusName, "Freeze") || NameContains(statusName, "Ice"))
    {
        Color ic = {100, 180, 255, 255};
        DrawRectangle(cx - 1, cy - size / 3, 2, size * 2 / 3, ic);
        DrawRectangle(cx - size / 3, cy - 1, size * 2 / 3, 2, ic);
        DrawRectangle(cx - size / 4, cy - size / 4, size / 2, 2, ic);
        DrawRectangle(cx - size / 4, cy + size / 4 - 2, size / 2, 2, ic);
    }
}
