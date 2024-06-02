#include "chip8screen.h"
#include <assert.h>
#include <memory.h>

static void chip8_screen_check_bounds(int x, int y) {
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void chip8_screen_set(struct chip8_screen *screen, int x, int y) {
    chip8_screen_check_bounds(x, y);
    screen->pixels[y][x] = true;
}

bool chip8_screen_is_set(struct chip8_screen *screen, int x, int y) {
    chip8_screen_check_bounds(x, y);
    return screen->pixels[y][x];
}

/**
 * Dxyn - DRW Vx, Vy, nibble
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 *
 * The interpreter reads n bytes from memory, starting at the address stored in I.
 * These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
 * Sprites are XORed onto the existing screen.
 * If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
 * If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
 */
bool chip8_screen_draw_sprite(struct chip8_screen *screen,int x, int y, const char *sprite, int num) {
    bool pixel_collision = false;

    for(int ly = 0; ly < num; ly++){
        char c = sprite[ly];

       for (int lx = 0; lx < 8; lx++) {

           if((c & (0b10000000 >> lx)) == 0)
               continue;

           if(screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH]) {
              pixel_collision = true;
           }

           //XOR
           screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH] ^= true;
       }
    }

    return pixel_collision;
}

void chip8_screen_clear(struct chip8_screen *screen) {
    memset(screen->pixels, 0, sizeof(screen->pixels));
}
