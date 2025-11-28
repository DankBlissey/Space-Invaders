#pragma once
#include <cstdint>
using std::uint8_t;
using std::uint16_t;

// Interface for implementing Intel 8080 CPU memory
class Memory {
    public:
        virtual ~Memory() = default;
        virtual uint8_t read(uint16_t) const = 0;
        virtual void write(uint16_t, uint8_t) = 0;
        virtual void clear() = 0;
        virtual std::size_t size() const = 0;
};