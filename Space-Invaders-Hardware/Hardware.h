#include "../Intel8080/CPU.h"
#include "SpaceInvadersMemory.h"
#include "ShiftRegister.h"
#include <string>
#include <memory>

// Class representing all the hardware for space invaders
class Hardware {
    public:
        Hardware();
        void setUpPorts();
        void frame();
        void updateFrameBuffer();
        uint8_t inputPort0();
        uint8_t inputPort1();
        uint8_t inputPort2();
        bool loadROMFile(const std::string&, size_t = 0);
        std::array<std::uint32_t, 224*256> frameBuffer {};
        bool fire {}, left {}, right {}, credit {}, twoPlayerStart {}, onePlayerStart {}, 
        playerOneShot {}, playerOneLeft {}, playerOneRight {}, tilt {}, 
        playerTwoShot {}, playerTwoLeft {}, playerTwoRight {};

        bool selfTest {false}, shipsBitL {false}, shipsBitM {false}, 
        extraShipAtHighOrLowScore {true}, disableCoinInfo {false};
    private:
        std::unique_ptr<SpaceInvadersMemory> memory {std::make_unique<SpaceInvadersMemory>(SpaceInvadersMemory())};
        CPU intel8080 {CPU(*memory)};
        ShiftRegister shiftRegister {ShiftRegister()};
        unsigned long long totalCycles {0};
        unsigned long long totalFrames {0};
        static constexpr unsigned long long cyclesPerFrame {33280};
        static constexpr unsigned long long cyclesPerHalfFrame {16640};
        // CPU clock speed is 1.9968 mhz, screen refresh rate is 60hz
        // 1996800 / 60 = 33280
        // 1996800 / 120 = 16640
};