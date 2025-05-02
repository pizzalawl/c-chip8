#include "instructions.h"
#include "emulator.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//file that loads a binary file into emulator memory
int loadFile(Chip8 *pc, FILE *file, int START_ADDRESS){
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);
    uint8_t buffer[4096];

    fread(buffer, sizeof(uint8_t), size, file);
    for(int i = 0; i < sizeof(buffer); i++){
        pc->memory[START_ADDRESS + i] = buffer[i];
    }

    fclose(file);
    return 0;
}

void printMemory(Chip8 emulator){
    for(int i = 0; i < sizeof(emulator.memory); i++){
        printf("%x: %i\n", i, emulator.memory[i]);
    }
}