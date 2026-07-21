#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <string>
#include <vector>

enum class SoundEffect
{
    Hit,
    CriticalHit,
    Heal,
    LevelUp,
    ItemDrop,
    MenuSelect,
    MenuConfirm,
    MenuCancel,
    QuestComplete,
    AchievementUnlock,
    CoinGain,
    Defeat,
    Dodge,
    SpellCast,
    ButtonClick
};

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    void Initialize();
    void Shutdown();
    void Play(SoundEffect effect);
    void SetMasterVolume(float volume); // 0.0 - 1.0
    float GetMasterVolume() const { return masterVolume; }
    bool IsInitialized() const { return initialized; }

private:
    bool initialized = false;
    float masterVolume = 0.5f;

    struct ToneDef
    {
        float frequency;
        float duration;
        float volume;
        int waveType; // 0=sine, 1=square, 2=sawtooth, 3=noise
    };

    ToneDef GetToneDef(SoundEffect effect) const;
    void PlayTone(const ToneDef& tone);
};

#endif
