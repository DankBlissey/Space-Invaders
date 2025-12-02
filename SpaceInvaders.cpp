#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Hardware.h"
#include <memory>
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"

constexpr int screenWidth {256};
constexpr int screenHeight {224};

constexpr int adjustedHeight {screenWidth * 4};
constexpr int adjustedWidth {screenHeight * 3};

SDL_Window* window {nullptr};

SDL_Renderer* renderer {nullptr};

SDL_Texture* videoTexture {nullptr};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (window = SDL_CreateWindow("Space Invaders", screenWidth, screenHeight, 0); window == nullptr) {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (renderer = SDL_CreateRenderer(window, nullptr); renderer == nullptr) {
        SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (videoTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight); videoTexture == nullptr) {
        SDL_Log("Texture could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}


void close() {
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_Quit();
}

int main( int argc, char* args[]) {
    std::unique_ptr<Hardware> spaceInvadersHardware = std::make_unique<Hardware>(Hardware());
    if (init() == false) {
        SDL_Log("Unable to initialize program\n");
        return 1;
    }
    bool quit(false);
    SDL_Event e;
    SDL_zero(e);
    while(quit == false) {
        while(SDL_PollEvent(&e) == true) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        // Do emulation
        
    }
    close();
    return 0;
}