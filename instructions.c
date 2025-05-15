#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "emulator.h"


//00E0 - CLS
void OP_CLS(Chip8 *emulator, uint16_t opcode){
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 32; y++){
            emulator->screen[x][y] = false;
        }
    }
}

//00EE - RET
void OP_RET(Chip8 *emulator, uint16_t opcode){
    emulator->sp--;
    emulator->counter = emulator->stack[emulator->sp];
}

//1nnn - JP addr
void OP_JP(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->counter = address;
}

//2nnn CALL addr
void OP_CALL(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->stack[emulator->sp] = emulator->counter;
    emulator->sp++;
    emulator->counter = address;
}

//3xkk - SE Vx, byte
void OP_SE_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = opcode & 0x0F00;
    uint16_t byte = opcode & 0x00FF;

    if(emulator->registers[reg] == byte){
        emulator->counter += 2;
    }
}

//4xkk - SNE Vx, byte
void OP_SNE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    if(emulator->registers[reg] != byte){
        emulator->counter += 2;
    }
}

//5xy0 - SE Vx, Vy
void OP_SE_REGISTER(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] == emulator->registers[reg2]){
        emulator->counter += 2;
    }
}

//6xkk - LD Vx, byte
void OP_LD_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    emulator->registers[reg] = byte;
}

//7xkk - ADD Vx, byte
void OP_ADD_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    emulator->registers[reg] += byte;
}

//8xy0 - LD Vx, Vy
void OP_LOAD_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] = emulator->registers[reg2];
}

//8xy1 - OR Vx, Vy
void OP_OR_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] |= emulator->registers[reg2];
}

//8xy2 - AND Vx, Vy
void OP_AND_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] &= emulator->registers[reg2];
}

//8xy3 - XOR Vx, Vy
void OP_XOR_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] ^= emulator->registers[reg2];
}

//8xy4 - ADD Vx, Vy
void OP_ADD_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if((emulator->registers[reg1] + emulator->registers[reg2]) > 255){
        emulator->registers[0xF] = 1;
    }
    else{
        emulator->registers[0xF] = 0;
    }

    if(emulator->registers[0xF] != 1){
        emulator->registers[reg1] += emulator->registers[reg2];
    }
    else{
        emulator->registers[reg1] = (emulator->registers[reg1] + emulator->registers[reg2]) & 0xFF;
    }
}

//8xy5 - SUB Vx, Vy
void OP_SUB_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] > emulator->registers[reg2]){
        emulator->registers[0xF] = 1;
    }
    else{
        emulator->registers[0xF] = 0;
    }

    emulator->registers[reg1] -= emulator->registers[reg2];
}

//8xy6 - SHR Vx
void OP_SHR(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] % 2){
        emulator->registers[0xF] = 1;
    }
    else{
        emulator->registers[0xF] = 0;
    }

    emulator->registers[reg1] /= 2;
}

//8xy7 - SUBN Vx, Vy
void OP_SUBN(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg2] > emulator->registers[reg1]){
        emulator->registers[0xF] = 1;
    }
    else{
        emulator->registers[0xF] = 0;
    }

    emulator->registers[reg2] -= emulator->registers[reg1];
}

//8xyE - SHL Vx {, Vy}
void OP_SHL(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if((emulator->registers[reg1] & 0x80) != 0){
        emulator->registers[0xF] = 1;
    }
    else{
        emulator->registers[0xF] = 0;
    }

    emulator->registers[reg1] *= 2;
}

//9xy0 - SNE Vx, Vy
void OP_SNE_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] != emulator->registers[reg2]){
        emulator->counter += 2;
    }
}

//Annn - LD I, addr
void OP_LD_I(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->index = address;
}

//Bnnn - JP V0, addr
void OP_JP_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->counter = (address + emulator->registers[0]);
}

//Cxkk - RND Vx, byte
void OP_RAND(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;
    uint16_t random = (rand() % 255) + 1;

    emulator->registers[reg] = random & byte;
}

//Dxyn - DRW Vx, Vy, nibble
void OP_DRW(Chip8 *emulator, uint16_t opcode){

}

//Ex9E - SKP Vx
void OP_SKP(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;
    uint8_t key = emulator->registers[reg];

    if(emulator->keypad[key]){
        emulator->counter += 2;
    }
}

//ExA1 - SKNP Vx
void OP_SKNP(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;
    uint8_t key = emulator->registers[reg];

    if(!emulator->keypad[key]){
        emulator->counter += 2;
    }
}

//Fx07 - LD Vx, DT
void OP_LD_DT_REG(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    emulator->registers[reg] = emulator->timer;
}

//Fx0A - LD Vx, K
void OP_LD_KEY(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    if(emulator->keypad[0]){
        emulator->registers[reg] = 1;
    }
    else if(emulator->keypad[1]){
        emulator->registers[reg] = 2;
    }
    else if(emulator->keypad[2]){
        emulator->registers[reg] = 3;
    }
    else if(emulator->keypad[3]){
        emulator->registers[reg] = 4;
    }
    else if(emulator->keypad[4]){
        emulator->registers[reg] = 5;
    }
    else if(emulator->keypad[5]){
        emulator->registers[reg] = 6;
    }
    else if(emulator->keypad[6]){
        emulator->registers[reg] = 7;
    }
    else if(emulator->keypad[7]){
        emulator->registers[reg] = 8;
    }
    else if(emulator->keypad[8]){
        emulator->registers[reg] = 9;
    }
    else if(emulator->keypad[9]){
        emulator->registers[reg] = 10;
    }
    else if(emulator->keypad[10]){
        emulator->registers[reg] = 11;
    }
    else if(emulator->keypad[11]){
        emulator->registers[reg] = 12;
    }
    else if(emulator->keypad[12]){
        emulator->registers[reg] = 13;
    }
    else if(emulator->keypad[13]){
        emulator->registers[reg] = 14;
    }
    else if(emulator->keypad[14]){
        emulator->registers[reg] = 15;
    }
    else if(emulator->keypad[15]){
        emulator->registers[reg] = 16;
    }
    else{
        emulator->counter -= 2;
    }
}

//Fx15 - LD DT, Vx
void OP_LD_REG_DT(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    emulator->timer = emulator->registers[reg];
}

//Fx18 - LD ST, Vx
void OP_LD_REG_ST(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    emulator->sound_timer = emulator->registers[reg];
}

//Fx1E - ADD I, Vx
void OP_ADD_I(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    emulator->index += emulator->registers[reg];
}

//Fx29 - LD F, Vx
void OP_LD_DIG(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;
    uint8_t digit = emulator->registers[reg];

    emulator->index = 0x50 + (5 * digit);
}

//Fx33 - LD B, Vx
void OP_LD_BCD(Chip8 *emulator, uint16_t opcode){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = emulator->registers[Vx];

	// Ones-place
	emulator->memory[emulator->index + 2] = value % 10;
	value /= 10;

	// Tens-place
	emulator->memory[emulator->index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	emulator->memory[emulator->index] = value % 10;
    
}

//Fx55 - LD [I], Vx
void OP_LD_ARR_STORE(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    for(int i = 0; i <= reg; i++){
        emulator->memory[emulator->index + i] = emulator->registers[i];
    }
}

//Fx65 - LD Vx, [I]
void OP_LD_ARR_READ(Chip8 *emulator, uint16_t opcode){
    uint8_t reg = (opcode & 0x0F00) >> 8;

    for(int i = 0; i <= reg; i++){
        emulator->registers[i] = emulator->memory[emulator->index + i];
    }
}