#ifndef JOB_ENVIRONMENT_HPP
#define JOB_ENVIRONMENT_HPP

#include <string>
#include <vector>
#include "Job.hpp"

enum class WeatherType
{
    Clear,
    Rainy,
    Stormy,
    Windy,
    Foggy,
    Snowy,
    Scorching,
    count
};

inline const char* WeatherName(WeatherType w)
{
    switch (w)
    {
        case WeatherType::Clear:      return "Clear";
        case WeatherType::Rainy:      return "Rainy";
        case WeatherType::Stormy:     return "Stormy";
        case WeatherType::Windy:      return "Windy";
        case WeatherType::Foggy:      return "Foggy";
        case WeatherType::Snowy:      return "Snowy";
        case WeatherType::Scorching:  return "Scorching";
        default: return "Unknown";
    }
}

struct WeatherEffect
{
    float speedMod = 1.0f;
    float qualityMod = 1.0f;
    float fatigueMod = 1.0f;
    float rareFindMod = 1.0f;
    float xpMod = 1.0f;
    std::string description;
};

class JobEnvironment
{
public:
    JobEnvironment();

    void RollWeather();
    WeatherType GetCurrentWeather() const { return currentWeather; }
    const WeatherEffect& GetEffect() const { return effect; }

    WeatherEffect GetWeatherEffect(WeatherType weather) const;
    std::string GetWeatherDescription() const;

    void UpdateDaily();

private:
    WeatherType currentWeather;
    WeatherEffect effect;
    int daysUntilChange;
};

#endif
