#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "emulator.h"

int main(void) {
    //initialize emulator, stack pointer, and random number generator
    Chip8 emulator;
    emulator.sp = 0;
    srand(time(NULL));

    // load rom and standard font into memory
    FILE *rom = fopen("zero.ch8", "rb");
    FILE *font = fopen("font.bin", "rb");
    loadFile(&emulator, rom, 0x200);
    loadFile(&emulator, font, 0x50);

    //debug memory
    printMemory(emulator);

    return 0;
}