#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "game.h"

char symbol(Game_object obj) {
    switch (obj) {
        case EMPTY:
            return '.';
            break;
        default:
            assert(false && "Object Rendering not Implemented");
    }
}

void render_board(void)
{
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            printf("%c", symbol(get_board_object(i, j)));
        }
        printf("\n");
    }
}