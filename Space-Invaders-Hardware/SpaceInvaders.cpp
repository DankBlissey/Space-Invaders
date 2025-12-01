#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Hardware.h"

constexpr int screenWidth {256};
constexpr int screenHeight {224};

constexpr int adjustedHeight {screenWidth * 4};
constexpr int adjustedWidth {screenHeight * 3};

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow {nullptr};

SDL_Surface* gScreenSurface {nullptr};

SDL_Surface* gHellowWorld {nullptr};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (gWindow = SDL_CreateWindow("SDL3 Tutorial: Hellow SDL3", screenWidth, screenHeight, 0); gWindow == nullptr) {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}


void close() {
    //SDL_DestroySurface(gHellowWorld);
    //gHellowWorld = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gScreenSurface = nullptr;
    SDL_Quit();
}

int main( int argc, char* args[]) {
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
        SDL_FillSurfaceRect(gScreenSurface, nullptr, SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(gWindow);
    }
    close();
    return 0;
}