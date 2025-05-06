#ifndef EMULATOR_H_
#define EMULATOR_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    uint8_t memory[4096];
    uint16_t stack[15];
    uint8_t sp;
    uint16_t counter;
    uint16_t index;
    uint8_t timer;
    uint8_t sound_timer;
    uint8_t registers[15];
    bool screen[64][32];
    bool keypad[15];
} Chip8;

int loadFile(Chip8 *pc, FILE *file, int START_ADDRESS);
void printMemory(Chip8 emulator);

#endif