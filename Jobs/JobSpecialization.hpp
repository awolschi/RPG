#ifndef JOB_SPECIALIZATION_HPP
#define JOB_SPECIALIZATION_HPP

#include <string>
#include <vector>

enum class JobType;

enum class SpecializationType
{
    None,
    // Mining
    Gemcutter,
    Blacksmith,
    // Lumberjacking
    Carpenter,
    Herbalist,
    // Fishing
    Angler,
    Oceanographer,
    // Smithing
    Weaponsmith,
    Armorsmith
};

struct JobSpecialization
{
    SpecializationType type;
    std::string name;
    std::string description;
    std::string bonusDescription;
    JobType parentJob;

    int rareFindBonus;
    int sellValueBonus;
    int doubleChanceBonus;
    int qualityBonus;
    int xpBonus;
    std::string resourceBonusName;

    JobSpecialization();
};

class SpecializationManager
{
public:
    SpecializationManager();

    static const JobSpecialization& GetSpecialization(SpecializationType type);
    static SpecializationType GetAlternative(SpecializationType type);
    static std::string GetSpecializationName(SpecializationType type);
    static std::string GetSpecializationDescription(SpecializationType type);

    static SpecializationType GetSpecA(JobType job);
    static SpecializationType GetSpecB(JobType job);

private:
    static void InitializeSpecs();
    static std::vector<JobSpecialization> specs;
    static bool initialized;
};

#endif
