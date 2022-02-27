#include <map>
#include <vector>

class Key {
    private:
        std::vector<int> pressed;
        std::map<int, uint8_t> keymap;
        int updatePressed();
    public:
        Key();
        bool isPressed(uint8_t key);
        uint8_t waitForPress();
        void press(int key);
};
