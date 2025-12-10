#pragma once
#include "SoundChip.h"
#include <SDL3/SDL_audio.h>

typedef struct Sound {
    Uint8 *wav_data;
    Uint32 wav_data_len;
    SDL_AudioStream *stream;
    bool played{ false };
} Sound;

class SimpleSoundChip : public SoundChip {
    public:
        void destroyAudioStreams();
        bool init();

        void port3(uint8_t) override;
        void port5(uint8_t) override;

        void playSound(Sound&);
        void playRepeatingSound(Sound&);
        bool soundEnabled {false};
    private:
        SDL_AudioDeviceID audioDevice {0};
        bool ufoOnScreen {false};
        Sound ufoSound;
        Sound shotSound;
        Sound playerDieSound;
        Sound invaderDieSound;
        Sound extraLifeSound;
        Sound fleetMovement1Sound;
        Sound fleetMovement2Sound;
        Sound fleetMovement3Sound;
        Sound fleetMovement4Sound;
        Sound ufoHitSound;
};