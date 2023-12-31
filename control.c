#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "trender.h"

void cursor_up(void)
{
    int i = get_cursor_i();
    int j = get_cursor_j();
    if (i <= 0) {
        return;
    }
    set_cursor(i - 1, j);
}

void cursor_down(void)
{
    int i = get_cursor_i();
    int j = get_cursor_j();
    if (i >= FRAME_HEIGHT - 1) {
        return;
    }
    set_cursor(i + 1, j);
}

void cursor_left(void)
{
    int i = get_cursor_i();
    int j = get_cursor_j();
    if (j <= 0) {
        return;
    }
    set_cursor(i, j - 1);
}

void cursor_right(void)
{
    int i = get_cursor_i();
    int j = get_cursor_j();
    if (j >= FRAME_WIDTH - 1) {
        return;
    }
    set_cursor(i, j + 1);
}

char take_input(void)
{
    char buf[2];
    fgets(buf, sizeof(buf), stdin);

    return buf[0];
}

void click(void)
{
    Selected s = get_selected();
    switch (s)
    {
        case SEL_EMPTY:
            next_phase();
            return;
        case SEL_BOARD:
            interact_board(get_board_i(), get_board_j());
            return;
        case SEL_STACK_PLAYER1:
            assert(false && "Selecting Player Stack 1 Not Implemented");
        case SEL_STACK_PLAYER2:
            assert(false && "Selecting Player Stack 2 Not Implemented");
    }
}

int input(void)
{
    char ch = take_input();

    switch (ch) {
        case 'q':
            printf("Exiting Game\n");
            
            exit(0);
        case 'w':
            cursor_up();
            return 0;
        case 's':
            cursor_down();
            return 0;
        case 'a':
            cursor_left();
            return 0;
        case 'd':
            cursor_right();
            return 0;
        case 'z':
            click();
            return 0;
        case 'r':
            init_game();
            return 1;
        case '\n':
            return 1;
        default:
            printf("Input %c is not recognized\n", ch);
            return 0;
    }
}

