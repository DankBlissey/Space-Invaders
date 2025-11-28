#include "Memory.h"
#include <array>

class BasicMemory : public Memory {
    public:
        uint8_t read(uint16_t) const;
        void write(uint16_t, uint8_t);
        void clear();
        std::size_t size() const;
    private:
        std::array<uint8_t, 65536> mem {0};
};