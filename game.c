#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    Game_state state;
    Game_object board[HEIGHT][WIDTH];
} Game;

Game game;

void set_game_state(Game_state state)
{
    switch (state) {
        case INIT:
            game.state = INIT;
            break;
        default:
            assert(false && "Setting state not Implemented");
    }
}

void board_empty(void)
{
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            game.board[i][j] = EMPTY;
        }
    }
}

Game_object get_board_object(int i, int j)
{
    return game.board[i][j];
}

void init_game(void)
{
    set_game_state(INIT);
    board_empty();

}