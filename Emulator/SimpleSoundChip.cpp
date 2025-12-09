#include "SimpleSoundChip.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

static bool initializeSound(const char* fname, Sound* sound, SDL_AudioDeviceID audio_device) {
    SDL_AudioSpec spec;
    if (!SDL_LoadWAV(fname, &spec, &sound->wav_data, &sound->wav_data_len)) {
        SDL_Log("Couldn't load .wav file! %s\n", SDL_GetError());
        return false;
    }

    sound->stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!sound->stream) {
        SDL_Log("Couldn't open audio stream for '%s': %s\n", fname, SDL_GetError());
        return false;
    }
    SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(sound->stream));
    return true;
}

bool SimpleSoundChip::init() {
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
    if (!sound.played) {
        SDL_PutAudioStreamData(sound.stream, sound.wav_data, sound.wav_data_len);
        sound.played = true;
    }
}

void SimpleSoundChip::playRepeatingSound(Sound& sound) {
    if (SDL_GetAudioStreamQueued(sound.stream) < ((int) sound.wav_data_len)) {
        SDL_PutAudioStreamData(sound.stream, sound.wav_data, sound.wav_data_len);
    }
}

void SimpleSoundChip::port3(uint8_t soundData) {
    if (!soundEnabled) {
        return;
    }
    if ((soundData & 0b00000001) != 0) {
        playRepeatingSound(ufoSound);
    }
    if ((soundData & 0b00000010) != 0) {
        playSound(shotSound);
    }
    else {
        shotSound.played = false;
    }
    if ((soundData & 0b00000100) != 0) {
        playSound(playerDieSound);
    }
    else {
        playerDieSound.played = false;
    }
    if ((soundData & 0b00001000) != 0) {
        playSound(invaderDieSound);
    }
    else {
        invaderDieSound.played = false;
    }
    if ((soundData & 0b00010000) != 0) {
        playSound(extraLifeSound);
    }
    else {
        extraLifeSound.played = false;
    }
}

void SimpleSoundChip::port5(uint8_t soundData) {
    if (!soundEnabled) {
        return;
    }

    if ((soundData & 0b00000001) != 0) {
        playSound(fleetMovement1Sound);
    }
    else {
        fleetMovement1Sound.played = false;
    }
    if ((soundData & 0b00000010) != 0) {
        playSound(fleetMovement2Sound);
    }
    else {
        fleetMovement2Sound.played = false;
    }
    if ((soundData & 0b00000100) != 0) {
        playSound(fleetMovement3Sound);
    }
    else {
        fleetMovement3Sound.played = false;
    }
    if ((soundData & 0b00001000) != 0) {
        playSound(fleetMovement4Sound);
    }
    else {
        fleetMovement4Sound.played = false;
    }
    if ((soundData & 0b00010000) != 0) {
        playSound(ufoHitSound);
    }
    else {
        ufoHitSound.played = false;
    }
}

