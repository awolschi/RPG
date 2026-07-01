cmake_minimum_required(VERSION 3.15)
project(RPG)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    Main.cpp
    
    # Character system
    Characters/Character.cpp
    Characters/Player.cpp
    Characters/Monster.cpp
    Characters/Stats/Stats.cpp
    Characters/Classes/Classes.cpp
    
    # Skills
    Skills/Skill.cpp
    Skills/Skills.cpp
    Skills/CommonSkills/CommonAttack.cpp
    Skills/ClassSkills/Warrior/Warrior.cpp
    Skills/ClassSkills/Mage/Mage.cpp
    Skills/ClassSkills/Priest/Priest.cpp
    Skills/ClassSkills/Archer/Archer.cpp
    
    # Inventory and Items
    Inventory/Inventory.cpp
    Items/Equipment/Equipment.cpp
    Items/Loot.cpp
    
    # Engine
    Engine/Game.cpp
    Engine/Combat.cpp
    Engine/SaveGame.cpp
    Engine/LevelUp/LevelUp.cpp
    
    # World
    World/Areas/Area.cpp
    World/Quests/Quest.cpp
    World/Enemies/Enemies.cpp
    
    # Jobs
    Jobs/Jobs.cpp
    Jobs/JobSystem.cpp
    Items/Resources/Resources.cpp
)

# Executable
add_executable(rpg ${SOURCES})

# Include directories
target_include_directories(rpg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler flags
if(MSVC)
    target_compile_options(rpg PRIVATE /W4)
else()
    target_compile_options(rpg PRIVATE -Wall -Wextra -Wpedantic)
endif()
