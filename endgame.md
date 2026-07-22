Implementation Plan: Endgame Systems
Feature 1: Endgame Unlock [DONE]
What exists: Nothing. No chronosDefeated flag, no endgame trigger.
What to build:
- [DONE] Add bool chronosDefeated = false to Game.hpp, persist in save v14
- [DONE] In ProcessVictory() (Game.cpp:4752), check if defeated enemy is Chronos — set flag + show "Endgame Unlocked!" notification
- [DONE] Gate all endgame content behind this flag (area select, inn options, crafting tier)
- [DONE] Files: Game.hpp, Game.cpp, SaveGame.cpp
Feature 2: Forbidden Citadel (New Area) [DONE]
What exists: Nothing. 10 areas exist (Felrona through Chronos Depths).
What to build:
- [DONE] Add area 11: "Forbidden Citadel" in InitializeAreas() (Game.cpp)
- [DONE] Define difficulty 11, boss-only (no dungeon rooms)
- [DONE] 10 new bosses: Abyssal Sentinel (8000 HP), Void Empress (9000), Infernal Colossus (10000), Glacial Wraith (8500), Storm Arbiter (9500), Plague Sovereign (11000), Holy Arbiter (12000), Chrono Overlord (13000), Void Monarch (15000), The Unbroken (20000)
- [DONE] Connect to area 10 (Chronos Depths) in the area connections graph
- [DONE] World map node graph in StateAreaSelect() has 11th node (row 4 at y=700)
- [DONE] Citadel locked (red node) until chronosDefeated
- [DONE] StateCitadelBossSelect() — boss picker UI with name, element, HP/STR, Challenge button
- [DONE] Victory returns to CitadelBossSelect
- [DONE] Files: Game.hpp/cpp (StateCitadelBossSelect, area 11), Enemies.hpp/cpp (10 boss factories)
Feature 3: Boss Summon Items [DONE]
What exists: Nothing. Bosses are dungeon-only via bossFactory lambdas. No field-use items.
What to build:
- [DONE] Add ItemType::Summoning enum value to Item.hpp
- [DONE] Create SummoningItem class (extends Item) with a bossFactory callback field
- [DONE] Add field-use mechanic: in StateInventory() "Use" tab, "Summon" button triggers StartCombatWithEnemy(bossFactory())
- [DONE] Register 10 summoning items (one per boss) via SummoningRegistry
- [DONE] Drop sources: 5% from Citadel bosses, reputation vendors, crafting recipes
- [DONE] Files: Item.hpp (new enum), Items/SummoningItem.hpp, Items/SummoningItems.cpp, Game.cpp, SaveGame.cpp
Feature 4: Summoning Item Drop Sources [DONE]
What exists: Boss materials drop at 100% from each boss. No other summon items.
What to build:
- [DONE] Low drop rate (5%) from each Citadel boss
- [DONE] Craftable at Smithing level 12 using boss materials (10 recipes in Crafting.cpp)
- [DONE] Purchasable from reputation vendors at Ally+ rank
- [DONE] Files: Enemies.cpp (add special drops), Crafting.cpp (new recipes), FactionReputation.cpp (vendor stock)
Feature 5: Legendary Crafting [DONE]
What exists: Crafting recipes go up to Epic (rarity 4) at Smithing level 9. Boss materials exist but are unused in crafting.
What to build:
- [DONE] Add CraftingRecipe entries at the bottom of InitializeRecipes() (Crafting.cpp)
- [DONE] Require Smithing level 12
- [DONE] Use boss materials as ingredients (Sentinel's Helm, Empress's Crown, etc.)
- [DONE] Result items: 8 new Legendary (rarity 5) weapons/armor/accessories
- [DONE] 8 recipes total, each using 2-3 different boss materials
- [DONE] Files: Crafting.cpp (new recipes)
Feature 6: Skill Mastery [DONE]
What exists: Skills cap at level 50 (Skill.cpp:39). Further XP is discarded. No mastery system.
What to build:
- [DONE] Add int masteryXP and int masteryLevel to Skill class
- [DONE] When skill hits level 50, overflow XP goes to masteryXP instead of being discarded
- [DONE] Mastery level-up formula: masteryLevel * 200 XP per level (cap at 20)
- [DONE] Mastery talent tree: 3 branches (Damage, Utility, Special) with 5 nodes each
- [DONE] Node effects: Damage (+5% per node), Utility (-CD, -MP), Special (+10% XP gain)
- [DONE] Unlock nodes by spending mastery levels
- [DONE] Persist mastery data in save v14
- [DONE] Files: Skill.hpp/cpp (mastery fields + tree), Game.cpp (mastery UI in StateSkillUpgrade), SaveGame.cpp (serialization)
Feature 7: Class Evolution
What exists: Nothing. 5 classes with no upgrade path.
What to build:
- Add CharacterClass::WarriorHero, PriestSage, MageArchmage, ArcherRanger, MerchantTycoon (or similar evolution names) to the enum
- bool evolved flag on Player, persists in save
- Evolution quest: defeat Chronos + collect 3 specific boss materials + reach devotion rank X
- On evolution: base stats +20%, unlock 2 new skill slots, new passive ability per class
- Evolution UI: triggered from a new "Evolution" NPC or Inn option after conditions met
- Files: Classes.hpp (new enum values), Player.hpp/cpp (evolution flag + stat boost), Game.cpp (evolution quest logic + UI)
Feature 8: Reputation Vendor Rewards [DONE]
What exists: Reputation system is largely complete (10 factions, 6 ranks, repeatable quests, serialization). Missing: vendor UI with faction-locked items.
What to build:
- [DONE] Add vendor stock to each faction: 3-5 items per rank tier (6 tiers = 18-30 items per faction)
- [DONE] Items include: endgame equipment (at Champion+), potions (at Stranger+), accessories
- [DONE] Vendor UI: "Shop" tab in StateReputation() with pagination (VENDOR_ITEMS_PER_PAGE = 6)
- [DONE] Discount already works via rank (5-25% off)
- [DONE] 10 new Citadel boss factions added (FactionID enum, InitializeFaction, repeatable quests, vendor stock)
- [DONE] Faction list pagination (FACTIONS_PER_PAGE = 8) for 20 total factions
- [DONE] Files: FactionReputation.hpp/cpp (20 factions, vendor stock), Game.hpp/cpp (pagination, vendor UI)
Feature 9: Special Boss Pets (Legendary Tier) [DONE]
What exists: 10 boss pets at 8% drop rate. 3-tier evolution (base/evolved/ascended). No rarity system.
What to build:
- [DONE] Add PetRarity enum: Common, Rare, Epic, Legendary
- [DONE] Add rarity field to Pet struct
- [DONE] 10 new Citadel boss pets at Legendary rarity with 2% drop rate
- [DONE] 4th evolution tier ("Mythic") at level 40 — multiplier 3.5x
- [DONE] Visual indicator: Legendary pets have gold name color + gold border in Pet UI
- [DONE] Mythic pets have purple glow border
- [DONE] Updated pet UI: collection summary shows mythic count, source labels show "Citadel"
- [DONE] Files: Pet.hpp (PetRarity, mythic tier, rarity field), Pet.cpp (10 new pets, AddCitadelPet, 2% Citadel drops), Game.cpp (pet UI updates)
Feature 10: Save Data Expansion [DONE]
What exists: v13 save format with version-gated loading. Pattern is well-established.
What to build:
- [DONE] Bump SAVE_VERSION to 14 in SaveGame.hpp
- [DONE] chronosDefeated (bool) — persisted in save v14
- [DONE] bossKillCounts[10] (int array) — persisted in save v14, incremented on Citadel boss kills
- [DONE] masteryXP[] + masteryLevel[] per skill — persisted in save v14
- [DONE] masteryNodes[] (unlocked node IDs) — persisted in save v14
- [DONE] summoningItems[] (inventory of summon items) — handled by SummoningItem serialization (subtype "S")
- [DONE] legendaryRecipesUnlocked (uint32_t bitmask) — persisted in save v14
- [DONE] Gate all reads with if (saveVersion >= 14)
- [DONE] Files: SaveGame.hpp (version bump + new params), SaveGame.cpp (serialize/deserialize), Game.hpp (new fields), Game.cpp (call sites + kill tracking)
Recommended Implementation Order
Phase	Features	Effort	Dependencies
1	Endgame unlock flag + save v14 [DONE]	Small	None
2	Reputation vendor rewards [DONE]	Small	Phase 1
3	Forbidden Citadel (new area + boss) [DONE]	Medium	Phase 1
4	Legendary crafting recipes [DONE]	Medium	Phase 3 (boss materials)
5	Summoning items + field-use system [DONE]	Large	Phase 3
6	Skill Mastery [DONE]	Large	Phase 1
7	Class Evolution	Large	Phase 1, 3
8	Special boss pets (rarity) [DONE]	Small	Phase 1
9	UI polish + balance	Medium	All above
Total estimated scope: ~15-20 new/modified files, primarily Game.cpp, SaveGame.cpp, Skill.hpp/cpp, Crafting.cpp, FactionReputation.cpp, Pet.hpp/cpp, and 2-3 new files for summoning items and evolution.
▣  Build · Big Pickle · 4m 12s
Feature 2 should be a new area with only bosses. It should have about 10 new bosses, that are all really strong. Feature 9 should be for the new area with bosses only. Those bosses on difficulty 11 should be able to drop new really strong pets. Can you change your plan according to my changes please
+ Thought: 852ms
Got it. Here's the revised plan for those two features:
Feature 2 (Revised): Forbidden Citadel — Boss-Only Area
Concept: Area 11 at difficulty 11+. No dungeon rooms, no regular enemies. The entire area is a boss gauntlet — 10 new bosses that are significantly stronger than anything in the game.
What exists: Nothing. 10 areas exist, all with dungeon rooms + regular enemies + 1 boss each.
What to build:
New Bosses (10 total, difficulty 11)
Scale well beyond Chronos (who has 5000 base HP, 100 STR). These bosses should feel like a real wall.
#	Boss Name	Element	Base HP	Base STR	Signature Mechanic	Special Drop
1	Abyssal Sentinel	Physical	8000	120	Damage immunity phases (every 25% HP)	Sentinel's Helm
2	Void Empress	Arcane	9000	110	Summons void adds each turn	Empress's Crown
3	Infernal Colossus	Fire	10000	130	Stacking burn on player (10%/turn)	Colossus Core
4	Glacial Wraith	Ice	8500	100	Freezes player skills randomly (skip cooldown)	Wraith's Shroud
5	Storm Arbiter	Lightning	9500	125	Reflects 30% of damage taken	Arbiter's Scales
6	Plague Sovereign	Poison	11000	90	Permanent poison + reduces healing by 50%	Sovereign's Filigree
7	Holy Arbiter	Holy	12000	115	Heals to full at 20% HP once	Arbiter's Halo
8	Chrono Overlord	Arcane	13000	140	Takes extra turn every 3rd turn	Overlord's Chronoshard
9	Void Monarch	Arcane	15000	150	Disables player's strongest skill each fight	Monarch's Scepter
10	The Unbroken	Physical	20000	170	Scales up every 5 turns (+10% all stats)	Unbroken's Heart
Area definition:
- No dungeon rooms (minEnemies=0, maxEnemies=0 on all rooms)
- Boss select screen instead of dungeon explore: player picks which boss to fight from a list
- Each boss has a cooldown after defeat (can't re-fight same boss for X in-game days)
- Beating all 10 unlocks a secret 11th boss: "The Primordial Sovereign" (25000 HP, 200 STR)
Files to modify:
- Game.cpp — InitializeAreas() (add area 11), StateDungeonSelect() or new StateCitadelBossSelect() for boss picker UI
- Enemies.cpp — 10 new Create*() factory methods
- Enemies.hpp — 10 new declarations
- Game.hpp — new StateCitadelBossSelect() method, boss cooldown tracking
Feature 9 (Revised): Legendary Boss Pets from Citadel
Concept: Each of the 10 Forbidden Citadel bosses has a unique legendary-tier pet with a very low drop rate (2-3%). These pets are mechanically stronger than existing pets and have a 4th evolution tier.
What exists: 10 boss pets at 8% drop rate, 3-tier evolution (base/evolved/ascended), no rarity system.
What to build:
New Legendary Pets (10 total)
Each pet has significantly higher base stats and a unique passive that existing pets don't have.
Pet Name	Boss Source	Element	Req Lvl	Base ATK	Unique Passive
Sentinel's Bulwark	Abyssal Sentinel	Physical	50	40	+15% damage reduction
Void Empress's Shadow	Void Empress	Arcane	50	38	+20% crit damage
Infernal Ember	Infernal Colossus	Fire	50	42	Burn enemies on hit (3%/turn)
Glacial Echo	Glacial Wraith	Ice	50	36	10% freeze chance on hit
Storm Conduit	Storm Arbiter	Lightning	50	44	+15% attack speed
Sovereign's Plague	Plague Sovereign	Poison	50	35	-25% enemy healing
Arbiter's Light	Holy Arbiter	Holy	50	37	+20 HP/turn regen
Chrono Fragment II	Chrono Overlord	Arcane	50	45	-1 skill cooldown
Monarch's Whisper	Void Monarch	Arcane	50	43	+25% spell damage
The Unbroken's Heart	The Unbroken	Physical	50	50	+10% all stats
Pet system changes:
- Add PetRarity enum: Common, Rare, Epic, Legendary to Pet.hpp
- Add rarity field to Pet struct
- Existing 20 pets: Common/Rare based on source difficulty
- New 10 Citadel pets: Legendary
- 4th evolution tier ("Mythic") at pet level 50 + Chronos Eye: multiplier 3.5x, second ability upgrade
- Drop rate: 2% from Citadel bosses (vs. 8% from regular bosses)
- Visual indicator: Legendary pets have gold name color in Pet UI
Files to modify:
- Pet.hpp — PetRarity enum, rarity field, CanMythic() method
- Pet.cpp — 10 new pet definitions, rarity assignments on all 30 pets, mythic evolution logic
- Game.cpp — ProcessVictory() boss pet roll needs to check Citadel boss and use 2% rate
- Pet.cpp — RollBossDropPet() needs Citadel-specific pool
Updated Implementation Order
Phase	Features	Effort
1	Endgame unlock flag + save v14 [DONE]	Small
2	Reputation vendor rewards [DONE]	Small
3	Forbidden Citadel — 10 bosses + area definition + boss select UI [DONE]	Large
4	Legendary crafting recipes (using Citadel boss materials) [DONE]	Medium
5	Summoning items + field-use system [DONE]	Large
6	Skill Mastery [DONE]	Large
7	Class Evolution	Large
8	Legendary boss pets from Citadel (rarity system + 4th evolution tier) [DONE]	Medium
9	Save data expansion (bossKillCounts, legendaryRecipesUnlocked) [DONE]	Small
10	UI polish + balance	Medium