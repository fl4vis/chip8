#include <SDL2/SDL.h>
#include <unistd.h>
#include "chip8.h"
#include "chip8keyboard.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-sign"
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
    chip8_init(&chip8);
    chip8.registers.sound_timer = 3;

    chip8_screen_draw_sprite(&chip8.screen, 0, 0, &chip8.memory.memory[0x05], 5);
    chip8_screen_draw_sprite(&chip8.screen, 5, 0, &chip8.memory.memory[0x00], 5);
    chip8_screen_draw_sprite(&chip8.screen, 62, 10, &chip8.memory.memory[0x00], 5);
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < CHIP8_WIDTH; x++) {
           for(int y = 0; y < CHIP8_HEIGHT; y++) {
               if(chip8_screen_is_set(&chip8.screen, x, y)) {
                   SDL_Rect r = {
                       x * CHIP8_WINDOW_MULTIPLIER,
                       y * CHIP8_WINDOW_MULTIPLIER,
                       CHIP8_WINDOW_MULTIPLIER,
                       CHIP8_WINDOW_MULTIPLIER,
                   };

                   SDL_RenderFillRect(renderer, &r);
               }
           }
        }
        SDL_RenderPresent(renderer);

        if(chip8.registers.delay_timer > 0) {
            usleep(100 * 1000);
            chip8.registers.delay_timer -=1;
            printf("Delay!!!\n");
        }

        if(chip8.registers.sound_timer > 0) {
            unsigned char duration = chip8.registers.sound_timer;
            char command[100];

            sprintf(command, "play -nq -t alsa synth %d sine 440", duration);

            // Execute the command
            system(command);
            chip8.registers.sound_timer = 0;
        }
    }

    out:
    SDL_DestroyWindow(window);
    return 0;
}
#pragma clang diagnostic pop