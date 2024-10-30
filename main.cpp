#include <cstdint>
#include <iostream>

constexpr unsigned int START_ADDRESS = 0x200;

struct Chip8 {

private:
    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t program_counter{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint8_t keypad[16]{};
    uint32_t video[64 * 32]{};
    uint16_t opcode{};

public:
    Chip8() = default;

    // call
    void OP_0nnn();

    // display
    void OP_00e0();
    void OP_dxyn();

    // flow
    void OP_00ee();
    void OP_1nnn();
    void OP_2nnn();
    void OP_bnnn();

    // cond
    void OP_3xnn();
    void OP_4xnn();
    void OP_5xy0();
    void OP_9xy0();

    //const
    void OP_6xnn();
    void OP_7xnn();

    // assign
    void OP_8xy0();

    // bit op
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy6();
    void OP_8xye();

    // math
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy7();

    // memory
    void OP_annn();
    void OP_fx1e();
    void OP_fx29();
    void OP_fx55();
    void OP_fx65();

    // rand
    void OP_cxnn();

    // key op
    void OP_ex9e();
    void OP_exa1();
    void OP_fx0a();

    // timer
    void OP_fx07();
    void OP_fx15();

    // bcd
    void OP_fx33();

};

int main() {

}
