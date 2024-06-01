#include "chip8stack.h"
#include "chip8.h"
#include "assert.h"

static void chip8_stack_inbounds(struct chip8 *chip8) {
    assert(chip8->registers.SP < sizeof(chip8->stack.stack));
}

/**
* 2nnn - CALL addr
* Call subroutine at nnn.
* The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
**/
void chip8_stack_push(struct chip8 *chip8, unsigned short val) {
    chip8->registers.SP += 1;
    chip8_stack_inbounds(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
}

/**
* 00EE - RET
* Return from a subroutine.
* The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
**/
unsigned short chip8_stack_pop(struct chip8 *chip8) {
    chip8_stack_inbounds(chip8);

    unsigned short result = chip8->stack.stack[chip8->registers.SP];
    chip8->registers.SP -= 1;
    return result;
}

