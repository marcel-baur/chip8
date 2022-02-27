#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include "key.h"

class Engine {
    private:
        uint8_t mem[4096]; // Memory of the chip (4KB)
        uint8_t V[16]; // V-Registers
        uint8_t I; // I-Register

        uint16_t pc; // Program Counter
        uint8_t sp; // Stack pointer
        uint16_t stack[16];

        uint8_t sound;
        uint8_t delay;

        uint8_t currcode;

        void launch();
        void run();
        void execute();
        void clear();

    public:
        uint16_t keyboard[16]; // Input device
        Key kb;
        uint8_t display[64 * 32]; // Display
};

#endif
