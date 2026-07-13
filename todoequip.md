# Equipment Overhaul Plan — Mage Focus

## Overview

Overhaul the equipment system to make gear more interesting with formalized rarity tiers, unique named items, set bonuses, and balanced drop tables. **Mage-first implementation** — other classes added later.

---

## 1. Formalize Rarity System

**File:** `Items/Item.hpp`

Replace the implicit `int rarity` with a named enum and helper functions.

```cpp
enum class Rarity : int
{
    Common    = 1,  // White
    Uncommon  = 2,  // Green
    Rare      = 3,  // Blue
    Epic      = 4,  // Purple
    Legendary = 5   // Orange
};

const char* RarityName(Rarity r);
Color RarityColor(Rarity r);  // Returns raylib Color for display
```

**Color mapping:**
| Rarity | Name | Color |
|--------|------|-------|
| 1 Common | White | `{255, 255, 255, 255}` |
| 2 Uncommon | Green | `{80, 220, 80, 255}` |
| 3 Rare | Blue | `{80, 150, 255, 255}` |
| 4 Epic | Purple | `{180, 80, 255, 255}` |
| 5 Legendary | Orange | `{255, 165, 0, 255}` |

**Existing item rarity shifts:**
- Current `rarity 1` (Common) stays at 1
- Current `rarity 2` (called "Rare") becomes `rarity 2` (Uncommon)
- Current `rarity 3` (Boss) becomes `rarity 3` (Rare)
- Current `rarity 4` (Boss+diff>3) becomes `rarity 4` (Epic) — but ONLY for unique named items
- Current `rarity 5` (Legendary) stays at 5 — but ONLY for unique named items

**Key rule:** Rarity 4 (Epic) and 5 (Legendary) items are NEVER randomly generated. They come from a unique item registry.

---

## 2. Drop Tier → Rarity Mapping

**File:** `Items/Loot.cpp` — Update `GenerateLoot()` and `GenerateBossLoot()`

New drop table per area:

| Area | dropTier | Normal Enemy Drops | Boss Drops |
|------|----------|-------------------|------------|
| Felrona (1) | 1-2 | Common, 20% Uncommon | Uncommon guaranteed, 30% Rare |
| Dark Woods (2) | 3-4 | Common 60%, Uncommon 35%, Rare 5% | Uncommon guaranteed, 50% Rare |
| Coast (3) | 5-6 | Common 40%, Uncommon 40%, Rare 20% | Rare guaranteed, 20% Epic (unique) |
| Dragon's Peak (4) | 7-8 | Uncommon 50%, Rare 40%, Epic 10% | Rare guaranteed, 40% Epic (unique) |
| Highlands (5) | 9-10 | Uncommon 30%, Rare 50%, Epic 20% | Rare guaranteed, 60% Epic (unique) |
| Shadow Dungeon (6) | 11-12 | Rare 50%, Epic 40%, Legendary 10% | Epic guaranteed, 30% Legendary (unique) |
| Celestial Spire (7) | 13-14 | Rare 30%, Epic 50%, Legendary 20% | Epic guaranteed, 50% Legendary (unique) |
| Void Beyond (8) | 15-16 | Epic 50%, Legendary 30% | Legendary guaranteed |
| Arcane Sanctum (9) | 17-18 | Epic 40%, Legendary 40% | Legendary guaranteed |
| Chronos Depths (10) | 19-20 | Epic 30%, Legendary 50% | Legendary guaranteed |

**Updated formulas:**

```cpp
// GenerateLoot — normal enemies
static vector<shared_ptr<Item>> GenerateLoot(int difficulty, int dropTier);

// GenerateBossLoot — bosses
static vector<shared_ptr<Item>> GenerateBossLoot(int difficulty, int dropTier);

// GenerateUniqueDrop — new function for Epic/Legendary pulls
static shared_ptr<Item> GenerateUniqueDrop(int dropTier, CharacterClass charClass);
```

---

## 3. Unique Item Registry

**New file:** `Items/Uniques/UniqueItems.hpp` and `UniqueItems.cpp`

Central registry of all unique (Epic + Legendary) items. Each item has:
- Fixed name, stats, and elemental properties
- A `dropSource` (which enemy/boss drops it)
- A `setId` (if it belongs to a set)

```cpp
struct UniqueItemDef
{
    std::string name;
    Rarity rarity;
    ItemType type;
    // Type-specific fields...
    std::string dropSource;  // enemy name, "" = any boss
    int setId;               // -1 = not a set item
};

class UniqueItemRegistry
{
public:
    static void Initialize();
    static const UniqueItemDef* Find(const std::string& name);
    static std::shared_ptr<Item> Create(const std::string& name);
    static std::vector<UniqueItemDef> GetByDropSource(const std::string& enemyName);
    static std::vector<UniqueItemDef> GetByRarity(Rarity r);
    static std::vector<UniqueItemDef> GetBySet(int setId);
};
```

---

## 4. Mage Unique Epics (Rarity 4)

### Weapons

| Name | Type | DMG | Mana | Element | Elem DMG | Drop Source |
|------|------|-----|------|---------|----------|-------------|
| Staff of Frozen Memories | Staff | 45 + diff*3 | 40 | Ice | 15 + diff*2 | Giant Spider (area 2) |
| Wand of the Blazing Sun | Wand | 35 + diff*3 | 50 | Fire | 20 + diff*2 | Fire Elemental (area 4) |
| Tome of Whispered Truths | Wand | 40 + diff*3 | 55 | Arcane | 18 + diff*2 | Lich (area 6) |
| Stormweaver's Focus | Staff | 50 + diff*2 | 35 | Lightning | 22 + diff*2 | Sea Serpent (area 3) |
| Void-Touched Scepter | Staff | 55 + diff*3 | 45 | Arcane | 25 + diff*2 | Void Knight (area 6) |

### Armor (Cloth)

| Name | Piece | DEF | Resist | Drop Source |
|------|-------|-----|--------|-------------|
| Hood of the Frozen Eye | Helmet | 12 + diff*2 | Ice 5 + diff | Wolf (area 2) |
| Mantle of the Flamecaller | Chest | 15 + diff*2 | Fire 6 + diff | Drake (area 4) |
| Gloves of Arcane Weaving | Gloves | 10 + diff*2 | Arcane 4 + diff | Dark Elf Scout (area 2) |
| Leggings of the Storm | Pants | 11 + diff*2 | Lightning 5 + diff | Highland Warrior (area 5) |
| Sandals of the Void | Boots | 9 + diff*2 | Arcane 6 + diff | Shadow (area 6) |

### Accessories

| Name | HP | Mana | Element | Elem DMG | Drop Source |
|------|-----|------|---------|----------|-------------|
| Circlet of Withered Dreams | 30 + diff*5 | 50 + diff*8 | Arcane | 10 + diff*2 | Wraith (area 2) |
| Pendant of the Tidal Sage | 40 + diff*5 | 60 + diff*8 | Ice | 12 + diff*2 | Siren (area 3) |
| Ring of the Ember Court | 25 + diff*5 | 45 + diff*8 | Fire | 15 + diff*2 | Orc (area 4) |

**Total: 5 weapons + 5 armor + 3 accessories = 13 unique epics for mage**

---

## 5. All Legendary Items (Rarity 5)

Every boss drops a pool of legendaries. Each boss has 8-15 items covering all equipment slots and all classes. **101 total legendaries.**

### Passive Effect Reference

| Passive ID | Description |
|------------|-------------|
| `FireNoCd20` | Fire spells 20% chance to ignore cooldown |
| `IceFreezePlus1` | Ice spells freeze +1 turn |
| `LightningStun15` | Lightning spells 15% chance to stun 1 turn |
| `ArcaneCrit30` | Arcane spells 30% chance for double damage |
| `PoisonDmgPlus25` | Poison effects deal +25% damage |
| `HolyHealOnKill` | Killing an enemy heals 10% max HP |
| `ManaCostReduce10` | All spells cost 10% less mana |
| `ManaCostReduce15` | All spells cost 15% less mana |
| `ManaRegen5` | +5 mana restored per turn |
| `ManaRegen10` | +10 mana restored per turn |
| `SpellDmgBoost15` | +15% spell damage |
| `SpellDmgBoost20` | +20% spell damage |
| `SpellDmgLowHp30` | +30% spell damage when HP < 50% |
| `PhysDmgBoost15` | +15% physical damage |
| `PhysDmgBoost20` | +20% physical damage |
| `PhysDmgLowHp30` | +30% physical damage when HP < 50% |
| `AtkSpeed20` | +20% attack speed (reduces turn delay) |
| `CritChance20` | 20% chance for critical hit (1.5x damage) |
| `CritChance30` | 30% chance for critical hit (1.5x damage) |
| `Dodge15` | 15% chance to dodge any attack |
| `Dodge20` | 20% chance to dodge any attack |
| `DamageReduce10` | Take 10% less damage from all sources |
| `DamageReduce15` | Take 15% less damage from all sources |
| `HealOnKill10` | Killing an enemy restores 10% max HP |
| `HealOnKill15` | Killing an enemy restores 15% max HP |
| `Lifesteal15` | Heal for 15% of damage dealt |
| `Lifesteal25` | Heal for 25% of damage dealt |
| `Thorns25` | Reflect 25% of damage taken back to attacker |
| `Thorns40` | Reflect 40% of damage taken back to attacker |
| `PhoenixRevive` | Revive once per battle with 30% HP |
| `Revive50` | Revive once per battle with 50% HP |
| `TauntChance25` | 25% chance to force single-target attacks on self |
| `ManaShield30` | 30% of damage taken is absorbed by mana instead of HP |
| `DoubleCast15` | 15% chance spells hit twice (second at 50% damage) |
| `DoubleCast25` | 25% chance spells hit twice (second at 50% damage) |
| `CooldownReduce1` | All cooldowns reduced by 1 turn |
| `CooldownReduce2` | All cooldowns reduced by 2 turns |
| `StatusExtend1` | All status effects you apply last +1 turn |
| `StatusExtend2` | All status effects you apply last +2 turns |
| `ExpBoost20` | +20% experience from kills |
| `GoldFind30` | +30% gold from kills |
| `FireResistPlus10` | +10 fire resistance |
| `IceResistPlus10` | +10 ice resistance |
| `LightningResistPlus10` | +10 lightning resistance |
| `AllResist5` | +5 resistance to all elements |
| `AllResist10` | +10 resistance to all elements |
| `PhysDmgReflect20` | Reflect 20% of physical damage |
| `StunImmune` | Immune to stun effects |
| `FreezeImmune` | Immune to freeze effects |
| `BurnImmune` | Immune to burn effects |
| `PoisonImmune` | Immune to poison effects |
| `ManaOnKill15` | Restore 15 mana on kill |
| `ManaOnKill25` | Restore 25 mana on kill |
| `HpOnSkillUse10` | Heal 10 HP each time you use a skill |
| `ManaOnSkillUse10` | Restore 10 mana each time you use a skill |

---

### Boss 1: Farm Overseer (Area 1 — Abandoned Farm)

*Theme: Farm tools, rusted iron, simple but effective.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 1 | Overseer's Cleaver | Weapon (Axe) | Warrior | DMG 55, Mana 5 | `PhysDmgBoost15` | — |
| 2 | Pitchfork of the Fields | Weapon (Spear) | Warrior | DMG 50, Mana 0 | `Thorns25` | — |
| 3 | Rusty Spellgrimoire | Weapon (Wand) | Mage | DMG 35, Mana 30 | `ManaRegen5` | — |
| 4 | Scarecrow's Hood | Helmet | Any | DEF 12, Fire Resist 5 | `Dodge15` | — |
| 5 | Straw-Filled Vest | Chest | Any | DEF 15, Ice Resist 3 | `DamageReduce10` | — |
| 6 | Work-Worn Gloves | Gloves | Any | DEF 8 | `CritChance20` | — |
| 7 | Fieldwalker Boots | Boots | Any | DEF 7 | `HealOnKill10` | — |
| 8 | Ring of the Harvest | Ring | Any | HP 40, Mana 20 | `GoldFind30` | — |
| 9 | Pendant of the Landlord | Amulet | Any | HP 30, Mana 15 | `ExpBoost20` | — |

---

### Boss 2: Treant King (Area 2 — Twilight Hollow)

*Theme: Nature, living wood, forest magic.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 10 | Briarwood Staff | Weapon (Staff) | Mage | DMG 80, Mana 60, Fire:30 | `FireNoCd20` | Verdant |
| 11 | Thornwhip | Weapon (Dagger) | Archer | DMG 70, Mana 10, Poison:25 | `PoisonDmgPlus25` | — |
| 12 | Living Root Bow | Weapon (Bow) | Archer | DMG 75, Mana 15, Poison:20 | `StatusExtend1` | — |
| 13 | Crown of Thorns | Helmet | Any | DEF 18, Fire Resist 8 | `Thorns40` | Verdant |
| 14 | Barkheart Chestguard | Chest | Warrior | DEF 25, Ice Resist 6 | `DamageReduce15` | Verdant |
| 15 | Mantle of the Wild | Chest | Mage/Priest | DEF 16, Fire Resist 10 | `SpellDmgBoost15` | Verdant |
| 16 | Sapweave Gloves | Gloves | Any | DEF 10, Poison Resist 8 | `Lifesteal15` | — |
| 17 | Rootwalker Leggings | Pants | Any | DEF 12, Ice Resist 5 | `Dodge15` | — |
| 18 | Seedkeeper Boots | Boots | Any | DEF 9, Poison Resist 6 | `HealOnKill10` | Verdant |
| 19 | Ring of Living Wood | Ring | Any | HP 50, Mana 25 | `AllResist5` | — |
| 20 | Amulet of the Forest Heart | Amulet | Any | HP 35, Mana 40 | `ManaRegen5` | — |

---

### Boss 3: Kraken (Area 3 — Sunken Grotto)

*Theme: Ocean, water, tentacles, deep sea pressure.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 21 | Tidecaller's Staff | Weapon (Staff) | Mage | DMG 100, Mana 70, Ice:35 | `IceFreezePlus1` | Abyssal |
| 22 | Kraken's Grip | Weapon (Mace) | Priest | DMG 85, Mana 40, Ice:30 | `HolyHealOnKill` | Abyssal |
| 23 | Inkshot Longbow | Weapon (Bow) | Archer | DMG 90, Mana 20 | `CritChance30` | — |
| 24 | Abyssal Helm | Helmet | Warrior | DEF 22, Ice Resist 12 | `DamageReduce10` | Abyssal |
| 25 | Tide-Sworn Robes | Chest | Mage | DEF 18, Ice Resist 14 | `ManaCostReduce10` | Abyssal |
| 26 | Mantle of the Deep | Chest | Priest | DEF 17, Ice Resist 10 | `HealOnKill15` | — |
| 27 | Tentacle-Woven Bracers | Gloves | Any | DEF 11, Lightning Resist 8 | `CritChance20` | — |
| 28 | Pressure-Tossed Leggings | Pants | Any | DEF 14, Ice Resist 6 | `Dodge15` | — |
| 29 | Coral Stalkers | Boots | Any | DEF 10, Ice Resist 8 | `FreezeImmune` | Abyssal |
| 30 | Ring of the Drowned King | Ring | Any | HP 60, Mana 30, Ice:15 | `AllResist5` | — |
| 31 | Abyssal Pearl Pendant | Amulet | Any | HP 45, Mana 50, Ice:20 | `ManaOnKill15` | — |

---

### Boss 4: Elder Dragon (Area 4 — The Summit)

*Theme: Fire, dragon scales, raw elemental power.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 32 | Staff of the Eternal Flame | Weapon (Staff) | Mage | DMG 140, Mana 90, Fire:50 | `FireNoCd20` | Dragonfire |
| 33 | Dragonbone Wand | Weapon (Wand) | Mage | DMG 120, Mana 100, Fire:45 | `DoubleCast15` | Dragonfire |
| 34 | Fang of the Elder | Weapon (Sword) | Warrior | DMG 150, Mana 10, Fire:30 | `CritChance30` | Dragonfire |
| 35 | Drake's Eye Scepter | Weapon (Scepter) | Priest | DMG 110, Mana 80, Holy:40 | `HolyHealOnKill` | — |
| 36 | Dragonhide Helm | Helmet | Warrior | DEF 28, Fire Resist 15 | `DamageReduce15` | Dragonfire |
| 37 | Drakefire Cowl | Helmet | Mage | DEF 18, Fire Resist 12 | `SpellDmgBoost20` | Dragonfire |
| 38 | Scales of the Eternal | Chest | Warrior | DEF 35, Fire Resist 10, Ice Resist 5 | `Thorns40` | Dragonfire |
| 39 | Mantle of Living Flame | Chest | Mage/Priest | DEF 20, Fire Resist 14 | `SpellDmgLowHp30` | — |
| 40 | Gauntlets of the Brood | Gloves | Warrior | DEF 14, Fire Resist 8 | `PhysDmgBoost20` | — |
| 41 | Dragonclaw Boots | Boots | Any | DEF 12, Fire Resist 6 | `Dodge20` | — |
| 42 | Ring of the Broodmother | Ring | Any | HP 70, Mana 35, Fire:20 | `AllResist10` | — |
| 43 | Heart of the Dragon | Amulet | Any | HP 50, Mana 60, Fire:35 | `Lifesteal25` | — |

---

### Boss 5: Aran Warlord (Area 5 — The Citadel)

*Theme: War, steel, battle-honored relics.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 44 | Warlord's Decree | Weapon (Staff) | Mage | DMG 150, Mana 80, Lightning:40 | `LightningStun15` | — |
| 45 | Blade of the Aran | Weapon (Sword) | Warrior | DMG 170, Mana 5 | `PhysDmgBoost20` | Warlord's |
| 46 | Warbringer's Mace | Weapon (Mace) | Priest | DMG 130, Mana 60, Holy:35 | `StunImmune` | Warlord's |
| 47 | Siegebow of Aran | Weapon (Bow) | Archer | DMG 155, Mana 20 | `AtkSpeed20` | — |
| 48 | Warlord's Helm | Helmet | Warrior | DEF 30, Lightning Resist 10 | `TauntChance25` | Warlord's |
| 49 | Mantle of the Battle-Mage | Chest | Mage | DEF 22, Lightning Resist 12 | `ManaCostReduce15` | — |
| 50 | Battle-Hardened Plate | Chest | Warrior | DEF 40, Lightning Resist 8 | `DamageReduce15` | Warlord's |
| 51 | Bracers of Command | Gloves | Any | DEF 13, Lightning Resist 6 | `CritChance25` | — |
| 52 | Greaves of Conquest | Pants | Any | DEF 15 | `PhysDmgBoost15` | — |
| 53 | Warboots of the Vanguard | Boots | Any | DEF 11 | `HealOnKill15` | — |
| 54 | Signet of the Warlord | Ring | Any | HP 80, Mana 30 | `AllResist10` | — |
| 55 | Amulet of Broken Chains | Amulet | Any | HP 60, Mana 40 | `StunImmune` | — |

---

### Boss 6: Void Lord (Area 6 — Abyssal Depths)

*Theme: Void, shadow, anti-magic, entropy.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 56 | Wand of the Void | Weapon (Wand) | Mage | DMG 160, Mana 120, Arcane:60 | `ManaCostReduce10` + `ManaRegen5` | Voidborn |
| 57 | Null-Staff | Weapon (Staff) | Mage | DMG 145, Mana 100, Arcane:55 | `ArcaneCrit30` | Voidborn |
| 58 | Soulreaper's Scythe | Weapon (Axe) | Warrior | DMG 180, Mana 15, Arcane:30 | `Lifesteal25` | Voidborn |
| 59 | Voidchaser Bow | Weapon (Bow) | Archer | DMG 165, Mana 25, Arcane:25 | `CritChance30` | — |
| 60 | Void-Touched Hood | Helmet | Mage | DEF 20, Arcane Resist 18 | `ManaShield30` | Voidborn |
| 61 | Helm of the Null Knight | Helmet | Warrior | DEF 32, Arcane Resist 14 | `DamageReduce15` | Voidborn |
| 62 | Void Shroud | Chest | Mage/Priest | DEF 24, Arcane Resist 16 | `SpellDmgBoost20` | Voidborn |
| 63 | Voidforged Plate | Chest | Warrior | DEF 42, Arcane Resist 10 | `Thorns40` | — |
| 64 | Null-Grip Gauntlets | Gloves | Any | DEF 14, Arcane Resist 8 | `Dodge20` | — |
| 65 | Shadowthread Leggings | Pants | Any | DEF 16, Arcane Resist 6 | `CooldownReduce1` | — |
| 66 | Voidstep Treads | Boots | Any | DEF 13, Arcane Resist 10 | `FreezeImmune` | — |
| 67 | Band of the Void Lord | Ring | Any | HP 90, Mana 50, Arcane:25 | `AllResist10` | — |
| 68 | Pendant of Entropy | Amulet | Any | HP 70, Mana 70, Arcane:30 | `CooldownReduce2` | — |

---

### Boss 7: Seraphim Council (Area 7 — Heavenly Ascent)

*Theme: Light, holy, divine judgment, angelic power.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 69 | Staff of Radiant Truth | Weapon (Staff) | Mage | DMG 175, Mana 110, Holy:55 | `DoubleCast15` + `HolyHealOnKill` | Seraphic |
| 70 | Scepter of Judgment | Weapon (Scepter) | Priest | DMG 150, Mana 100, Holy:60 | `HolyHealOnKill` | Seraphic |
| 71 | Blade of Dawn | Weapon (Sword) | Warrior | DMG 190, Mana 15, Holy:35 | `PhysDmgBoost20` + `HealOnKill10` | Seraphic |
| 72 | Celestial Longbow | Weapon (Bow) | Archer | DMG 170, Mana 30, Holy:30 | `CritChance30` | — |
| 73 | Halo of Ascension | Helmet | Mage/Priest | DEF 22, Holy Resist 16 | `SpellDmgBoost20` | Seraphic |
| 74 | Helm of the Fallen Seraph | Helmet | Warrior | DEF 34, Holy Resist 12 | `DamageReduce15` + `TauntChance25` | Seraphic |
| 75 | Raiment of the Celestial Choir | Chest | Priest | DEF 26, Holy Resist 18 | `HealOnKill15` | Seraphic |
| 76 | Mantle of Divine Light | Chest | Mage | DEF 24, Holy Resist 14 | `ManaCostReduce15` | — |
| 77 | Seraphim's Grace Gloves | Gloves | Any | DEF 15, Holy Resist 10 | `Lifesteal15` | — |
| 78 | Sandals of the Ascended | Boots | Any | DEF 14, Holy Resist 8 | `Revive50` | — |
| 79 | Radiant Band | Ring | Any | HP 80, Mana 60, Holy:20 | `AllResist10` | — |
| 80 | Amulet of the Divine Eye | Amulet | Any | HP 65, Mana 80, Holy:30 | `ManaRegen10` | — |

---

### Boss 8: The Primordial One (Area 8 — The Endless Void)

*Theme: Ultimate power, all elements, primordial chaos.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 81 | Archmage Staff *(existing)* | Weapon (Staff) | Mage | DMG 200, Mana 100, Arcane:50 | — | Primordial |
| 82 | Titan's Greatsword *(existing)* | Weapon (Sword) | Warrior | DMG 250, Mana 20, Physical:30 | — | Primordial |
| 83 | Divine Scepter *(existing)* | Weapon (Scepter) | Priest | DMG 180, Mana 80, Holy:40 | — | Primordial |
| 84 | Shadowstrike Bow *(existing)* | Weapon (Bow) | Archer | DMG 220, Mana 30, Poison:35 | — | Primordial |
| 85 | Gilded Blade *(existing)* | Weapon (Sword) | Merchant | DMG 200, Mana 40, Fire:25 | — | Primordial |
| 86 | Archmage Orb *(existing)* | Offhand (Orb) | Mage | DMG 80, Mana 200, Arcane:50 | `ManaRegen10` | Primordial |
| 87 | Archmage Robes *(existing)* | Chest | Mage | DEF 35, Arcane Resist 8 | — | Primordial |
| 88 | Crown of the Primordial | Helmet | Any | DEF 30, All Resist 10 | `AllResist10` | Primordial |
| 89 | Primordial Plate | Chest | Warrior | DEF 50, All Resist 8 | `DamageReduce15` | — |
| 90 | Gloves of Creation | Gloves | Any | DEF 18, All Resist 6 | `Lifesteal25` | — |
| 91 | Boots of the First Age | Boots | Any | DEF 16, All Resist 5 | `Dodge20` | — |
| 92 | Ring of the Primordial | Ring | Any | HP 100, Mana 80, All Elem:20 | `AllResist10` | Primordial |
| 93 | Amulet of the End | Amulet | Any | HP 80, Mana 100, All Elem:25 | `PhoenixRevive` | — |

---

### Boss 9: Arcane Construct (Area 9 — Sanctum of Secrets)

*Theme: Pure arcane, magical constructs, forbidden knowledge.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 94 | Tome of Infinite Spells | Offhand (Book) | Mage | DMG 60, Mana 150, Arcane:45 | `DoubleCast25` | Arcanum |
| 95 | Construct's Core Staff | Weapon (Staff) | Mage | DMG 190, Mana 130, Arcane:65 | `ArcaneCrit30` + `ManaRegen10` | Arcanum |
| 96 | Arcane Shard Dagger | Weapon (Dagger) | Archer | DMG 170, Mana 20, Arcane:40 | `CritChance30` | — |
| 97 | Manaheart Crown | Helmet | Mage | DEF 24, Arcane Resist 20 | `ManaCostReduce15` + `ManaRegen5` | Arcanum |
| 98 | Robes of the Infinite Weave | Chest | Mage | DEF 28, Arcane Resist 18 | `DoubleCast15` + `SpellDmgBoost15` | Arcanum |
| 99 | Construct's plated Carapace | Chest | Warrior | DEF 45, Arcane Resist 12 | `Thorns40` + `DamageReduce10` | — |
| 100 | Spell-Etched Gloves | Gloves | Any | DEF 16, Arcane Resist 10 | `CooldownReduce1` | Arcanum |
| 101 | Leggings of the Arcane Path | Pants | Any | DEF 18, Arcane Resist 8 | `ManaShield30` | — |
| 102 | boots of Mana Conduit | Boots | Any | DEF 15, Arcane Resist 12 | `ManaOnKill25` | Arcanum |
| 103 | Band of the Construct | Ring | Any | HP 90, Mana 100, Arcane:30 | `AllResist10` | — |
| 104 | Pendant of the Forgotten Lore | Amulet | Any | HP 70, Mana 120, Arcane:40 | `CooldownReduce2` + `ManaRegen5` | — |

---

### Boss 10: Chronos, the Time Ender (Area 10 — Chronos Depths)

*Theme: Time, reality, cosmic, the ultimate challenge.*

| # | Name | Slot | Class | Stats | Passive | Set |
|---|------|------|-------|-------|---------|-----|
| 105 | Staff of Collapsed Timelines | Weapon (Staff) | Mage | DMG 220, Mana 150, Arcane:75 | `CooldownReduce2` + `DoubleCast15` | Chrono |
| 106 | Chronoweaver Wand | Weapon (Wand) | Mage | DMG 200, Mana 160, Ice:60 | `IceFreezePlus1` + `ManaCostReduce15` | Chrono |
| 107 | Temporal Greatsword | Weapon (Sword) | Warrior | DMG 260, Mana 20, Physical:40 | `CritChance30` + `PhysDmgBoost20` | — |
| 108 | Epoch Bow | Weapon (Bow) | Archer | DMG 230, Mana 35, Lightning:45 | `AtkSpeed20` + `CritChance30` | — |
| 109 | Chronolord's Helm | Helmet | Any | DEF 35, All Resist 15 | `AllResist10` + `DamageReduce10` | Chrono |
| 110 | Robes of the Timeless Weave | Chest | Mage | DEF 30, Arcane Resist 20 | `SpellDmgBoost20` + `ManaRegen10` | Chrono |
| 111 | Temporal Plate of the Infinite | Chest | Warrior | DEF 55, All Resist 10 | `DamageReduce15` + `Thorns40` | — |
| 112 | Gauntlets of Frozen Time | Gloves | Any | DEF 20, All Resist 8 | `StatusExtend2` | Chrono |
| 113 | Leggings of the Void Walk | Pants | Any | DEF 22, Arcane Resist 12 | `Dodge20` | — |
| 114 | Sandals of Dimensional Rift | Boots | Any | DEF 18, All Resist 10 | `Dodge15` + `FreezeImmune` | — |
| 115 | Loop of Eternity | Ring | Any | HP 120, Mana 100, All Elem:25 | `AllResist10` + `ManaRegen5` | Chrono |
| 116 | Pendant of the Time Ender | Amulet | Any | HP 100, Mana 130, Arcane:50 | `Revive50` + `CooldownReduce1` | — |

---

**Grand Total: 116 Legendary Items**

| Boss | Drops | Mage Items | Warrior Items | Priest Items | Archer Items | Merchant Items |
|------|-------|-----------|--------------|-------------|-------------|---------------|
| Farm Overseer | 9 | 1 | 2 | 0 | 1 | 0 |
| Treant King | 11 | 2 | 2 | 0 | 2 | 0 |
| Kraken | 11 | 2 | 2 | 1 | 1 | 0 |
| Elder Dragon | 12 | 3 | 3 | 1 | 1 | 0 |
| Aran Warlord | 12 | 2 | 4 | 1 | 1 | 0 |
| Void Lord | 13 | 3 | 3 | 1 | 1 | 0 |
| Seraphim Council | 12 | 3 | 2 | 2 | 1 | 0 |
| Primordial One | 13 | 3 | 2 | 1 | 1 | 1 |
| Arcane Construct | 11 | 4 | 1 | 0 | 1 | 0 |
| Chronos | 12 | 3 | 2 | 0 | 1 | 0 |
| **Total** | **116** | **26** | **23** | **7** | **11** | **1** |

> Note: Merchant and other class legendaries will be expanded when those classes are implemented. Current count skews toward Mage/Warrior as they're the most developed classes.

---

## 6. Passive Effect System

**File:** `Items/Item.hpp` — Add to Item base class

```cpp
enum class ItemPassive : int
{
    None = 0,
    // Weapon passives
    FireNoCd20,             // 20% chance fire spell doesn't trigger cooldown
    IceFreezePlus1,         // Ice spells freeze +1 turn
    LightningStun15,        // Lightning spells 15% chance to stun 1 turn
    ArcaneCrit30,           // Arcane spells 30% chance for double damage
    PoisonDmgPlus25,        // Poison effects deal +25% damage
    PhysDmgBoost15,         // +15% physical damage
    PhysDmgBoost20,         // +20% physical damage
    AtkSpeed20,             // +20% attack speed
    CritChance20,           // 20% chance for critical hit (1.5x)
    CritChance30,           // 30% chance for critical hit (1.5x)
    Lifesteal15,            // Heal for 15% of damage dealt
    Lifesteal25,            // Heal for 25% of damage dealt
    SpellDmgBoost15,        // +15% spell damage
    SpellDmgBoost20,        // +20% spell damage
    // Armor passives
    Dodge15,                // 15% chance to dodge attacks
    Dodge20,                // 20% chance to dodge attacks
    DamageReduce10,         // Take 10% less damage from all sources
    DamageReduce15,         // Take 15% less damage from all sources
    Thorns25,               // Reflect 25% of damage taken
    Thorns40,               // Reflect 40% of damage taken
    HealOnKill10,           // Kill restores 10% max HP
    HealOnKill15,           // Kill restores 15% max HP
    StunImmune,             // Immune to stun
    FreezeImmune,           // Immune to freeze
    BurnImmune,             // Immune to burn
    PoisonImmune,           // Immune to poison
    TauntChance25,          // 25% chance to force single-target attacks on self
    StatusExtend1,          // Status effects you apply last +1 turn
    StatusExtend2,          // Status effects you apply last +2 turns
    // Accessory / offhand passives
    PhoenixRevive,          // Revive once per battle at 30% HP
    Revive50,               // Revive once per battle at 50% HP
    ManaRegen5,             // +5 mana restored per turn
    ManaRegen10,            // +10 mana restored per turn
    ManaCostReduce10,       // All spells cost 10% less mana
    ManaCostReduce15,       // All spells cost 15% less mana
    ManaShield30,           // 30% of damage taken absorbed by mana
    ManaOnKill15,           // Restore 15 mana on kill
    ManaOnKill25,           // Restore 25 mana on kill
    ManaOnSkillUse10,       // Restore 10 mana each time you use a skill
    HpOnSkillUse10,         // Heal 10 HP each time you use a skill
    DoubleCast15,           // 15% chance spells hit twice (2nd at 50% dmg)
    DoubleCast25,           // 25% chance spells hit twice (2nd at 50% dmg)
    CooldownReduce1,        // All cooldowns reduced by 1 turn
    CooldownReduce2,        // All cooldowns reduced by 2 turns
    HolyHealOnKill,         // Killing an enemy heals 10% max HP (holy themed)
    SpellDmgLowHp30,        // +30% spell damage when HP < 50%
    PhysDmgLowHp30,         // +30% physical damage when HP < 50%
    // Utility passives
    AllResist5,             // +5 resistance to all elements
    AllResist10,            // +10 resistance to all elements
    ExpBoost20,             // +20% experience from kills
    GoldFind30,             // +30% gold from kills
};

struct PassiveEffect
{
    ItemPassive type = ItemPassive::None;
    int value = 0;  // secondary value if needed
};
```

**Each item can have up to 2 passive effects** (stored as `PassiveEffect passive1` and `PassiveEffect passive2` on the `Item` class). Most items have 0-1; top-tier legendaries have 2.

**Integration points:**
- `Item` gets `PassiveEffect passive1` and `PassiveEffect passive2` fields (default `None`)
- `Equipment::GetPassiveEffects()` — collects all active passives from equipped gear
- Combat system checks passives at relevant moments (skill use, turn start, damage taken)

**Files to modify:**
- `Items/Item.hpp` — add PassiveEffect enum + fields
- `Items/Equipment/Equipment.hpp` — add GetPassiveEffects()
- `Engine/Combat.cpp` — check passives during combat

---

## 7. Set System

### Set Definitions

**New file:** `Items/Sets/SetBonuses.hpp` and `SetBonuses.cpp`

```cpp
struct SetBonus
{
    int piecesRequired;
    std::string description;
    enum BonusType { SpellDamagePercent, ManaCostReduction, ElementalDamage,
                     ElementalResist, ManaRegen, CooldownReduction, PhysicalDamage,
                     CritChance, DodgeChance, HealOnKill, Thorns, DamageReduction };
    BonusType type;
    int value;  // percentage or flat
};

struct ItemSet
{
    int setId;
    std::string setName;
    std::vector<std::string> pieceNames;  // names of all items in this set
    std::vector<SetBonus> bonuses;        // sorted by piecesRequired ascending
};
```

### All Sets (11 total)

#### MAGE SETS

##### Set 1: Archmage's Regalia (Epic, 4 pieces — Mage)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Archmage's Circlet | Helmet | DEF 14+diff*2, Arcane Resist 5+diff |
| Archmage's Mantle | Chest | DEF 18+diff*2, Arcane Resist 8+diff |
| Archmage's Handwraps | Gloves | DEF 11+diff*2, Arcane Resist 4+diff |
| Archmage's Focus | Offhand (Orb) | DMG 8+diff, Mana 30+diff*3, Arcane 5+diff |

**Set Bonuses:**
- **2 pieces:** +10% spell damage
- **3 pieces:** Spells cost 15% less mana
- **4 pieces (full):** +20% spell damage, +10 mana regen/turn

**Drop source:** Arcane Sanctum boss (Arcane Construct)

##### Set 2: Frostweaver's Raiment (Legendary, 3 pieces — Mage)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Frostweaver's Crown | Helmet | DEF 25, Ice Resist 10 |
| Frostweaver's Robe | Chest | DEF 30, Ice Resist 12 |
| Frostweaver's Focus | Accessory (Amulet) | HP 60, Mana 180, Ice Elem DMG 45 |

**Set Bonuses:**
- **2 pieces:** Ice spells deal +25% damage
- **3 pieces (full):** Ice spells have 30% chance to freeze ALL enemies for 2 turns

**Drop source:** Chronos Depths boss (Chronos, the Time Ender)

##### Set 3: Voidborn Shroud (Legendary, 4 pieces — Mage)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Void-Touched Hood | Helmet | DEF 20, Arcane Resist 18 |
| Void Shroud | Chest | DEF 24, Arcane Resist 16 |
| Null-Grip Gauntlets | Gloves | DEF 14, Arcane Resist 8 |
| Voidstep Treads | Boots | DEF 13, Arcane Resist 10 |

**Set Bonuses:**
- **2 pieces:** +15% spell damage
- **3 pieces:** Spells cost 10% less mana, +10 mana regen/turn
- **4 pieces (full):** 15% chance to cast spells twice, +15 Arcane Resist

**Drop source:** Void Lord boss (Abyssal Depths)

#### WARRIOR SETS

##### Set 4: Warlord's Battlegear (Legendary, 5 pieces — Warrior)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Warlord's Helm | Helmet | DEF 30, Lightning Resist 10 |
| Battle-Hardened Plate | Chest | DEF 40, Lightning Resist 8 |
| Bracers of Command | Gloves | DEF 13, Lightning Resist 6 |
| Greaves of Conquest | Pants | DEF 15 |
| Warboots of the Vanguard | Boots | DEF 11 |

**Set Bonuses:**
- **2 pieces:** +15% physical damage
- **3 pieces:** +10% critical hit chance
- **4 pieces:** Take 10% less damage from all sources
- **5 pieces (full):** +25% physical damage, killing an enemy restores 10% HP

**Drop source:** Aran Warlord boss (The Citadel)

##### Set 5: Dragonfire Plate (Legendary, 4 pieces — Warrior)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Dragonhide Helm | Helmet | DEF 28, Fire Resist 15 |
| Scales of the Eternal | Chest | DEF 35, Fire Resist 10, Ice Resist 5 |
| Gauntlets of the Brood | Gloves | DEF 14, Fire Resist 8 |
| Dragonclaw Boots | Boots | DEF 12, Fire Resist 6 |

**Set Bonuses:**
- **2 pieces:** +15% physical damage
- **3 pieces:** Reflect 25% of damage taken
- **4 pieces (full):** Immune to burn, +10 Fire Resist, +10% critical hit chance

**Drop source:** Elder Dragon boss (The Summit)

##### Set 6: Primordial Vanguard (Legendary, 3 pieces — Warrior)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Crown of the Primordial | Helmet | DEF 30, All Resist 10 |
| Primordial Plate | Chest | DEF 50, All Resist 8 |
| Gloves of Creation | Gloves | DEF 18, All Resist 6 |

**Set Bonuses:**
- **2 pieces:** +15% physical damage, +10 all resist
- **3 pieces (full):** +25% physical damage, take 10% less damage, reflect 25% of damage

**Drop source:** Primordial One boss (The Endless Void)

#### PRIEST SETS

##### Set 7: Seraphic Vestments (Legendary, 4 pieces — Priest)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Halo of Ascension | Helmet | DEF 22, Holy Resist 16 |
| Raiment of the Celestial Choir | Chest | DEF 26, Holy Resist 18 |
| Seraphim's Grace Gloves | Gloves | DEF 15, Holy Resist 10 |
| Sandals of the Ascended | Boots | DEF 14, Holy Resist 8 |

**Set Bonuses:**
- **2 pieces:** +15% healing power
- **3 pieces:** Killing an enemy heals 10% max HP
- **4 pieces (full):** Revive once per battle with 50% HP, +15 Holy Resist

**Drop source:** Seraphim Council boss (Heavenly Ascent)

#### ARCHER SETS

##### Set 8: Abyssal Marksman (Legendary, 3 pieces — Archer)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Abyssal Hood | Helmet | DEF 18, Ice Resist 10 |
| Tentacle-Woven Bracers | Gloves | DEF 11, Lightning Resist 8 |
| Coral Stalkers | Boots | DEF 10, Ice Resist 8 |

**Set Bonuses:**
- **2 pieces:** +20% critical hit chance
- **3 pieces (full):** +15% physical damage, 15% chance to dodge

**Drop source:** Kraken boss (Sunken Grotto)

#### MERCHANT SETS

##### Set 9: Gilded Fortune (Legendary, 3 pieces — Merchant)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Coin-Lord's Crown | Helmet | DEF 20, All Resist 5 |
| Merchant Prince's Vest | Chest | DEF 30, All Resist 8 |
| Gilded Signet | Ring | HP 80, Mana 40, All Elem:10 |

**Set Bonuses:**
- **2 pieces:** +50% gold from kills
- **3 pieces (full):** +100% gold from kills, +20% experience from kills

**Drop source:** Farm Overseer boss (Abandoned Farm) + crafting

#### CROSS-CLASS SETS

##### Set 10: Elementalist's Vestments (Epic, 4 pieces — Any class)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Elementalist's Circlet | Helmet | DEF 12+diff*2, Fire Resist 4+diff |
| Elementalist's Mantle | Chest | DEF 16+diff*2, Ice Resist 4+diff |
| Elementalist's Leggings | Pants | DEF 13+diff*2, Lightning Resist 4+diff |
| Elementalist's Sandals | Boots | DEF 10+diff*2, Fire Resist 3+diff |

**Set Bonuses:**
- **2 pieces:** +8% elemental damage
- **3 pieces:** +15 elemental resistance (all elements)
- **4 pieces (full):** Spells have 20% chance to apply a random status effect

**Drop source:** Celestial Spire boss (Seraphim Council) — each piece from a different elite enemy in the area

##### Set 11: Chrono Warden (Legendary, 5 pieces — Any class)
| Piece | Slot | Base Stats |
|-------|------|-----------|
| Chronolord's Helm | Helmet | DEF 35, All Resist 15 |
| Gauntlets of Frozen Time | Gloves | DEF 20, All Resist 8 |
| Sandals of Dimensional Rift | Boots | DEF 18, All Resist 10 |
| Loop of Eternity | Ring | HP 120, Mana 100, All Elem:25 |
| Pendant of the Time Ender | Amulet | HP 100, Mana 130, Arcane:50 |

**Set Bonuses:**
- **2 pieces:** +10 all resist
- **3 pieces:** All cooldowns reduced by 1 turn
- **4 pieces:** 15% chance to dodge any attack, +15% damage
- **5 pieces (full):** +20 all resist, revive once per battle with 50% HP, +20% all damage

**Drop source:** Chronos boss (Chronos Depths) — the ultimate set

### Set Bonus Application

**File:** `Items/Equipment/Equipment.hpp`

```cpp
class Equipment
{
public:
    // Existing methods...
    std::vector<std::pair<int, int>> GetActiveSetBonuses() const;
    // Returns vector of (setId, piecesEquipped) for sets with active bonuses

    int GetSetBonusSpellDamage() const;
    int GetSetBonusManaCostReduction() const;
    int GetSetBonusElementalDamage() const;
    int GetSetBonusElementalResist() const;
    int GetSetBonusPhysicalDamage() const;
    int GetSetBonusCritChance() const;
    int GetSetBonusDodgeChance() const;
    int GetSetBonusHealOnKill() const;
    int GetSetBonusDamageReduction() const;
    int GetSetBonusCooldownReduction() const;
};
```

**Combat integration:**
- `GetSetBonusSpellDamage()` applied as a multiplier in skill damage calculation
- `GetSetBonusManaCostReduction()` applied before mana deduction
- `GetSetBonusElementalDamage()` added to `GetElementalBonus()`
- `GetSetBonusPhysicalDamage()` applied as multiplier in physical attack calculation
- `GetSetBonusCritChance()` added to critical hit chance roll
- `GetSetBonusDodgeChance()` added to dodge chance roll

---

## 8. Mage Crafting Expansion

**File:** `Items/Crafting/Crafting.cpp`

### New Recipes

#### Tier 3 (Job Lv 5-6) — Cloth Armor
| Recipe | Resources | Result |
|--------|-----------|--------|
| Reinforced Cloth Robe | Silver Ingot x2, Maple Wood x2 | Cloth Chest, DEF 10, Rarity 2 |
| Arcane Circlet | Silver Ingot x1, Maple Wood x1, Salmon x1 | Cloth Helmet, DEF 7, Rarity 2 |
| Mana Focus Orb | Silver Ingot x1, Magical Wood x1 | Offhand (Orb), Mana 20, Arcane DMG 5, Rarity 2 |

#### Tier 4 (Job Lv 7-8) — Cloth + Staff/Wand + Offhand
| Recipe | Resources | Result |
|--------|-----------|--------|
| Enchanted Cloth Armor | Gold Ingot x2, Ebony Wood x2 | Cloth Chest, DEF 16, Rarity 3 |
| Ebony Staff | Ebony Wood x2, Gold Ingot x1 | Staff, DMG 20, Mana 35, Rarity 3 |
| Arcane Tome | Ebony Wood x1, Gold Ingot x1, Arcane Crystal x1 | Offhand (Book), Mana 40, Arcane DMG 10, Rarity 3 |

#### Tier 5 (Job Lv 9-10) — High Cloth + Wand + Offhand
| Recipe | Resources | Result |
|--------|-----------|--------|
| Mythril Woven Robes | Mythril Ingot x2, Magical Wood x2 | Cloth Chest, DEF 24, Rarity 4 |
| Mythril Focus | Mythril Ingot x1, Magical Wood x2 | Wand, DMG 28, Mana 55, Rarity 4 |
| Void Shard Focus | Mythril Ingot x2, Arcane Crystal x2 | Offhand (Orb), Mana 80, Arcane DMG 18, Rarity 4 |

#### New Offhand Recipes (all classes)
| Recipe | Resources | Result |
|--------|-----------|--------|
| Wooden Buckler | Maple Wood x3 | Offhand (Shield), DEF 8, Rarity 1 |
| Iron Buckler | Iron Ingot x2, Maple Wood x1 | Offhand (Shield), DEF 15, Rarity 2 |
| Reinforced Shield | Gold Ingot x2, Iron Ingot x1 | Offhand (Shield), DEF 22, Rarity 3 |
| Mythril Aegis | Mythril Ingot x2, Gold Ingot x1 | Offhand (Shield), DEF 30, Rarity 4 |
| Leather Satchel | Leather x3 | Offhand (Bag), HP 20, Rarity 1 |
| Reinforced Satchel | Leather x2, Iron Ingot x1 | Offhand (Bag), HP 40, Rarity 2 |
| Woven Tome | Maple Wood x2, Magical Wood x1 | Offhand (Book), Mana 25, Arcane DMG 3, Rarity 1 |

### Set Item Crafting (using rare materials)
| Recipe | Resources | Result |
|--------|-----------|--------|
| Archmage's Circlet | Arcane Crystal x3, Mythril Ingot x2, Silver Ingot x1 | Set Piece, Cloth Helmet, Rarity 4 |
| Archmage's Mantle | Arcane Crystal x3, Mythril Ingot x3 | Set Piece, Cloth Chest, Rarity 4 |
| Archmage's Handwraps | Arcane Crystal x2, Mythril Ingot x1, Magical Wood x1 | Set Piece, Cloth Gloves, Rarity 4 |
| Archmage's Focus | Arcane Crystal x4, Mythril Ingot x2 | Set Piece, Offhand (Orb), Rarity 4 |

**New resource:** `Arcane Crystal` — drops from Arcane Sanctum enemies (tier 9)

---

## 9. Loot Table Integration

### Unique Drop Logic

**File:** `Items/Loot.cpp`

```cpp
shared_ptr<Item> LootTable::GenerateUniqueDrop(int dropTier, CharacterClass charClass)
{
    // 1. Get all uniques whose dropSource matches enemies in this tier range
    // 2. Filter by class-appropriate equipment
    // 3. Weighted random (rarer items less likely)
    // 4. Return nullptr if no unique rolls
}
```

### Enemy → Unique Mapping

| Enemy | Area | Unique Drops |
|-------|------|-------------|
| Giant Spider | Dark Woods | Staff of Frozen Memories |
| Wraith | Dark Woods | Circlet of Withered Dreams |
| Dark Elf Scout | Dark Woods | Gloves of Arcane Weaving |
| Sea Serpent | Coast | Stormweaver's Focus |
| Siren | Coast | Pendant of the Tidal Sage |
| Orc | Dragon's Peak | Ring of the Ember Court |
| Fire Elemental | Dragon's Peak | Wand of the Blazing Sun |
| Drake | Dragon's Peak | Mantle of the Flamecaller |
| Highland Warrior | Highlands | Leggings of the Storm |
| Shadow | Shadow Dungeon | Sandals of the Void |
| Lich | Shadow Dungeon | Tome of Whispered Truths |
| Void Knight | Shadow Dungeon | Void-Touched Scepter |
| Arcane Construct (boss) | Arcane Sanctum | Archmage's Regalia set (4 pieces) |
| Void Lord (boss) | Abyssal Depths | Voidborn Shroud set (4 pieces) |
| Seraphim Council (boss) | Celestial Spire | Elementalist's Vestments (4) + Seraphic Vestments (4) |
| Chronos (boss) | Chronos Depths | Frostweaver's Raiment (3) + Chrono Warden (5) + Legendaries |
| Primordial One (boss) | The Endless Void | Primordial Vanguard (3) + Legendaries |
| Aran Warlord (boss) | The Citadel | Warlord's Battlegear (5) + Legendaries |
| Elder Dragon (boss) | The Summit | Dragonfire Plate (4) + Legendaries |
| Kraken (boss) | Sunken Grotto | Abyssal Marksman (3) + Legendaries |
| Farm Overseer (boss) | Abandoned Farm | Gilded Fortune (3) + Legendaries |
| Treant King (boss) | Twilight Hollow | Legendaries |

### Set Drop Rules
- **Epic sets** (Archmage's Regalia, Elementalist's Vestments): Drop from area 7-8 bosses
- **Legendary sets**: Drop from area 8-10 bosses only
- Each boss drops 1-2 pieces of a specific set
- Duplicate protection: if player already has all pieces, drops regular Epic instead
- **Cross-class sets** (Chrono Warden) drop from the final boss only

---

## 10. UI Updates

### Inventory Screen (`Engine/Game.cpp: StateInventory`)
- Item names colored by rarity using `RarityColor()`
- Set items show `[SetName]` tag after the name
- Set items with active bonuses show a small set icon

### Equipment Panel (`Engine/Game.cpp: StateInventory`)
- Slot labels colored by rarity
- Set bonus indicator: "Set: Archmage's Regalia (2/3)" shown below equipped items
- Active set bonuses listed with green text

### Item Comparison Panel
- Show rarity color in comparison text
- Show set info if item belongs to a set
- Show passive effect description for Epics/Legendaries

### Wiki/Codex (`Engine/Wiki.cpp`)
- Add "Uniques" tab or sub-tab under Equipment
- List all 116 legendary items with drop sources, slot, class, and passive effects
- List all 13 epic items with drop sources
- Show set pieces grouped by set with bonus descriptions (11 sets)
- Filter by: class, slot, boss/area, set membership

### Loot Notification (`Engine/Game.cpp: StateCombat`)
- When Epic/Legendary drops, show special colored text
- "EPIC DROP: Staff of Frozen Memories!" in purple
- "LEGENDARY DROP: Staff of the Eternal Flame!" in orange

---

## 11. Balance Targets

### Mage Stat Progression by Area

| Area | Level Range | Expected INT | Common Wep DMG | Uncommon Wep DMG | Rare Wep DMG | Epic Wep DMG | Legendary Wep DMG | Common DEF | Rare DEF |
|------|-------------|-------------|----------------|-----------------|-------------|-------------|-------------------|-----------|---------|
| Felrona | 1-5 | 14-34 | 15 | 20 | 28 | — | 55 | 3 | 7 |
| Dark Woods | 5-10 | 34-54 | 35 | 44 | 60 | 65 | 80-100 | 7 | 15 |
| Coast | 10-15 | 54-74 | 60 | 72 | 100 | 110 | 140-175 | 12 | 25 |
| Dragon's Peak | 15-20 | 74-94 | 85 | 100 | 140 | 155 | 190-220 | 17 | 35 |
| Highlands | 20-25 | 94-114 | 110 | 130 | 180 | 200 | 250-260 | 22 | 45 |
| Shadow Dungeon | 25-30 | 114-134 | 135 | 160 | 220 | 250 | 260+ | 27 | 55 |

### Offhand Progression

| Area | Common Shield DEF | Rare Shield DEF | Common Orb Mana | Rare Orb Mana | Common Book Mana | Rare Book Mana |
|------|-------------------|-----------------|-----------------|---------------|------------------|----------------|
| Felrona | 5 | 9 | 15 | 30 | 12 | 25 |
| Dark Woods | 10 | 18 | 30 | 55 | 25 | 45 |
| Coast | 15 | 28 | 50 | 80 | 40 | 65 |
| Dragon's Peak | 20 | 38 | 70 | 110 | 55 | 90 |
| Highlands | 25 | 48 | 90 | 140 | 70 | 115 |
| Shadow Dungeon | 30 | 58 | 110 | 170 | 85 | 140 |

### Damage Expectations (Mage at Level 15, using Fireball INT*2)

| Gear Quality | INT | Weapon DMG | Offhand Mana | Elemental DMG | Fireball Hit | % of Enemy HP |
|-------------|-----|-----------|--------------|---------------|-------------|---------------|
| All Common | 74 | 60 | 0 | 0 | 148 | 22-27% |
| All Uncommon | 74 | 72 | +10 | 5 | 161 | 24-29% |
| All Rare | 74 | 100 | +25 | 15 | 193 | 29-34% |
| 1 Epic weapon | 74 | 110 | +25 | 22 | 205 | 31-37% |
| 1 Legendary weapon + orb | 74 | 140 | +80 | 35 | 243 | 36-43% |
| Full Legendary (w/ sets) | 74 | 140 | +80 | 65 | 273 | 41-49% |

**Target:** A well-geared player should kill normal enemies in 2-3 skill uses, bosses in 8-12 skill uses. Legendaries should feel powerful but not trivialize content.

---

## 12. Implementation Order

### Phase 1: Foundation
1. **Rarity enum + helpers** (`Item.hpp`, `Item.cpp`)
2. **Offhand equipment slot** (`Equipment.hpp` — add `offhand` slot, update `CanEquip()`, `Equip()`, `Unequip()`)
3. **PassiveEffect enum + system** (`Item.hpp`, combat integration)
4. **UniqueItem registry** (`Uniques/UniqueItems.hpp`, `UniqueItems.cpp`)

### Phase 2: Content
5. **13 Mage unique Epics** (in registry)
6. **116 Legendary items** (all bosses, all classes — in registry)
7. **Set system** (`Sets/SetBonuses.hpp`, `SetBonuses.cpp` — 11 sets)
8. **Set definitions** (all 11 sets in registry)

### Phase 3: Integration
9. **Loot table update** (`Loot.cpp` — drop tier mapping + unique drop function)
10. **Equipment class update** (`Equipment.hpp` — set bonus calculations)
11. **Crafting expansion** (`Crafting.cpp` — new recipes + offhand items)
12. **Enemy `dropTier` assignment** (all enemies in all areas)

### Phase 4: UI
13. **UI rarity colors** (inventory, equipment, shop, wiki)
14. **UI set display** (equipment panel — set name + pieces equipped + active bonuses)
15. **UI offhand display** (equipment panel — new slot rendering)
16. **UI loot notifications** (combat victory screen — "LEGENDARY DROP!" text)
17. **Wiki uniques tab** (all unique items + drop sources + set pieces)

### Phase 5: Balance
18. **Balance testing** (play through areas 1-6, verify feel)
19. **Stat tuning** (adjust drop rates, passive proc chances, set bonus values)
