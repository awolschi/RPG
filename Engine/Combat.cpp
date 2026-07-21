#include "Combat.hpp"
#include "../Items/Passives.hpp"
#include "../Characters/Character.hpp"
#include "../Engine/RNG.hpp"
#include <sstream>

namespace {
    // Roll a critical hit using the attacker's pet bonus crit chance.
    // Returns the crit damage multiplier (e.g. 1.0 normal, 1.5 crit w/ 50% bonus).
    float PetCritMultiplier(const Character& attacker)
    {
        float chance = attacker.GetPetBonusCritChance();
        if (chance <= 0.0f) return 1.0f;
        int rolled = RNG::Next(1000);
        if (rolled < static_cast<int>(chance * 1000.0f))
            return 1.5f + attacker.GetPetBonusCritDamage();
        return 1.0f;
    }
}

bool CombatSystem::StartCombat(std::shared_ptr<Character> player,
                               std::shared_ptr<Character> enemy)
{
    if (!player || !enemy || !player->IsAlive() || !enemy->IsAlive())
        return false;
    player->ResetTempDefense();
    player->ClearAllEffects();
    enemy->ClearAllEffects();
    return true;
}

std::string CombatSystem::ExecuteTurn(std::shared_ptr<Character> attacker,
                                       std::shared_ptr<Character> defender,
                                       CombatAction action,
                                       int skillIndex)
{
    if (!attacker || !defender)
        return "Invalid combatants.";

    // Process effects on both characters
    std::string effectLog = attacker->ProcessEffects();
    effectLog += defender->ProcessEffects();

    // Turn start: Mana regen from passives
    int manaRegen = Passives::GetManaRegenBonus(attacker->GetEquipment());
    if (manaRegen > 0)
    {
        attacker->RestoreMana(manaRegen);
        effectLog += attacker->GetName() + " regenerates " + std::to_string(manaRegen) + " mana. ";
    }

    // Check if attacker is stunned (immune check)
    if (attacker->IsStunned())
    {
        if (!Passives::IsStunImmune(attacker->GetEquipment()))
        {
            attacker->GetSkills().UpdateCooldowns();
            return effectLog + attacker->GetName() + " is stunned and cannot act!";
        }
        else
        {
            effectLog += attacker->GetName() + " resists stun! ";
            attacker->RemoveEffect(EffectType::Stun);
        }
    }

    std::string actionMsg;
    switch (action)
    {
        case CombatAction::Attack:
        {
            // Dodge check
            if (Passives::RollProc(Passives::GetDodgePercent(defender->GetEquipment())))
            {
                attacker->GetSkills().UpdateCooldowns();
                actionMsg = defender->GetName() + " dodges the attack! ";
                break;
            }

            auto attackSkill = attacker->GetSkills().GetSkill(skillIndex);
            if (attackSkill)
            {
                int hpBefore = defender->GetCurrentHealth();
                attackSkill->Use(*attacker, *defender);

                // Apply physical damage boost
                int physBoost = Passives::GetPhysicalDamageBoostPercent(attacker->GetEquipment());
                int baseDmg = hpBefore - defender->GetCurrentHealth();
                if (physBoost > 0 && baseDmg > 0)
                {
                    int bonusDmg = baseDmg * physBoost / 100;
                    defender->TakeDamage(bonusDmg, ElementType::Physical);
                }

                // Pet damage bonus + crit roll (applied as bonus hit)
                float petDmgPct = attacker->GetPetBonusDamage();
                float critMult = PetCritMultiplier(*attacker);
                int afterPhys = defender->GetCurrentHealth();
                int dealtSoFar = hpBefore - afterPhys;
                if (dealtSoFar > 0 && (petDmgPct > 0.0f || critMult > 1.0f))
                {
                    int bonusDmg = static_cast<int>(dealtSoFar * petDmgPct * critMult);
                    if (bonusDmg > 0)
                    {
                        defender->TakeDamage(bonusDmg, ElementType::Physical);
                        if (critMult > 1.0f)
                            actionMsg += " (CRITICAL pet strike +" + std::to_string(bonusDmg) + ")";
                    }
                }

                attacker->GetSkills().UpdateCooldowns();
                int dmg = hpBefore - defender->GetCurrentHealth();
                ElementType effective = attacker->GetEffectiveElement(attackSkill->element);
                std::string elemStr = (effective != ElementType::Physical)
                    ? " [" + std::string(ElementName(effective)) + "]" : "";
                if (dmg > 0)
                    actionMsg = attacker->GetName() + " attacks " + defender->GetName()
                        + elemStr + " (" + std::to_string(dmg) + " damage)!";
                else
                    actionMsg = attacker->GetName() + " attacks " + defender->GetName() + elemStr + "!";

                // Lifesteal
                int lifestealPct = Passives::GetLifestealPercent(attacker->GetEquipment());
                if (lifestealPct > 0 && dmg > 0)
                {
                    int heal = dmg * lifestealPct / 100;
                    if (heal < 1) heal = 1;
                    attacker->RestoreHealth(heal);
                    actionMsg += " (" + std::to_string(heal) + " lifesteal)";
                }
            }
            else
                actionMsg = attacker->GetName() + " has no attack!";
            break;
        }

        case CombatAction::UseSkill:
        {
            auto skill = attacker->GetSkills().GetSkill(skillIndex);
            if (!skill)
                return effectLog + "Invalid skill!";

            if (!skill->IsReady())
                return effectLog + skill->name + " is on cooldown! (" +
                       std::to_string(skill->currentCooldown) + " turns)";

            int effectiveManaCost = skill->GetEffectiveManaCost();
            int manaCostReduction = Passives::GetManaCostReductionPercent(attacker->GetEquipment());
            if (manaCostReduction > 0)
                effectiveManaCost = effectiveManaCost * (100 - manaCostReduction) / 100;

            if (attacker->GetCurrentMana() < effectiveManaCost)
                return effectLog + "Not enough mana for " + skill->name + "!";

            int hpBefore = defender->GetCurrentHealth();
            skill->Use(*attacker, *defender);
            attacker->ReduceMana(effectiveManaCost);

            // Cooldown reduction from passives
            int cdReduction = Passives::GetCooldownReductionTurns(attacker->GetEquipment());
            for (int i = 0; i < cdReduction; ++i)
                attacker->GetSkills().GetSkill(skillIndex)->ReduceCooldown();

            attacker->GetSkills().UpdateCooldowns();

            // Mana on skill use
            int manaOnSkill = Passives::GetManaOnSkillUseAmount(attacker->GetEquipment());
            if (manaOnSkill > 0)
                attacker->RestoreMana(manaOnSkill);

            // HP on skill use
            int hpOnSkill = Passives::GetHpOnSkillUseAmount(attacker->GetEquipment());
            if (hpOnSkill > 0)
                attacker->RestoreHealth(hpOnSkill);

            int hpAfter = defender->GetCurrentHealth();
            int diff = hpAfter - hpBefore;
            ElementType effective = attacker->GetEffectiveElement(skill->element);
            std::string elemStr = (effective != ElementType::Physical)
                ? " [" + std::string(ElementName(effective)) + "]" : "";
            if (diff > 0)
                actionMsg = attacker->GetName() + " uses " + skill->name + elemStr
                    + " (+" + std::to_string(diff) + " HP)!";
            else if (diff < 0)
                actionMsg = attacker->GetName() + " uses " + skill->name + elemStr
                    + " (" + std::to_string(-diff) + " damage)!";
            else
                actionMsg = attacker->GetName() + " uses " + skill->name + elemStr + "!";

            // Spell damage boost
            int spellBoost = Passives::GetSpellDamageBoostPercent(attacker->GetEquipment());
            if (spellBoost > 0 && diff < 0)
            {
                int bonusDmg = (-diff) * spellBoost / 100;
                defender->TakeDamage(bonusDmg, effective);
                actionMsg += " (+" + std::to_string(bonusDmg) + " spell boost)";
            }

            // Pet damage bonus + crit roll for damage-dealing skills
            float petDmgPct = attacker->GetPetBonusDamage();
            float critMult = PetCritMultiplier(*attacker);
            if (diff < 0 && (petDmgPct > 0.0f || critMult > 1.0f))
            {
                int baseSkillDmg = -diff;
                int bonusDmg = static_cast<int>(baseSkillDmg * petDmgPct * critMult);
                if (bonusDmg > 0)
                {
                    defender->TakeDamage(bonusDmg, effective);
                    if (critMult > 1.0f)
                        actionMsg += " (CRITICAL pet +" + std::to_string(bonusDmg) + ")";
                    else
                        actionMsg += " (pet +" + std::to_string(bonusDmg) + ")";
                }
            }

            // Lifesteal on skill
            int lifestealPct = Passives::GetLifestealPercent(attacker->GetEquipment());
            if (lifestealPct > 0 && diff < 0)
            {
                int heal = (-diff) * lifestealPct / 100;
                if (heal < 1) heal = 1;
                attacker->RestoreHealth(heal);
                actionMsg += " (+" + std::to_string(heal) + " lifesteal)";
            }
            break;
        }

        case CombatAction::Defend:
            attacker->IncreaseTempDefense(5 + attacker->GetLevel() / 2 + attacker->GetStats().defense / 4);
            attacker->GetSkills().UpdateCooldowns();
            actionMsg = attacker->GetName() + " takes a defensive stance!";
            break;

        case CombatAction::Flee:
            actionMsg = attacker->GetName() + " attempts to flee!";
            break;

        case CombatAction::UseItem:
            attacker->GetSkills().UpdateCooldowns();
            actionMsg = attacker->GetName() + " uses an item!";
            break;

        default:
            actionMsg = "Unknown action.";
            break;
    }

    return effectLog + actionMsg;
}

std::string CombatSystem::GetStatusLine(std::shared_ptr<Character> c)
{
    if (!c) return "";
    std::ostringstream os;
    os << c->GetName() << " HP:" << c->GetCurrentHealth()
       << "/" << c->GetStats().health
       << " MP:" << c->GetCurrentMana()
       << "/" << c->GetStats().mana;
    for (const auto& e : c->GetEffects())
    {
        switch (e.type)
        {
            case EffectType::Poison: os << " [Poison " << e.duration << "]"; break;
            case EffectType::Burn:   os << " [Burn " << e.duration << "]"; break;
            case EffectType::Stun:   os << " [Stunned]"; break;
            case EffectType::Freeze: os << " [Frozen]"; break;
            default: break;
        }
    }
    return os.str();
}

std::vector<std::string> CombatSystem::GetSkillOptions(std::shared_ptr<Character> c)
{
    std::vector<std::string> opts;
    if (!c) return opts;
    for (size_t i = 0; i < c->GetSkills().GetSkillCount(); ++i)
    {
        auto s = c->GetSkills().GetSkill(i);
        if (s)
        {
            std::string entry = std::to_string(i) + ". " + s->name
                + " (Mana:" + std::to_string(s->GetEffectiveManaCost())
                + " CD:" + std::to_string(s->currentCooldown) + "/" + std::to_string(s->GetEffectiveCooldown())
                + " Req:" + std::to_string(s->requiredLevel) + ")";
            opts.push_back(entry);
        }
    }
    return opts;
}
