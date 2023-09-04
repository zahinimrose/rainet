#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    enum {PLAYER1, PLAYER2} owner;
    enum {LINK, VIRUS} type;
    enum {HIDDEN, REVEALED} visibility;
    Game_object position;
} Card;

typedef struct
{
    Game_state state;
    Game_object board[BOARD_HEIGHT][BOARD_WIDTH];
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
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
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

void start_game(void)
{
    game.state = SETUP_PLAYER1;
    assert(false && "Not Implemented");
}

Game_state get_state(void)
{
    assert(false && "Not Implemented");
}

Success interact_board(int i, int j)
{
    assert(false && "Interact_board not implemented");
}