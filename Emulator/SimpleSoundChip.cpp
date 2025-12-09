#include "SimpleSoundChip.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

static bool initializeSound(const char* fname, Sound* sound, SDL_AudioDeviceID audio_device) {
    SDL_AudioSpec spec;
    if (!SDL_LoadWAV(fname, &spec, &sound->wav_data, &sound->wav_data_len)) {
        SDL_Log("Couldn't load .wav file! %s\n", SDL_GetError());
        return false;
    }

    sound->stream = SDL_CreateAudioStream(&spec, NULL);
    if (!sound->stream) {
        SDL_Log("Couldn't create audio stream! %s\n", SDL_GetError());
        return false;
    }
    if (SDL_BindAudioStream(audio_device, sound->stream)) {
        SDL_Log("Failed to bind '%s' stream to device! %s\n", fname, SDL_GetError());
        return false;
    }
    return true;
}

bool SimpleSoundChip::init() {
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audioDevice == 0) {
        SDL_Log("Could not open audio device! %s\n", SDL_GetError());
        return false;
    }
    return initializeSound("Audio/0.wav", &ufoSound, audioDevice) && 
        initializeSound("Audio/1.wav", &shotSound, audioDevice) &&
        initializeSound("Audio/2.wav", &playerDieSound, audioDevice) &&
        initializeSound("Audio/3.wav", &invaderDieSound, audioDevice) &&
        initializeSound("Audio/4.wav", &fleetMovement1Sound, audioDevice) &&
        initializeSound("Audio/5.wav", &fleetMovement2Sound, audioDevice) &&
        initializeSound("Audio/6.wav", &fleetMovement3Sound, audioDevice) &&
        initializeSound("Audio/7.wav", &fleetMovement4Sound, audioDevice) &&
        initializeSound("Audio/8.wav", &ufoHitSound, audioDevice) &&
        initializeSound("Audio/9.wav", &extraLifeSound, audioDevice);
}

void SimpleSoundChip::playSound(Sound& sound) {
    SDL_PutAudioStreamData(sound.stream, sound.wav_data, sound.wav_data_len);
}

void SimpleSoundChip::port3(uint8_t soundData) {
    if (!soundEnabled) {
        return;
    }
    if ((soundData & 0b00000001) != 0) {
        playSound(ufoSound);
    }
    if ((soundData & 0b00000010) != 0) {
        playSound(shotSound);
    }
    if ((soundData & 0b00000100) != 0) {
        playSound(playerDieSound);
    }
    if ((soundData & 0b00001000) != 0) {
        playSound(invaderDieSound);
    }
    if ((soundData & 0b00010000) != 0) {
        playSound(extraLifeSound);
    }
}

void SimpleSoundChip::port5(uint8_t soundData) {
    if (!soundEnabled) {
        return;
    }
    if ((soundData & 0b00000001) != 0) {
        playSound(fleetMovement1Sound);
    }
    if ((soundData & 0b00000010) != 0) {
        playSound(fleetMovement2Sound);
    }
    if ((soundData & 0b00000100) != 0) {
        playSound(fleetMovement3Sound);
    }
    if ((soundData & 0b00001000) != 0) {
        playSound(fleetMovement4Sound);
    }
    if ((soundData & 0b00010000) != 0) {
        playSound(ufoHitSound);
    }
}

