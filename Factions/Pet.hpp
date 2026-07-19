#ifndef PET_HPP
#define PET_HPP

#include <string>
#include <vector>
#include <map>
#include "../Skills/Skill.hpp"
#include "../Characters/Character.hpp"
#include "FactionReputation.hpp"

enum class PetSource
{
    FactionLegend,
    BossDrop,
    PetCount
};

struct Pet
{
    std::string id;
    std::string name;
    std::string description;
    ElementType element;
    int baseAttack;
    int reqLevel;

    // Passive bonuses (base values, scale with pet level)
    float xpBonus = 0.0f;
    float goldFind = 0.0f;
    float critChance = 0.0f;
    float critDamage = 0.0f;
    float damageBonus = 0.0f;
    float defenseBonus = 0.0f;
    float healthBonus = 0.0f;
    float manaBonus = 0.0f;
    int healOnKill = 0;

    // Leveling
    int level = 1;
    int experience = 0;
    static constexpr int MAX_PET_LEVEL = 50;
    static int CalculateRequiredXP(int level);

    // Evolution
    int evolutionTier = 0;  // 0=base, 1=evolved, 2=ascended
    std::string evolvedName;
    std::string ascendedName;
    static constexpr int EVOLVE_LEVEL = 15;
    static constexpr int ASCEND_LEVEL = 30;

    // Special ability (unlocked at evolution tier 1+)
    EffectType specialAbility = EffectType::None;
    float abilityPotencyMult = 0.0f;
    int baseAbilityDuration = 0;

    // Level + evolution multiplier
    float GetLevelMultiplier() const
    {
        float evoMult = 1.0f;
        if (evolutionTier == 1) evoMult = 1.4f;
        else if (evolutionTier == 2) evoMult = 2.0f;
        return evoMult * (1.0f + (level - 1) * 0.03f);
    }

    // Scaled getters (base * level multiplier)
    int GetScaledAttack() const { return static_cast<int>(baseAttack * GetLevelMultiplier()); }
    float GetScaledXPBonus() const { return xpBonus * GetLevelMultiplier(); }
    float GetScaledGoldFind() const { return goldFind * GetLevelMultiplier(); }
    float GetScaledCritChance() const { return critChance * GetLevelMultiplier(); }
    float GetScaledCritDamage() const { return critDamage * GetLevelMultiplier(); }
    float GetScaledDamageBonus() const { return damageBonus * GetLevelMultiplier(); }
    float GetScaledDefenseBonus() const { return defenseBonus * GetLevelMultiplier(); }
    int GetScaledHealthBonus() const { return static_cast<int>(healthBonus * GetLevelMultiplier()); }
    int GetScaledManaBonus() const { return static_cast<int>(manaBonus * GetLevelMultiplier()); }
    int GetScaledHealOnKill() const { return static_cast<int>(healOnKill * GetLevelMultiplier()); }

    // Evolution helpers
    bool CanEvolve() const { return !obtained && false; }
    bool CanEvolveNow() const { return obtained && evolutionTier == 0 && level >= EVOLVE_LEVEL; }
    bool CanAscendNow() const { return obtained && evolutionTier == 1 && level >= ASCEND_LEVEL; }

    std::string GetCurrentName() const
    {
        if (evolutionTier == 2 && !ascendedName.empty()) return ascendedName;
        if (evolutionTier == 1 && !evolvedName.empty()) return evolvedName;
        return name;
    }

    std::string GetEvolutionLabel() const
    {
        if (evolutionTier == 2) return "Ascended";
        if (evolutionTier == 1) return "Evolved";
        return "Base";
    }

    std::string GetAbilityName() const;
    std::string GetAbilityDescription() const;

    int GetAbilityPotency() const
    {
        if (specialAbility == EffectType::None) return 0;
        if (specialAbility == EffectType::Stun || specialAbility == EffectType::Freeze)
            return 0;
        return static_cast<int>(GetScaledAttack() * abilityPotencyMult);
    }

    PetSource source;
    FactionID faction = FactionID::GuardiansOfFelrona;
    std::string bossName;
    bool obtained = false;
    bool equipped = false;
};

struct PetNotification
{
    std::string petName;
    std::string message;
    float life;
    float maxLife;

    PetNotification(const std::string& name, const std::string& msg, float dur = 3.0f)
        : petName(name), message(msg), life(dur), maxLife(dur) {}
};

struct PetAbilityResult
{
    bool procced = false;
    EffectType effect = EffectType::None;
    int potency = 0;
    int duration = 0;
    std::string abilityName;
    std::string message;
};

class PetManager
{
public:
    PetManager();

    void Initialize();

    const std::vector<Pet>& GetPets() const { return pets; }
    std::vector<Pet>& GetPets() { return pets; }
    Pet* GetPet(const std::string& id);
    const Pet* GetPet(const std::string& id) const;
    Pet* GetEquippedPet();
    const Pet* GetEquippedPet() const;

    void ObtainPet(const std::string& id);
    bool EquipPet(const std::string& id);
    void UnequipAll();

    // Pet XP and leveling
    void GainPetXP(int xp);
    bool HasLeveledUp() const;
    void ClearLevelUpFlag();
    bool HasEvolved() const;
    void ClearEvolveFlag();

    // Combat (level-scaled)
    int CalculatePetDamage(int playerLevel) const;
    ElementType GetPetElement() const;
    std::string ExecutePetAttack(int playerLevel, const std::string& targetName);

    // Special ability proc
    PetAbilityResult TryProcPetAbility(int playerLevel) const;

    // Passive bonuses from equipped pet (level-scaled)
    float GetXPBonus() const;
    float GetGoldFind() const;
    float GetCritChance() const;
    float GetCritDamage() const;
    float GetDamageBonus() const;
    float GetDefenseBonus() const;
    int GetHealthBonus() const;
    int GetManaBonus() const;
    int GetHealOnKill() const;

    std::string GetFactionLegendPetID(FactionID faction) const;
    std::string RollBossDropPet(int bossLevel, const std::string& bossName) const;

    std::string Serialize() const;
    void Deserialize(const std::string& data);

    std::vector<PetNotification> notifications;
    void AddNotification(const std::string& petName, const std::string& message);
    void UpdateNotifications(float dt);

private:
    std::vector<Pet> pets;
    bool leveledUpFlag = false;
    bool evolvedFlag = false;

    void SetEvolutionData(const std::string& id,
                          const std::string& evolvedName,
                          const std::string& ascendedName,
                          EffectType ability, float potencyMult, int baseDuration);

    void AddFactionPet(FactionID faction, const std::string& id, const std::string& name,
                       const std::string& desc, ElementType element, int baseAtk,
                       int reqLevel, float xpBonus, float goldFind, float critChance,
                       float critDamage, float dmgBonus, float defBonus,
                       float hpBonus, float mpBonus, int healOnKill);
    void AddBossPet(const std::string& id, const std::string& name,
                    const std::string& desc, ElementType element, int baseAtk,
                    int reqLevel, float xpBonus, float goldFind, float critChance,
                    float critDamage, float dmgBonus, float defBonus,
                    float hpBonus, float mpBonus, int healOnKill,
                    const std::string& bossName);
};

#endif
