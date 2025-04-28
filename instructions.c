#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "emulator.h"

void OP_CLS(Chip8 *emulator){
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 32; y++){
            emulator->memory[x][y] = false;
        }
    }
}