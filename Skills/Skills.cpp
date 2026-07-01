#include "Skills.hpp"

void SkillSet::AddSkill(std::shared_ptr<Skill> skill)
{
    if (skill)
    {
        skills.push_back(skill);
    }
}

void SkillSet::RemoveSkill(size_t index)
{
    if (index < skills.size())
    {
        skills.erase(skills.begin() + index);
    }
}

std::shared_ptr<Skill> SkillSet::GetSkill(size_t index)
{
    if (index < skills.size())
    {
        return skills[index];
    }
    return nullptr;
}

std::shared_ptr<Skill> SkillSet::GetSkillByName(const std::string& name)
{
    for (auto& skill : skills)
    {
        if (skill->name == name)
        {
            return skill;
        }
    }
    return nullptr;
}

void SkillSet::UpdateCooldowns()
{
    for (auto& skill : skills)
    {
        skill->ReduceCooldown();
    }
}
