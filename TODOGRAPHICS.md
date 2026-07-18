# TODOGRAPHICS — Photographic Battle Screen

## Goal
Replace the current procedural-shape combat screen with a cinematic, photography-based
battle UI inspired by the goat.jpeg reference. The player should see:

- A **full-width landscape photo** as the top background
- A **centered creature photo** overlaid on the landscape
- A **dark vignette** around the edges focusing attention on the creature
- The **lower half fades to black** for the battle interface
- **Serif font** creature name centered below the creature
- **Red HP text** below the name
- **White battle message** floating text
- **Glossy square ability buttons** with orange glow and painterly icons
- **Player HP/Energy** at the very bottom
- Generous negative space, clean readable UI

## Art Style Summary

| Layer | Style |
|---|---|
| Background | Desaturated alpine photograph |
| Creature | Full-color photographic cutout or centered photo |
| Vignette | Dark edges, cinematic focus |
| UI elements | High-saturation, glossy, orange/white/red on black |
| Fonts | Serif (creature name), Sans-serif (UI text) |

---

## Directory Structure (user provides images)

```
assets/
├── creatures/          # One JPEG/PNG per enemy type
│   ├── goat.jpeg       # Example: "Flozzlegoat"
│   ├── wolf.jpeg
│   ├── skeleton.jpeg
│   ├── dragon.jpeg
│   └── ...
├── backgrounds/        # One JPEG/PNG per area
│   ├── alpine.jpeg
│   ├── forest.jpeg
│   ├── desert.jpeg
│   ├── dungeon.jpeg
│   └── ...
├── ui/                 # Ability button icons (square, ~64x64 PNG)
│   ├── attack.png      # Red/orange fist icon
│   ├── flare.png       # Fireball icon
│   ├── heal.png        # Green/white heal icon
│   ├── defend.png      # Shield icon
│   └── ...
└── fonts/              # TTF fonts
    ├── serif.ttf       # Georgia/Trajan-style for creature names
    └── sans.ttf        # Clean sans-serif for UI (or use raylib default)
```

### Naming Convention
- Creature images are matched to enemy names via lowercase substring:
  - `"Flozzlegoat"` → looks for `assets/creatures/goat.jpeg`
  - `"Shadow Wolf"` → looks for `assets/creatures/wolf.jpeg`
  - `"Fire Dragon"` → looks for `assets/creatures/dragon.jpeg`
  - Falls back to procedural shape if no image found
- Background images are matched to area names the same way

---

## Implementation Tasks

### Phase 1: Texture Infrastructure

#### 1.1 Create TextureManager class
- **File:** `Graphics/TextureManager.hpp`, `Graphics/TextureManager.cpp`
- [x] Singleton or member of GRenderer
- [x] Methods: `Load`, `Get`, `Has`, `UnloadAll`
- [x] Uses raylib's `LoadTexture()` / `UnloadTexture()`
- [x] Stores in `std::unordered_map<std::string, Texture2D>`
- [x] Handles missing files gracefully

#### 1.2 Add texture drawing to GRenderer
- **File:** `Graphics/GRenderer.hpp`, `Graphics/GRenderer.cpp`
- [x] New methods: `DrawTexture`, `DrawTextureCentered`, `DrawTextureFit`, `DrawTextureCropped`
- [x] Add `TextureManager textureManager` member

#### 1.3 Create assets/ directory structure
- [x] Create empty directories: `assets/creatures/`, `assets/backgrounds/`, `assets/ui/`, `assets/fonts/`

---

### Phase 2: Photo Effects

#### 2.1 Vignette overlay
- **File:** `Graphics/PhotoEffects.hpp`, `Graphics/PhotoEffects.cpp`
- [x] Function: `DrawVignette` with edge gradients and bottom fade to black

#### 2.2 Background desaturation
- [x] Function: `Desaturate` using pixel-level grayscale lerp
- [x] Cache support via `GetOrCreateDesaturated`

#### 2.3 Background contrast boost
- [x] Function: `ApplyContrast` with pixel-level contrast adjustment

---

### Phase 3: New Battle Screen Layout

#### 3.1 Define BattleLayout struct
- **File:** `Graphics/BattleLayout.hpp`, `Graphics/BattleLayout.cpp`
- [x] Struct with photo area, creature position, UI positions
- [x] `Calculate(screenW, screenH)` method

#### 3.2 New combat drawing function
- **File:** `Graphics/BattleRenderer.hpp`, `Graphics/BattleRenderer.cpp`
- [x] `DrawBattleScreen` function with all layers
- [x] Background photo with gradient fallback
- [x] Creature photo with bobbing animation
- [x] Vignette overlay
- [x] Creature name, HP, battle message
- [x] Combat log, player bar

#### 3.3 Glossy button renderer
- [x] `DrawAbilityButton` with gradient background, orange border, hover glow

---

### Phase 4: Font Loading

#### 4.1 Add font support to GRenderer
- [x] `Font serifFont`, `Font sansFont` members
- [x] Methods: `DrawTextFont`, `DrawCenteredTextFont`, `GetSerifFont`, `GetSansFont`
- [x] Fallback to default font if files not found

---

### Phase 5: Creature Image Matching

#### 5.1 Enemy-to-image mapping
- **File:** `Graphics/CreatureImageMapper.hpp`, `Graphics/CreatureImageMapper.cpp`
- [x] `GetCreatureTexture` with prefix stripping and keyword matching
- [x] `GetBackgroundTexture` for area backgrounds

#### 5.2 Procedural fallback
- [x] Falls back to `DrawEnemyIcon()` when no image found

---

### Phase 6: Integration

#### 6.1 Refactor StateCombat
- **File:** `Engine/Game.cpp`
- [x] Extract combat drawing into `BattleRenderer::DrawBattleScreen()`
- [x] Keep combat logic in Game.cpp
- [x] Updated button positioning to use BattleLayout

#### 6.2 Victory/Defeat screens
- [x] Victory uses new photo-based rendering
- [x] Defeat uses new photo-based rendering

#### 6.3 Wiki enemy entries
- [ ] Update `Wiki.cpp` to use creature photos in detail panel

---

### Phase 7: Polish

#### 7.1 Particle effects on buttons
- [ ] Hover sparkle particles
- [ ] Use burst particles

#### 7.2 Creature breathing animation
- [x] Subtle sine wave bob on creature photo

#### 7.3 Background parallax (optional)
- [ ] Slight horizontal shift based on mouse position

---

## File Changes Summary

| Action | File |
|---|---|
| **NEW** | `Graphics/TextureManager.hpp` |
| **NEW** | `Graphics/TextureManager.cpp` |
| **NEW** | `Graphics/PhotoEffects.hpp` |
| **NEW** | `Graphics/PhotoEffects.cpp` |
| **NEW** | `Graphics/BattleLayout.hpp` |
| **NEW** | `Graphics/BattleRenderer.hpp` |
| **NEW** | `Graphics/BattleRenderer.cpp` |
| **NEW** | `Graphics/CreatureImageMapper.hpp` |
| **NEW** | `Graphics/CreatureImageMapper.cpp` |
| **NEW** | `assets/creatures/` (directory) |
| **NEW** | `assets/backgrounds/` (directory) |
| **NEW** | `assets/ui/` (directory) |
| **NEW** | `assets/fonts/` (directory) |
| **MODIFY** | `Graphics/GRenderer.hpp` — add texture/font support |
| **MODIFY** | `Graphics/GRenderer.cpp` — implement texture/font methods |
| **MODIFY** | `Graphics/IconRenderer.cpp` — keep as fallback |
| **MODIFY** | `Engine/Game.cpp` — refactor StateCombat to use BattleRenderer |
| **MODIFY** | `CMakeLists.txt` — add new .cpp files to SOURCES |

---

## What the User Needs to Provide

1. **Creature photos** — One JPEG/PNG per enemy type, placed in `assets/creatures/`
   - Recommended: 512x512 or larger, square-ish aspect ratio
   - The photo can be any style (realistic, illustrated, AI-generated)
   - Naming: `goat.jpeg`, `wolf.jpeg`, `skeleton.jpeg`, etc.

2. **Background photos** — One JPEG/PNG per area, placed in `assets/backgrounds/`
   - Recommended: 1024x600 or wider (landscape orientation)
   - Naming: `alpine.jpeg`, `forest.jpeg`, `desert.jpeg`, `dungeon.jpeg`, etc.

3. **UI icons** (optional) — Square PNGs for ability buttons, placed in `assets/ui/`
   - Recommended: 64x64 PNG with transparency
   - Naming: `attack.png`, `flare.png`, `heal.png`, `defend.png`
   - If not provided, buttons will use colored shapes as placeholders

4. **Fonts** (optional) — TTF files in `assets/fonts/`
   - `serif.ttf` — for creature names (Georgia, Times New Roman, Trajan, etc.)
   - `sans.ttf` — for UI text (Arial, Helvetica, etc.)
   - If not provided, raylib's default font is used

---

## Priority Order

1. **Phase 1** (Texture Infrastructure) — Foundation, must come first
2. **Phase 3** (Battle Screen Layout) — Core visual result
3. **Phase 4** (Font Loading) — Needed for proper text rendering
4. **Phase 2** (Photo Effects) — Vignette and desaturation
5. **Phase 5** (Creature Image Matching) — Auto-mapping images to enemies
6. **Phase 6** (Integration) — Wire everything together
7. **Phase 7** (Polish) — Nice-to-have enhancements
