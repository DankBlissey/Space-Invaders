#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Hardware.h"
#include "LTimer.h"
#include <sstream>
#include <memory>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"

constexpr int screenWidth {256};
constexpr int screenHeight {224};

constexpr int screenFPS {60};

constexpr std::uint64_t nsPerFrame = 1000000000 / screenFPS;

constexpr int adjustedHeight {screenWidth * 3};
constexpr int adjustedWidth {screenHeight * 3};

bool quit{false};

SDL_Window* window {nullptr};

SDL_Renderer* renderer {nullptr};

SDL_Texture* videoTexture {nullptr};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (window = SDL_CreateWindow("Space Invaders", 480, 640, SDL_WINDOW_RESIZABLE); window == nullptr) {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (renderer = SDL_CreateRenderer(window, nullptr); renderer == nullptr) {
        SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_SetRenderVSync(renderer, 1) == false) {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (videoTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight); videoTexture == nullptr) {
        SDL_Log("Texture could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_SetTextureScaleMode(videoTexture, SDL_SCALEMODE_NEAREST) == false) {
        SDL_Log("Could not enable nearest neighbour scaling SDL error: %s\n", SDL_GetError());
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

void handleInput(Hardware& hardware, SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_A:
                hardware.left = true;
                hardware.playerOneLeft = true;
                hardware.playerTwoLeft = true;
                break;
            case SDLK_D:
                hardware.right = true;
                hardware.playerOneRight = true;
                hardware.playerTwoRight = true;
                break;
            case SDLK_SPACE:
                hardware.fire = true;
                hardware.playerOneShot = true;
                hardware.playerTwoShot = true;
                break;
            case SDLK_C:
                hardware.credit = true;
                break;
            case SDLK_1:
                hardware.onePlayerStart = true;
                break;
            case SDLK_2:
                hardware.twoPlayerStart = true;
                break;
            case SDLK_T:
                hardware.tilt = true;
                break;
        }
    } else if (e.type == SDL_EVENT_KEY_UP) {
        switch (e.key.key) {
            case SDLK_A:
                hardware.left = false;
                hardware.playerOneLeft = false;
                hardware.playerTwoLeft = false;
                break;
            case SDLK_D:
                hardware.right = false;
                hardware.playerOneRight = false;
                hardware.playerTwoRight = false;
                break;
            case SDLK_SPACE:
                hardware.fire = false;
                hardware.playerOneShot = false;
                hardware.playerTwoShot = false;
                break;
            case SDLK_1:
                hardware.onePlayerStart = false;
                break;
            case SDLK_2:
                hardware.twoPlayerStart = false;
                break;
            case SDLK_T:
                hardware.tilt = false;
                break;
        }
    }
}

int main( int argc, char* args[]) {
    std::unique_ptr<Hardware> spaceInvadersHardware = std::make_unique<Hardware>();
    spaceInvadersHardware->setUpPorts();
    if (init() == false) {
        SDL_Log("Unable to initialize program\n");
        return 1;
    }
    if (!spaceInvadersHardware->loadROMFile("SpaceInvadersROM/9316b-0869_m739h.h1")) {
        SDL_Log("Unable to load invaders.h1");
        return 1;
    }
    if (!spaceInvadersHardware->loadROMFile("SpaceInvadersROM/9316b-0856_m739g.g1", 0x0800)) {
        SDL_Log("Unable to load invaders.g1");
        return 1;
    }
    if (!spaceInvadersHardware->loadROMFile("SpaceInvadersROM/9316b-0855_m739f.f1", 0x1000)) {
        SDL_Log("Unable to load invaders.f1");
        return 1;
    }
    if (!spaceInvadersHardware->loadROMFile("SpaceInvadersROM/9316b-0854_m739e.e1", 0x1800)) {
        SDL_Log("Unable to load invaders.e1");
        return 1;
    }
    SDL_Event e;
    SDL_zero(e);
    bool vsyncEnabled {true};
    bool fpsCapEnabled {true};
    LTimer capTimer;
    std::uint64_t renderingNS {0};
    std::stringstream timeText;

    //Main rendering loop
    while(quit == false) {
        capTimer.start();
        while(SDL_PollEvent(&e) == true) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else {
                handleInput(*spaceInvadersHardware, e);
            }
        }
        // Run one frame's worth of emulation and interrupts
        spaceInvadersHardware->frame();
        // calculate render target size based on window size
        int renderW, renderH;
        SDL_GetRenderOutputSize(renderer, &renderW, &renderH);

        float maxW = renderH;
        float maxH = renderW;

        float targetH = maxH;
        float targetW = targetH * (4.0f/3.0f);

        if (targetW > maxW) {
            targetW = maxW;
            targetH = targetW * (3.0f/4.0f);
        }

        SDL_FRect dstRect;
        dstRect.w = targetW;
        dstRect.h = targetH;
        dstRect.x = (renderW - targetW) * 0.5f;
        dstRect.y = (renderH - targetH) * 0.5f;
        SDL_UpdateTexture(videoTexture, nullptr, spaceInvadersHardware->frameBuffer.data(), 256 * sizeof(std::uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTextureRotated(renderer, videoTexture, nullptr, &dstRect, -90.0, nullptr, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
        renderingNS = capTimer.getTicksNS();
        if (renderingNS < nsPerFrame) {
            SDL_DelayNS(nsPerFrame - renderingNS);

            renderingNS = capTimer.getTicksNS();
        }
    }
    close();
    return 0;
}