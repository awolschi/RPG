#include "JobEnvironment.hpp"
#include "../Engine/RNG.hpp"
#include <algorithm>

JobEnvironment::JobEnvironment()
    : currentWeather(WeatherType::Clear), daysUntilChange(1)
{
    RollWeather();
}

WeatherEffect JobEnvironment::GetWeatherEffect(WeatherType weather) const
{
    WeatherEffect e;
    switch (weather)
    {
        case WeatherType::Clear:
            e.speedMod = 1.0f;
            e.qualityMod = 1.0f;
            e.fatigueMod = 1.0f;
            e.rareFindMod = 1.0f;
            e.xpMod = 1.0f;
            e.description = "No weather effects";
            break;
        case WeatherType::Rainy:
            e.speedMod = 0.85f;
            e.qualityMod = 0.95f;
            e.fatigueMod = 1.15f;
            e.rareFindMod = 1.0f;
            e.xpMod = 1.0f;
            e.description = "Rain slows work and increases fatigue";
            break;
        case WeatherType::Stormy:
            e.speedMod = 0.70f;
            e.qualityMod = 0.90f;
            e.fatigueMod = 1.30f;
            e.rareFindMod = 0.80f;
            e.xpMod = 1.0f;
            e.description = "Storms make work difficult and dangerous";
            break;
        case WeatherType::Windy:
            e.speedMod = 0.90f;
            e.qualityMod = 0.90f;
            e.fatigueMod = 1.10f;
            e.rareFindMod = 1.20f;
            e.xpMod = 1.05f;
            e.description = "Wind scatters resources but reveals hidden finds";
            break;
        case WeatherType::Foggy:
            e.speedMod = 0.80f;
            e.qualityMod = 0.85f;
            e.fatigueMod = 1.20f;
            e.rareFindMod = 1.30f;
            e.xpMod = 1.0f;
            e.description = "Fog makes it hard to see but rare finds are more common";
            break;
        case WeatherType::Snowy:
            e.speedMod = 0.75f;
            e.qualityMod = 1.10f;
            e.fatigueMod = 1.25f;
            e.rareFindMod = 0.90f;
            e.xpMod = 1.15f;
            e.description = "Snow slows work but improves quality and yields more XP";
            break;
        case WeatherType::Scorching:
            e.speedMod = 0.80f;
            e.qualityMod = 0.80f;
            e.fatigueMod = 1.35f;
            e.rareFindMod = 1.10f;
            e.xpMod = 1.0f;
            e.description = "Extreme heat drains energy quickly";
            break;
        default:
            break;
    }
    return e;
}

void JobEnvironment::RollWeather()
{
    int roll = RNG::Next(100);
    if (roll < 30)      currentWeather = WeatherType::Clear;
    else if (roll < 50) currentWeather = WeatherType::Rainy;
    else if (roll < 62) currentWeather = WeatherType::Windy;
    else if (roll < 72) currentWeather = WeatherType::Foggy;
    else if (roll < 82) currentWeather = WeatherType::Stormy;
    else if (roll < 92) currentWeather = WeatherType::Snowy;
    else                currentWeather = WeatherType::Scorching;

    effect = GetWeatherEffect(currentWeather);
    daysUntilChange = 1 + RNG::Next(3);
}

std::string JobEnvironment::GetWeatherDescription() const
{
    std::string desc = "Weather: ";
    desc += WeatherName(currentWeather);
    desc += " — ";
    desc += effect.description;
    return desc;
}

void JobEnvironment::UpdateDaily()
{
    daysUntilChange--;
    if (daysUntilChange <= 0)
        RollWeather();
}
