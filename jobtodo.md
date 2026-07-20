# Job System Overhaul — Making Jobs Fun & Engaging

## Current Problems

- Jobs are just "click button → get resources" with no strategy
- No visual feedback on progress (just text numbers)
- No risk/reward mechanics
- No meaningful choices (just work until max level)
- Resources pile up with no real purpose
- No replayability after maxing out

---

## 1. Visual Progress Bars & XP System

### 1.1 XP Progress Bar for Each Job
- [x] Add visual XP progress bar in Jobs screen (like character XP bar)
- [x] Show "Mining Lv.5 ████░░░░░░ 120/350 XP"
- [x] Animated fill when gaining XP
- [x] Color-coded bars per job (Mining=brown, Lumberjacking=green, Fishing=blue, Smithing=orange)
- [x] Show XP gain floating text when working (+15 Mining XP!)

### 1.2 Level-Up Celebration
- [x] Golden flash effect on level up
- [x] Floating text: "Mining Level 6!"
- [x] Show new perks unlocked at this level
- [x] Sound effect placeholder (visual feedback for now)

### 1.3 Milestone Indicators
- [x] Show next milestone at levels 5, 10, 25, 50, 75, 100
- [x] Milestones unlock new resource types or recipes
- [x] Preview what you'll unlock at next level

---

## 2. Job Working Mechanics Overhaul

### 2.1 Instead of Instant Collection, Add "Work Shifts"
- [x] Each "Work" action represents a 1-hour shift
- [ ] Player chooses how many hours to work (1-8) — currently fixed at 3 or 5
- [x] More hours = more resources but fatigue builds
- [x] Fatigue reduces efficiency after threshold (reduces sell value and XP)
- [x] Rest at Inn resets fatigue

### 2.2 Random Events While Working
- [x] ~17% chance per hour of a random event:
  - **Lucky Find:** Double resources this hour
  - **Rare Discovery:** Find a special rare resource
  - **Tool Break:** Lose 1 hour of progress (can prevent with perks)
  - **Injury:** Take 10% max HP damage (can prevent with perks)
  - **Mentor Visit:** Gain bonus XP
  - **Rival Encounter:** Compete for resources (mini-challenge)
- [ ] Event messages need to be displayed in UI after working

### 2.3 Job Quality System
- [x] Each resource collected has a quality (Normal, Fine, Masterwork)
- [x] Quality affects sell value: Fine = 1.5x, Masterwork = 3x
- [x] Higher job level = higher chance for better quality
- [x] Perks can boost quality chances
- [x] Masterwork items can be used in special crafting recipes

### 2.4 Resource Chains
- [x] Resources can be combined into higher-tier materials:
  - Copper Ore + Copper Ore = Copper Ingot (Smithing)
  - Maple Wood + Maple Wood = Sturdy Plank (Smithing)
  - Raw Fish + Fire = Cooked Fish (better healing)
- [x] Unlock new combinations at higher job levels
- [x] Chain crafting gives bonus XP

---

## 3. Job Specialization Paths

### 3.1 Sub-Class System (Level 5+)
- [x] At level 5, choose a specialization:
  - **Mining → Gemcutter or Blacksmith**
    - Gemcutter: Better rare finds, gem crafting
    - Blacksmith: Better ingots, weapon crafting bonus
  - **Lumberjacking → Carpenter or Herbalist**
    - Carpenter: Better wood, furniture/bow crafting
    - Herbalist: Find herbs, potion ingredients
  - **Fishing → Angler or Oceanographer**
    - Angler: Better fish, unique catches
    - Oceanographer: Find sunken treasures, maps
  - **Smithing → Weaponsmith or Armorsmith**
    - Weaponsmith: Weapon crafting bonuses
    - Armorsmith: Armor crafting bonuses

### 3.2 Specialization Perks
- [x] Each path has unique bonuses (rare find, sell value, double chance, quality, XP)
- [x] Cannot switch specialization (meaningful choice)
- [x] Specialization affects resource collection bonuses
- [x] Specialization UI in Jobs screen with choice buttons at level 5

---

## 4. Job Quests & Challenges

### 4.1 Daily Job Quests
- [x] Each day (real-time or in-game), get 1-2 job quests:
  - "Mine 10 Copper Ore"
  - "Catch 5 Salmon"
  - "Chop 8 Maple Logs"
- [x] Quest rewards: bonus XP, gold, rare materials
- [x] Complete all daily quests for bonus reward
- [ ] UI screen to view/accept/manage job quests (backend only for now)

### 4.2 Job Mastery Challenges
- [x] At mastery ranks (10, 25, 50, 75, 100): special challenge quests
  - Level 10: "Collect 50 resources in one session"
  - Level 25: "Find 5 rare resources"
  - Level 50: "Find 3 Masterwork items"
  - Level 75: "Work 10 hours"
  - Level 100: "Collect 200 resources"
- [x] Mastery challenges unlock exclusive perks
- [ ] Mastery perks not yet defined in JobPerk system (enum exists but unused)

### 4.3 Job Storylines
- [ ] Each job has a mini-storyline:
  - Mining: "The Lost Mine" — discover a hidden vein
  - Lumberjacking: "The Ancient Tree" — find a legendary tree
  - Fishing: "The Big One" — catch a legendary fish
  - Smithing: "The Master's Forge" — craft a legendary weapon
- [ ] Story quests unlock at certain job levels
- [ ] Rewards: unique job-specific items

---

## 5. Job Synergies & Combos

### 5.1 Cross-Job Bonuses
- [x] Mining + Smithing: "Ore to Ingot" bonus (+25% Smithing XP when using mined ores)
- [x] Lumberjacking + Smithing: "Wood & Steel" bonus (+20% crafting speed)
- [x] Fishing + Cooking: "Fresh Catch" bonus (better fish quality)
- [x] Mining + Lumberjacking: "Explorer" bonus (+15% rare finds in both)
- [x] Mining + Fishing: "Deep Prospector" bonus (+10% XP in both jobs)
- [x] Lumberjacking + Fishing: "Nature's Bounty" bonus (+20% resource value)

### 5.2 Combo Actions
- [ ] "Smelt & Smith" — Mine ore then immediately smith it for bonus XP
- [ ] "Chop & Build" — Gather wood then craft for bonus
- [ ] Combos require both jobs at level 3+

### 5.3 Job Party System
- [ ] Hire NPCs to work alongside you (costs gold)
- [ ] NPC workers give passive resource income
- [ ] Higher job level = better NPCs available
- [ ] NPCs can specialize in one job

---

## 6. Job Equipment & Tools

### 6.1 Tool Slots
- [ ] Each job has a tool slot (Pickaxe, Axe, Rod, Hammer)
- [ ] Tools have stats: Efficiency, Durability, Luck
- [ ] Better tools = more resources per hour
- [ ] Tools can break (durability system)

### 6.2 Tool Crafting
- [ ] Smithing job can craft tools for other jobs:
  - Iron Pickaxe: +20% mining efficiency
  - Steel Rod: +15% fishing luck
  - Masterwork Axe: +25% lumberjacking speed

### 6.3 Tool Upgrades
- [ ] Tools can be upgraded at Smithing level 5+
- [ ] Upgrades improve stats but cost resources
- [ ] Legendary tools from special quests

---

## 7. Job Rewards & Unlockables

### 7.1 Resource Shop
- [ ] Sell resources for gold (existing)
- [ ] New: Sell resources for Job Points (alternate currency)
- [ ] Job Points buy exclusive cosmetic items

### 7.2 Job-Exclusive Recipes
- [ ] Each job unlocks unique crafting recipes:
  - Mining: Ore→Ingot→Weapon progression
  - Lumberjacking: Wood→Plank→Bow/Staff progression
  - Fishing: Fish→Cooked Fish→Feast (party buff)
  - Smithing: All metal/wood crafting

### 7.3 Job Cosmetics
- [ ] Unlock titles: "Master Miner", "Grand Lumberjack", etc.
- [ ] Unlock visual effects: mining sparkles, fishing ripples
- [ ] Unlock housing decorations from job resources

### 7.4 Job Mastery Rewards
- [ ] Unlock rewards at each mastery rank:
  - Level 10 (Apprentice): +5% job XP bonus
  - Level 25 (Journeyman): Unlock auto-work feature
  - Level 50 (Expert): Permanent +10% combat stats
  - Level 75 (Master): Access to job-specific dungeons
  - Level 100 (Grandmaster): Unique legendary recipes + cosmetics

---

## 8. Job UI Improvements

### 8.1 Visual Resource Counter
- [ ] Show current resource counts per job type
- [ ] Mining: "Copper: 12, Iron: 5, Silver: 2"
- [ ] Lumberjacking: "Maple: 8, Oak: 4, Ebony: 1"
- [ ] Fishing: "Raw Fish: 10, Salmon: 3, Tuna: 1"
- [ ] Smithing: "Copper Ingot: 6, Iron Ingot: 2"

### 8.2 Job Statistics
- [ ] Total resources collected per job
- [ ] Total hours worked
- [ ] Rare finds count
- [ ] Best quality item found
- [ ] Job level history graph

### 8.3 Job Navigation
- [ ] Arrow keys to select job
- [ ] Enter to work selected job
- [ ] Tab to switch between Work/Perks/Stats views
- [ ] Number keys 1-4 for quick job selection

### 8.4 Animated Job Screen
- [ ] Background animation per job (mining sparks, fishing waves, etc.)
- [ ] Resource icons animate when collected
- [ ] Progress bar fills smoothly

---

## 9. Job Difficulty & Scaling

### 9.1 Resource Tier System
- [x] Resources have tiers matching area difficulty:
  - Tier 1-2: Felrona (Copper, Maple, Small Fish) — Job Level 1-20
  - Tier 3-4: Dark Woods (Iron, Oak, Salmon) — Job Level 21-40
  - Tier 5-6: Coast (Silver, Ebony, Tuna) — Job Level 41-60
  - Tier 7-8: Dragon's Peak (Gold, Magical Wood, Exotic Fish) — Job Level 61-80
  - Tier 9-10: Highlands+ (Mythril, Ancient Wood, Legendary Fish) — Job Level 81-100

### 9.2 Job Level Requirements
- [x] Higher-tier resources require higher job level
- [x] Can't mine Mythril at Mining level 1
- [x] Creates natural progression curve across 100 levels

### 9.3 Job Scaling
- [x] XP required increases per level (piecewise formula, see Section 11)
- [x] Resources per hour scales with level (level% chance for bonus resource, capped at 50%)
- [x] Higher levels = better quality chances (RollQuality breakpoints at 5/10/20/35/50/75/100)

---

## 10. Implementation Priority

### Phase 1: Visual Feedback (Quick Wins)
1. [x] Add XP progress bars to Jobs screen
2. [x] Add floating XP gain text
3. [x] Add level-up celebration effect
4. [x] Add resource counters per job

### Phase 2: Core Mechanics
5. [x] Implement work shifts (1-8 hours)
6. [x] Add fatigue system
7. [x] Implement random events
8. [x] Add resource quality system

### Phase 3: Progression
9. [x] Add job specialization paths (level 5+) — 8 specializations (2 per job)
10. [x] Add job quests (daily + mastery) — backend implemented, needs UI
11. [ ] Add job storylines
12. [ ] Add tool system
13. [x] Add skill tree system (3 branches x 5 tiers, 4 jobs) — wired, UI, save/load
14. [x] Add weather/environment system — effects collected
15. [x] Add location upgrades — 7 types, 5 levels each

### Phase 4: Polish
13. [x] Add job synergies — implemented in JobSystem
14. [ ] Add job cosmetics
15. [ ] Add job statistics
16. [x] Add achievement system — definitions + UI + tracking wired, needs rewards applied + notifications
17. [ ] Add trophy room UI — partially done via StateAchievements

---

## 11. Balance Targets

### XP Per Hour (Base)
| Job Level | XP Required | XP/Hour | Hours to Next Level |
|-----------|-------------|---------|---------------------|
| 1 | 100 | 12 | ~8 hours |
| 5 | 180 | 20 | ~9 hours |
| 10 | 250 | 30 | ~8 hours |
| 15 | 450 | 40 | ~11 hours |
| 20 | 650 | 50 | ~13 hours |
| 25 | 850 | 60 | ~14 hours |
| 30 | 1150 | 70 | ~16 hours |
| 40 | 1750 | 90 | ~19 hours |
| 50 | 2350 | 110 | ~21 hours |
| 60 | 3350 | 130 | ~26 hours |
| 75 | 4850 | 160 | ~30 hours |
| 90 | 7100 | 190 | ~37 hours |
| 100 | 8350 | 210 | MAX |

### XP Formula (Piecewise)
- Level 1-9: `80 + level * 20`
- Level 10-24: `250 + (level - 10) * 40`
- Level 25-49: `850 + (level - 25) * 60`
- Level 50-74: `2350 + (level - 50) * 100`
- Level 75-100: `4850 + (level - 75) * 150`

### Per-Collection XP
- Base XP per resource: `10 + jobLevel * 2`
- Modified by fatigue multiplier and random events

### Resources Per Hour Scaling
- Base: 1 resource per hour
- Level-based bonus: `level`% chance for an extra resource per hour (capped at 50%)
- Level 10 = 10% bonus, Level 25 = 25%, Level 50+ = 50% cap
- Stacks with perk-based double chance and rare find rolls

### Resource Value Scaling
| Tier | Base Value | With Perks |
|------|------------|------------|
| 1 | 5 gold | 7 gold |
| 2 | 12 gold | 16 gold |
| 3 | 25 gold | 33 gold |
| 4 | 50 gold | 66 gold |
| 5 | 100 gold | 133 gold |

### Fatigue Penalties
| Hours Worked | Efficiency |
|--------------|------------|
| 1-4 | 100% |
| 5 | 90% |
| 6 | 80% |
| 7 | 70% |
| 8 | 60% |

---

## 12. File Changes Required

### New Files
- `Jobs/JobEvent.hpp` — Random event definitions (events defined inline in Jobs.cpp)
- `Jobs/JobQuest.hpp` — Job quest system ✓
- `Jobs/JobQuest.cpp` — Job quest implementation ✓
- `Jobs/ResourceChain.hpp` — Resource chain system ✓
- `Jobs/ResourceChain.cpp` — Resource chain implementation ✓
- `Jobs/JobSpecialization.hpp` — Sub-class definitions ✓
- `Jobs/JobSpecialization.cpp` — Sub-class implementation ✓
- `Jobs/JobTools.hpp` — Tool system
- `Jobs/JobStatistics.hpp` — Stats tracking
- `Achievements/Achievement.hpp` — Achievement definitions and tracking ✓
- `Achievements/Achievement.cpp` — Achievement implementation ✓
- `Achievements/AchievementUI.hpp` — Trophy room and notification UI
- `Achievements/AchievementUI.cpp` — Trophy room implementation

### Modified Files
- `Jobs/Job.hpp` — Add specialization, tools, fatigue ✓ (fatigue implemented)
- `Jobs/Job.cpp` — Implement new mechanics ✓
- `Jobs/JobSystem.hpp` — Add quests, events, synergies ✓
- `Jobs/JobSystem.cpp` — Implement new systems ✓
- `Engine/Game.cpp` — Update StateJobs UI ✓
- `Graphics/Colors.hpp` — Add job-specific colors (job colors defined in Game.cpp StateJobs)

### UI Changes
- `StateJobs()` — Complete rewrite with progress bars, tools, quests ✓ (progress bars done)
- `StateJobPerks()` — Update for specialization perks
- New `StateJobTools()` — Tool management screen
- New `StateJobQuests()` — Quest log for job quests (backend exists, needs UI)

---

## 13. Job Reputation System

### 14.1 Reputation Factions
- [x] Each job has a reputation faction:
  - Mining: Miners' Guild
  - Lumberjacking: Woodcutters' Union
  - Fishing: Fisher's Fellowship
  - Smithing: Artisan's Circle

### 14.2 Reputation Ranks
- [x] Stranger → Acquaintance → Friend → Ally → Champion → Legend
- [x] Higher ranks unlock better rewards
- [x] Reputation earned by working the job + completing quests

### 14.3 Reputation Rewards
- [x] Rank 1 (Friend): Access to basic job shop
- [x] Rank 2 (Ally): Discount on job supplies
- [x] Rank 3 (Champion): Exclusive recipes
- [x] Rank 4 (Legend): Legendary tool quest

### 14.4 Reputation Decay
- [ ] Reputation slowly decays if job not worked for many days
- [ ] Encourages regular play
- [ ] Can be prevented with perks

---

## 15. Achievements & Trophies

### 15.1 Achievement Categories

#### Job Achievements
- [x] **Resource Collector**: Mine 100 / 500 / 1000 ore
- [x] **Woodcutter**: Chop 100 / 500 / 1000 logs
- [x] **Angler**: Catch 100 / 500 / 1000 fish
- [x] **Artisan**: Craft 50 / 200 / 500 items
- [x] **Quality Master**: Find 10 / 50 / 100 Masterwork items
- [x] **Speed Demon**: Complete a work shift in record time
- [x] **Perfectionist**: Reach level 100 in all jobs
- [x] **Combo Master**: Use all job synergies
- [x] **Dedicated Worker**: Work 100 / 500 / 1000 total hours
- [x] **Lucky Find**: Get 10 / 50 / 100 rare finds

#### Combat Achievements
- [x] **Monster Slayer**: Defeat 10 / 50 / 100 / 500 / 1000 monsters
- [x] **Boss Hunter**: Defeat 1 / 5 / 10 bosses
- [x] **Area Conqueror**: Defeat all enemy types in an area
- [x] **World Guardian**: Defeat all enemy types in the game
- [x] **Untouchable**: Win a battle without taking damage
- [x] **Underdog**: Defeat an enemy 10+ levels above you
- [x] **Speed Fighter**: Win a battle in 3 turns or less
- [x] **Elemental Master**: Defeat enemies using all element types
- [x] **Critical Champion**: Land 50 / 200 / 500 critical hits
- [x] **First Blood**: Defeat your first monster

#### Quest Achievements
- [x] **Quest Starter**: Complete 1 / 5 / 10 quests
- [x] **Quest Explorer**: Complete 25 / 50 / 100 quests
- [x] **Quest Master**: Complete all quests in an area
- [x] **Quest Legend**: Complete all quests in the game
- [x] **Kill Quest Pro**: Complete 10 / 25 / 50 kill quests
- [x] **Gatherer**: Complete 10 / 25 / 50 gather quests
- [x] **Pathfinder**: Complete 5 / 10 / 15 explore quests
- [x] **Quest Completionist**: Complete all quest types

#### Exploration Achievements
- [x] **Traveler**: Visit 3 / 6 / all areas
- [x] **Cartographer**: Discover all locations in an area
- [x] **Treasure Hunter**: Find 5 / 15 / 30 hidden treasures
- [x] **Secret Finder**: Discover 3 / 5 / 10 secret areas
- [x] **Lore Keeper**: Read all lore entries in an area

#### Progression Achievements
- [x] **Level Up**: Reach character level 10 / 25 / 50
- [x] **Well Rounded**: Reach level 10 in all jobs
- [x] **Master of None**: Reach level 50 in any job
- [x] **True Master**: Reach level 100 in any job
- [x] **Gold Hoarder**: Accumulate 1000 / 10000 / 100000 gold
- [x] **Collector**: Own 25 / 50 / 100 unique items
- [x] **Fashionable**: Own 10 / 25 / 50 cosmetic items

#### Social & Miscellaneous Achievements
- [x] **First Steps**: Complete the tutorial
- [x] **Helping Hand**: Help 5 / 10 / 25 NPCs
- [x] **Shopaholic**: Buy 50 / 200 / 500 items from shops
- [x] **Merchant**: Sell 100 / 500 / 1000 items
- [x] **Restful**: Rest at the Inn 10 / 50 / 100 times
- [x] **Prepared**: Have 10 / 25 / 50 potions in inventory
- [x] **Survivor**: Survive a battle with 1 HP

### 15.2 Achievement Tiers
- [x] Each achievement has 3 tiers (Bronze, Silver, Gold) — defined in Achievement.hpp
- [x] Bronze: Easy target (first milestone)
- [x] Silver: Medium target (second milestone)
- [x] Gold: Hard target (third milestone)
- [ ] Higher tiers give better rewards — rewards are fixed per-achievement, not scaled by tier

### 15.3 Trophy Display
- [x] Show achievements in a Trophy Room (accessible from Explore menu as "Achievements")
- [ ] Each achievement has a visual trophy icon
- [x] Trophy Room shows completion percentage
- [x] Recently unlocked achievements highlighted — golden pulse border for 5 seconds
- [x] Filter by category (Job, Combat, Quest, etc.)
- [ ] Sort by completion status, date unlocked, or rarity

### 15.4 Achievement Rewards

#### Stat Bonuses (Permanent)
- [x] **Bronze Job Achievements**: +2% job XP bonus per achievement
- [x] **Silver Job Achievements**: +5% job XP bonus per achievement
- [x] **Gold Job Achievements**: +10% job XP bonus per achievement
- [x] **Bronze Combat Achievements**: +1% ATK/DEF per achievement
- [x] **Silver Combat Achievements**: +3% ATK/DEF per achievement
- [x] **Gold Combat Achievements**: +5% ATK/DEF per achievement
- [x] **Bronze Quest Achievements**: +5% quest XP per achievement
- [x] **Silver Quest Achievements**: +10% quest XP per achievement
- [x] **Gold Quest Achievements**: +15% quest XP per achievement
- [x] Achievement rewards defined in AchievementSystem (gold, stats, XP bonuses)
- [x] Rewards applied to player combat stats (ATK/DEF/HP at combat start)
- [x] XP bonuses applied to combat XP gains
- [x] Gold bonuses applied to combat gold gains
- [x] Tier-based bonuses applied to job XP and quest XP

#### Titles (Cosmetic)
- [x] **Monster Slayer titles**: "Novice Slayer" → "Expert Slayer" → "Legendary Slayer"
- [x] **Quest titles**: "Adventurer" → "Veteran" → "Hero of the Realm"
- [x] **Job titles**: "Apprentice" → "Journeyman" → "Grandmaster"
- [x] **Exploration titles**: "Scout" → "Pathfinder" → "World Walker"
- [x] Titles displayed next to player name in top bar (best title shown)

#### Gold Rewards
- [x] **Bronze achievements**: 50 / 100 / 200 gold
- [x] **Silver achievements**: 500 / 1000 / 2000 gold
- [x] **Gold achievements**: 5000 / 10000 / 25000 gold

#### Item Rewards
- [x] **Combat achievements**: Unique weapons/armor (ach_slayer_blade, ach_legendary_blade, ach_boss_amulet, ach_critical_ring)
- [x] **Job achievements**: Rare tools and crafting materials (ach_masters_pickaxe, ach_masters_axe, ach_masters_rod, ach_masters_hammer, ach_masters_lens, ach_workers_medallion, ach_fortune_charm)
- [x] **Quest achievements**: Special accessories with unique effects (ach_quest_talisman, ach_quest_crown, ach_hunters_cloak, ach_gatherers_pouch, ach_explorers_compass)
- [x] **Exploration achievements**: Maps to hidden areas (ach_world_map, ach_shadow_cloak)
- [x] **Completionist achievements**: Legendary equipment sets (ach_heroes_crown, ach_tycoons_ring, ach_archivists_satchel, ach_merchants_crown, ach_innkeepers_key)

#### Unlockables
- [x] **100% Area Completion**: Unlock secret dungeon in that area
- [x] **All Quests Complete**: Unlock New Game+ mode
- [x] **All Monsters Defeated**: Unlock Boss Rush mode
- [x] **All Jobs Maxed**: Unlock Master class skill tree
- [x] **All Achievements**: Unlock developer commentary mode

### 15.5 Achievement Notifications
- [x] Pop-up notification when achievement unlocked — slide-in panel from right
- [x] Show achievement name, description, and reward
- [ ] Sound effect for achievement unlock
- [ ] Flash effect on screen
- [ ] Log all achievements in journal
- [x] Notification infrastructure exists (HasUnnotifiedAchievement, GetNextUnnotified, MarkNotified)
- [x] Notification methods called from game loop (CheckAchievementNotifications + DrawAchievementNotifications)

---

## 16. Job Seasons & Events

### 16.1 Seasonal Job Events
- [ ] Each season (every 30 in-game days) has a job event:
  - Spring: "Planting Season" — bonus Lumberjacking XP
  - Summer: "Fishing Tournament" — bonus Fishing XP
  - Autumn: "Harvest Festival" — bonus Mining XP
  - Winter: "Forge Fires" — bonus Smithing XP

### 16.2 Limited-Time Resources
- [ ] Season-only resources appear during events
- [ ] Seasonal resources used in special crafting
- [ ] Miss the season? Wait until next year (or use rare item)

### 16.3 Event Leaderboards
- [ ] Track who collected most resources during event
- [ ] Top players get bonus rewards
- [ ] Leaderboard resets each season

---

## 17. Job Crafting Dependencies

### 17.1 Multi-Job Recipes
- [ ] Some recipes require resources from multiple jobs:
  - "Warrior's Axe" = Iron Ingot (Smithing) + Oak Handle (Lumberjacking)
  - "Enchanted Rod" = Silver Ingot (Smithing) + Magical Wood (Lumberjacking) + Rare Gem (Mining)
  - "Feast Table" = Sturdy Plank (Lumberjacking) + Cooked Fish (Fishing)

### 17.2 Crafting Chains
- [ ] Resources can be processed multiple times:
  - Raw Ore → Refined Ore → Ingot → Weapon
  - Raw Wood → Plank → Component → Final Item
  - Each step adds value and XP

### 17.3 Recipe Discovery
- [ ] Recipes not all known from start
- [ ] Discover recipes by experimenting or buying from NPCs
- [ ] Higher job level = more recipes discovered

---

## 18. Job-Specific Merchants

### 18.1 NPC Merchants per Job
- [ ] Mining: "Ore Trader" — buys/sells ores
- [ ] Lumberjacking: "Wood Dealer" — buys/sells wood
- [ ] Fishing: "Fish Monger" — buys/sells fish
- [ ] Smithing: "Forge Master" — sells tools, buys ingots

### 18.2 Merchant Inventories
- [ ] Merchants sell job-specific supplies:
  - Mining: Pickaxes, torches, explosives
  - Lumberjacking: Axes, saws, rope
  - Fishing: Rods, bait, nets
  - Smithing: Coal, flux, hammers

### 18.3 Merchant Relationships
- [ ] Higher reputation = better stock
- [ ] Merchants offer special deals at high reputation
- [ ] Can invest in merchants for passive income

---

## 19. Job Mastery System

### 19.1 Mastery Ranks
- [x] Mastery ranks unlock at specific job levels:
  - Level 10: Apprentice
  - Level 25: Journeyman
  - Level 50: Expert
  - Level 75: Master
  - Level 100: Grandmaster
- [x] Each rank grants access to new perks and abilities
- [x] Mastery titles displayed next to job level

### 19.2 Mastery Perks
- [ ] Unlock powerful passive abilities:
  - "Double Yield": 20% chance for double resources
  - "Quality Boost": +10% chance for Masterwork quality
  - "Speed Demon": Work 20% faster
  - "Lucky Break": Rare resources 50% more common
- [x] MasteryBonus enum type exists in JobPerk.hpp but no perks use it yet

### 19.3 Mastery Challenges
- [x] Special challenges at each mastery rank — defined in JobQuestSystem
- [x] Challenges are harder than regular job quests
- [ ] Rewards include unique cosmetics and titles — rewards are XP/gold only

---

## 20. Job Skill Trees (Separate from Perks)

### 20.1 Skill Tree Structure
- [x] Each job has a skill tree (3 branches, 5 skills each) — JobSkillTree.hpp/cpp
- [x] Skills unlocked with Job Skill Points (earned at level up)
- [x] Skills provide passive bonuses to job performance
- [x] Skill tree UI integrated into Jobs screen (StateJobSkillTree)
- [x] Skill tree bonuses wired into WorkJob (speed, quality, rare find, double, XP, sell value)
- [x] Save/load support for skill tree data

### 20.2 Mining Skill Tree
- [x] **Branch 1: Efficiency** — speed, fatigue reduction, auto-pickup, double/triple yield
- [x] **Branch 2: Quality** — quality chance, fine/masterwork/perfect/legendary quality up to Masterwork
- [x] **Branch 3: Luck** — rare find %, ultra-rare/mythical items

### 20.3 Other Job Trees
- [x] Similar trees for Lumberjacking, Fishing, Smithing (Efficiency/Quality/Luck per job)
- [x] Each branch focuses on different aspects
- [ ] Can only fully master one branch per job (no restriction implemented)

---

## 21. Job Environment & Atmosphere

### 21.1 Job Locations
- [ ] Each job has a dedicated location:
  - Mining: Crystal Caverns
  - Lumberjacking: Verdant Forest
  - Fishing: Serene Lake
  - Smithing: Ancient Forge

### 21.2 Environmental Effects
- [x] Weather system implemented (JobEnvironment.hpp/cpp):
  - Clear, Rainy, Stormy, Windy, Foggy, Snowy, Scorching
  - Each weather type modifies speed, quality, fatigue, rare finds, XP
  - Weather changes every 1-3 days, rolled randomly
  - Effects wired into WorkJob function
  - `UpdateDaily()` call needed in game loop

### 21.3 Location Upgrades
- [x] Location upgrade system (LocationUpgrades.hpp/cpp):
  - 7 upgrade types: Tool Quality, Workspace, Storage, Lighting, Ventilation, Automation, Enchantment
  - Each upgrade has 5 levels with increasing gold costs
  - Bonuses: speed, quality, fatigue reduction, rare find, XP, storage
  - Integrated into WorkJob via LocationUpgrades member in JobSystem
  - UI integration pending

---

## 22. Job Tutorials & Introductions

### 22.1 Tutorial Quests
- [ ] Each job has a tutorial quest chain:
  - Mining: "First Steps" → "Finding Ore" → "Going Deeper"
  - Lumberjacking: "Chopping Basics" → "Wood Types" → "Master Woodcutting"
  - Fishing: "Casting Off" → "Fish Types" → "Big Game Hunter"
  - Smithing: "Heating Up" → "Basic Forging" → "Master Smith"

### 22.2 Tutorial Rewards
- [ ] Complete tutorial = free tool
- [ ] Tutorial resources are worth bonus XP
- [ ] Unlock job-specific emotes/animations

### 22.3 Mentor NPCs
- [ ] Each job has a mentor NPC
- [ ] Mentors give advice and quests
- [ ] Higher reputation = better mentor dialogue

---

## 23. Job Collectibles & Set Items

### 23.1 Job-Specific Collectibles
- [ ] Each job has 10 unique collectibles:
  - Mining: Gem collection (10 different gems)
  - Lumberjacking: Wood samples (10 different woods)
  - Fishing: Fish album (10 different fish)
  - Smithing: Ingot samples (10 different ingots)

### 23.2 Collectible Benefits
- [ ] Complete a set = permanent bonus:
  - Gem Set: +5% all stats
  - Wood Set: +10% crafting speed
  - Fish Set: +15% healing from fish
  - Ingot Set: +10% weapon damage

### 23.3 Collectible Display
- [ ] Show collectibles in Trophy Room
- [ ] Each collectible has a visual display
- [ ] Progress tracker for each collection

---

## 24. Final Notes

### 24.1 Core Philosophy
- Jobs should feel rewarding, not tedious
- Visual feedback is crucial for satisfaction
- Meaningful choices create engagement
- Progress should be visible and measurable

### 24.2 Must-Have Features (MVP)
1. [x] XP progress bars
2. [x] Level-up celebrations
3. [x] Resource counters
4. [x] Job quests — backend implemented, needs UI
5. [x] Basic achievement system — definitions + UI + tracking + rewards + notifications + save/load

### 24.3 Nice-to-Have Features
1. [ ] Job seasons/events
2. [x] Full skill trees — implemented with 3 branches × 5 tiers per job
3. [ ] Job collectibles
4. [x] Environmental effects — JobEnvironment with 7 weather types
5. [ ] Location upgrades — system done, needs UI
6. [ ] Job tutorials
7. [ ] Animated job screen

### 24.4 Future Expansion
1. [ ] New job types (Herbalism, Alchemy, etc.)
2. [ ] Job parties/co-op
3. [ ] Job competitions/PvP
4. [ ] Job housing
5. [ ] Job storylines (full narratives)
