#ifndef SKILLS_HPP
#define SKILLS_HPP

#include <vector>
#include <memory>
#include "Skill.hpp"

class SkillSet
{
public:
    SkillSet() = default;

    void AddSkill(std::shared_ptr<Skill> skill);
    bool AddSkillIfLevelMet(std::shared_ptr<Skill> skill, int characterLevel);
    void RemoveSkill(size_t index);
    void ClearAll();
    std::shared_ptr<Skill> GetSkill(size_t index);
    std::shared_ptr<Skill> GetSkillByName(const std::string& name);

    void UpdateCooldowns();
    size_t GetSkillCount() const { return skills.size(); }

private:
    std::vector<std::shared_ptr<Skill>> skills;
};

#endif
