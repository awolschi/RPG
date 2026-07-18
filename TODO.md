# TODO

## 1. Keyboard Controls (Full Keyboard Navigation)

### 1.1 Core Infrastructure
- [x] Create a `KeyboardNav` manager class to track focused element per screen
- [x] Define a focus index system: each interactive element gets a sequential index
- [x] Add focus highlight rendering (e.g., glowing border around focused element)

### 1.2 Navigation Keys
- [x] Arrow keys (Up/Down/Left/Right) to move between focusable elements
- [x] Tab / Shift+Tab to cycle through elements
- [x] Enter / Space to activate the focused button
- [x] Escape to go back (close panel, return to previous screen)
- [x] Number keys (1-9) as shortcuts for inventory tabs, skill slots, action menus

### 1.3 Combat Screen
- [x] Arrow keys to select between Attack, Skills, Items, Defend, Flee
- [x] Number keys (1-4) to select skill slots during SkillSelect phase
- [x] Arrow keys to navigate enemy targets during multi-target skills
- [x] Enter to confirm selection

### 1.4 Inventory / Equipment Screen
- [x] Arrow keys to navigate item grid/list
- [x] Tab to switch between All / Weapons / Armor / Accessories / Consumables / Other tabs
- [x] Enter to equip/use, Delete key to drop
- [x] Arrow keys to select equipment slots in comparison view

### 1.5 World / Travel Screen
- [x] Arrow keys or Up/Down to select area from travel list
- [x] Enter to travel to selected area

### 1.6 Menu Screens (Main Menu, Save/Load, Religion, Crafting, etc.)
- [x] Arrow keys to navigate menu options
- [x] Enter to select
- [x] Number shortcuts where applicable

### 1.7 Wiki / Codex Screen
- [x] Arrow keys to navigate between tabs (Equipment, Skills, Enemies, etc.)
- [x] Arrow keys to paginate and select entries
- [x] Enter to open detailed entry view

---

## 2. Skill Overview Pages

### 2.1 Skill List Pagination
- [x] Add page-based pagination to the skill overview/management screen
- [x] Show N skills per page (e.g., 8-10 per page, depending on layout)
- [x] Display "Page X of Y" indicator

### 2.2 Navigation Controls
- [x] Left/Right arrow keys (or buttons) to move between pages
- [x] Show page number indicator (e.g., "1 / 3")
- [x] Disable previous button on first page, next button on last page

### 2.3 Skill Detail View
- [x] Clicking/Enter on a skill expands to show full details (damage, mana cost, cooldown, element, description)
- [x] Show upgrade tree preview inline
- [x] Show current skill points available and equipped status

### 2.4 Visual Improvements
- [x] Class-colored skill cards (Warrior=red, Mage=blue, etc.)
- [x] Highlight skills that are ready to level up or have unspent skill points
- [x] Show equipped skills with a badge/marker

---

## 3. Edible Fish (Fishing Catch → Consumable Healing)

### 3.1 Resource → Consumable Conversion
- [x] Define fish resource items with HP heal values (added to Resource class)
- [x] Assign heal amounts per fish rarity: Common fish = small heal, Rare fish = medium, etc.
- [x] Create healing logic: use fish item → restore HP amount

### 3.2 Fish Types & Heal Values
- [x] Define 5 fish types with increasing heal potency:
  - [x] Raw Fish (Tier 1) → heal 20 HP
  - [x] Salmon (Tier 2) → heal 40 HP + 8 MP
  - [x] Tuna (Tier 3) → heal 70 HP + 15 MP
  - [x] Exotic Fish (Tier 4) → heal 120 HP + 30 MP
  - [x] Legendary Fish (Tier 5) → heal 200 HP + 50 MP

### 3.3 UI Integration
- [x] Fish appear in the Resources inventory tab with "Eat" button
- [x] Fish are usable from the inventory screen ("Eat" button)
- [x] Fish are usable during combat via the "Use Item" action
- [x] Fish icon uses existing procedural IconRenderer (Fish shape)

### 3.4 Balance
- [x] Fish healing is balanced relative to potion healing
- [x] Cooked fish heals more than raw fish (optional)

---

## 4. Clickable Codex / Wiki Enemies

### 4.1 Enemy Entry Expansion
- [x] Make each enemy entry in the Wiki's Enemies tab clickable
- [x] Clicking opens a detailed enemy info panel

### 4.2 Enemy Detail Panel Content
- [x] Enemy name and portrait (using existing IconRenderer shapes)
- [x] Stats breakdown (HP, MP, STR, DEF, XP, Gold)
- [x] Loot table overview (drop tiers, special drops)
- [x] Which areas/dungeons this enemy appears in

### 4.3 Strategy Info
- [x] Strategy hints based on enemy type (mage, elemental, dragon, void, boss, etc.)
- [x] Recommended gear approach per enemy category

### 4.4 UI Polish
- [x] Modal popup for detailed view with semi-transparent overlay
- [x] Back button to return to the enemy list
- [x] Keyboard navigation: Enter to open, Escape to close
- [x] Highlight enemies the player has defeated vs. undiscovered

---

## 5. Better Graphics

### 5.1 Procedural Texture Improvements
- [x] Enhance GRenderer panel rendering with subtle gradients and inner shadows
- [x] Add button states: normal, hover, pressed (visual feedback with text offset)
- [x] Improve health/mana/XP bars with gradient highlights and edge shadows

### 5.2 Enemy Portrait Improvements
- [x] Expand IconRenderer with more enemy shape variants (currently ~10 shapes)
- [x] Add idle animation (subtle floating/bobbing effect)
- [x] Add hit animation (flash white on damage taken)
- [x] Add defeat animation (fade out + particles on death)
- [x] Boss-specific visual flair (glowing outline, pulsing effect)

### 5.3 Item Icon Improvements
- [x] Add glowing border for Epic/Legendary items in inventory
- [x] Rarity-colored item frames (border around item icon matches rarity)
- [x] Animated sparkle/glow for Legendary items
- [x] Add weapon type visual variety (different sword shapes, etc.)

### 5.4 UI Enhancements
- [x] Screen transition effects (fade in/out between screens)
- [x] Panel slide-in animations when opening menus
- [x] Subtle background patterns for different areas (e.g., forest bg for Dark Woods)
- [x] Tooltip popups on hover (show item stats, skill details, etc.)

### 5.5 Particle Effects
- [x] Particle burst on enemy defeat (gold burst)
- [x] Healing particles when using potions/fish (green rising particles)
- [x] Level-up visual effect (golden flash, text popup) — via floating text
- [x] Critical hit visual flash (yellow burst)
- [x] Damage particles on hit (red burst)
- [x] Status effect indicators (poison bubbles, burn flames, etc.)

### 5.6 Font & Text
- [ ] Load a custom fantasy-style font (or use a bundled TTF)
- [x] Better text shadowing/outline for readability over backgrounds
- [x] Animated damage numbers (floating up and fading on hit)

---

## 6. Equipment Overhaul (see todoequip.md)
- [ ] Refer to separate todoequip.md for the full equipment system plan
