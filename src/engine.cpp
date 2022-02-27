#include "engine.h"
#include <map>
#include <functional>
#include <thread>

unsigned char display_sprites[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};


void Engine::launch() {
    pc = 0x200;
    I = 0;

    sp = 0;
    pc = 0;

    sound = 0;
    delay = 0;

    kb = Key();
    for (int i = 0; i < 4096; i++) {
        mem[i] = 0;
    }

    for (int i = 0; i < 16; i++) {
        V[i] = 0;
        stack[i] = 0;
        keyboard[i] = 0;
    }

    for (int i = 0; i < 80; i++) {
        mem[i] = display_sprites[i];
    }

}

void Engine::execute() {
    // any uint16_t & 0x0F00 will set all digits except for the one at the F position to 0
    // This allows us to extract the necessary digits according to the specification.
    std::map<uint16_t, std::function<void(uint16_t&)>> full_match {
        {0x00e0, [this](uint16_t& op) { clear(); }},
        {0x00ee, [this](uint16_t& op) { pc = stack[--sp]; }}
    };

    std::map<uint16_t, std::function<void(uint16_t&)>> three_quart_match {
        {0xe09e, [this](uint16_t& op) { if (keyboard[V[(currcode & 0x0F00) >> 8]] != 0) pc += 2; }}, // SKP Vx: Skip next instruciton if key Vx is pressed
        {0xe0a1, [this](uint16_t& op) { if (keyboard[V[(currcode & 0x0F00) >> 8]] == 0) pc += 2; }}, // SKNP Vx: Skip next instruciton if key Vx is not pressed
        {0xf007, [this](uint16_t& op) { V[(currcode & 0x0F00) >> 8] = delay; }}, // Set value of reg to delay
        {0xf00a, [this](uint16_t& op) { V[(currcode & 0x0F00) >> 8] = kb.waitForPress(); }}, // Store the value of the key in Vx
        {0xf015, [this](uint16_t& op) { delay = V[(currcode & 0x0F00) >> 8]; }}, // Set delay
        {0xf018, [this](uint16_t& op) { sound = V[(currcode & 0x0F00) >> 8]; }},
        {0xf01e, [this](uint16_t& op) { I = I + V[(currcode & 0x0F00) >> 8]; }},
        {0xf029, [this](uint16_t& op) { I = display_sprites[(currcode & 0x0F00) >> 8]; }},
        {0xf033, [this](uint16_t& op) {
                                          mem[I] = (V[(currcode & 0x0F00) >> 8] / 100);
                                          mem[I+1] = ((V[(op & 0x0F00) >> 8] % 100) / 10);
                                          mem[I+2] = ((V[(op & 0x0F00) >> 8] % 10) / 1);
                                      }},
        {0xf055, [this](uint16_t& op) {
                                          for (int i = 0; i <= ((currcode & 0x0F00) >> 8); ++i) {
                                              mem[I + 1] = V[i];
                                          }
                                          I += ((currcode & 0x0F00) >> 8) + 1;
                                      }},
        {0xf065, [this](uint16_t& op) {
                                          for (int i = 0; i <= ((currcode & 0x0F00) >> 8); ++i) {
                                              V[i] = mem[I + 1];
                                          }
                                          I += ((currcode & 0x0F00) >> 8) + 1;
                                      }},
    };

    std::map<uint16_t, std::function<void(uint16_t&)>> fourth_match {
    };

    std::map<uint16_t, std::function<void(uint16_t&)>> fist_match {
    };
}

void Engine::clear() {
    for ( int i = 0; i < 2048; i++ ) {
        display[i] = 0;
    }
}

void Engine::run() {
    // Load operation code, run assigned command after.
    // The operation code is stored in two bytes.
    currcode = mem[pc] << 8 | mem[pc + 1];
    execute();
    pc += 2; // Increment program counter by two.
}
