# Work Log — list.md Remaining Items

## Actual Status (corrected 2026-07-21)

### Verified DONE (in code)
- **1a** (list.md 49,52,55,75,76): Stale claims checked off
- **1b** (36): Save confirmation dialog (`saveConfirmSlot` in Game.hpp)
- **1c** (38): Enum range validation via `SafeCastEnum` helper in SaveGame.cpp
- **1d** (50): Area encounter rest/choice — "Keep Fighting" / "Return to Camp" buttons at Game.cpp:4110,4130
- **1e** (51): Fatigue natural decrease — `ReduceAllFatigue(10)` wired to area entry in Game.cpp
- **1g** (54): Set bonus 3-piece tier — `threePiece` in SetBonuses.hpp, bonuses for 3 sets
- **1h** (69): Synergy consolidation — `IsSynergyActive` helper in JobSystem.hpp
- **1i** (89): Death punishment — 10% gold + 10% XP loss on defeat, respawn at area with 50% HP/MP (Game.cpp)
- **2a** (63): Serialization helpers — `SerializeItem`/`DeserializeItem` in SaveGame.cpp
- **2b** (65): Full rand()→RNG migration — zero `rand()`/`srand()`/`<cstdlib>` remaining
- **2c** (85): Merchant-specific uniques — 8 legendaries + 11 epics in UniqueItemRegistry
- **2d** (86): Achievement item rewards — 9 items wired to achievement unlock (CheckAchievementNotifications)
- **2e** (48): Inventory capacity limit — base 20 capacity, `IsFull()` check, Bag offhand bonuses
- **2f** (84): Crafting recipe gating — `requiredJobLevel`/`requiredJob` on all 41 recipes
- **Class-specific items**: Warrior (10 leg + 10 epic), Priest (10 leg + 10 epic), Archer (10 leg + 10 epic), Merchant (8 leg + 11 epic)
- **Class-specific sets**: Bulwark (Warrior, 14), Redeemer's Light (Priest, 15), Deadeye (Archer, 16), Trade Baron's (Merchant, 17)
- **Shop stock** (partial): `shopItems.erase` on purchase at Game.cpp:3458; `shopItems.clear()` at inn rest (3562)

---

## Session History

### Session 1 (completed)
- Items 1-21 in list.md (marked [x]): gatherItemName save, religion quest persistence, prayer devotion, Arcane Crystal drops, quest targets Zombie→Wraith, Pet::CanEvolve, Offhand ItemType, DrawRectLines thick, WorkJob alias, loot rates+class-appropriate, flee chance, Defend scaling, cout→AddCombatLog, statics→members, dead init removal, SafeStoi, XP consolidation, build verified.
- Created `Engine/RNG.hpp` (header-only mt19937 wrapper)
- Updated AGENTS.md

### Session 2 — Phase 1 (partial)
- **1a**: Checked off stale claims
- **1b**: Save confirmation dialog
- **1c**: SafeCastEnum helper + enum validation
- **1d**: Area encounter rest/choice (victory phase buttons)
- **1g**: 3-piece set bonus tier
- **1h**: Synergy consolidation
- **2a**: SerializeItem/DeserializeItem helpers

### Session 2 — Phase 2b
- Migrated all remaining `rand()` calls: Game.cpp (14 sites), Combat.cpp (1), Loot.cpp (~35), Resources.cpp (1), Passives.cpp (1), Area.cpp (2), Pet.cpp (3)
- Removed `srand()` call from Game.cpp constructor
- Removed all `<cstdlib>` includes from 11 files
- Build verified clean

### Session 3 (THIS SESSION)
- **1e**: `ReduceAllFatigue(int)` in JobSystem, wired to area entry (-10 per travel)
- **1i**: Death penalty — 10% gold + 10% XP loss on defeat, respawn at area with 50% HP/MP
- **2c**: 8 Merchant-specific legendary items (Coinlord's Cudgel, Auctioneer's Blade, Ledger of Endless Wealth, etc.)
- **2d**: 9 achievement reward items + `CheckAchievementNotifications()` wiring
- **2e**: Inventory capacity (base 20, Bag offhand bonus = defense*2)
- **2f**: Crafting recipe job-level gating (41 recipes, `requiredJobLevel`/`requiredJob` fields)
- **3a**: SoundManager.hpp/cpp — procedural audio system via raylib Wave API (15 sound effects)
- **3b**: World map — node graph in StateAreaSelect (10 areas, 4 rows, color-coded, click-to-travel, connections)
- **3c**: Tutorial — session-scoped hints (8 triggers: explore, combat, inventory, travel, quests, crafting, skills, inn rest)
- **Class-specific items**: Warrior 10 leg + 10 epic, Priest 10 leg + 10 epic, Archer 10 leg + 10 epic, Merchant 8 leg + 11 epic
- **Class-specific sets**: Bulwark (14), Redeemer's Light (15), Deadeye (16), Trade Baron's (17)
- **Area connections**: Added `connections` vector to Area class, defined adjacency graph for all 10 areas
- **GRenderer**: Added DrawCircle, DrawCircleLines, DrawLine primitives
- Build verified clean after all changes

### Session 3 continued — Skill Expansion
- **Warrior**: 17 new skills added (Cleave, Shield Wall, Rallying Cry, Hammer Slam, Victory Rush, Charge, Cleave Strike, Intimidate, Berserker Rage, Intercept, Fortress, Shield Charge, Devastate, Warbanner, Colossus Strike, Rampage, Titan's Grip) — now 25 total
- **Priest**: 23 new skills added (Prayer of Mending, Cleansing Light, Holy Smite II, Radiance, Smite Undead, Barrier, Prayer of Healing, Purify, Benediction, Holy Fire, Consecrated Ground, Serenity, Archangel's Blessing, Judgement, Ascension, Exorcism, Divine Hurricane, Penance, Rapture, Holy Prism, Grace of God, Celestial Judgment, Apotheosis) — now 31 total
- **Archer**: 24 new skills added (Poisoned Arrow, Evade, Aimed Shot, Volley, Trap Shot, Hawk Eye, Exploit Weakness, Concussive Shot, Camouflage, Double Tap, Scatter Shot, Mark Target, Headshot, Barrage, Shadowmeld, Storm of Blades, Evasive Maneuvers, Rapid Fire, Windrunner, Piercing Barrage, Shadow Step, Wind Arrow, Eagle's Wrath, Phantom Volley) — now 31 total
- **Merchant**: 24 new skills added (Purchase Order, Assess Weakness, Price Check, Market Swing, Bulk Deal, Liquidate, Hostile Takeover, Insurance Policy, Short Sell, Predatory Lending, Gold Reserves, Margin Call, Monopoly, Market Crash, Taxation, Plunder, Corner Market, Fortune's Favor, Hostile Bid, Economic Collapse, Monopolize, Hostile Merger, Golden Handshake, Legendary Bargain) — now 31 total
- All 88 new skills wired into `Player::InitializeClassSkills()` and `Player::CheckNewSkills()` — 120 total `addIfMissing` calls
- Skills follow existing patterns: `Use()`, `GetDescription()`, `Upgrade()` — each has unique mechanical identity
- Build verified clean

---

## Remaining Work (verified in code)

### Medium Priority
- **3d** (87): Escort quests (destination area, completion on travel)

### Large Features (completed)
- **3a** (56): Audio system — SoundManager.hpp/cpp (procedural tones via raylib Wave)
- **3b** (57): World map — node graph in StateAreaSelect with connections
- **3c** (58): Tutorial — session-scoped hints with 8 contextual triggers

### Tier D (deferred)
- Game.cpp decomposition (62)
- Event-driven notifications (81)
- Structured save format (82)
- Central config system (83)
- Magic numbers (64)
- String concat optimization (73)
- dynamic_pointer_cast optimization (74)
- NG+/BossRush/MasterClass modes (88)
