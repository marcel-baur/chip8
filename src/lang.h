#ifndef LANG_H
#define LANG_H

#include <stdint.h>

class Lang {
    private:
        uint8_t mem[4096]; // Memory of the chip (4KB)
        uint8_t V[16]; // V-Registers
        uint8_t I; // I-Register

        uint16_t pc; // Program Counter
        uint8_t sp; // Stack pointer
        uint16_t stack[16];

        uint8_t sound;
        uint8_t delay;

    public:
        uint16_t keyboard[16]; // Input device
        uint8_t display[63 * 32]; // Display
};

#endif
