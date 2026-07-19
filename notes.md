Succesfully finished dungeon page should be controllable by keyboard (return).

After I loaded a savegame with a merchant level 1, I could not use the coin throw skill anymore. It does not even show up at the skillselection.

Offhand weapon does not give any bonuses at the moment.

Show exp bar in the fighting screen next to hp and mana. Please also show the expbar of the equipped pet. And also show the reputation bar of the faction of the current area.

Bonus stats from legendary equipment and sets dissapear after saving and loading. They are not shown anymore (green text before the save, but not after loading). Please fix this.

Save achievements in savegames. [DONE]

Implement all the creatureimages in the game. For example overseer does not load correctly. [DONE]

Do quests give rewars? They should, but currently they don't. Implementing a reward system for completing quests would enhance the gameplay experience and provide players with tangible incentives for their efforts. [DONE]

Savegames 3 and 4 say SAVE_VERSION 3 ? Lv.50 Warrior, but they are not warriors and not level 50. You should also erase the Questionmark. [DONE]

Make throw coins skill from merchant cost coins. [DONE]

At the moment the buttons for attack, skills, defend , items and flee are colliding with the battlelog. [DONE]

When the enemies health gets reduced to 0 by poison dot the enemy lives for another round, even though it should die. [DONE]

Items should be sellable in the shop. [DONE]

[•] Create JobSkillTree.hpp/cpp - skill tree system (3 branches x 5 skills per job) [DONE]
[•] Implement Mining skill tree (Efficiency, Quality, Luck branches) [DONE]
[•] Implement Lumberjacking, Fishing, Smithing skill trees [DONE]
[ ] Wire skill tree bonuses into job collection
[ ] Add skill tree UI to Jobs screen
[ ] Create JobEnvironment.hpp/cpp - weather system
[ ] Implement weather effects on job performance
[ ] Implement location upgrades system
[•] Add CMakeLists entries [DONE]
[ ] Update jobtodo.md
[•] Build and verify [DONE]

[•] Faction Reputation System [DONE]
[•] 10 factions (one per area) with 6 reputation ranks each [DONE]
[•] Repeatable quests per faction (kill-based, rank-gated) [DONE]
[•] Reputation gains from kills, dungeons, and quest completion [DONE]
[•] Rank rewards: shop discounts, XP bonuses, combat stat bonuses, gold [DONE]
[•] Save/load support (SAVE_VERSION 4) [DONE]
[•] Reputation UI with faction list, bars, repeatable quest board [DONE]
[•] Floating reputation gain notifications [DONE]
[•] Repeatable quest pagination (5 quests per page) [DONE]

[•] Pet System [DONE]
[•] 20 pets total: 10 faction Legend-rank rewards + 10 boss drops [DONE]
[•] Elemental pets: Fire, Ice, Lightning, Arcane, Poison, Holy [DONE]
[•] Pet auto-attacks every combat round (after player action, before enemy turn) [DONE]
[•] Pet damage scales with player level (2% per level) [DONE]
[•] Pet passive bonuses: XP bonus, gold find, crit chance/damage, damage/defense bonus, HP/MP bonus, heal on kill [DONE]
[•] Faction Legend pets unlocked automatically at Legend rank [DONE]
[•] Boss pet drops: 8% chance from named bosses [DONE]
[•] Pet management screen (Pets button in explore menu) [DONE]
[•] Equipped pet shown in combat (name + element color + ATK near player bar) [DONE]
[•] Pet attack shows floating damage text and combat log entries [DONE]
[•] Pet damage can kill enemies (triggers ProcessVictory) [DONE]
[•] Pet XP/gold bonuses applied in ProcessVictory [DONE]
[•] Pet heal-on-kill triggers on enemy defeat [DONE]
[•] Pet notifications slide in from right [DONE]
[•] Save/load support (SAVE_VERSION 7) [DONE]
[•] BattleRenderer shows pet evolved name/element/ATK [DONE]
[•] Pet leveling: pets gain 50% of player XP from combat [DONE]
[•] Pet XP formula: level * 100 required per level, max level 50 [DONE]
[•] Passive bonuses scale with pet level (10 GetScaled* methods) [DONE]
[•] Pet level/XP persisted in save files [DONE]
[•] Pet Gallery: all 20 pets shown (obtained + silhouettes for unobtained) [DONE]
[•] Pet evolution: 3 tiers (Base, Evolved at lv15, Ascended at lv30) [DONE]
[•] Evolution stat multiplier: 1.4x (evolved), 2.0x (ascended) [DONE]
[•] Special abilities: Burn/Ignite, Poison/Toxic Cloud, Freeze/Frostbite, Stun/Static Shock [DONE]
[•] Ability proc: 30% chance (evolved), 50% chance (ascended) [DONE]
[•] Ability duration extends by +1 turn at ascended tier [DONE]
[•] Evolution names: 20 unique evolved + 20 unique ascended names [DONE]
[•] Evolution triggers automatically on level-up during combat [DONE]
[•] Evolution notifications in combat log and slide-in panel [DONE]
[•] Pet Gallery shows evolution tier, ability name, XP bar, element color [DONE]
[•] Evolution tier border colors: green (evolved), gold (ascended) [DONE]

[•] Add Pets tab to WikiTab enum
[•] Add petEntries vector and BuildPetDatabase to Wiki class
[•] Add pet discovery tracking (obtainedPets set)
[•] Update DrawTabBar to show Pets tab
[•] Handle Pets tab in DrawTabPage
[•] Wire up MarkPetObtained in Game.cpp (all 6 ObtainPet sites + load-game sync)
[•] Build and verify compilation
[•] Pagination pass for long UI lists (screen is 1024x768, many lists overflowed):
[•]   StatePets        - 9 pets/page  (was: 20 pets overflowed off-screen)
[•]   StateInventory   - 18 items/page (was: hard cap 15, items beyond #15 invisible)
[•]   StateStats       - 16 skills/page (was: high-level Mage ~27 skills overflowed)
[•]   StateJobPerks   - 8 perks/page  (was: 20 perks overflowed)
[•]   StateShop sell   - 18 items/page (was: full inv iterated, hoarders overflowed)
[•]   StateAchievements - 12/page (was: hard cap 12, achievements beyond #12 invisible)
[•] Faction quest text reworked: real enemy/boss names, exact kill matching, no cross-faction bleed
