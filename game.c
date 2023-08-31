#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    Game_state state;
    Game_object board[HEIGHT][WIDTH];
    Game_object stacks[PLAYER_COUNT];
    Card cards[CARDS_PER_PLAYER * PLAYER_COUNT];
} Game;

static Game game;

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

void place_port(void)
{
    game.board[0][3] = PORT;
    game.board[0][4] = PORT;
    game.board[7][3] = PORT;
    game.board[7][4] = PORT;

}

Game_object get_board_object(int i, int j)
{
    return game.board[i][j];
}

void init_game(void)
{
    set_game_state(INIT);
    board_empty();
    place_port();

}