#include "../Intel8080/CPU.h"
#include "SpaceInvadersMemory.h"
#include "ShiftRegister.h"

// Class representing all the hardware for space invaders
class Hardware {
    public:
        Hardware();
        void frame();
        void updateFrameBuffer();
        std::array<std::uint32_t, 224*256> frameBuffer {};
    private:
        SpaceInvadersMemory memory {SpaceInvadersMemory()};
        CPU intel8080 {CPU(memory)};
        ShiftRegister shiftRegister {ShiftRegister()};
        unsigned long long totalCycles {0};
        unsigned long long totalFrames {1};
        static constexpr unsigned long long cyclesPerFrame {33280};
        static constexpr unsigned long long cyclesPerHalfFrame {16640};
        // CPU clock speed is 1.9968 mhz, screen refresh rate is 60hz
        // 1996800 / 60 = 33280
        // 1996800 / 120 = 16640
};