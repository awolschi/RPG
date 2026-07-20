cmake_minimum_required(VERSION 3.15)
project(RPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable PIE to avoid linker issues with raylib static library
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

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
    Items/Uniques/UniqueItems.cpp
    Items/Passives.cpp
    Items/SetBonuses.cpp

    # Engine
    Engine/Game.cpp
    Engine/Combat.cpp
    Engine/SaveGame.cpp
    Engine/LevelUp/LevelUp.cpp
    Engine/Religion.cpp
    Engine/KeyboardNav.cpp

    # World
    Engine/Wiki.cpp
    World/Areas/Area.cpp
    World/Quests/Quest.cpp
    World/Enemies/Enemies.cpp
    World/NPCs/NPC.cpp

    # Jobs
    Jobs/Jobs.cpp
    Jobs/JobSystem.cpp
    Jobs/ResourceChain.cpp
    Jobs/JobQuest.cpp
    Jobs/JobSpecialization.cpp
    Jobs/JobSkillTree.cpp
    Jobs/JobEnvironment.cpp
    Jobs/LocationUpgrades.cpp
    Items/Resources/Resources.cpp
    Items/Crafting/Crafting.cpp

    # Achievements
    Achievements/Achievement.cpp

    # Factions
    Factions/FactionReputation.cpp
    Factions/Pet.cpp

    # Graphics
    Graphics/GRenderer.cpp
    Graphics/IconRenderer.cpp
    Graphics/TextureManager.cpp
    Graphics/PhotoEffects.cpp
    Graphics/BattleLayout.cpp
    Graphics/BattleRenderer.cpp
    Graphics/CreatureImageMapper.cpp
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
