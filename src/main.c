#include <SDL2/SDL.h>
#include "chip8.h"

int main() {
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
            if (event.type == SDL_QUIT) {
                goto out;
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