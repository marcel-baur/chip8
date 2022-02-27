#include "key.h"
#include <map>
#include <vector>

Key::Key() {
    keymap = std::map<int, uint8_t> {
        {'1', 0x1},
        {'2', 0x2},
        {'3', 0x3},
        {'4', 0xc},
        {'q', 0x4},
        {'w', 0x5},
        {'e', 0x6},
        {'r', 0xd},
        {'a', 0x7},
        {'s', 0x8},
        {'d', 0x9},
        {'f', 0xe},
        {'z', 0xa},
        {'x', 0x0},
        {'c', 0xb},
        {'v', 0xf},
    };
    pressed = std::vector<int>(16, 0);
}

void Key::press(int key) {
    auto it = keymap.find(key);
    if (it != keymap.end()) {
        pressed[it->second] += 1;
    }
}

bool Key::isPressed(uint8_t key) {
    if (pressed[key] != 0) {
        pressed[key] = 0;
        return true;
    }
    return false;
}

uint8_t Key::waitForPress() {
    do {
        for (int i = 0; i < 16; i++) {
            if (pressed[i] > 0) {
                pressed[i] = 0;
                return i;
            }
        }
        updatePressed();
    } while (true);

    return 0;
}
