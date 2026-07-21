# AGENTS.md — RPG Project

## Build & Run

```bash
mkdir -p build && cd build
cmake .. && make -j$(nproc)
./rpg
```

- C++17, CMake 3.15+, **no external deps** aside from raylib (fetched via FetchContent, raylib 5.0).
- `-no-pie` linker flag is **required** to avoid static-library link failures with raylib.
- Include paths use the project root (`target_include_directories(rpg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})`). All `#include` paths are relative to the repo root (e.g. `#include "Characters/Player.hpp"`).

## Architecture

- **Entry**: `Main.cpp` instantiates `Game` and calls `Run()`.
- **Monolith**: `Engine/Game.hpp`/`Engine/Game.cpp` (~6000 lines) contains the game loop, all 25+ state methods, rendering, input, and game logic in one class.
- **States**: `GameState` enum drives a state machine (MainMenu, InCombat, Inventory, etc.). Each `State*()` method mixes rendering + input + logic.
- **Raylib**: All drawing goes through `Graphics/GRenderer.hpp`/`.cpp` wrapper. Graphics subsystem: `TextureManager`, `PhotoEffects`, `BattleRenderer`, `BattleLayout`, `IconRenderer`, `CreatureImageMapper`.
- **Memory**: Heavy use of `std::shared_ptr`. Characters, items, enemies are all shared.
- **Save/Load**: Pipe-delimited text format in `Engine/SaveGame.cpp`. Uses `SafeStoi()` helper for safe integer parsing. Save version is v12 (persisting gatherItemName, religion quest details, item requiredLevel).
- **RNG**: New code uses `Engine/RNG.hpp` (header-only `std::mt19937` wrapper). Legacy `rand()`/`srand()` still present in older code; do not add more.

## Editing Patterns

- When adding a new `.cpp` file, register it in `CMakeLists.txt` under `SOURCES`.
- When adding rendering, use `GRenderer` methods — do not call raylib drawing functions directly from Game.cpp.
- New game states go in the `GameState` enum and get a `State*()` method in Game.cpp. New subsystems are wired via member variables in `Game`.
- New header-only types (structs, enums) can go directly in the relevant `.hpp`. Implementation-heavy classes need both `.hpp` and `.cpp`.

## Notable Gotchas

- `Game.cpp` is massive — avoid adding code unrelated to the change. Consider extracting new features into their own files and calling them from the state method.
- `dynamic_pointer_cast` in hot paths (Combat, Equipment). Prefer `ItemType` enum checks on the base rather than RTTI casts.
- Raw `rand()`/`srand()` used throughout. Do **not** add more; use `std::mt19937` + `<random>` for new RNG.
- Save/Load has ~50+ unchecked `std::stoi()` calls. Corrupted saves crash the game. Validate inputs when touching serialization code.
- `ItemType::Offhand` was added at the end of the enum (value 6) to preserve existing save format values. Old saves with offhands (saved as type 0 with "OH" subtype tag) still load correctly.
- `assets/` directories exist for creature/background photos, UI icons, and fonts. Images are matched to enemy/area names via substring, with procedural-shape fallback.
- `saves/` directory stores save slot files.

## Documentation Files

- **`list.md`**: Most up-to-date bug list and improvement notes. Authoritative for known issues.
- **`TODO.md`**: Completed task checklist (keyboard nav, skill pagination, fish, wiki, graphics).
- **`TODOGRAPHICS.md`**: Battle screen photo-based rendering spec — mostly implemented, Wiki enemy photos still pending.
- **`todoequip.md`**: Equipment overhaul plan (rarity, uniques, sets, passives) — large design doc, partially built.
- **`jobtodo.md`**: Job system overhaul plan with completion status.
- **`BUILD_GUIDE.md`**, **`README.md`**: Project overview and build instructions (somewhat stale vs. current codebase).
- **`SKILLREADME.md`**: Skill XP/upgrade tree design notes.
- **`IMAGETODO.md`**: Image asset checklist with AI prompt templates.
- **`notes.md`**: Quick bug notes (skill on standard attack, save-file skill loading, UI overlaps).
