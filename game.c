#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    enum {PLAYER1, PLAYER2} owner;
    enum {LINK, VIRUS} type;
    enum {HIDDEN, REVEALED} visibility;
    Game_object* position;
} Card;

typedef struct
{
    Game_state state;
    Game_object board[BOARD_HEIGHT][BOARD_WIDTH];
    Game_object stacks[PLAYER_COUNT];
    Card cards[TOTAL_CARDS];
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
            game.board[i][j] = GAME_EMPTY;
        }
    }
}

void place_port(void)
{
    game.board[0][3] = GAME_PORT;
    game.board[0][4] = GAME_PORT;
    game.board[7][3] = GAME_PORT;
    game.board[7][4] = GAME_PORT;

}

Board_object get_board_card(Game_object * obj)
{
    assert(false && "Get board card not implemented");
}


Board_object get_board_object(int i, int j)
{
    Game_object* obj = &(game.board[i][j]);
    switch (*obj)
    {
        case GAME_EMPTY: 
            return BOARD_BLANK;
        case GAME_PORT:
            return BOARD_PORT;
        case GAME_CARD:
            return get_board_card(obj);
        default:
            assert(false && "Unreachable");
    }
}

void update_board_cards(void)
{
    for(int i = 0; i < TOTAL_CARDS; i++)
    {
        *(game.cards[i].position) = GAME_CARD;
    }
}

void init_cards(void)
{
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        game.cards[i] = (Card){PLAYER1, LINK, HIDDEN, &(game.board[6][i])};
    }
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        game.cards[i + 4] = (Card){PLAYER1, VIRUS, HIDDEN, &(game.board[6][i + 4])};
    }
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        game.cards[i + 8] = (Card){PLAYER2, LINK, HIDDEN, &(game.board[1][i])};
    }
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        game.cards[i + 12] = (Card){PLAYER2, LINK, HIDDEN, &(game.board[1][i + 4])};
    }
    update_board_cards();
}

void init_game(void)
{
    set_game_state(INIT);
    board_empty();
    place_port();
    init_cards();
}

void start_game(void)
{
    assert(game.state == INIT);
    game.state = SETUP_PLAYER1;
    assert(false && "Not Implemented");
}

Game_state get_game_state(void)
{
    return game.state;
}

Success interact_board(int i, int j)
{
    switch(game.state)
    {
        case INIT:
            start_game();
            return VALID;
        default:
            assert(false && "Interacting in this game state not Implemented");
            return INVALID;
    }
}