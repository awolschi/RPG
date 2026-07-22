#include "Pet.hpp"
#include "../Engine/RNG.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

PetManager::PetManager()
{
    pets.reserve(25);
    Initialize();
}

void PetManager::Initialize()
{
    AddFactionPet(FactionID::GuardiansOfFelrona,
        "ember_sentinel", "Ember Sentinel",
        "A living flame that burns with the loyalty of Felrona's defenders.",
        ElementType::Fire, 18, 5,
        0.03f, 0.05f, 0.02f, 0.08f, 0.04f, 0.0f, 15.0f, 0.0f, 3);

    AddFactionPet(FactionID::ShadowWardens,
        "frost_wisp", "Frost Wisp",
        "A crystalline spirit from the deepest winter of the Dark Woods.",
        ElementType::Ice, 15, 10,
        0.05f, 0.03f, 0.0f, 0.05f, 0.03f, 0.05f, 0.0f, 10.0f, 0);

    AddFactionPet(FactionID::TideCallers,
        "storm_tide", "Storm Tide",
        "A crackling energy from the heart of a coastal tempest.",
        ElementType::Lightning, 16, 15,
        0.03f, 0.08f, 0.05f, 0.10f, 0.03f, 0.0f, 0.0f, 0.0f, 5);

    AddFactionPet(FactionID::DragonScholars,
        "arcane_drake", "Arcane Drake",
        "A hatchling infused with the forbidden knowledge of dragon scholars.",
        ElementType::Arcane, 17, 20,
        0.05f, 0.05f, 0.03f, 0.08f, 0.05f, 0.03f, 10.0f, 10.0f, 0);

    AddFactionPet(FactionID::HighlandClans,
        "venom_shard", "Venom Shard",
        "A toxic fragment of the highland serpent, pulsing with deadly energy.",
        ElementType::Poison, 20, 25,
        0.02f, 0.06f, 0.04f, 0.12f, 0.06f, 0.0f, 0.0f, 0.0f, 8);

    AddFactionPet(FactionID::VoidSeekers,
        "inferno_guardian", "Inferno Guardian",
        "A being of pure flame that feeds on the void's corruption.",
        ElementType::Fire, 22, 30,
        0.03f, 0.05f, 0.05f, 0.10f, 0.06f, 0.02f, 20.0f, 0.0f, 5);

    AddFactionPet(FactionID::CelestialOrder,
        "celestial_spark", "Celestial Spark",
        "A fragment of celestial light that restores vigor to the worthy.",
        ElementType::Holy, 14, 35,
        0.08f, 0.03f, 0.02f, 0.05f, 0.03f, 0.05f, 25.0f, 15.0f, 0);

    AddFactionPet(FactionID::VoidExarchs,
        "void_mote", "Void Mote",
        "A sliver of the void itself, bent to serve the light.",
        ElementType::Arcane, 24, 40,
        0.03f, 0.04f, 0.06f, 0.15f, 0.08f, 0.0f, 0.0f, 0.0f, 10);

    AddFactionPet(FactionID::ArcaneConclave,
        "mana_weaver", "Mana Weaver",
        "A sentient thread of raw mana, weaving power from nothing.",
        ElementType::Arcane, 16, 45,
        0.06f, 0.04f, 0.03f, 0.08f, 0.04f, 0.03f, 10.0f, 25.0f, 0);

    AddFactionPet(FactionID::ChronosWardens,
        "chrono_spark", "Chrono Spark",
        "A spark frozen in a single moment, crackling with temporal energy.",
        ElementType::Lightning, 20, 50,
        0.04f, 0.07f, 0.07f, 0.12f, 0.05f, 0.02f, 0.0f, 0.0f, 8);

    AddBossPet("goblin_familiar", "Goblin Familiar",
        "A mischievous goblin spirit that aids its master.",
        ElementType::Physical, 10, 1,
        0.02f, 0.10f, 0.0f, 0.05f, 0.02f, 0.0f, 10.0f, 0.0f, 0,
        "Farm Overseer");

    AddBossPet("treant_sapling", "Treant Sapling",
        "A living sapling with the ancient wisdom of the forest.",
        ElementType::Ice, 12, 5,
        0.05f, 0.02f, 0.02f, 0.05f, 0.03f, 0.08f, 20.0f, 0.0f, 0,
        "Treant King");

    AddBossPet("kraken_ink", "Kraken Ink",
        "A vial of ink from the depths, shimmering with oceanic power.",
        ElementType::Poison, 14, 10,
        0.03f, 0.06f, 0.03f, 0.08f, 0.04f, 0.02f, 0.0f, 15.0f, 3,
        "Kraken");

    AddBossPet("dragon_whelp", "Dragon Whelp",
        "A tiny dragon that yearns to breathe fire.",
        ElementType::Fire, 18, 20,
        0.03f, 0.03f, 0.05f, 0.12f, 0.06f, 0.02f, 15.0f, 0.0f, 5,
        "Elder Dragon");

    AddBossPet("warlord_banner", "Warlord Banner",
        "The tattered banner of a fallen warlord, still radiating authority.",
        ElementType::Physical, 20, 30,
        0.02f, 0.05f, 0.04f, 0.10f, 0.08f, 0.05f, 30.0f, 0.0f, 0,
        "Aran Warlord");

    AddBossPet("void_shard", "Void Shard",
        "A fragment of pure void, cold to the touch and humming with power.",
        ElementType::Arcane, 22, 35,
        0.04f, 0.04f, 0.06f, 0.15f, 0.07f, 0.0f, 0.0f, 20.0f, 8,
        "Void Lord");

    AddBossPet("seraph_feather", "Seraph Feather",
        "A radiant feather from a celestial being.",
        ElementType::Holy, 16, 40,
        0.08f, 0.03f, 0.03f, 0.08f, 0.03f, 0.08f, 35.0f, 20.0f, 0,
        "Seraphim Council");

    AddBossPet("primordial_heart", "Primordial Heart",
        "The still-beating heart of a primordial entity.",
        ElementType::Fire, 28, 50,
        0.05f, 0.05f, 0.07f, 0.18f, 0.10f, 0.03f, 25.0f, 0.0f, 10,
        "Primordial One");

    AddBossPet("construct_core", "Construct Core",
        "The arcane core of an ancient construct, pulsing with raw magic.",
        ElementType::Arcane, 20, 45,
        0.06f, 0.04f, 0.04f, 0.10f, 0.06f, 0.04f, 15.0f, 25.0f, 0,
        "Arcane Construct");

    AddBossPet("chrono_fragment", "Chrono Fragment",
        "A shard of crystallized time, moments frozen in amber.",
        ElementType::Lightning, 24, 55,
        0.04f, 0.06f, 0.08f, 0.15f, 0.07f, 0.02f, 0.0f, 10.0f, 6,
        "Chronos");

    AddCitadelPet("shadowfang_relic", "Shadowfang Relic",
        "A fang of pure darkness, still dripping with cursed ichor.",
        ElementType::Arcane, 35, 55,
        0.06f, 0.05f, 0.08f, 0.20f, 0.10f, 0.05f, 30.0f, 25.0f, 12,
        "Umbraxis, the Void Warden", PetRarity::Legendary);

    AddCitadelPet("flamecrown_shard", "Flamecrown Shard",
        "A shard of crown-fire, burning with the fury of a dead dynasty.",
        ElementType::Fire, 38, 55,
        0.05f, 0.06f, 0.09f, 0.22f, 0.12f, 0.03f, 25.0f, 0.0f, 15,
        "Ignaroth, the Ashborn King", PetRarity::Legendary);

    AddCitadelPet("abyssal_crown", "Abyssal Crown",
        "A crown of liquid shadow that whispers of dominion over the deep.",
        ElementType::Ice, 32, 55,
        0.07f, 0.04f, 0.06f, 0.18f, 0.08f, 0.10f, 40.0f, 30.0f, 10,
        "Thalassion, the Abyssal Sovereign", PetRarity::Legendary);

    AddCitadelPet("tempest_core", "Tempest Core",
        "A crackling heart of storm, alive with unending fury.",
        ElementType::Lightning, 36, 55,
        0.05f, 0.07f, 0.10f, 0.20f, 0.10f, 0.04f, 20.0f, 15.0f, 8,
        "Voltarath, the Storm Titan", PetRarity::Legendary);

    AddCitadelPet("rotbloom_heart", "Rotbloom Heart",
        "A pulsing flower of decay, beautiful and utterly lethal.",
        ElementType::Poison, 34, 55,
        0.06f, 0.05f, 0.07f, 0.20f, 0.11f, 0.03f, 15.0f, 0.0f, 14,
        "Nethys, the Rotweaver", PetRarity::Legendary);

    AddCitadelPet("seraph_halo", "Seraph Halo",
        "A radiant ring of celestial fire that purifies all it touches.",
        ElementType::Holy, 30, 55,
        0.09f, 0.04f, 0.05f, 0.15f, 0.07f, 0.12f, 50.0f, 35.0f, 8,
        "Zariel, the Celestial Arbiter", PetRarity::Legendary);

    AddCitadelPet("verdant_serpent", "Verdant Serpent",
        "A coiled serpent of living jade, its scales shimmering with ancient power.",
        ElementType::Poison, 37, 55,
        0.05f, 0.06f, 0.08f, 0.22f, 0.12f, 0.05f, 25.0f, 10.0f, 12,
        "Ouroboros, the Emerald Wyrm", PetRarity::Legendary);

    AddCitadelPet("warlords_bloodstone", "Warlord's Bloodstone",
        "A blood-red gemstone that pulses with the rage of a thousand battles.",
        ElementType::Physical, 40, 55,
        0.04f, 0.05f, 0.10f, 0.25f, 0.14f, 0.03f, 30.0f, 0.0f, 18,
        "Dravok, the Conqueror", PetRarity::Legendary);

    AddCitadelPet("voidheart_gem", "Voidheart Gem",
        "A gem of compressed nothingness that bends reality around it.",
        ElementType::Arcane, 36, 55,
        0.06f, 0.05f, 0.09f, 0.22f, 0.10f, 0.06f, 20.0f, 30.0f, 10,
        "Malachar, the Dimensional Rift", PetRarity::Legendary);

    AddCitadelPet("chronos_hourglass", "Chronos Hourglass",
        "A tiny hourglass filled with crystallized moments of eternity.",
        ElementType::Lightning, 33, 55,
        0.07f, 0.08f, 0.08f, 0.18f, 0.08f, 0.05f, 15.0f, 25.0f, 10,
        "Aeonis, the Timeless One", PetRarity::Legendary);

    // Evolution names and special abilities
    SetEvolutionData("ember_sentinel", "Inferno Sentinel", "Ember Lord",
                     EffectType::Burn, 0.35f, 3);
    SetEvolutionData("frost_wisp", "Glacial Wisp", "Frost Specter",
                     EffectType::Freeze, 0.0f, 2);
    SetEvolutionData("storm_tide", "Tempest Tide", "Storm Sovereign",
                     EffectType::Stun, 0.0f, 1);
    SetEvolutionData("arcane_drake", "Mystic Drake", "Arcane Wyrm",
                     EffectType::Burn, 0.30f, 2);
    SetEvolutionData("venom_shard", "Toxic Shard", "Venom Core",
                     EffectType::Poison, 0.30f, 3);
    SetEvolutionData("inferno_guardian", "Hellfire Guardian", "Inferno Overlord",
                     EffectType::Burn, 0.40f, 3);
    SetEvolutionData("celestial_spark", "Radiant Spark", "Celestial Flame",
                     EffectType::Burn, 0.35f, 2);
    SetEvolutionData("void_mote", "Abyssal Mote", "Void Essence",
                     EffectType::Burn, 0.35f, 2);
    SetEvolutionData("mana_weaver", "Arcane Weaver", "Mana Sovereign",
                     EffectType::Burn, 0.30f, 2);
    SetEvolutionData("chrono_spark", "Temporal Spark", "Chrono Warden",
                     EffectType::Stun, 0.0f, 1);

    SetEvolutionData("goblin_familiar", "Goblin Champion", "Goblin Warlord",
                     EffectType::Stun, 0.0f, 1);
    SetEvolutionData("treant_sapling", "Treant Guardian", "Treant Elder",
                     EffectType::Freeze, 0.0f, 2);
    SetEvolutionData("kraken_ink", "Kraken Venom", "Kraken Dread",
                     EffectType::Poison, 0.30f, 3);
    SetEvolutionData("dragon_whelp", "Drake", "Elder Dragon",
                     EffectType::Burn, 0.40f, 3);
    SetEvolutionData("warlord_banner", "Warlord Standard", "Warlord Dread",
                     EffectType::Stun, 0.0f, 1);
    SetEvolutionData("void_shard", "Void Crystal", "Void Core",
                     EffectType::Burn, 0.35f, 2);
    SetEvolutionData("seraph_feather", "Seraph Wing", "Seraph Halo",
                     EffectType::Burn, 0.35f, 2);
    SetEvolutionData("primordial_heart", "Primordial Core", "Primordial Soul",
                     EffectType::Burn, 0.45f, 3);
    SetEvolutionData("construct_core", "Construct Matrix", "Construct Prime",
                     EffectType::Burn, 0.35f, 2);
    SetEvolutionData("chrono_fragment", "Chrono Shard", "Chrono Crystal",
                     EffectType::Stun, 0.0f, 1);

    SetMythicEvolutionData("ember_sentinel", "Ember God");
    SetMythicEvolutionData("frost_wisp", "Frost Eternal");
    SetMythicEvolutionData("storm_tide", "Storm God");
    SetMythicEvolutionData("arcane_drake", "Arcane Overlord");
    SetMythicEvolutionData("venom_shard", "Venom God");
    SetMythicEvolutionData("inferno_guardian", "Inferno God");
    SetMythicEvolutionData("celestial_spark", "Celestial God");
    SetMythicEvolutionData("void_mote", "Void God");
    SetMythicEvolutionData("mana_weaver", "Mana God");
    SetMythicEvolutionData("chrono_spark", "Chrono God");

    SetEvolutionData("shadowfang_relic", "Shadowfang Totem", "Shadowfang Icon",
                     EffectType::Burn, 0.45f, 4);
    SetEvolutionData("flamecrown_shard", "Flamecrown Gem", "Flamecrown Heart",
                     EffectType::Burn, 0.50f, 4);
    SetEvolutionData("abyssal_crown", "Abyssal Diadem", "Abyssal Regalia",
                     EffectType::Freeze, 0.0f, 3);
    SetEvolutionData("tempest_core", "Tempest Heart", "Tempest Core Ascendant",
                     EffectType::Stun, 0.0f, 2);
    SetEvolutionData("rotbloom_heart", "Rotbloom Bloom", "Rotbloom Eternal",
                     EffectType::Poison, 0.45f, 4);
    SetEvolutionData("seraph_halo", "Seraph Radiance", "Seraph Divinity",
                     EffectType::Burn, 0.40f, 3);
    SetEvolutionData("verdant_serpent", "Verdant Wyrm", "Verdant Ascendant",
                     EffectType::Poison, 0.45f, 4);
    SetEvolutionData("warlords_bloodstone", "Warlord's Wrath", "Warlord's Fury",
                     EffectType::Stun, 0.0f, 2);
    SetEvolutionData("voidheart_gem", "Voidheart Crystal", "Voidheart Singularity",
                     EffectType::Burn, 0.50f, 4);
    SetEvolutionData("chronos_hourglass", "Chronos Eternal", "Chronos Infinite",
                     EffectType::Stun, 0.0f, 2);

    SetMythicEvolutionData("shadowfang_relic", "Shadowfang God");
    SetMythicEvolutionData("flamecrown_shard", "Flamecrown God");
    SetMythicEvolutionData("abyssal_crown", "Abyssal God");
    SetMythicEvolutionData("tempest_core", "Tempest God");
    SetMythicEvolutionData("rotbloom_heart", "Rotbloom God");
    SetMythicEvolutionData("seraph_halo", "Seraph God");
    SetMythicEvolutionData("verdant_serpent", "Verdant God");
    SetMythicEvolutionData("warlords_bloodstone", "Warlord God");
    SetMythicEvolutionData("voidheart_gem", "Voidheart God");
    SetMythicEvolutionData("chronos_hourglass", "Chronos God");
}

void PetManager::SetEvolutionData(const std::string& id,
    const std::string& evName, const std::string& ascName,
    EffectType ability, float potencyMult, int baseDuration)
{
    Pet* p = GetPet(id);
    if (!p) return;
    p->evolvedName = evName;
    p->ascendedName = ascName;
    p->specialAbility = ability;
    p->abilityPotencyMult = potencyMult;
    p->baseAbilityDuration = baseDuration;
}

void PetManager::SetMythicEvolutionData(const std::string& id, const std::string& mythicName)
{
    Pet* p = GetPet(id);
    if (!p) return;
    p->mythicName = mythicName;
}

void PetManager::AddFactionPet(FactionID faction, const std::string& id,
    const std::string& name, const std::string& desc, ElementType element,
    int baseAtk, int reqLevel, float xpBonus, float goldFind, float critChance,
    float critDamage, float dmgBonus, float defBonus, float hpBonus, float mpBonus,
    int healOnKill)
{
    Pet p;
    p.id = id;
    p.name = name;
    p.description = desc;
    p.element = element;
    p.baseAttack = baseAtk;
    p.reqLevel = reqLevel;
    p.xpBonus = xpBonus;
    p.goldFind = goldFind;
    p.critChance = critChance;
    p.critDamage = critDamage;
    p.damageBonus = dmgBonus;
    p.defenseBonus = defBonus;
    p.healthBonus = hpBonus;
    p.manaBonus = mpBonus;
    p.healOnKill = healOnKill;
    p.source = PetSource::FactionLegend;
    p.faction = faction;
    p.obtained = false;
    p.equipped = false;
    pets.push_back(p);
}

void PetManager::AddBossPet(const std::string& id, const std::string& name,
    const std::string& desc, ElementType element, int baseAtk, int reqLevel,
    float xpBonus, float goldFind, float critChance, float critDamage,
    float dmgBonus, float defBonus, float hpBonus, float mpBonus, int healOnKill,
    const std::string& bossName)
{
    Pet p;
    p.id = id;
    p.name = name;
    p.description = desc;
    p.element = element;
    p.baseAttack = baseAtk;
    p.reqLevel = reqLevel;
    p.xpBonus = xpBonus;
    p.goldFind = goldFind;
    p.critChance = critChance;
    p.critDamage = critDamage;
    p.damageBonus = dmgBonus;
    p.defenseBonus = defBonus;
    p.healthBonus = hpBonus;
    p.manaBonus = mpBonus;
    p.healOnKill = healOnKill;
    p.source = PetSource::BossDrop;
    p.bossName = bossName;
    p.obtained = false;
    p.equipped = false;
    pets.push_back(p);
}

void PetManager::AddCitadelPet(const std::string& id, const std::string& name,
    const std::string& desc, ElementType element, int baseAtk, int reqLevel,
    float xpBonus, float goldFind, float critChance, float critDamage,
    float dmgBonus, float defBonus, float hpBonus, float mpBonus, int healOnKill,
    const std::string& bossName, PetRarity rarity)
{
    Pet p;
    p.id = id;
    p.name = name;
    p.description = desc;
    p.element = element;
    p.baseAttack = baseAtk;
    p.reqLevel = reqLevel;
    p.xpBonus = xpBonus;
    p.goldFind = goldFind;
    p.critChance = critChance;
    p.critDamage = critDamage;
    p.damageBonus = dmgBonus;
    p.defenseBonus = defBonus;
    p.healthBonus = hpBonus;
    p.manaBonus = mpBonus;
    p.healOnKill = healOnKill;
    p.source = PetSource::CitadelDrop;
    p.bossName = bossName;
    p.rarity = rarity;
    p.obtained = false;
    p.equipped = false;
    pets.push_back(p);
}

Pet* PetManager::GetPet(const std::string& id)
{
    for (auto& p : pets)
        if (p.id == id) return &p;
    return nullptr;
}

const Pet* PetManager::GetPet(const std::string& id) const
{
    for (const auto& p : pets)
        if (p.id == id) return &p;
    return nullptr;
}

Pet* PetManager::GetEquippedPet()
{
    for (auto& p : pets)
        if (p.equipped) return &p;
    return nullptr;
}

const Pet* PetManager::GetEquippedPet() const
{
    for (const auto& p : pets)
        if (p.equipped) return &p;
    return nullptr;
}

void PetManager::ObtainPet(const std::string& id)
{
    auto* p = GetPet(id);
    if (p) p->obtained = true;
}

bool PetManager::EquipPet(const std::string& id)
{
    for (auto& p : pets)
        p.equipped = false;

    auto* p = GetPet(id);
    if (!p || !p->obtained) return false;
    p->equipped = true;
    return true;
}

void PetManager::UnequipAll()
{
    for (auto& p : pets)
        p.equipped = false;
}

std::string PetManager::GetFactionLegendPetID(FactionID faction) const
{
    for (const auto& p : pets)
    {
        if (p.source == PetSource::FactionLegend && p.faction == faction)
            return p.id;
    }
    return "";
}

std::string PetManager::RollBossDropPet(int bossLevel, const std::string& bossName) const
{
    // Check for Citadel boss drops first (2% rate)
    std::vector<std::string> citadelCandidates;
    for (const auto& p : pets)
    {
        if (p.source == PetSource::CitadelDrop && p.bossName == bossName && !p.obtained)
            citadelCandidates.push_back(p.id);
    }
    if (!citadelCandidates.empty() && RNG::Percent() < 2)
    {
        return citadelCandidates[RNG::Next(static_cast<int>(citadelCandidates.size()))];
    }

    // Regular boss drops (8% rate)
    if (RNG::Percent() >= 8) return "";

    std::vector<std::string> candidates;
    for (const auto& p : pets)
    {
        if (p.source == PetSource::BossDrop && p.bossName == bossName && !p.obtained)
            candidates.push_back(p.id);
    }

    if (candidates.empty()) return "";
    return candidates[RNG::Next(static_cast<int>(candidates.size()))];
}

int PetManager::CalculatePetDamage(int playerLevel) const
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return 0;
    float scaling = 1.0f + (playerLevel - 1) * 0.02f;
    return static_cast<int>(pet->GetScaledAttack() * scaling);
}

ElementType PetManager::GetPetElement() const
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return ElementType::Physical;
    return pet->element;
}

std::string PetManager::ExecutePetAttack(int playerLevel, const std::string& targetName)
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return "";

    int damage = CalculatePetDamage(playerLevel);
    std::string elemStr;
    if (pet->element != ElementType::Physical)
        elemStr = " [" + std::string(ElementName(pet->element)) + "]";

    return pet->GetCurrentName() + " attacks " + targetName + elemStr
        + " for " + std::to_string(damage) + " damage!";
}

PetAbilityResult PetManager::TryProcPetAbility(int playerLevel) const
{
    PetAbilityResult result;
    const Pet* pet = GetEquippedPet();
    if (!pet || pet->evolutionTier == 0) return result;
    if (pet->specialAbility == EffectType::None) return result;

    int procChance = (pet->evolutionTier == 1) ? 30 : (pet->evolutionTier == 2) ? 50 : 70;
    if (RNG::Percent() >= procChance) return result;

    result.procced = true;
    result.effect = pet->specialAbility;
    result.potency = pet->GetAbilityPotency();
    result.duration = pet->baseAbilityDuration + (pet->evolutionTier >= 2 ? 1 : 0) + (pet->evolutionTier == 3 ? 1 : 0);
    result.abilityName = pet->GetAbilityName();
    result.message = pet->GetCurrentName() + " uses " + result.abilityName + "!";

    return result;
}

float PetManager::GetXPBonus() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledXPBonus() : 0.0f;
}

float PetManager::GetGoldFind() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledGoldFind() : 0.0f;
}

float PetManager::GetCritChance() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledCritChance() : 0.0f;
}

float PetManager::GetCritDamage() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledCritDamage() : 0.0f;
}

float PetManager::GetDamageBonus() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledDamageBonus() : 0.0f;
}

float PetManager::GetDefenseBonus() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledDefenseBonus() : 0.0f;
}

int PetManager::GetHealthBonus() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledHealthBonus() : 0;
}

int PetManager::GetManaBonus() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledManaBonus() : 0;
}

int PetManager::GetHealOnKill() const
{
    const Pet* pet = GetEquippedPet();
    return pet ? pet->GetScaledHealOnKill() : 0;
}

int PetManager::GetEquippedPetSlot() const
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return -1;
    for (int i = 0; i < static_cast<int>(pets.size()); ++i)
    {
        if (pets[i].equipped) return i;
    }
    return -1;
}

float PetManager::GetBossDamageReduction() const
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return 0.0f;
    if (pet->source != PetSource::CitadelDrop) return 0.0f;
    if (pet->evolutionTier == 0) return 0.0f;
    if (pet->evolutionTier == 1) return 0.05f;
    if (pet->evolutionTier == 2) return 0.10f;
    return 0.15f; // tier 3
}

float PetManager::GetBossXPBonus() const
{
    const Pet* pet = GetEquippedPet();
    if (!pet) return 0.0f;
    if (pet->source != PetSource::CitadelDrop) return 0.0f;
    if (pet->evolutionTier == 0) return 0.0f;
    if (pet->evolutionTier == 1) return 0.10f;
    if (pet->evolutionTier == 2) return 0.25f;
    return 0.50f; // tier 3
}

int Pet::CalculateRequiredXP(int level)
{
    return level * 100;
}

void PetManager::GainPetXP(int xp)
{
    Pet* pet = GetEquippedPet();
    if (!pet || pet->level >= Pet::MAX_PET_LEVEL) return;

    pet->experience += xp;

    while (pet->experience >= Pet::CalculateRequiredXP(pet->level) && pet->level < Pet::MAX_PET_LEVEL)
    {
        pet->experience -= Pet::CalculateRequiredXP(pet->level);
        pet->level++;
        leveledUpFlag = true;

        // Check for evolution
        if (pet->CanEvolveNow())
        {
            pet->evolutionTier = 1;
            evolvedFlag = true;
        }
        else if (pet->CanAscendNow())
        {
            pet->evolutionTier = 2;
            evolvedFlag = true;
        }
        else if (pet->CanMythicNow())
        {
            pet->evolutionTier = 3;
            evolvedFlag = true;
        }
    }

    if (pet->level >= Pet::MAX_PET_LEVEL)
    {
        pet->experience = 0;
    }
}

bool PetManager::HasLeveledUp() const
{
    return leveledUpFlag;
}

void PetManager::ClearLevelUpFlag()
{
    leveledUpFlag = false;
}

bool PetManager::HasEvolved() const
{
    return evolvedFlag;
}

void PetManager::ClearEvolveFlag()
{
    evolvedFlag = false;
}

std::string Pet::GetAbilityName() const
{
    switch (specialAbility)
    {
        case EffectType::Burn:    return "Ignite";
        case EffectType::Poison:  return "Toxic Cloud";
        case EffectType::Freeze:  return "Frostbite";
        case EffectType::Stun:    return "Static Shock";
        default:                  return "None";
    }
}

std::string Pet::GetAbilityDescription() const
{
    if (specialAbility == EffectType::None) return "No special ability";
    std::string name = GetAbilityName();
    std::string target;
    switch (specialAbility)
    {
        case EffectType::Burn:    target = "burn"; break;
        case EffectType::Poison:  target = "poison"; break;
        case EffectType::Freeze:  target = "freeze"; break;
        case EffectType::Stun:    target = "stun"; break;
        default: break;
    }
    int procChance = (evolutionTier == 1) ? 30 : (evolutionTier == 2) ? 50 : 70;
    int dur = baseAbilityDuration + (evolutionTier >= 2 ? 1 : 0) + (evolutionTier == 3 ? 1 : 0);
    std::string desc = std::to_string(procChance) + "% chance to " + target
                     + " for " + std::to_string(dur) + " turns";
    if (specialAbility == EffectType::Burn || specialAbility == EffectType::Poison)
        desc += " (potency scales with ATK)";
    return desc;
}

void PetManager::AddNotification(const std::string& petName, const std::string& message)
{
    notifications.emplace_back(petName, message);
}

void PetManager::UpdateNotifications(float dt)
{
    for (int i = static_cast<int>(notifications.size()) - 1; i >= 0; --i)
    {
        notifications[i].life -= dt;
        if (notifications[i].life <= 0.0f)
            notifications.erase(notifications.begin() + i);
    }
}

std::string PetManager::Serialize() const
{
    std::ostringstream ss;
    for (const auto& p : pets)
    {
        ss << p.id << ":" << (p.obtained ? "1" : "0")
           << ":" << (p.equipped ? "1" : "0")
           << ":" << p.level << ":" << p.experience
           << ":" << p.evolutionTier << ":" << static_cast<int>(p.rarity) << ";";
    }
    return ss.str();
}

void PetManager::Deserialize(const std::string& data)
{
    if (data.empty()) return;
    if (pets.empty()) Initialize();
    std::istringstream ss(data);
    std::string entry;
    while (std::getline(ss, entry, ';'))
    {
        if (entry.empty()) continue;
        size_t c1 = entry.find(':');
        if (c1 == std::string::npos) continue;
        std::string id = entry.substr(0, c1);
        size_t c2 = entry.find(':', c1 + 1);
        if (c2 == std::string::npos) continue;
        std::string obtStr = entry.substr(c1 + 1, c2 - c1 - 1);
        std::string rest = entry.substr(c2 + 1);
        Pet* p = GetPet(id);
        if (p)
        {
            p->obtained = (obtStr == "1");

            // Parse: equipped:level:experience:evolutionTier:rarity
            size_t c3 = rest.find(':');
            if (c3 != std::string::npos)
            {
                std::string eqStr = rest.substr(0, c3);
                std::string remainder = rest.substr(c3 + 1);
                p->equipped = (eqStr == "1");

                size_t c4 = remainder.find(':');
                if (c4 != std::string::npos)
                {
                    p->level = std::stoi(remainder.substr(0, c4));
                    std::string remainder2 = remainder.substr(c4 + 1);

                    size_t c5 = remainder2.find(':');
                    if (c5 != std::string::npos)
                    {
                        p->experience = std::stoi(remainder2.substr(0, c5));
                        std::string remainder3 = remainder2.substr(c5 + 1);

                        size_t c6 = remainder3.find(':');
                        if (c6 != std::string::npos)
                        {
                            p->evolutionTier = std::stoi(remainder3.substr(0, c6));
                            std::string rarityStr = remainder3.substr(c6 + 1);
                            if (!rarityStr.empty())
                                p->rarity = static_cast<PetRarity>(std::stoi(rarityStr));
                        }
                        else
                        {
                            p->evolutionTier = std::stoi(remainder3);
                        }
                    }
                    else
                    {
                        p->experience = std::stoi(remainder2);
                    }
                }
            }
        }
    }
}
