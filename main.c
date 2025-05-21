#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "emulator.h"
#include "instructions.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_main.h"

// Define constants for display
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define PIXEL_SIZE 10

void OP_Table0(Chip8 *emulator, uint16_t opcode);
void OP_Table8(Chip8 *emulator, uint16_t opcode);
void OP_TableE(Chip8 *emulator, uint16_t opcode);
void OP_TableF(Chip8 *emulator, uint16_t opcode);

void updateDisplay(Chip8 *emulator, SDL_Renderer *renderer){
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 32; y++){
            if(emulator->screen[x][y]){
            SDL_FRect rect = {x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    //test print
    printf("Main Function Started\n");

    //initialize emulator, stack pointer, and random number generator
    Chip8 emulator;
    emulator.sp = 0;
    srand(time(NULL));

    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    printf("SDL initialized.\n");

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("Could not create window: %s.\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, "chip8");
    if (!renderer) {
        printf("Could not create renderer: %s.\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(-1);
    }

    //initialize function table
    void (*table[16])(Chip8*, uint16_t);

    table[0x0] = &OP_Table0;
    table[0x1] = &OP_JP;
    table[0x2] = &OP_CALL;
    table[0x3] = &OP_SE_BYTE;
    table[0x4] = &OP_SNE;
    table[0x5] = &OP_SE_REGISTER;
    table[0x6] = &OP_LD_BYTE;
    table[0x7] = &OP_ADD_BYTE;
    table[0x8] = &OP_Table8;
    table[0x9] = &OP_SNE_REG;
    table[0xA] = &OP_LD_I;
    table[0xB] = &OP_JP_REG;
    table[0xC] = &OP_RAND;
    table[0xD] = &OP_DRW;
    table[0xE] = &OP_TableE;
    table[0xF] = &OP_TableF;

    // load rom and standard font into memory
    FILE *rom = fopen("zero.ch8", "rb");
    FILE *font = fopen("font.bin", "rb");
    loadFile(&emulator, rom, 0x200);
    loadFile(&emulator, font, 0x50);

    //Fetch, Debug, Execute
    while(true){
        //get opcode
        uint16_t opcode = emulator.memory[emulator.counter];

        //add to counter
        emulator.counter += 2;

        //decode instruction
        table[opcode >> 12](&emulator, opcode);

        //decrement timers
        if(emulator.timer > 0){
            emulator.timer--;
        }
        if(emulator.sound_timer > 0){
            emulator.sound_timer--;
        }

        //update display
        updateDisplay(&emulator, renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void OP_Table0(Chip8 *emulator, uint16_t opcode) {
    void (*table0[2])(Chip8*, uint16_t);

    table0[0x0] = &OP_CLS;
    table0[0xE] = &OP_RET;

    table0[opcode & 0x000F](emulator, opcode);
}

void OP_Table8(Chip8 *emulator, uint16_t opcode) {
    void (*table8[9])(Chip8*, uint16_t);

    table8[0x0] = &OP_LOAD_REG;
    table8[0x1] = &OP_OR_REG;
    table8[0x2] = &OP_AND_REG;
    table8[0x3] = &OP_XOR_REG;
    table8[0x4] = &OP_ADD_REG;
    table8[0x5] = &OP_SUB_REG;
    table8[0x6] = &OP_SHR;
    table8[0x7] = &OP_SUBN;
    table8[0xE] = &OP_SHL;

    table8[opcode & 0x000F](emulator, opcode);
}

void OP_TableE(Chip8 *emulator, uint16_t opcode) {
    void (*tableE[2])(Chip8*, uint16_t);

    tableE[0x1] = &OP_SKNP;
    tableE[0xE] = &OP_SKP;

    tableE[opcode & 0x000F](emulator, opcode);
}

void OP_TableF(Chip8 *emulator, uint16_t opcode) {
    void (*tableF[9])(Chip8*, uint16_t);

    tableF[0x07] = &OP_LD_DT_REG;
    tableF[0x0A] = &OP_LD_KEY;
    tableF[0x15] = &OP_LD_REG_DT;
    tableF[0x18] = &OP_LD_REG_ST;
    tableF[0x1E] = &OP_ADD_I;
    tableF[0x29] = &OP_LD_DIG;
    tableF[0x33] = &OP_LD_BCD;
    tableF[0x55] = &OP_LD_ARR_STORE;
    tableF[0x65] = &OP_LD_ARR_READ;

    tableF[opcode & 0x00FF](emulator, opcode);
}