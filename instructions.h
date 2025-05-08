#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// Opcode function declarations
void OP_CLS(Chip8 *emulator, uint16_t opcode);               // 00E0 - Clear the display
void OP_RET(Chip8 *emulator, uint16_t opcode);               // 00EE - Return from a subroutine
void OP_JP(Chip8 *emulator, uint16_t opcode);                // 1nnn - Jump to location nnn
void OP_CALL(Chip8 *emulator, uint16_t opcode);              // 2nnn - Call subroutine at nnn
void OP_SE_BYTE(Chip8 *emulator, uint16_t opcode);           // 3xkk - Skip next instruction if Vx = kk
void OP_SNE(Chip8 *emulator, uint16_t opcode);               // 4xkk - Skip next instruction if Vx != kk
void OP_SE_REGISTER(Chip8 *emulator, uint16_t opcode);       // 5xy0 - Skip next instruction if Vx = Vy
void OP_LD_BYTE(Chip8 *emulator, uint16_t opcode);           // 6xkk - Set Vx = kk
void OP_ADD_BYTE(Chip8 *emulator, uint16_t opcode);          // 7xkk - Set Vx = Vx + kk
void OP_LOAD_REG(Chip8 *emulator, uint16_t opcode);          // 8xy0 - Set Vx = Vy
void OP_OR_REG(Chip8 *emulator, uint16_t opcode);            // 8xy1 - Set Vx = Vx OR Vy
void OP_AND_REG(Chip8 *emulator, uint16_t opcode);           // 8xy2 - Set Vx = Vx AND Vy
void OP_XOR_REG(Chip8 *emulator, uint16_t opcode);           // 8xy3 - Set Vx = Vx XOR Vy
void OP_ADD_REG(Chip8 *emulator, uint16_t opcode);           // 8xy4 - Set Vx = Vx + Vy, set VF = carry
void OP_SUB_REG(Chip8 *emulator, uint16_t opcode);           // 8xy5 - Set Vx = Vx - Vy, set VF = NOT borrow
void OP_SHR(Chip8 *emulator, uint16_t opcode);               // 8xy6 - Set Vx = Vx SHR 1
void OP_SUBN(Chip8 *emulator, uint16_t opcode);              // 8xy7 - Set Vx = Vy - Vx, set VF = NOT borrow
void OP_SHL(Chip8 *emulator, uint16_t opcode);               // 8xyE - Set Vx = Vx SHL 1
void OP_SNE_REG(Chip8 *emulator, uint16_t opcode);           // 9xy0 - Skip next instruction if Vx != Vy
void OP_LD_I(Chip8 *emulator, uint16_t opcode);              // Annn - Set I = nnn
void OP_JP_REG(Chip8 *emulator, uint16_t opcode);            // Bnnn - Jump to location nnn + V0
void OP_RAND(Chip8 *emulator, uint16_t opcode);              // Cxkk - Set Vx = random byte AND kk
void OP_DRW(Chip8 *emulator, uint16_t opcode);               // Dxyn - Display n-byte sprite at (Vx, Vy), set VF = collision
void OP_SKP(Chip8 *emulator, uint16_t opcode);               // Ex9E - Skip next instruction if key with value Vx is pressed
void OP_SKNP(Chip8 *emulator, uint16_t opcode);              // ExA1 - Skip next instruction if key with value Vx is not pressed
void OP_LD_DT_REG(Chip8 *emulator, uint16_t opcode);         // Fx07 - Set Vx = delay timer value
void OP_LD_KEY(Chip8 *emulator, uint16_t opcode);            // Fx0A - Wait for a key press, store the value of the key in Vx
void OP_LD_REG_DT(Chip8 *emulator, uint16_t opcode);         // Fx15 - Set delay timer = Vx
void OP_LD_REG_ST(Chip8 *emulator, uint16_t opcode);         // Fx18 - Set sound timer = Vx
void OP_ADD_I(Chip8 *emulator, uint16_t opcode);             // Fx1E - Set I = I + Vx
void OP_LD_DIG(Chip8 *emulator, uint16_t opcode);            // Fx29 - Set I = location of sprite for digit Vx
void OP_LD_BCD(Chip8 *emulator, uint16_t opcode);            // Fx33 - Store BCD representation of Vx in memory locations I, I+1, and I+2
void OP_LD_ARR_STORE(Chip8 *emulator, uint16_t opcode);      // Fx55 - Store registers V0 through Vx in memory starting at location I
void OP_LD_ARR_READ(Chip8 *emulator, uint16_t opcode);       // Fx65 - Read registers V0 through Vx from memory starting at location I

#endif