#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "game.h"
#include "trender.h"

const char sep = '_';
const char cursor = 'A';

static char buf[FRAME_HEIGHT][FRAME_WIDTH];
static int cursor_i, cursor_j;

char symbol(Game_object obj) {
    switch (obj) {
        case EMPTY:
            return '.';
        case PORT:
            return 'P';
        default:
            assert(false && "Object Rendering not Implemented");
    }
}

void render_board(int x, int y)
{
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            buf[x + i][y + j] = symbol(get_board_object(i, j));
        }
    }
}

void render_sep(int x, int y)
{
    for (int i = 0; i < FRAME_WIDTH; i++)
    {
        buf[x][y + i] = sep;
    }
}

void render_cursor()
{
    buf[cursor_i][cursor_j] = cursor;
}

void flush()
{
    for (int i = 0; i < FRAME_HEIGHT; i++)
    {
        fwrite(buf[i], FRAME_WIDTH, sizeof(char), stdout);
        fwrite("\n", 1, 1, stdout);
    }
}

void new_frame(void) {
    render_sep(0, 0);
    render_board(1, 0);
    render_cursor();
    flush();
}

void set_cursor(int i, int j)
{
    if(i >= HEIGHT || i < 0 || j < 0 || j >= WIDTH) {
        assert(false && "Cursor out of Bound not Implemented");
    }
    cursor_i = i;
    cursor_j = j;
}