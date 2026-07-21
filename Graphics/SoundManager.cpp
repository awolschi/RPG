#include "SoundManager.hpp"
#include "raylib.h"
#include <cmath>
#include <cstring>

SoundManager::SoundManager() {}

SoundManager::~SoundManager()
{
    Shutdown();
}

void SoundManager::Initialize()
{
    if (initialized) return;
    initialized = true;
}

void SoundManager::Shutdown()
{
    initialized = false;
}

SoundManager::ToneDef SoundManager::GetToneDef(SoundEffect effect) const
{
    switch (effect)
    {
        case SoundEffect::Hit:             return {220.0f, 0.08f, 0.6f, 1};
        case SoundEffect::CriticalHit:     return {440.0f, 0.12f, 0.7f, 1};
        case SoundEffect::Heal:            return {523.0f, 0.15f, 0.4f, 0};
        case SoundEffect::LevelUp:         return {659.0f, 0.30f, 0.5f, 0};
        case SoundEffect::ItemDrop:        return {330.0f, 0.10f, 0.3f, 0};
        case SoundEffect::MenuSelect:      return {800.0f, 0.04f, 0.2f, 0};
        case SoundEffect::MenuConfirm:     return {1000.0f, 0.06f, 0.3f, 0};
        case SoundEffect::MenuCancel:      return {200.0f, 0.08f, 0.2f, 1};
        case SoundEffect::QuestComplete:   return {784.0f, 0.25f, 0.5f, 0};
        case SoundEffect::AchievementUnlock: return {880.0f, 0.35f, 0.5f, 0};
        case SoundEffect::CoinGain:        return {1200.0f, 0.06f, 0.3f, 0};
        case SoundEffect::Defeat:          return {150.0f, 0.40f, 0.4f, 1};
        case SoundEffect::Dodge:           return {600.0f, 0.05f, 0.3f, 0};
        case SoundEffect::SpellCast:       return {440.0f, 0.10f, 0.35f, 0};
        case SoundEffect::ButtonClick:     return {900.0f, 0.03f, 0.15f, 0};
        default:                           return {440.0f, 0.05f, 0.3f, 0};
    }
}

void SoundManager::PlayTone(const ToneDef& tone)
{
    if (!initialized) return;

    const int sampleRate = 44100;
    int sampleCount = static_cast<int>(tone.duration * sampleRate);
    if (sampleCount <= 0) return;

    float vol = tone.volume * masterVolume;
    if (vol <= 0.0f) return;

    Wave wave = {0};
    wave.frameCount = static_cast<unsigned int>(sampleCount);
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;
    wave.data = malloc(sampleCount * sizeof(short));

    if (!wave.data) return;

    short* samples = static_cast<short*>(wave.data);
    for (int i = 0; i < sampleCount; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float sample = 0.0f;

        switch (tone.waveType)
        {
            case 0: // sine
                sample = sinf(2.0f * 3.14159265f * tone.frequency * t);
                break;
            case 1: // square
                sample = (sinf(2.0f * 3.14159265f * tone.frequency * t) >= 0.0f) ? 1.0f : -1.0f;
                break;
            case 2: // sawtooth
            {
                float period = 1.0f / tone.frequency;
                float phase = fmodf(t, period) / period;
                sample = 2.0f * phase - 1.0f;
                break;
            }
            case 3: // noise
                sample = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                break;
            default:
                sample = sinf(2.0f * 3.14159265f * tone.frequency * t);
                break;
        }

        // Apply envelope (quick fade in/out)
        float env = 1.0f;
        float fadeLen = 0.01f;
        float fadeSamples = fadeLen * sampleRate;
        if (i < fadeSamples)
            env = static_cast<float>(i) / fadeSamples;
        else if (i > sampleCount - fadeSamples)
            env = static_cast<float>(sampleCount - i) / fadeSamples;

        samples[i] = static_cast<short>(sample * vol * env * 32767.0f);
    }

    Sound sound = LoadSoundFromWave(wave);
    PlaySound(sound);
    free(wave.data);
}

void SoundManager::Play(SoundEffect effect)
{
    if (!initialized) return;
    PlayTone(GetToneDef(effect));
}

void SoundManager::SetMasterVolume(float volume)
{
    masterVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f ? 1.0f : volume);
}
