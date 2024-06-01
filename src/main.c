#include <SDL2/SDL.h>
#include "chip8.h"
#include "chip8keyboard.h"

/**
* The computers which originally used the Chip-8 Language had a 16-key hexadecimal keypad with the following layout:
*    1	 2	 3	 C
*    4	 5	 6	 D
*    7	 8	 9	 E
*    A	 0	 B	 F
* This layout must be mapped into various other configurations to fit the keyboards of today's platforms.
**/

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3,
        SDLK_4, SDLK_5, SDLK_6, SDLK_7,
        SDLK_8, SDLK_9, SDLK_a, SDLK_b,
        SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main() {
    struct chip8 chip8;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
            EMULATOR_WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
            CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
            SDL_WINDOW_SHOWN);


    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while (1) {
        //Handle Event
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    goto out;
                break;

                case SDL_KEYDOWN: {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);
                    
                    printf("key down %x\n", vkey);
                    fflush(stdout); // Flush the output buffer

                    if(vkey != -1) {
                        chip8_keyboard_down(&chip8.keyboard, vkey);
                    }
                    break;
                }

                case SDL_KEYUP: {
                    char key = event.key.keysym.sym;
                    int vkey = chip8_keyboard_map(keyboard_map, key);

                    printf("key up %x\n", vkey);
                    fflush(stdout); // Flush the output buffer

                    if(vkey != -1) {
                        chip8_keyboard_up(&chip8.keyboard, vkey);
                    }
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // draw white rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_Rect r = {0, 0, 40, 40};

        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }

    out:
    SDL_DestroyWindow(window);
    return 0;
}