# C++ Text RPG - Complete Implementation

A turn-based RPG written in C++ with a complete game structure including character creation, combat system, inventory management, skills, and progression.

## Project Features

### ✅ Implemented Systems

**Character System**
- 4 playable classes: Warrior, Priest, Mage, Archer
- Base/leveled stats system (HP, Mana, Strength, Intelligence, Dexterity, Defense, etc.)
- Level progression with experience system
- Class-specific stat growth

**Combat System**
- Turn-based combat with player choice and enemy AI
- Multiple combat actions: Attack, Use Skill, Defend, Flee
- Health/Mana management during combat
- Damage calculation with defense mitigation

**Skill System**
- Base Skill class with inheritance hierarchy
- Class-specific skills:
  - **Warrior**: Power Strike, Whirlwind, Defensive Stance
  - **Mage**: Fireball, Ice Bolt, Meteor
  - **Priest**: Holy Smite, Heal, Mass Heal
  - **Archer**: Piercing Shot, Multi Shot
  - **Common**: Basic Attack (all classes)
- Skill progression (leveling, XP tracking)
- Cooldown management

**Inventory & Equipment**
- 20-slot inventory system
- Equipment system with:
  - Weapons (main hand, off-hand)
  - Armor (helmet, chest, gloves, pants, boots)
  - Accessories (2 rings, 1 amulet)
- Item types: Weapons, Armor, Accessories, Consumables
- Defense and bonus stat calculations from equipment

**Game Engine**
- Main game loop with state machine
- Character creation menu
- Exploration interface
- Inventory management
- Statistics display
- Encounters with enemy monsters

**World & Enemies**
- Enemy database with predefined monsters (Goblin, Orc, Wolf, Dragon)
- Area system for world exploration (skeleton)
- Quest system (skeleton)
- Job system for gathering professions

**Persistence**
- Save/Load system (skeleton for expansion)
- Level Up manager with XP curve calculations

## Build Instructions

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Linux/macOS/Windows

### Building on Linux/macOS

```bash
# Navigate to project directory
cd /path/to/RPG

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile
make

# Run
./rpg
```

### Building on Windows (Visual Studio)

```bash
# Navigate to project directory
cd \path\to\RPG

# Create build directory
mkdir build
cd build

# Generate Visual Studio project files
cmake .. -G "Visual Studio 16 2019"

# Build (using Visual Studio)
# - Open RPG.sln in Visual Studio, or
# - Use cmake to build:
cmake --build . --config Release

# Run
.\Release\rpg.exe
```

### Building on Windows (MinGW)

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## Project Structure

```
RPG/
├── Main.cpp                          # Entry point
├── CMakeLists.txt                    # Build configuration
├── README.md                         # This file
│
├── Characters/                       # Character classes
│   ├── Character.hpp/.cpp           # Base character class
│   ├── Player.hpp/.cpp              # Player character (extends Character)
│   ├── Monster.hpp/.cpp             # Enemy characters (extends Character)
│   ├── Classes/                     # Class definitions
│   │   └── Classes.hpp/.cpp         # Class database with stats
│   └── Stats/                       # Stat structure
│       └── Stats.hpp                # Stat definitions
│
├── Skills/                          # Skill system
│   ├── Skill.hpp/.cpp              # Base skill class
│   ├── Skills.hpp/.cpp             # Skill set manager
│   ├── CommonSkills/               # Universal skills
│   │   └── CommonAttack.hpp/.cpp   # Basic attack
│   └── ClassSkills/                # Class-specific skills
│       ├── Warrior/Warrior.hpp/.cpp
│       ├── Mage/Mage.hpp/.cpp
│       ├── Priest/Priest.hpp/.cpp
│       └── Archer/Archer.hpp/.cpp
│
├── Items/                           # Item system
│   ├── Item.hpp                    # Base item class
│   ├── Equipment/                  # Equipment management
│   │   ├── Equipment.hpp/.cpp
│   │   └── EquipmentPieces/        # Equipment categories (structure)
│   │       ├── Armor/
│   │       ├── Weapons/
│   │       └── Accessories/
│   └── Resources/                  # Gathering system
│       └── Resources.hpp/.cpp
│
├── Inventory/                       # Inventory management
│   ├── Inventory.hpp/.cpp
│
├── Engine/                          # Game engine
│   ├── Game.hpp/.cpp               # Main game class and loop
│   ├── Combat.hpp/.cpp             # Combat system
│   ├── SaveGame.hpp/.cpp           # Save/Load system (skeleton)
│   └── LevelUp/
│       ├── LevelUp.hpp/.cpp        # Level progression
│       └── ExperienceCurve.cpp     # XP calculations
│
├── World/                           # World content
│   ├── Areas/                      # Area system
│   │   ├── Area.hpp/.cpp
│   │   └── [Individual areas]
│   ├── Enemies/                    # Enemy definitions
│   │   ├── Enemies.hpp/.cpp
│   │   └── [Enemy types]
│   └── Quests/                     # Quest system
│       ├── Quest.hpp/.cpp
│       └── [Quest types]
│
└── Jobs/                            # Job/profession system
    ├── Job.hpp
    ├── Jobs.cpp                    # Job implementations
    └── [Specific jobs]
```

## Gameplay Guide

### Starting the Game
1. Run the compiled executable: `./rpg`
2. Choose "New Game" from the main menu
3. Enter your character name
4. Select a class:
   - **Warrior**: High HP and Defense, strong melee attacks
   - **Priest**: Balanced stats with healing abilities
   - **Mage**: High Intelligence, powerful magic spells
   - **Archer**: High Dexterity, ranged attacks

### Combat
- **Attack**: Basic attack using your equipped weapon/stats
- **Use Skill**: Select from your class-specific skills
  - Each skill has mana cost and cooldown
  - Skills deal damage or provide healing
- **Defend**: Increase defense for this turn
- **Flee**: Escape from combat (may fail)

### Progression
- Gain XP from defeating enemies
- Level up to increase stats
- Skills gain XP independently and can level up
- Collect gold from defeated enemies
- Find and equip better items (in future expansions)

## Code Architecture

### Design Patterns Used
1. **Inheritance**: Character base class with Player and Monster subclasses
2. **Polymorphism**: Skill system with virtual Use() methods
3. **State Pattern**: Game engine with GameState enum
4. **Singleton Pattern**: ClassDatabase and EnemyDatabase
5. **Memory Management**: Smart pointers (std::shared_ptr) for dynamic objects

### Key Classes

**Character**
- Base class for all characters
- Manages health, mana, stats, skills, equipment
- Provides combat methods (TakeDamage, RestoreHealth, etc.)

**Player** (extends Character)
- Inherits character functionality
- Adds inventory system
- Class-specific skill initialization
- Special level up behavior

**Skill**
- Base class for all skills
- Manages cooldowns and XP progression
- Pure virtual Use() method for subclasses
- Skill leveling system

**CombatSystem**
- Manages turn-based combat flow
- Player input handling
- Enemy AI decisions
- Combat status display

**Game**
- Main game loop controller
- State machine for different game screens
- Menu management
- NPC encounter generation

## Future Expansion Ideas

### Tier 1: Essential Features
- [ ] Save/Load system completion (currently skeleton)
- [ ] More enemy types and encounters
- [ ] Boss battles
- [ ] Equipment drops from enemies
- [ ] Item rarities and stats

### Tier 2: Medium Features
- [ ] Full quest system implementation
- [ ] Job/profession system gameplay
- [ ] Area exploration system
- [ ] NPC interactions
- [ ] Trading system

### Tier 3: Advanced Features
- [ ] Multiplayer support
- [ ] Procedural dungeon generation
- [ ] Advanced enemy AI
- [ ] Skill point allocation system
- [ ] PvP arenas
- [ ] Guilds/Clans

## Technical Notes

### Compilation
- Uses C++17 features (std::shared_ptr, auto)
- Standard library only (no external dependencies)
- CMake for cross-platform builds
- Header/Implementation file organization

### Performance
- Efficient memory management with smart pointers
- Direct stats calculations (no heavy physics)
- Suitable for educational purposes and expansion

### Known Limitations
- Text-based UI (could be expanded to graphical)
- Save system is skeleton implementation
- Limited enemy variety
- No procedural generation yet
- Single-threaded

## Contributing

This is a complete skeleton project ready for expansion. You can:
- Add new skills and classes
- Implement full quest system
- Add graphical interface
- Expand world content
- Create procedural generation systems

## License

This is a learning project. Feel free to modify and expand as needed.

---

**Happy adventuring!** 🎮⚔️✨
