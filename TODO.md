# RPG Project — TODO

## Quick Wins (high impact, low effort)

- [x] **1. Add a Shop** — Buy/sell UI in town; potions and gear for sale.

- [x] **2. Fix Religion** — Karosh devotion now sets `attackBonus` on Character, integrated into `GetWeaponDamage()`.

- [x] **3. Save Quest Progress** — Save/load now persists area index, religion state, and full quest data.

- [x] **4. Show Offhand Slot** — Added to inventory equipment display.

- [x] **5. Add Consumables** — `Consumable` class with health/mana potions, dropped as loot and sold in shop.

- [x] **6. Make enemy loot visible** — Combat log shows loot on victory; item names appear with rarity icons.

- [x] **10. Remove the duplicate Fireball** — Deleted `Mage/Fire/Fireball.cpp` and `.hpp`.

## Medium Improvements

- [x] **7. Status effects** — Poison (DoT), Burn (DoT), Stun (skip turn), Freeze (skip turn). Archer PiercingShot poisons, Mage Fireball burns, LightningBolt stuns, FrostNova freezes. Effects shown in status line and combat log.

- [x] **8. Enemy AI variety** — Every enemy uses the same random attack/skill/defend AI. Giving different enemy types different behaviors (e.g., beasts always attack, mages always use skills) would add tactical depth.

- [x] **9. Equipment comparison** — When clicking an item in inventory, show the currently equipped item's stats alongside it.

- [x] **11. Make skill `requiredLevel` work** — Skills have `requiredLevel` but it's not enforced by the skill system; instead `Player::InitializeClassSkills()` uses `if (level >= X)` blocks. Centralizing this would be cleaner.

## Bigger Features

- [x] **13. Quest givers** — Added NPC dialogue system with quest acceptance/completion. NPCs placed in each area with unique dialogue.

- [x] **14. Elemental types** — Fire/Ice/Lightning are just names. An actual rock-paper-scissors resistance system would make skill choice matter.

---

*Generated from codebase analysis on 2026-07-09.*
