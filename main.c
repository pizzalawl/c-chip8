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

    //Fetch, Debug, Execute
    while(true){
        //get opcode
        uint8_t opcode = emulator.memory[emulator.counter];

        //add to counter
        emulator.counter += 2;

        //decode instruction
        
        //decrement timers
        if(emulator.timer > 0){
            emulator.timer--
        }
        if(emulator.sound_timer > 0){
            emulator.sound_timer--
        }
    }

    return 0;
}