cmake_minimum_required(VERSION 3.15)
project(RPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Raylib via FetchContent ---
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Source files
set(SOURCES
    Main.cpp

    # Character system
    Characters/Character.cpp
    Characters/Player.cpp
    Characters/Monster.cpp
    Characters/Stats/Stats.cpp
    Characters/Classes/Classes.cpp
    Characters/Races/Races.cpp

    # Skills
    Skills/Skill.cpp
    Skills/Skills.cpp
    Skills/CommonSkills/CommonAttack.cpp
    Skills/ClassSkills/Warrior/Warrior.cpp
    Skills/ClassSkills/Mage/Mage.cpp
    Skills/ClassSkills/Priest/Priest.cpp
    Skills/ClassSkills/Archer/Archer.cpp
    Skills/ClassSkills/Merchant/Merchant.cpp

    # Inventory and Items
    Inventory/Inventory.cpp
    Items/Equipment/Equipment.cpp
    Items/Loot.cpp

    # Engine
    Engine/Game.cpp
    Engine/Combat.cpp
    Engine/SaveGame.cpp
    Engine/LevelUp/LevelUp.cpp
    Engine/Religion.cpp

    # World
    Engine/Wiki.cpp
    World/Areas/Area.cpp
    World/Quests/Quest.cpp
    World/Enemies/Enemies.cpp
    World/NPCs/NPC.cpp

    # Jobs
    Jobs/Jobs.cpp
    Jobs/JobSystem.cpp
    Items/Resources/Resources.cpp
    Items/Crafting/Crafting.cpp

    # Graphics
    Graphics/GRenderer.cpp
    Graphics/IconRenderer.cpp
)

# Executable
add_executable(rpg ${SOURCES})

# Include directories
target_include_directories(rpg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(rpg raylib)

if(WIN32)
    set_target_properties(rpg PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(MSVC)
    target_compile_options(rpg PRIVATE /W4)
else()
    target_compile_options(rpg PRIVATE -Wall -Wextra -Wpedantic)
endif()
