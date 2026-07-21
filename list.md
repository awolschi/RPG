# Eluna -- A CozyQuest Tale: Improvement List

Please also add level restrictions for items and in general a higher level item should be stronger. [DONE: requiredLevel field on Item base, enforced in CanEquip, displayed in inventory/shop tooltips, persisted in save v12, set from loot difficulty]
[x] Fix quest gatherItemName not saved in SaveGame.cpp
[x] Fix religion quest data lost on save/load
[x] Fix prayer devotion gain never applied
[x] Fix Arcane Crystal resource unobtainable
[x] Fix religion quest target enemies that don't exist
[x] Fix Pet::CanEvolve() always returns false
[x] Fix Offhand using ItemType::Weapon
[x] Fix DrawRectLines ignoring thick parameter
[x] Fix JobSystem::WorkJob self-assignment shadow
[x] Lower drop rates + class-appropriate loot drops
[x] Fix flee always succeeds (add success chance)
[x] Fix Defend action underpowered (scale with level)
[x] Remove std::cout debug output from Religion.cpp
[x] Fix static local variables persisting across sessions
[x] Remove dead static bool init variable
[x] Add try/catch around std::stoi in save/load
[x] Fix Duplicate XP formula (consolidate)
[x] Build and verify all changes


## Critical Bugs

- [x] **Quest `gatherItemName` not saved** -- `SaveGame.cpp:162-173`: When saving quests, `gatherItemName` is never written. On load, Collect quests have an empty `gatherItemName`, breaking gather quest display and tracking.
- [x] **Religion quest data lost on save/load** -- `Religion.cpp:408-426`: `Deserialize` does not restore `description`, `targetEnemy`, `targetCount`, or `rewardDevotion` for the active quest. In-progress quest targets are lost.
- [x] **Prayer devotion gain is dead code** -- `Religion.cpp:161-163`: The code announces Devotion +1 on first daily prayer but never actually increments `devotionLevel`.
- [x] **`Arcane Crystal` resource unobtainable** -- `Crafting.cpp:277,300,360-396`: Several recipes require "Arcane Crystal" but no `ResourceFactory` entry or mining tier produces it. These items cannot be crafted.
- [x] **Religion quest target enemies don't exist** -- `Religion.cpp:307-318`: Quest targets like "Zombie" and "Golem" don't appear as enemy names in `Enemies.cpp`.
- [x] **`Pet::CanEvolve()` always returns false** -- `Pet.hpp:78`: `!obtained && false` is always false. Likely dead code or bug; `CanEvolveNow()` on line 79 is the real check.
- [x] **`Offhand` uses `ItemType::Weapon`** -- `Item.hpp:~349`: Offhands are categorized as weapons in the inventory, which is semantically incorrect and could cause filtering bugs.

## High-Priority Improvements

- [x] **No save confirmation dialog** -- `Game.cpp:4876-4880`: Clicking a save slot that already has data immediately overwrites with no confirmation.
- [x] **No bounds checking on `areaIndex` in save/load** -- `SaveGame.cpp:661`: No validation that the saved area index is within valid range. Corrupted data causes out-of-bounds access.
- [x] **Unchecked `static_cast` from integers to enums in save/load** -- `SaveGame.cpp` throughout: `static_cast<WeaponType>(std::stoi(...))` etc. with no range validation.
- [x] **No try/catch around `std::stoi` in save/load** -- `SaveGame.cpp` throughout (~50+ calls): Corrupted save files crash the game.
- [x] **Remove `std::cout` debug output** -- `Religion.cpp:87,96`: Religion system writes directly to stdout instead of using the game's notification system.
- [x] **Fix `DrawRectLines` ignoring `thick` parameter** -- `GRenderer.cpp:68-70`: The `thick` parameter is accepted but never used. Should use `DrawRectangleLinesEx`.
- [x] **Defend action is underpowered** -- `Combat.cpp:229`: Defend gives only +5 temp defense. Should scale with level or defense stat.
- [x] **Flee always succeeds with no failure chance** -- `Game.cpp:3876-3882`: Fleeing always succeeds immediately. Should have a success probability based on level difference.
- [x] **`JobSystem::WorkJob` shadows member variable** -- `JobSystem.cpp:51`: `LocationUpgrades& locUpgrades = locationUpgrades;` is a self-assignment that does nothing.

## Medium-Priority Features

- [ ] **No inventory capacity limit** -- `Inventory.hpp`: Players can accumulate unlimited items. Consider adding a bag slot upgrade system.
- [x] **No item comparison UI** -- No side-by-side stat comparison when equipping items. Players must manually remember equipped item stats. [CLOSED: comparison panel exists at Game.cpp:1793-1986]
- [x] **Area encounters chain indefinitely with no choice** -- `Game.cpp:4051-4067`: After defeating an area encounter, the player is immediately thrown into another fight with no rest option.
- [ ] **Job fatigue never naturally decreases** -- `Jobs.cpp:364-374`: No game loop call reduces fatigue over time (e.g., per in-game day). Players can become permanently fatigued.
- [x] **Weather `UpdateDaily()` may never be called** -- `JobEnvironment.hpp:58`: The call may not be wired into the game loop. [CLOSED: wired at Game.cpp:843 via Rest at Inn]
- [x] **Shop inventory has no restock mechanic** -- `Game.cpp:3350-3361`: Shop items are generated once when entering and cleared on leaving.
- [x] **Set bonuses only support 2-piece and 4-piece** -- `SetBonuses.hpp:62-69`: No support for 3-piece or 5-piece bonuses.
- [x] **`SetBonuses::CountSetPieces` may not count weapons/accessories** -- `SetBonuses.hpp:79`: Many sets include weapon/ring/amulet pieces that may not be counted. [CLOSED: all 10 slots counted at SetBonuses.cpp:334-343]
- [ ] **No audio/sound system** -- The entire game has no sound effects or music despite raylib supporting audio.
- [x] **No visual world map** -- Replaced flat list with node graph in StateAreaSelect. 10 areas in 4 rows, connected by lines. Color-coded: gold=current, green=reachable, dark=locked. Hover tooltips, click-to-travel. Area connections added to Area class.
- [x] **No tutorial or guided onboarding** -- Session-scoped tutorial hints: 8 contextual triggers (explore, combat, inventory, travel, quests, crafting, skills, inn rest). Slide-in notification toast, auto-dismiss after 6s.

## Code Quality

- [ ] **Game.cpp is ~6000 lines** -- All 25+ `State*` methods mix rendering, input, and game logic. Should be decomposed into separate state classes.
- [x] **Massive duplication in save/load serialization** -- `SaveGame.cpp:54-101 vs 408-522`: Nearly identical `dynamic_pointer_cast` chains repeated dozens of times. Should be helper functions.
- [ ] **Magic numbers throughout** -- Hard-coded pixel coordinates (70, 130, 320, 500, etc.) scattered across all UI functions. Should use centralized layout constants.
- [x] **Raw `rand()` instead of `<random>`** -- `Loot.cpp`, `Combat.cpp`, `Jobs.cpp`: `srand`/`rand` produces poor randomness. Should use `std::mt19937`.
- [x] **Duplicate XP formula implementations** -- Three different XP formulas exist in `LevelUp.cpp`, `ExperienceCurve.cpp` (unused), and `Character.hpp`. Should be consolidated.
- [x] **Dead `static bool init` variable** -- `Game.cpp:4945-4953`: Set but never read. Dead code.
- [x] **Static local variables persist across sessions** -- `Game.cpp:2963,5207`: `static int selectedSkillIdx` and `static int selectedCategory` survive across game sessions.
- [x] **Synergy queries duplicate iteration logic** -- `JobSystem.cpp:465-582`: `GetSynergyXPBonus`, `GetSynergyRareFindBonus`, etc. all iterate synergies independently. Should be consolidated.

## Performance

- [ ] **String concatenation in draw loops** -- Throughout `Game.cpp`: Strings built via `+` concatenation every frame, causing heap allocations on every draw call.
- [ ] **`dynamic_pointer_cast` in hot paths** -- `Combat.cpp`, `Game.cpp`, `Equipment.cpp`: RTTI has non-trivial cost. Consider storing `ItemType` enum on the base class.
- [x] **Wiki may rebuild databases every frame** -- `Wiki.hpp:89-97`: Nine `Build*Database()` methods. Should be built once and cached. [CLOSED: built once in ctor, cached in member vectors]
- [x] **`GetMaxHealth()` recomputes every call** -- `Character.hpp:40`: Calls into `Equipment::GetHealthBonus()` which iterates accessories. Should be cached and invalidated on equip/unequip. [CLOSED: GetHealthBonus is O(1) — 3 branch checks, no iteration; caching gain negligible]

## Long-Term / Architecture

- [ ] **Separate UI state from game logic** -- Each `State*` method should be its own class with clear separation of concerns.
- [ ] **Event-driven architecture for notifications** -- Achievement, quest, reputation, and pet notifications are handled by separate draw methods. An event bus would decouple systems.
- [ ] **Structured save format** -- Current pipe-delimited text format is fragile and hard to extend. JSON or binary would be safer.
- [ ] **Central config system** -- Combat formulas, XP curves, drop rates, UI dimensions, and balance values are scattered across dozens of files.
- [x] **Crafting recipes should unlock dynamically** -- All 41 recipes now have `requiredJobLevel`/`requiredJob` fields. `CanCraft` enforces level check. `IsLocked` helper added.
- [x] **Merchant class has almost no unique content** -- Now has 8 legendaries + 11 epics. All classes have class-specific items: Warrior (10L+10E), Priest (10L+10E), Archer (10L+10E). 4 new class-specific sets added.
- [x] **Achievement rewards are cosmetic strings only** -- 9 achievement reward items registered in UniqueItemRegistry. `CheckAchievementNotifications()` wired in Game.cpp.
- [ ] **`Escort` quest type defined but never implemented** -- `Quest.hpp:20`: Exists in the enum but has no implementation.
- [ ] **New Game+, Boss Rush, Master Class tracked but not implemented** -- `Achievement.cpp:510-528`: Flags exist but game modes don't.
- [x] **Add a punishment, if the player dies, the player should lose some gold and experience** -- 10% gold + 10% XP loss on defeat, respawn at area with 50% HP/MP (Game.cpp)
