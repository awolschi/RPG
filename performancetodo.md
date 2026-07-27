# Performance & Code Quality Review

Comprehensive audit of the codebase. Issues are ordered by impact.

---

## 1. Game.cpp Monolith (7751 lines)

### Architecture

- [ ] **Split `Game.cpp` into separate state classes** — All 30+ `State*()` methods live in one file. Each should be its own `.cpp` file (e.g. `StateCombat.cpp`, `StateInventory.cpp`, `StateWiki.cpp`). The Game class orchestrates, states handle themselves.
- [ ] **Extract a `UIHelper` namespace** — The same panel-drawing, tooltip, and pagination code is copy-pasted across every state. A shared `UIPanel`, `UITooltip`, `UIPagination` helper eliminates thousands of duplicate lines.
- [ ] **Extract a `CombatUI` class** — `StateCombat()` is ~500 lines mixing rendering, input, and combat logic. Separate rendering from `DoPlayerAttack`/`DoEnemyTurn` calls.

### Per-Frame Performance

- [ ] **String concatenation in draw loops** — ~150+ `std::to_string()` + `+` concatenations happen every frame (e.g. `Game.cpp:693,718,875,1318,1467-1488,1590,1748,1832-1865`). Each allocates heap memory. Use `std::string::reserve` + `append`, or better, use a stack-allocated `fmt::format` or pre-build display strings only when data changes.
- [ ] **Avoid building display strings for hidden states** — Strings are built even when the relevant state isn't visible. Only construct UI text when in the corresponding `GameState`.
- [ ] **`dynamic_pointer_cast` called 100+ times per frame in combat/inventory** — Each `dynamic_pointer_cast` does RTTI lookup. The same item is cast repeatedly (e.g. the inventory item is cast 5+ times in the comparison panel alone: lines 1830-1875, 1905-1941, 2091-2244). Cast once, store the result, reuse it.

### Dead Code

- [x] **`DisplayStats()` is never called** — `Character.cpp:263-276` writes to `std::cout`. This is a CLI debug function left in the codebase.
- [x] **`ListEquipment()` is never called** — `Player.cpp:389-433` writes to `std::cout`. Same issue.
- [x] **`CheckNewSkills()` uses `std::cout`** — `Player.cpp:636-641` prints to stdout instead of using the game's notification system.
- [x] **`GetSkillOptions()` in Combat.cpp** — `Combat.cpp:307-324` returns formatted strings. Used only for CLI display, never called in the GUI game loop.
- [x] **`GetStatusLine()` in Combat.cpp** — `Combat.cpp:285-305` builds an `ostringstream` status string. Never used in the GUI.
- [x] **`WeaponAllowedForClass()` and `ArmorAllowedForClass()` in Loot.cpp** — These try 8 random rolls to get class-appropriate loot. Should just pick directly from the allowed pool instead of random-retry.

### Input Handling

- [ ] **No debounce on rapid key presses** — The Escape handler in `Run()` (line 291-327) fires every frame. `IsKeyPressed` handles single presses, but mouse clicks through `Button()` lack debounce. Rapid clicking on shop/buy buttons can double-purchase.
- [ ] **`keyboardNav.Update()` called in some states but not others** — Focus navigation is inconsistent.

---

## 2. dynamic_pointer_cast Abuse (100+ call sites)

### The Problem

Every item type check uses `dynamic_pointer_cast`, which is O(n) RTTI lookup. The same item gets cast 3-6 times in a single UI frame. This is the single biggest performance issue in the codebase.

### Solution

- [x] **Add a `virtual` type query method to `Item`** — Add `virtual ItemType GetType() const { return type; }` (already exists as `item->type` enum). Use `item->type == ItemType::Weapon` checks instead of `dynamic_pointer_cast<Weapon>`. This is a direct enum comparison, no RTTI.
- [x] **Alternative: cache the casted pointer** — In inventory rendering, cast once and store in a local struct:
  ```cpp
  struct DisplayItem {
      std::shared_ptr<Item> item;
      Weapon* weapon = nullptr;
      Offhand* offhand = nullptr;
      Armor* armor = nullptr;
      Accessory* accessory = nullptr;
      // ... populated once per frame
  };
  ```
- [x] **Replace all 100+ `dynamic_pointer_cast` calls with enum checks** — Files affected: `Game.cpp` (~50), `Equipment.cpp` (~10), `SaveGame.cpp` (~10), `Player.cpp` (~15), `Loot.cpp` (~3), `Combat.cpp` (~2), `Wiki.cpp` (many).

---

## 3. Save/Load System

### Safety

- [x] **`SaveGame.cpp:382-886` — `LoadGame` has 50+ unguarded `file >> var` calls** — If the file is truncated or corrupted mid-read, subsequent reads get garbage. Add validation after each `file >>` call. At minimum, check `file.good()` after every 10 reads.
- [ ] **No save file integrity check** — A corrupted save silently produces a broken player. Add a checksum (e.g. CRC32) appended to the end of the file. Verify on load, offer to restore backup.
- [ ] **Offhand save format is fragile** — `SaveGame.cpp:136-149`: The OH sub-type reads 4 fields then an optional 5th via `getline`. If the 5th field is missing from an old save, it falls back to 0 — but the `getline` also reads the trailing `|` from requiredLevel, corrupting the next field. The fallback logic at line 144-145 is correct but brittle; consider a version check instead.
- [ ] **Equipment load uses `dynamic_pointer_cast` on every slot** — `SaveGame.cpp:528-537`: Each of the 10 equipment slots does a `dynamic_pointer_cast` that silently returns nullptr for wrong types. This means a corrupted save could put a Weapon in the Armor slot with no error.

### Architecture

- [ ] **No `Serialize()` / `Deserialize()` virtual methods on Item subclasses** — Serialization is scattered across `SaveGame.cpp` with giant if-else chains for each sub-type. Each Item subclass should own its serialization.
- [ ] **Duplicated deserialization logic** — The inventory loop (lines 447-487) and equipment loop (lines 489-539) have nearly identical parsing code. Extract a shared `ReadItemFromStream()` helper.
- [ ] **Pipe-delimited format is hard to extend** — Adding a new field requires updating both serialize and deserialize, plus version checks. JSON or a more structured format would be safer and self-describing.

---

## 4. Memory & Ownership

### shared_ptr Overuse

- [ ] **`std::shared_ptr<Player> player` in Game** — Player is owned exclusively by Game. Use `std::unique_ptr<Player>`. Same for `combatSystem`.
- [ ] **`std::shared_ptr<Monster> currentEnemy`** — Enemy is created for one fight and destroyed. `unique_ptr` is appropriate.
- [ ] **`std::vector<std::shared_ptr<Item>> shopItems`** — Items are created and destroyed when entering/leaving shop. `unique_ptr` is appropriate.
- [ ] **`std::vector<std::shared_ptr<Item>> roomQueue`** — Dungeon room enemies are temporary. `unique_ptr` is appropriate.
- [ ] **`std::shared_ptr` passed to `StartCombatWithEnemy()`, `ProcessVictory()`, `DoPlayerAttack()`** — These are short-lived calls that don't need shared ownership. Pass by reference or `unique_ptr&&`.
- [ ] **Item `Clone()` returns `shared_ptr`** — Every item clone allocates a new `shared_ptr` with ref-count overhead. If only used for equip replacement, `unique_ptr` is better.

### Floating Particles

- [ ] **`floatingTexts`, `particles` vectors grow unbounded in combat** — Particles are never pre-allocated. For a typical combat, 50+ particles are created per turn. Use a ring buffer or pool with a fixed max size.

---

## 5. Loot System (Loot.cpp)

### Code Quality

- [ ] **`CreateCommonWeapon` through `CreateLegendaryWeapon` are 5 near-identical switch statements** — Each creates the same weapon types with different stat multipliers. Consolidate into one `CreateWeapon(tier, difficulty)` function with a stat table.
- [ ] **`CreateCommonArmor` through `CreateLegendaryArmor` — same issue** — 5 functions with identical structure. Consolidate into `CreateArmor(tier, type, piece, difficulty)`.
- [x] **`CreateOffhand()` has a `default:` case that falls through to `CharacterClass::Merchant`** — The `switch(cc)` at line 620 has no `case CharacterClass::Merchant`, so Merchants always get a Leather Satchel. Should either give Merchants offhands or document the intentional behavior.

### Balance

- [ ] **Loot scaling is purely linear** — `dmg = 10 + (difficulty * 4)` for Common, `dmg = 55 + (difficulty * 40)` for Legendary. No diminishing returns, no breakpoints. Higher difficulties just produce proportionally larger numbers with no interesting behavior.
- [x] **`CreateClassAppropriateWeapon` retries 8 times then gives up** — `Loot.cpp:120-127`: If the random roll doesn't match the class after 8 attempts, it returns whatever was last rolled. This means Warriors can get Staves, Mages can get Axes, etc. Just filter the pool directly.
- [x] **Accessory `CreateAccessory()` can produce negative HP** — `Loot.cpp:602,610`: `hp - 10` and `hp - 5` can go negative at low difficulties, creating accessories with negative HP bonus.

---

## 6. Unique Items (UniqueItems.cpp, 2716 lines)

### Architecture

- [ ] **Split into registration files** — `RegisterOffhands()` alone is ~300 lines. Split into `UniqueItemsWarrior.cpp`, `UniqueItemsMage.cpp`, etc. Or use a data-driven approach: load item definitions from a text/JSON file instead of hardcoding.
- [x] **`RegisterOffhands()` has inconsistent push_back syntax** — Some use `s_offhands.push_back({...})`, which creates a temporary then copies. Use `emplace_back({...})` for in-place construction.

### Data Integrity

- [ ] **No validation that unique item names are unique** — Two items with the same name in different registration functions will silently shadow each other in `Find*()` lookups.
- [ ] **`dropSource` strings are magic strings** — `"Shadow Knight"`, `"Fire Elemental"`, etc. are compared against enemy names. If an enemy is renamed, all drop sources break silently. Use an enum or ID system.

---

## 7. Combat System (Combat.cpp)

### Performance

- [ ] **`ExecuteTurn()` builds strings via concatenation 15+ times** — `Combat.cpp:49,80,112,126,136,147,163,203,206,216,230,243,255,316-319`. Each `std::to_string()` + `+` allocates. These strings are only used for display. Consider building them lazily or using `std::ostringstream`.
- [x] **`ProcessEffects()` creates a `std::vector<size_t>` every turn** — `Character.cpp:169`: Allocates a vector, pushes indices, then erases in reverse. Use `std::remove_if` with erase instead.

### Balance

- [ ] **Damage formula has hard cap at 85% reduction** — `Character.cpp:30`: `std::min(0.85f, totalDefense / (totalDefense + 200.0f))` means defense above ~1133 is wasted. Should either increase the cap or remove it.
- [x] **Master class damage bonus is truncated to int then divided** — `Combat.cpp:122`: `static_cast<int>(bonus * 100) / 100` truncates the percentage. For 15% bonus: `dmg * 15 / 100` is correct, but the code does `dmg * (int)(0.15 * 100) / 100` which is the same thing but harder to read. Use `dmg * 15 / 100` directly.
- [ ] **Defend gives fixed 5 + level/2 + def/4** — `Combat.cpp:263`: At level 50 with 200 defense, Defend gives 5+25+50=80 temp defense. This is additive with existing defense, making it nearly meaningless since defense has diminishing returns. Consider making Defend give a percentage reduction instead.

---

## 8. Graphics & Rendering

### GRenderer

- [ ] **Fixed 1024x768 resolution** — `GRenderer.hpp:12-13`: Hardcoded window size. No DPI awareness, no fullscreen toggle, no resize. Consider supporting common resolutions (1280x720, 1920x1080).
- [ ] **`DrawText()` takes `std::string` by value** — `GRenderer.hpp:30`: Every text draw call copies the string. Should take `const std::string&`.
- [ ] **No texture atlas** — Each icon and background is a separate texture. GPU texture switches are expensive. Consider a texture atlas for UI icons.
- [ ] **`TextureManager` never unloads** — `TextureManager.hpp:20`: `UnloadAll()` exists but is only called in the destructor. Navigating between many areas accumulates textures in GPU memory forever.

### BattleRenderer

- [ ] **`BattleRenderer::DrawBattleScreen()` takes 17+ parameters** — This is a God function. Should be a struct:
  ```cpp
  struct BattleScreenData {
      Character& enemy;
      Player& player;
      std::vector<std::string>& combatLog;
      // ... all the other params
  };
  ```

---

## 9. Engine/Game.hpp

### State Management

- [ ] **30+ raw member variables for UI state** — `Game.hpp:196-277`: `questTab`, `craftPage`, `inventoryTab`, `skillOverviewPage`, etc. are all bare `int` members. Should be grouped into per-screen state structs:
  ```cpp
  struct InventoryState { int tab = 0; int page = 0; int sortMode = 0; int sortDir = 1; };
  struct QuestState { int tab = 0; int page = 0; bool checked = false; };
  ```
- [x] **`citadelBossKillCounts[10]` is a raw C array** — `Game.hpp:371`: Should be `std::array<int, 10>`. The raw array decays to pointer when passed to functions.
- [ ] **Tutorial booleans are 9 separate `bool` members** — `Game.hpp:378-386`: Should be a `std::bitset<9>` or a struct with named flags.
- [ ] **`previousState` is compared via `!=`** — This approach for detecting state transitions doesn't scale. Use an `OnEnter()` / `OnExit()` callback pattern instead.

---

## 10. Character & Stats

### Performance

- [ ] **`GetMaxHealth()` recomputes every call** — `Character.hpp:40`: `stats.health + equipment.GetHealthBonus() + petBonusHP + masteryBonusHP`. `GetHealthBonus()` iterates 3 accessories + offhand. In combat, `GetMaxHealth()` is called 10+ times per frame. Cache it.
- [ ] **`GetMaxMana()` same issue** — `Character.hpp:41`.
- [ ] **`GetWeaponDamage()` same issue** — `Character.hpp:45-48`: Calls `equipment.GetWeaponDamage()` which does 2 `dynamic_pointer_cast` calls + adds `masteryDamageBonus`. Called on every attack.
- [ ] **`GetTotalDefense()` iterates 6 slots + does 1 `dynamic_pointer_cast`** — `Equipment.cpp:3-20`: Called in `TakeDamage()` which fires on every hit.

### Invalidation Pattern

- [ ] **No dirty flag for cached stats** — When equipment changes, there's no mechanism to invalidate cached values. Add a `dirty` flag that's set on equip/unequip, cleared when cache is rebuilt.

---

## 11. Build System (CMakeLists.txt)

- [x] **No `-O2` or `-O3` optimization flag** — The build uses default (no optimization). For a release build, add `set(CMAKE_BUILD_TYPE Release)`.
- [x] **No `-DNDEBUG` for release** — Assert-like debug code runs in release builds.
- [ ] **`FetchContent_MakeAvailable(raylib)` re-downloads on every clean build** — Consider pinning to a specific git commit hash instead of a tarball URL for reproducibility.
- [ ] **All sources in flat list** — No per-directory organization in CMake. Use `file(GLOB_RECURSE)` or explicit per-directory `target_sources`.

---

## 12. Item System (Item.hpp)

### Design

- [ ] **`Item` base class has public fields everywhere** — `name`, `type`, `rarity`, `sellValue`, `count`, `setId`, `requiredLevel`, `passive1`, `passive2` are all public. No encapsulation. Any code can mutate items freely.
- [x] **`IsSameAs()` uses `dynamic_cast` on the raw pointer** — `Item.hpp:384-396`: Uses `dynamic_cast` (not `dynamic_pointer_cast`). This is virtual dispatch overhead. Use the `type` enum instead.
- [ ] **`Clone()` returns `shared_ptr`** — Should return `unique_ptr` for transfer semantics.
- [ ] **`Offhand` has both `arcaneDamage` and `damageBonus`** — These serve different purposes (elemental damage vs flat damage) but live on the same struct with confusingly similar names. Consider renaming `damageBonus` to `flatDamageBonus` or `weaponDamageBonus`.
- [ ] **No `SellValue` calculation from rarity** — `sellValue` is hardcoded to 10 in the constructor and overwritten elsewhere. Should be calculated from rarity automatically.

---

## 13. Wiki System

- [ ] **`Wiki::BuildWeaponDatabase()` etc. iterate all unique items every time** — These are called in the constructor and cached, which is correct. But `GetWikiEntries()` filters the cached list on every call with string comparisons. For the filtered view, consider pre-computing filtered sublists.
- [ ] **`Wiki.cpp` is 1602 lines** — Should be split into `WikiWeapons.cpp`, `WikiArmor.cpp`, `WikiUniques.cpp`, etc.

---

## 14. Known Bugs (from notes.md)

- [x] **First skill lost on save/load** — `notes.md:3`: "The first skill that a class gets at level 1, does not show up after loading a savefile." Root cause: `SaveGame.cpp:438-440` clears all skills, adds CommonAttack, then calls `CheckNewSkills()`. But `CheckNewSkills()` only adds skills based on current level — if level 1 skills are handled differently from level-up skills, they're lost. Fixed by making `CheckNewSkills()` add the same unconditional first skills as `InitializeClassSkills()`.
- [ ] **Combat UI overlaps** — `notes.md:5-6`: "Control buttons in the combat screen overlap with the reputation exp bar." The Goblin Familiar text collides with dungeon room number. EXP bar is too short for the numbers overlaid on it.
- [ ] **Escort quest type defined but unimplemented** — `Quest.hpp:20`: Exists in enum but no implementation.

---

## 15. Cross-Cutting Concerns

### Logging

- [x] **27 `std::cout` calls across 3 files** — `Player.cpp` (15), `Character.cpp` (11), `JobSystem.cpp` (1). These are debug prints from development. Remove them or route through a proper logger.

### RNG

- [x] **`RNG.hpp` creates a new distribution every call** — `RNG.hpp:16`: `std::uniform_int_distribution<int>(0, n - 1)(Engine())` creates, seeds, and destroys a distribution object every call. The distribution should be cached or the engine used directly with modulo:
  ```cpp
  inline int Next(int n) {
      static std::uniform_int_distribution<int> dist(0, n - 1);
      return dist(Engine());
  }
  ```
  Note: the current approach is technically correct because `uniform_int_distribution` handles modulo bias, but the overhead of construction is unnecessary. Better to keep the distribution static and adjust the range per call.

### Constants

- [ ] **Magic numbers everywhere** — `70, 130, 320, 500, 80, 120` are pixel coordinates scattered across Game.cpp. Extract to `namespace Layout { constexpr int TopBarHeight = 50; constexpr int SidebarWidth = 200; ... }`.

---

## 16. UI Layout & Overlap Issues (per-screen audit)

Every `State*()` method in `Game.cpp` uses hardcoded pixel positions. No screen uses relative/dynamic layout. This section catalogs all overlap risks and broken layouts per state.

### Cross-Screen Patterns

- [ ] **No text clipping anywhere** — Long item names, descriptions, or stat strings can extend beyond their container. No `MeasureText` + truncation for most draw calls. Only the inventory and shop truncate some strings.
- [ ] **Back button Y is inconsistent** — `GRenderer::H - 60`, `-70`, `-80`, `-100`, `-110` used across different states. Pagination and Back buttons frequently collide.
- [ ] **No scrollable panels** — Content that exceeds the screen area just draws off the bottom of the window. No clipping rectangles, no scroll views.
- [ ] **Fixed 1024x768 resolution** — All layouts are hardcoded for `GRenderer::W=1024, H=768`. No scaling.

### StateCombat (~lines 4574-4800)

- [ ] **Ability buttons can overflow horizontally** — `BattleLayout.cpp:21` hardcodes 5 button slots: `5 * 70 + 4 * 16 = 414px`. With Attack + 4 loadout skills + Defend + Items + Flee + up to 2 god abilities = 10 buttons = `10*70 + 9*16 = 844px`. Centered at `(1024-844)/2=90`, but the button row can extend near the edges.
- [ ] **Dungeon room info overlaps player bar** — `Game.cpp:4659`: Drawn at `layout.playerBarY - 16` (= `screenH - 90 - 16 = 662`), which is 22px above `playerBarY`. The player HP bar and ability buttons are also in this zone.
- [x] **Item select panel overlaps battle area** — `Game.cpp:4762-4767`: Panel starts at `layout.abilityY - 10` with height 200px. At 768 height, `abilityY = 768-90-70-20 = 588`. Panel covers Y=578 to Y=778, extending 10px off-screen. **Fixed**: playerBarY moved up 36px, panel now ends at y=742 (26px from bottom).
- [ ] **Combat log overlaps ability buttons** — The combat log is drawn by `BattleRenderer::DrawBattleScreen` at the bottom. With many log messages, text overlaps the button row.
- [ ] **Floating damage texts use hardcoded coordinates** — `Game.cpp:5015`: Enemy damage at `(680, 130)`, player damage at `(340, 130)`, heal at `(340, 150)`. These don't adapt to layout changes and can stack on top of each other.
- [ ] **Victory screen not shown in StateCombat** — `combatPhase == CombatPhase::Victory` just keeps showing the battle screen. No dedicated victory overlay with loot/XP display. The player must wait or the state auto-transitions elsewhere.

### StateInventory (~lines 1344-2408)

- [ ] **Left panel (item list) has no vertical clipping** — Items start at `y=100` and increment by 30px. With 40+ items, `y = 100 + 40*30 = 1300px`, far below the 768px screen. Items are drawn off-screen.
- [ ] **Right panel (equipment) can exceed screen** — 10 equipment slots at ~20px each + passives (~14px) + set bonus section = potentially 10*(20+14)+60 = 400px. Starting at y=100, ends at y=500. OK for now, but with longer passives or more sets, it overflows.
- [ ] **Comparison panel overlaps item description** — `Game.cpp:2100-2130`: Comparison stats are drawn at fixed `compX/compY` positions. When the selected item has a long description, the comparison text draws on top of it.
- [ ] **"Crafted" text overlaps next recipe** — `Game.cpp:3863`: After crafting, "Crafted: X!" is drawn at `cy+34`, which is only 14px below the Craft button. The next recipe's name starts 90px below, but the feedback text can still collide with the next recipe's requirements.
- [ ] **Set bonus section pushes Back button off-screen** — `Game.cpp:2399-2401`: `DrawSetBonusSection` appends after the equipment slots. If many sets are active, the Back button at `GRenderer::H - 80 = 688` may overlap with content.

### StateStats (~lines 2410-2483)

- [ ] **Stat lines + skill list can exceed screen** — Stats use 24px line spacing (~10 lines = 240px), skills use 22px spacing. Starting at y=110, with 8+ skills per page, the list reaches `110 + 10*24 + 8*22 = 526px`. Pagination at `GRenderer::H - 130 = 638` is OK, but "Skills:" header at y=472 plus 8 skills at 22px each reaches `472 + 8*22 = 648`, which is only 10px above the pagination buttons.
- [ ] **Pagination overlaps Back button** — `Game.cpp:2476`: Prev/Next at `GRenderer::H - 130 = 638`, Back at `GRenderer::H - 80 = 688`. Only 50px gap. With the page text at `navY+6`, this works but is tight.

### StateJobs (~lines 2486-2711)

- [ ] **Work button collides with job name when names are long** — `Game.cpp:2588`: Work button at `barX + barW + 10 = 400`. Job header text starts at x=70. Long job names (e.g., "Mining [Specialization]") can reach x=400+, overlapping the button.
- [ ] **"Milestone!" text overlaps Work button** — `Game.cpp:2562`: Drawn at `barX + barW + 10 = 400`, same X as the Work button but at `barY + 1` vs `barY - 2`. The milestone text and button are stacked at the same X, with the milestone text above.
- [ ] **Fatigue % and milestone text overlap** — `Game.cpp:2570`: Fatigue at `barY + 16`, milestone at `barY + 1`. Both at `barX + barW + 10`. They're vertically close but separate. However, both are at X=400, the same as the Work button.
- [ ] **"Next unlock" text can overlap job boundary** — `Game.cpp:2582`: Drawn at `barY + barH + 4 = barY + 20`. The next job card starts at `y += 90` from the top of this card. If the job area is tight, the next unlock hint from job N and the header of job N+1 can be close.

### StateJobPerks (~lines 2713-2859)

- [ ] **Perk entries can exceed screen with many perks** — Each perk takes ~44-50px (20px name + 20px desc + 6-30px unlock button). 5 perks per page at 50px = 250px + header = 360px. Starting at y=110, ending at ~470. Footer at `GRenderer::H - 154 = 614`. Gap is 144px — OK for 5 perks, but if a perk has a long description, it can extend further.
- [ ] **"Back to Jobs" and "Back" buttons overlap** — `Game.cpp:2847-2858`: Both at `GRenderer::H - 100 = 668`. "Back to Jobs" at x=70, "Back" centered. They're at the same Y but different X. However, if the panel is narrow, they could collide.

### StateJobSkillTree (~lines 2862-3019)

- [ ] **Same overlap pattern as JobPerks** — Footer buttons at fixed Y positions that may not align with content.

### StateJobUpgrades (~lines 3022-3109)

- [ ] **Upgrade button overlaps description text** — `Game.cpp:3074`: Bonus details at `y + 38`, upgrade button at `y - 50` (right-aligned). The bonus text and button are on the same Y row but at different X positions. However, if the bonus string is long enough, it extends rightward toward the button.
- [ ] **Weather panel overlaps upgrade panel** — `Game.cpp:3041-3045`: Weather at y=40-90, upgrades at y=100. Only 10px gap between panels.

### StateSkillLoadout (~lines 3111-3246)

- [x] **Skill entries with descriptions overflow** — Each skill entry is 60-74px (depending on damage estimate). 10 per page at 74px = 740px. Starting at y=140, ending at `140 + 10*74 = 880`, well past the 768 screen height. The bottom ~5 skills are drawn off-screen. **Fixed**: Reduced to 7 skills per page.
- [ ] **"Selected: N/4" text overlaps Save button** — `Game.cpp:3229`: Text at `bottomY - 50 = 618`, Save button at `bottomY - 20 = 648`. They're 30px apart, which is OK, but the selected count text can be wide and extend into the Save button area.
- [ ] **"EQUIP" and "ATK" badges overlap skill name** — `Game.cpp:3192-3199`: EQUIP badge at x=615-685, ATK badge at x=690-745. The skill button extends to x=690 (70+620). EQUIP badge overlaps the button's right edge.

### StateSkillUpgrade (~lines 3248-3802)

- [ ] **Mastery tree nodes overflow the panel** — `Game.cpp:3617-3668`: Mastery panel is 160px tall with 3 columns × 5 nodes at 24px each = 120px + 22px header = 142px. Tight fit but OK. However, the "Unlock" button at `bx + 200` extends 60px to the right. With 3 columns at `(W-160)/3 = 288px` width, the button at column 3 starts at `80 + 2*288 = 656` and extends to `656+200+60 = 916`, which is fine. But column 1's button at `80+200 = 280` extends to `340`, which is within column 1's boundary. OK.
- [ ] **Character Mastery grid can overflow** — `Game.cpp:3326-3357`: 3 columns × 5 nodes at 28px each = 140px + 24px header. Starting at y=110, ending at ~274. Summary below adds more text. If all bonuses are active, the summary string can be very long and extend off-screen to the right.
- [ ] **"Back to Skills" and "Back" overlap** — Both at `backY` but different X positions. Same pattern as JobPerks.

### StateCraft (~lines 3805-3882)

- [ ] **"Crafted" feedback text overlaps next recipe** — `Game.cpp:3863`: Text at `cy + 34`. Next recipe in the 2-column layout starts at `y += 90` from the column start. If the crafted item's name is long, the feedback text can extend into the next column's area.
- [ ] **Recipe requirements text can be very long** — `Game.cpp:3844-3853`: Requirements string is truncated at 280px, which is correct. But the truncation happens character-by-character with `pop_back()`, which can leave orphaned text.
- [ ] **Pagination and Back buttons at same Y** — `Game.cpp:3872-3881`: Both at `GRenderer::H - 80 = 688`. Prev/Next on the left, Back centered. They don't overlap but are at the same vertical position, which looks cluttered.

### StateReligion (~lines 3885-4010)

- [ ] **God list can push "Leave Shrine" off-screen** — 4 gods at 44px each = 176px + header = ~240px. Starting at y=110, ending at ~350. Leave Shrine at `y + 20 = 370`. OK. But when a god is chosen, the page is much longer: faith info + prayer + donate + god quest + god abilities + abandon + leave. With all sections active, the page easily exceeds 768px.
- [ ] **"Abandon Faith" and "Leave Shrine" buttons can overlap** — `Game.cpp:4005-4008`: Abandon at `y`, Leave at `y + 10`. They're 10px apart vertically and at different X positions (70 vs centered). But if the content above pushes `y` far down, both buttons may be off-screen.
- [ ] **God abilities text can be very long** — `Game.cpp:3984`: Ability name + description at x=90. With long descriptions, text extends to the right edge.

### StateShop (~lines 4012-4219)

- [ ] **Shop item names truncate but Buy button can overlap** — `Game.cpp:4063-4067`: Names truncated at 190px. Buy button at `xLeft + 220 = 290`. Item name starts at `xLeft + 24 = 94`, so max name width is `290 - 94 = 196px`. The 190px truncation is correct, but the icon at `xLeft` takes 20px, so effective name start is `xLeft + 24 = 94`. The Buy button at `xLeft + 220 = 290` means the name can extend to x=286. The truncation at 190px is measured from the text start (94), so max text end is `94 + 190 = 284`. Close to the button but OK.
- [ ] **Quick-sell buttons overlap with item list** — `Game.cpp:4155-4161`: Three quick-sell buttons at y=qsy, then `y += 28`. Item list starts at the new y. If there are 0 items, "No items to sell" is shown. But the quick-sell buttons are 22px tall and the gap to the item list is 28px, leaving only 6px gap.
- [ ] **Sell-side pagination overlaps item list** — `Game.cpp:4207`: Nav at `y + (sellEnd - sellStart) * 30 + 4`. If the last item on the page has a long name, the pagination buttons can overlap with it.

### StateAreaSelect (~lines 4222-4381)

- [ ] **Area names overlap node circles** — Node radius is 28px. Area names are drawn at `cy + nodeRadius + 4 = cy + 32`. Difficulty at `cy + 46`. For closely spaced nodes (e.g., row 0 at y=160: nodes at x=200, 512, 824), the names are far apart. But for row 1 at y=300: nodes at x=356, 668 — names are closer. "Dungeon of Shadows" is a long name that could extend into the neighboring node's area.
- [ ] **Tooltip can overlap nodes** — `Game.cpp:4371-4374`: Tooltip at `positions[hoveredNode].y + 34`, clamped to not go below `H - 80`. For bottom-row nodes (y=580, 700), the tooltip would need to go upward, which could overlap the node above.
- [x] **"Forbidden Citadel" node at y=700 is near the bottom** — Name at `700 + 28 + 4 = 732`, difficulty at `746`. Screen height is 768. Only 22px of margin below the difficulty text. The Back button at `GRenderer::H - 60 = 708` overlaps with the Citadel node at y=700. **Fixed**: Moved Citadel node up to y=660.

### StateQuestLog (~lines 4384-4567)

- [ ] **Quest rows can overlap with pagination** — Each quest row is 60px. 7 per page = 420px. Starting at y=145, ending at `145 + 7*60 = 565`. Pagination at `GRenderer::H - 80 = 688`. Gap is 123px — OK. But the "Claim" button at `GRenderer::W - 260` can extend to the right edge.
- [ ] **"Visit NPC to accept" text can overlap with quest description** — `Game.cpp:4534`: Fixed at `GRenderer::W - 260 = 764`. If the quest description is long, it extends toward x=764, potentially overlapping.

### StatePets (~lines 6965-7171)

- [ ] **Pet cards at 56px each, 8 per page = 448px** — Starting at y=148, ending at `148 + 8*56 = 596`. Pagination at `navY = GRenderer::H - 130 = 638`. Gap is 42px — tight but OK.
- [ ] **"Equip"/"Unequip"/"Info" buttons overlap with source label** — `Game.cpp:7110-7135`: Source label at `GRenderer::W - 250 = 774`, buttons at `GRenderer::W - 240 = 784` and `GRenderer::W - 150 = 874`. The source label is drawn at y+3, buttons at y+10. They're at the same X range but 7px vertically apart. The source text could extend into the button area.

### StatePetDetail (~lines 7594-7751)

- [ ] **Pet detail page is very long** — Header (80px) + stats (10 × 15 = 150px) + ability (30px) + skill tree (3 branches × 5 nodes × 14px = 210px) = ~470px. Starting at y=80, ending at ~550. Back button at `GRenderer::H - 60 = 708`. Gap is 158px — OK.
- [x] **Skill tree unlock buttons at 18px height can overlap** — `Game.cpp:7728-7733`: Buttons are 18px tall with no gap between nodes. Adjacent nodes are at `y += 18`, so they're exactly touching. No visual gap between skill tree entries. **Fixed**: Increased spacing to 16/20/16px for text/buttons/text, 6px between branches.

### StateJobQuests (~lines 7175-7263)

- [ ] **Quest entries at ~80px each** — Title (18) + description (16) + progress (16) + rewards (16) + button (24) = ~90px. 5 per page at 90px = 450px. Starting at y=126, ending at ~576. Back button at `GRenderer::H - 70 = 698`. OK.
- [ ] **"Accept"/"Claim" buttons at fixed X=460** — `Game.cpp:7247-7253`: If the rewards text is long, it can extend toward x=460.

### StateResourceChain (~lines 7267-7352)

- [ ] **Recipe entries at ~70px each** — Name (18) + description (16) + ingredients (16) + result (18) + gap = ~70px. 5 per page at 70px = 350px. Starting at y=110, ending at ~460. Pagination at `GRenderer::H - 110 = 658`. OK.
- [ ] **"Craft" button at X=460** — `Game.cpp:7323`: Same X as JobQuests. Long result names can extend into the button.

### StateFactions (~lines 6500-6919)

- [ ] **Faction list entries at 58px each, 5 per page = 290px** — Starting at y=140, ending at ~430. Pagination at varying Y. OK.
- [ ] **Faction detail view is very long** — Overview tab: rewards list + bonuses. Repeatable quests tab: 3-4 quests at ~120px each = 360-480px. Shop tab: items at ~64px each. All can overflow with many entries.
- [ ] **Back button can overlap with pagination** — Faction detail Back at `GRenderer::H - 80 = 688`. If pagination is at `y + 36`, and the last quest ends near y=650, the pagination and Back button can be very close.

### StateEvolution (~lines 7424-7592)

- [ ] **Requirement checklist can be long** — 4 requirements at 20px each = 80px. EVOLVE button at y+varies. Master Class section adds another ~120px. Total ~300px from y=120, ending at ~420. OK.
- [ ] **"Requirements not met" text overlaps EVOLVE button area** — `Game.cpp:7582`: Drawn at y where the button would be. Not an overlap but visually confusing.

### StateEscort (~lines 7364-7422)

- [ ] **Simple state, no overlap issues** — Short content, single quest display. Back button at `GRenderer::H - 70 = 698`. OK.

### StateMasterClass (~lines 7355-7361)

- [ ] **Redirects to StateEvolution** — No UI of its own.

---

## Priority Summary

| Priority | Items | Impact |
|----------|-------|--------|
| P0 - Critical | Game.cpp split, dynamic_pointer_cast removal, save/load safety | Architecture, maintainability, crash prevention |
| P1 - High | shared_ptr -> unique_ptr, string allocation in draw loops, dead code removal, combat UI overlap, skill loss bug | Performance, bug fixes, memory |
| P2 - Medium | Loot system consolidation, unique item validation, build optimization, layout constants, UI overlap fixes (combat, skill loadout, inventory overflow) | Code quality, balance, build, usability |
| P3 - Low | Graphics resolution, texture management, RNG optimization, Wiki split, remaining UI overlap fixes | Polish, long-term |
