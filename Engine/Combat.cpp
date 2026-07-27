#include "Combat.hpp"
#include "../Items/Passives.hpp"
#include "../Characters/Character.hpp"
#include "../Engine/RNG.hpp"

namespace {
    float PetCritMultiplier(const Character& attacker)
    {
        float chance = attacker.GetPetBonusCritChance();
        if (chance <= 0.0f) return 1.0f;
        int rolled = RNG::Next(1000);
        if (rolled < static_cast<int>(chance * 1000.0f))
            return 1.5f + attacker.GetPetBonusCritDamage();
        return 1.0f;
    }

    struct BonusResult {
        int critBonus = 0;
        bool didCrit = false;
        bool didDoubleStrike = false;
    };

    BonusResult ApplyEquipmentBonuses(Character& attacker, Character& defender,
                                       int baseDmg, ElementType effective,
                                       std::string& actionMsg)
    {
        BonusResult result;
        if (baseDmg <= 0) return result;

        int critChance = Passives::GetCritChancePercent(attacker.GetEquipment());
        if (critChance > 0 && Passives::RollProc(critChance))
        {
            result.critBonus = baseDmg;
            result.didCrit = true;
            defender.TakeDamageRaw(result.critBonus);
            actionMsg += " (CRITICAL +" + std::to_string(result.critBonus) + ")";
        }

        int atkSpeed = Passives::GetAttackSpeedPercent(attacker.GetEquipment());
        if (atkSpeed > 0 && Passives::RollProc(atkSpeed))
        {
            result.didDoubleStrike = true;
        }

        return result;
    }
}

bool CombatSystem::StartCombat(const std::shared_ptr<Character>& player,
                               const std::shared_ptr<Character>& enemy)
{
    if (!player || !enemy || !player->IsAlive() || !enemy->IsAlive())
        return false;
    player->ResetTempDefense();
    player->ClearAllEffects();
    enemy->ClearAllEffects();
    return true;
}

std::string CombatSystem::ExecuteTurn(const std::shared_ptr<Character>& attacker,
                                       const std::shared_ptr<Character>& defender,
                                       CombatAction action,
                                       int skillIndex)
{
    if (!attacker || !defender)
        return "Invalid combatants.";

    std::string effectLog = attacker->ProcessEffects();
    effectLog += defender->ProcessEffects();

    int manaRegen = Passives::GetManaRegenBonus(attacker->GetEquipment());
    if (manaRegen > 0)
    {
        attacker->RestoreMana(manaRegen);
        effectLog += attacker->GetName() + " regenerates " + std::to_string(manaRegen) + " mana. ";
    }

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
            {
                int totalDodge = Passives::GetDodgePercent(defender->GetEquipment());
                int masteryDodge = static_cast<int>(defender->GetMasteryDodgeChance() * 100);
                totalDodge += masteryDodge;
                if (Passives::RollProc(totalDodge))
                {
                    attacker->GetSkills().UpdateCooldowns();
                    actionMsg = defender->GetName() + " dodges the attack! ";
                    break;
                }
            }

            auto attackSkill = attacker->GetSkills().GetSkill(skillIndex);
            if (attackSkill)
            {
                int hpBefore = defender->GetCurrentHealth();
                attackSkill->Use(*attacker, *defender);

                int baseDmg = hpBefore - defender->GetCurrentHealth();

                int physBoost = Passives::GetPhysicalDamageBoostPercent(attacker->GetEquipment());
                if (physBoost > 0 && baseDmg > 0)
                {
                    int bonusDmg = baseDmg * physBoost / 100;
                    defender->TakeDamageRaw(bonusDmg);
                }

                ElementType effective = attacker->GetEffectiveElement(attackSkill->element);
                auto bonus = ApplyEquipmentBonuses(*attacker, *defender, baseDmg, effective, actionMsg);

                float petDmgPct = attacker->GetPetBonusDamage();
                float petCritMult = PetCritMultiplier(*attacker);
                int afterPhys = defender->GetCurrentHealth();
                int dealtSoFar = hpBefore - afterPhys;
                if (dealtSoFar > 0 && (petDmgPct > 0.0f || petCritMult > 1.0f))
                {
                    int bonusDmg = static_cast<int>(dealtSoFar * petDmgPct * petCritMult);
                    if (bonusDmg > 0)
                    {
                        defender->TakeDamageRaw(bonusDmg);
                        if (petCritMult > 1.0f)
                            actionMsg += " (CRITICAL pet strike +" + std::to_string(bonusDmg) + ")";
                    }
                }

                attacker->GetSkills().UpdateCooldowns();
                int dmg = hpBefore - defender->GetCurrentHealth();

                if (dmg > 0 && attacker->GetMasterClassDamageBonus() > 0.0f)
                {
                    int masterBonus = dmg * 15 / 100;
                    if (masterBonus > 0)
                    {
                        defender->TakeDamageRaw(masterBonus);
                        actionMsg += " (MASTER +" + std::to_string(masterBonus) + ")";
                    }
                }

                dmg = hpBefore - defender->GetCurrentHealth();
                std::string elemStr = (effective != ElementType::Physical)
                    ? " [" + std::string(ElementName(effective)) + "]" : "";
                if (dmg > 0)
                    actionMsg = attacker->GetName() + " attacks " + defender->GetName()
                        + elemStr + " (" + std::to_string(dmg) + " damage)!" + actionMsg;
                else
                    actionMsg = attacker->GetName() + " attacks " + defender->GetName() + elemStr + "!";

                if (bonus.didDoubleStrike && defender->IsAlive())
                {
                    int hpBefore2 = defender->GetCurrentHealth();
                    attackSkill->Use(*attacker, *defender);
                    int dmg2 = hpBefore2 - defender->GetCurrentHealth();
                    if (dmg2 > 0)
                    {
                        int physBoost2 = Passives::GetPhysicalDamageBoostPercent(attacker->GetEquipment());
                        if (physBoost2 > 0)
                        {
                            int bonusDmg2 = dmg2 * physBoost2 / 100;
                            defender->TakeDamageRaw(bonusDmg2);
                        }
                        ApplyEquipmentBonuses(*attacker, *defender, dmg2, effective, actionMsg);
                        actionMsg += " (DOUBLE STRIKE +" + std::to_string(hpBefore2 - defender->GetCurrentHealth()) + ")";
                    }
                }

                int lifestealPct = Passives::GetLifestealPercent(attacker->GetEquipment());
                int totalDmg = hpBefore - defender->GetCurrentHealth();
                if (lifestealPct > 0 && totalDmg > 0)
                {
                    int heal = totalDmg * lifestealPct / 100;
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
            int masteryManaReduce = static_cast<int>(attacker->GetMasteryManaCostReduction() * 100);
            manaCostReduction += masteryManaReduce;
            if (manaCostReduction > 0)
                effectiveManaCost = effectiveManaCost * (100 - manaCostReduction) / 100;

            if (attacker->GetCurrentMana() < effectiveManaCost)
                return effectLog + "Not enough mana for " + skill->name + "!";

            int hpBefore = defender->GetCurrentHealth();
            skill->Use(*attacker, *defender);
            attacker->ReduceMana(effectiveManaCost);

            int cdReduction = Passives::GetCooldownReductionTurns(attacker->GetEquipment());
            for (int i = 0; i < cdReduction; ++i)
                attacker->GetSkills().GetSkill(skillIndex)->ReduceCooldown();

            attacker->GetSkills().UpdateCooldowns();

            int manaOnSkill = Passives::GetManaOnSkillUseAmount(attacker->GetEquipment());
            if (manaOnSkill > 0)
                attacker->RestoreMana(manaOnSkill);

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

            int baseSkillDmg = (diff < 0) ? -diff : 0;

            if (baseSkillDmg > 0)
            {
                if (effective == ElementType::Physical)
                {
                    int physBoost = Passives::GetPhysicalDamageBoostPercent(attacker->GetEquipment());
                    if (physBoost > 0)
                    {
                        int bonusDmg = baseSkillDmg * physBoost / 100;
                        defender->TakeDamageRaw(bonusDmg);
                        actionMsg += " (+" + std::to_string(bonusDmg) + " phys boost)";
                    }
                }
                else
                {
                    int spellBoost = Passives::GetSpellDamageBoostPercent(attacker->GetEquipment());
                    if (spellBoost > 0)
                    {
                        int bonusDmg = baseSkillDmg * spellBoost / 100;
                        defender->TakeDamageRaw(bonusDmg);
                        actionMsg += " (+" + std::to_string(bonusDmg) + " spell boost)";
                    }
                }

                ApplyEquipmentBonuses(*attacker, *defender, baseSkillDmg, effective, actionMsg);

                float petDmgPct = attacker->GetPetBonusDamage();
                float petCritMult = PetCritMultiplier(*attacker);
                if (petDmgPct > 0.0f || petCritMult > 1.0f)
                {
                    int bonusDmg = static_cast<int>(baseSkillDmg * petDmgPct * petCritMult);
                    if (bonusDmg > 0)
                    {
                        defender->TakeDamageRaw(bonusDmg);
                        if (petCritMult > 1.0f)
                            actionMsg += " (CRITICAL pet +" + std::to_string(bonusDmg) + ")";
                        else
                            actionMsg += " (pet +" + std::to_string(bonusDmg) + ")";
                    }
                }
            }

            int lifestealPct = Passives::GetLifestealPercent(attacker->GetEquipment());
            int totalDmg = hpBefore - defender->GetCurrentHealth();
            if (lifestealPct > 0 && totalDmg > 0)
            {
                int heal = totalDmg * lifestealPct / 100;
                if (heal < 1) heal = 1;
                attacker->RestoreHealth(heal);
                actionMsg += " (+" + std::to_string(heal) + " lifesteal)";
            }

            if (totalDmg > 0 && attacker->GetMasterClassDamageBonus() > 0.0f)
            {
                int masterBonus = totalDmg * static_cast<int>(attacker->GetMasterClassDamageBonus() * 100) / 100;
                if (masterBonus > 0)
                {
                    defender->TakeDamageRaw(masterBonus);
                    actionMsg += " (MASTER +" + std::to_string(masterBonus) + ")";
                }
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
