#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "game.h"
#include "trender.h"

const char sep = '#';
const char cursor = 'A';
const char empty = '_';
const int board_i = (FRAME_HEIGHT - BOARD_HEIGHT) / 2 + 1;
const int board_j = (FRAME_WIDTH - BOARD_WIDTH) / 2 + 1;

static char buf[FRAME_HEIGHT][FRAME_WIDTH];
static int cursor_i, cursor_j;
static char cursor_temp;

void init_display(void)
{
    for (int i = 0; i < FRAME_HEIGHT; i++){
        for(int j = 0; j < FRAME_WIDTH; j++) {
            buf[i][j] = empty;
        }
    }
}

char symbol(Board_object obj) {
    switch (obj) {
        case BOARD_BLANK:
            return '.';
        case BOARD_PORT:
            return 'P';
        case BOARD_HIDDEN_CARD:
            return 'c';
        case BOARD_LINK_CARD:
            return 'L';
        case BOARD_VIRUS_CARD:
            return 'V';
        default:
            assert(false && "Object Rendering not Implemented");
    }
}

void render_board(int x, int y)
{
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
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

void render_stack(int x, int y, Player player) {
    Board_object stack[CARDS_PER_PLAYER];
    get_stack(stack, CARDS_PER_PLAYER, player);
    for (int i = 0; i < CARDS_PER_PLAYER; i++)
    {
        buf[x][y + i] = symbol(stack[i]);
    }
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
    render_board(board_i, board_j);
    render_stack(board_i + 9, board_j, PLAYER1);
    render_stack(board_i - 2, board_j, PLAYER2);
    render_cursor();
    flush();
}
    
void init_cursor()
{
    cursor_i = 0;
    cursor_j = 0;
}

void set_cursor(int i, int j)
{
    if(i >= FRAME_HEIGHT || i < 0 || j < 0 || j >= FRAME_WIDTH) {
        assert(false && "Cursor out of Bound not Implemented");
    }
    buf[cursor_i][cursor_j] = cursor_temp;
    cursor_temp = buf[i][j];
    cursor_i = i;
    cursor_j = j;
}

int get_cursor_i()
{
    return cursor_i;
}

int get_cursor_j()
{
    return cursor_j;
}

Selected get_selected(void)
{
    if (cursor_i >= board_i && 
        cursor_j >= board_j && 
        cursor_i < board_i + BOARD_WIDTH && 
        cursor_j < board_j + BOARD_HEIGHT)
    {
        return SEL_BOARD;
    }
    else {
        return SEL_EMPTY;
    }
}

int get_board_i(void)
{
    assert(get_selected() == SEL_BOARD);

    return cursor_i - board_i;
}
int get_board_j(void)
{
    assert(get_selected() == SEL_BOARD);

    return cursor_j - board_j;
}