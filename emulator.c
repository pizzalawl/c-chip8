#include "instructions.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct{
    uint8_t memory[4096];
    uint16_t counter;
    uint16_t index;
    uint8_t timer;
    uint8_t sound_timer;
    uint8_t registers[15];
    bool screen[64][32];
} Chip8;

//file that loads a binary file into emulator memory
int loadFile(Chip8 *pc, FILE *file, int START_ADDRESS){
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);
    uint8_t buffer[size];

    fread(buffer, sizeof(uint8_t), size, file);
    for(int i = 0; i < sizeof(buffer); i++){
        pc->memory[START_ADDRESS + i] = buffer[i];
    }

    fclose(file);
    return 0;
}