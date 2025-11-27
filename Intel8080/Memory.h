#include <cstdint>
#include <array>
using std::uint8_t;
using std::uint16_t;

class Memory {
    public:
        uint8_t read(uint16_t);
        void write(uint16_t, uint8_t);
        void clear();
        std::size_t size();
    protected:
        std::array<uint8_t, 65536> memory = std::array<uint8_t, 65536>();
};