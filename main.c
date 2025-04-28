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

int loadFile(Chip8 *pc, FILE *rom, int START_ADDRESS){
    fseek(rom, 0L, SEEK_END);
    int size = ftell(rom);
    rewind(rom);
    uint8_t buffer[size];

    fread(buffer, sizeof(uint8_t), size, rom);
    for(int i = 0; i < sizeof(buffer); i++){
        pc->memory[START_ADDRESS + i] = buffer[i];
    }

    fclose(rom);
    return 0;
}

int main(void) {
    Chip8 emulator;

    FILE *rom = fopen("zero.ch8", "rb");
    loadFile(&emulator, rom, 0x200);

    return 0;
}