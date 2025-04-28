#ifndef EMULATOR_H_
#define EMULATOR_H_

typedef struct {
    uint8_t memory[4096];
    uint16_t counter;
    uint16_t index;
    uint8_t timer;
    uint8_t sound_timer;
    uint8_t registers[15];
    bool screen[64][32];
} Chip8;
int loadFile(Chip8 *pc, FILE *file, int START_ADDRESS);

#endif