RPG
│
├── main.cpp
│
├── Engine
│   ├── Game.cpp
│   ├── Game.hpp
│   ├── Combat.cpp
│   ├── Combat.hpp
│   ├── SaveGame.cpp
│   └── SaveGame.hpp
│
├── Characters
│   ├── Character.cpp
│   ├── Character.hpp
│   ├── Player.cpp
│   ├── Player.hpp
│   ├── Monster.cpp
│   ├── Monster.hpp
│   ├── Stats.hpp
│   └── CharacterClass.hpp
│
├── Skills
│   ├── Skill.hpp
│   ├── Skill.cpp
│   │
│   ├── Mage
│   │   ├── Fire
│   │   ├── Frost
│   │   └── Lightning
│   │
│   ├── Warrior
│   │   ├── Guardian
│   │   ├── Berserker
│   │   └── WeaponMaster
│   │
│   ├── Priest
│   │   ├── Holy
│   │   ├── Nature
│   │   └── Shadow
│   │
│   ├── Archer
│   │   ├── Marksman
│   │   ├── Hunter
│   │   └── Trickshot
│   │
│   └── Common
│       ├── NormalAttack.cpp
│       ├── NormalAttack.hpp
│       ├── Dodge.cpp
│       └── Dodge.hpp
│
├── Items
│   ├── Item.hpp
│   ├── Weapon.hpp
│   ├── Armor.hpp
│   ├── Shield.hpp
│   ├── Ring.hpp
│   └── Potion.hpp
│
├── Inventory
│   ├── Inventory.cpp
│   └── Inventory.hpp
│
├── World
│   ├── Area.cpp
│   ├── Area.hpp
│   ├── Quest.cpp
│   └── Quest.hpp
│
├── Data
│   ├── Skills
│   ├── Monsters
│   ├── Items
│   └── Classes
│
└── Utils
    ├── Random.cpp
    ├── Random.hpp
    └── Logger.hpp