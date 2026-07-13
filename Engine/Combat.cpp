#include "Combat.hpp"
#include <sstream>
#include <cstdlib>

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

    // Check if attacker is stunned
    if (attacker->IsStunned())
    {
        attacker->GetSkills().UpdateCooldowns();
        return attacker->GetName() + " is stunned and cannot act!";
    }

    std::string actionMsg;
    switch (action)
    {
        case CombatAction::Attack:
        {
            auto attackSkill = attacker->GetSkills().GetSkill(0);
            if (attackSkill)
            {
                int hpBefore = defender->GetCurrentHealth();
                attackSkill->Use(*attacker, *defender);
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

            if (attacker->GetCurrentMana() < skill->manaCost)
                return effectLog + "Not enough mana for " + skill->name + "!";

            int hpBefore = defender->GetCurrentHealth();
            skill->Use(*attacker, *defender);
            attacker->ReduceMana(skill->manaCost);
            attacker->GetSkills().UpdateCooldowns();
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
            break;
        }

        case CombatAction::Defend:
            attacker->IncreaseTempDefense(5);
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
                + " (Mana:" + std::to_string(s->manaCost)
                + " CD:" + std::to_string(s->currentCooldown) + "/" + std::to_string(s->cooldown)
                + " Req:" + std::to_string(s->requiredLevel) + ")";
            opts.push_back(entry);
        }
    }
    return opts;
}
