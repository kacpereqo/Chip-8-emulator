#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

constexpr uint16_t START_ADDRESS = 0x200;
constexpr uint8_t STACK_SIZE = 16;
constexpr uint16_t MEMORY_SIZE = 4096;
constexpr uint8_t REGISTER_COUNT = 16;

constexpr uint8_t VIDEO_WIDTH = 64;
constexpr uint8_t VIDEO_HEIGHT = 32;

constexpr bool NEW_SHIFT = false;
constexpr bool NEW_MEMORY_LOADING = false;

enum Keys {
    PRESSED,
    NOT_PRESSED,
};

struct Chip8 {

public:
    uint8_t registers[REGISTER_COUNT]{};
    uint8_t memory[MEMORY_SIZE ]{};
    uint16_t program_counter{};
    uint16_t stack[STACK_SIZE]{};
    uint8_t stack_pointer{};
    uint32_t video[VIDEO_HEIGHT][VIDEO_WIDTH]{};
    uint16_t opcode{};
    uint16_t address_reg{};
    uint8_t delay_timer{};
    uint8_t sound_timer{};

    Chip8() {
        this->opcode = START_ADDRESS;
        this->load_font();
    };

    void loadRom(std::vector<uint8_t> rom) {
        if (rom.size() < MEMORY_SIZE) {
            throw std::invalid_argument("loaded rom is bigger than" + std::to_string(MEMORY_SIZE) + "B");
        }
        std::copy(rom.begin(), rom.end(), this->memory);
    }

    void loadOpcode() {
        this->opcode = this->memory[this->program_counter];
        this->opcode <<= 8;

        this->opcode |= this->memory[this->program_counter + 1];

        this->program_counter += 2;
    }

    void executeOpcode() const {
        uint8_t args[4];
        args[0] = this->opcode & 0x000F;
        args[1] = this->opcode & 0x00F0;
        args[2] = this->opcode & 0x0F00;
        args[3] = this->opcode & 0xF000;

        switch (args[0]) {

        }
    }

    void load_font() {
        const uint8_t font[16][5] =
        {
            {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
            {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
            {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
            {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
            {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
            {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
            {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
            {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
            {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
            {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
            {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
            {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
            {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
            {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
            {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
            {0xF0, 0x80, 0xF0, 0x80, 0x80} // F
        };

    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 5; j++) {
            const uint16_t address = (i * 5) + j;
            this->memory[address] = font[i][j];
            // std::cout << std::hex;
            // std::cout << static_cast<int>( memory[address]) << std::endl;

        }
        // std::cout << std::endl;
    }


    }
    void update_timers() {
        this->delay_timer--;
        this->sound_timer--;
    }

    void emualate() {
        for (size_t i = 0; i < 10; i++) {
            loadOpcode();
            executeOpcode();
            update_timers();
        }
    }


    void display_video() {
        std::cout<<"DISPLAY VIDEO" << std::endl;
        for (auto & row : this->video) {
            for (const uint16_t pixel : row) {
                if (pixel == 1)
                    std::cout << "#";
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void display_registers() {
        std::cout << std::setfill('0');
        std::cout << std::hex;

        std::cout<<"|";
        for (const auto reg : registers) {
            std::cout << std::setw(2);
            std::cout << static_cast<int16_t>(reg) << "|";
        }

        std::cout << std::resetiosflags(std::ios::showbase);
    }


    void display_memory()  {
        std::cout << std::setfill('0');
        std::cout << std::hex;

        std::cout<<"|";

        for (size_t i = 0 ; i < MEMORY_SIZE; i++) {
            std::cout << std::setw(2);
            std::cout << static_cast<int16_t>(this->memory[i]) << "|";

            if (i%16 == 15) {
                std::cout << std::endl;
                std::cout << "|";
            }
        }

        std::cout << std::resetiosflags(std::ios::showbase);
    }

    // #######
    // OPCODES
    // #######

    // call
    static void OP_0nnn(uint16_t address);

    // display
    void OP_00e0();
    void OP_dxyn(uint8_t xreg_address, uint8_t yreg_address, uint8_t value);

    // flow
    void OP_00ee();
    void OP_1nnn(uint16_t address) ;
    void OP_2nnn(uint16_t address);
    void OP_bnnn(uint16_t value);

    // cond
    void OP_3xnn(uint8_t xreg_address, uint8_t value);
    void OP_4xnn(uint8_t xreg_address, uint8_t value);
    void OP_5xy0(uint8_t xreg_address, uint8_t yreg_address);
    void OP_9xy0(uint8_t xreg_address, uint8_t yreg_address);

    //const
    void OP_6xnn(uint8_t, uint8_t value);
    void OP_7xnn(uint8_t reg_address, uint8_t value);

    // assign
    void OP_8xy0(uint8_t xreg_address, uint8_t yreg_address);

    // bit op
    void OP_8xy1(uint8_t reg_address, uint8_t yreg_addres);
    void OP_8xy2(uint8_t reg_address, uint8_t yreg_addres);
    void OP_8xy3(uint8_t reg_address, uint8_t yreg_addres);
    void OP_8xy6(uint8_t xreg_address, uint8_t yreg_address);
    void OP_8xye(uint8_t xreg_address, uint8_t yreg_address);

    // math
    void OP_8xy4(uint8_t xreg_address, uint8_t yreg_addres);
    void OP_8xy5(uint8_t xreg_address, uint8_t yreg_addres);
    void OP_8xy7(uint8_t xreg_address, uint8_t yreg_addres);

    // memory
    void OP_annn(uint16_t address);

    void OP_bnnn(uint16_t value) const;

    void OP_fx1e(uint8_t xreg_address);
    void OP_fx29(uint8_t xreg_address);
    void OP_fx55(uint8_t xreg_address);
    void OP_fx65(uint8_t xreg_address);

    // rand
    void OP_cxnn(uint8_t xreg_address, uint8_t value);

    // key op
    void OP_ex9e(uint8_t xreg_address);
    void OP_exa1(uint8_t xreg_address);
    void OP_fx0a(uint8_t xreg_address);

    // timer
    void OP_fx07(uint8_t xreg_address);
    void OP_fx15(uint8_t xreg_address);

    void fx18(uint8_t xreg_address);

    // bcd
    void OP_fx33(uint8_t xreg_address);

};

// flow

void Chip8::OP_annn(const uint16_t address)  {
    this->address_reg = address;
}

void Chip8::OP_bnnn(const uint16_t value) const {
    this->OP_0nnn(value+registers[0x0]);
}

void Chip8::OP_cxnn(const uint8_t xreg_address, const uint8_t value) {
    this->registers[xreg_address] = rand() & value;
}


void Chip8::OP_6xnn(const uint8_t reg_address, const uint8_t value)  {
    this->registers[reg_address] = value;
}

void Chip8::OP_7xnn(const uint8_t reg_address, const uint8_t value)  {
    this->registers[reg_address] += value;
}


void Chip8::OP_8xy1(const uint8_t xreg_address, const uint8_t yreg_addres)  {
    this->registers[xreg_address] |= this->registers[yreg_addres];
}

void Chip8::OP_8xy2(const uint8_t xreg_address, const uint8_t yreg_addres)  {
    this->registers[xreg_address] &= this->registers[yreg_addres];
}

void Chip8::OP_8xy3(const uint8_t xreg_address, const uint8_t yreg_addres)  {
    this->registers[xreg_address] ^= this->registers[yreg_addres];
}

void Chip8::OP_8xy4(const uint8_t xreg_address, const uint8_t yreg_addres)  {
    this->registers[xreg_address] += this->registers[yreg_addres];
}

void Chip8::OP_8xy5(const uint8_t xreg_address, const uint8_t yreg_addres)  {
    this->registers[xreg_address] = this->registers[xreg_address] - this->registers[yreg_addres];
    this->registers[0xF] =  this->registers[xreg_address] <= this->registers[yreg_addres];
}

void Chip8::OP_8xy7(const uint8_t xreg_address, const uint8_t yreg_address)  {
    this->registers[xreg_address] = this->registers[yreg_address] - this->registers[xreg_address];
    this->registers[0xF] =  this->registers[xreg_address] >= this->registers[yreg_address];
}


void Chip8::OP_fx1e(const uint8_t xreg_address) {
    this->address_reg += this->registers[xreg_address];
}

void Chip8::OP_dxyn(const uint8_t xreg_address, const uint8_t yreg_address, const uint8_t value){
    const uint8_t x = this->registers[xreg_address] % VIDEO_WIDTH;
    const uint8_t y = this->registers[yreg_address] % VIDEO_HEIGHT;

    this->registers[0xF] = 0;

    for (size_t i = 0; i < value ; i++) {

        if (y + i >= VIDEO_HEIGHT)
            return;

        const uint8_t sprite_row = this->memory[address_reg + i];

        for (size_t j = 0; j < 8; j++) {
            if (x + j >= VIDEO_WIDTH)
                break;
            else {
                const bool pixel = sprite_row & (0x80 >> j);

                // if (this->video[y + i][x + j] == pixel)
                    // this->registers[0xF] = 1;

                this->video[y + i][x + j] ^= pixel;
            }
        }
    }
}

void Chip8::OP_9xy0(const uint8_t xreg_address, const uint8_t yreg_address) {
    if (xreg_address == yreg_address)
        this->program_counter += 2;
}

void Chip8::OP_3xnn(const uint8_t xreg_address, const uint8_t value) {
    if (this->registers[xreg_address] != value)
        this->program_counter += 2;
}


void Chip8::OP_4xnn(const uint8_t xreg_address, const uint8_t value) {
    if (this->registers[xreg_address] == value)
        this->program_counter += 2;
}




void Chip8::OP_2nnn(const uint16_t address) {
    if (stack_pointer == STACK_SIZE - 1)
        throw std::domain_error("Stack overflow");

    this->stack[stack_pointer] = this->program_counter;
    this->stack_pointer++;

    this->program_counter = address;
}

void Chip8::OP_0nnn(uint16_t address) {
    throw std::domain_error("Opcode don't supported");
}


void Chip8::OP_00ee() {
    this->program_counter =  this->stack[this->stack_pointer];
    this->stack_pointer--;
}

void Chip8::OP_00e0() {
    memset(this->video,0,sizeof(this->video));
}

void Chip8::OP_1nnn(const uint16_t address) {
    this->program_counter = address;
}

void Chip8::OP_5xy0(const uint8_t xreg_address, const uint8_t yreg_address) {
    if (xreg_address != yreg_address)
        this->program_counter += 2;
}

void Chip8::OP_8xy0(const uint8_t xreg_address, const uint8_t yreg_address) {
    this->registers[xreg_address] = this->registers[yreg_address];
}

void Chip8::OP_8xye(const uint8_t xreg_address, const uint8_t yreg_address) {
    // TODO:
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy6-and-8xye-shift
    // make instruction configurable

    this->registers[xreg_address] = this->registers[yreg_address];
    this->registers[xreg_address] >> 1;
    this->registers[0xF] = this->registers[yreg_address] & 0x01; // first digit
}

void Chip8::OP_8xy6(const uint8_t xreg_address, const uint8_t yreg_address) {
    // TODO:
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy6-and-8xye-shift
    // make instruction configurable

    this->registers[xreg_address] = this->registers[yreg_address];
    this->registers[xreg_address] >> 1;
    this->registers[0xF] = this->registers[yreg_address] & 0x80; // last digit
}

void Chip8::OP_fx55(const uint8_t xreg_address) {
    // TODO:
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#fx55-and-fx65-store-and-load-memory
    // make instruction configurable

    for (size_t i = 0; i < xreg_address; i++)
        this->memory[address_reg + i] = this->registers[i];
}

void Chip8::OP_fx65(const uint8_t xreg_address) {
    // TODO:
    // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#fx55-and-fx65-store-and-load-memory
    // make instruction configurable

    for (size_t i = 0; i < xreg_address; i++)
        this->registers[i] = this->memory[address_reg + i];
}

void Chip8::OP_ex9e(const uint8_t xreg_address) {
    if (this->registers[xreg_address] == PRESSED)
        this->program_counter += 2;
}

void Chip8::OP_exa1(const uint8_t xreg_address) {
    if (this->registers[xreg_address] == NOT_PRESSED)
        this->program_counter += 2;
}

void Chip8::OP_fx07(const uint8_t xreg_address) {
    this->registers[xreg_address] = this->delay_timer;
}

void Chip8::OP_fx15(const uint8_t xreg_address) {
    this->delay_timer = this->registers[xreg_address];
}

void Chip8::fx18(const uint8_t xreg_address) {
    this->sound_timer = this->registers[xreg_address];
}

void Chip8::OP_fx33(const uint8_t xreg_address) {
    const uint8_t reg_value = this->registers[xreg_address];

    this->memory[address_reg + 0] =  reg_value        / 100; // 1 digit
    this->memory[address_reg + 1] = (reg_value % 100) / 10;  // 2 digit
    this->memory[address_reg + 2] =  reg_value % 10 ;        // 3 digit
}

void Chip8::OP_fx29(const uint8_t xreg_address) {
    const uint8_t address = (this->registers[xreg_address] & 0x0F) * 5 ; // each sign takes 5B of space
    this->address_reg = address;
}

void Chip8::OP_fx0a(const uint8_t xreg_address) {
    // TODO
}


int main() {
    Chip8 emulator;


    // emulator.display_memory();
    // emulator.display_registers();
    emulator.OP_6xnn(1,0x1);
    emulator.OP_fx29(1);
    emulator.OP_dxyn(0,0,5);

    emulator.display_video();
}

