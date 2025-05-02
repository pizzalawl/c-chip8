#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "emulator.h"

void OP_CLS(Chip8 *emulator, uint16_t opcode){
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 32; y++){
            emulator->screen[x][y] = false;
        }
    }
}

void OP_RET(Chip8 *emulator, uint16_t opcode){
    emulator->sp--;
    emulator->counter = emulator->stack[emulator->sp];
}

void OP_JP(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->counter = address;
}

void OP_CALL(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->stack[emulator->sp] = emulator->counter;
    emulator->sp++;
    emulator->counter = address;
}

void OP_SE_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = opcode & 0x0F00;
    uint16_t byte = opcode & 0x00FF;

    if(emulator->registers[reg] == byte){
        emulator->counter += 2;
    }
}

void OP_SNE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    if(emulator->registers[reg] != byte){
        emulator->counter += 2;
    }
}

void OP_SE_REGISTER(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] == emulator->registers[reg2]){
        emulator->counter += 2;
    }
}

void OP_LD_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    emulator->registers[reg] = byte;
}

void OP_ADD_BYTE(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;

    emulator->registers[reg] += byte;
}

void OP_LOAD_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] = emulator->registers[reg2];
}

void OP_OR_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] |= emulator->registers[reg2];
}

void OP_AND_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] &= emulator->registers[reg2];
}

void OP_XOR_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    emulator->registers[reg1] ^= emulator->registers[reg2];
}

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

void OP_SNE_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t reg1 = (opcode & 0x0F00) >> 8;
    uint16_t reg2 = (opcode & 0x00F0) >> 4;

    if(emulator->registers[reg1] != emulator->registers[reg2]){
        emulator->counter += 2;
    }
}

void OP_LD_I(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->index = address;
}

void OP_JP_REG(Chip8 *emulator, uint16_t opcode){
    uint16_t address = opcode & 0x0FFF;

    emulator->counter = (address + emulator->registers[0]);
}

void OP_RAND(Chip8 *emulator, uint16_t opcode){
    uint16_t reg = (opcode & 0x0F00) >> 8;
    uint16_t byte = opcode & 0x00FF;
    uint16_t random = (rand() % 255) + 1;

    emulator->registers[reg] = random & byte;
}

void OP_DRW(Chip8 *emulator, uint16_t opcode){

}

void OP_SKP(Chip8 *emulator, uint16_t opcode){

}

void OP_SKNP(Chip8 *emulator, uint16_t opcode){

}

void OP_LD_DT_REG(Chip8 *emulator, uint16_t opcode){

}

void OP_LD_KEY(Chip8 *emulator, uint16_t opcode){
    
}

void OP_LD_REG_DT(Chip8 *emulator, uint16_t opcode){

}

void OP_LD_REG_ST(Chip8 *emulator, uint16_t opcode){

}

void OP_ADD_I(Chip8 *emulator, uint16_t opcode){

}

void 

