#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    Player owner;
    enum {LINK, VIRUS} type;
    enum {HIDDEN, REVEALED} visibility; // Visibility to all player
    Game_object* position;
} Card;

typedef struct
{
    Game_state state;
    Player turn;
    Game_object board[BOARD_HEIGHT][BOARD_WIDTH];
    Game_object stacks[PLAYER_COUNT];
    Card cards[TOTAL_CARDS];
    Card* picked_up_card;
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

Card* get_card_on_obj(Game_object * obj)
{
    if (*obj == GAME_EMPTY)
    {
        return 0;
    }
    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        Card* c = &(game.cards[i]);
        if (c->position == obj)
        {
            return c;
        }
    }
    assert(false && "Unreachable");
}

Board_object get_board_card(Game_object * obj)
{
    Card* c = get_card_on_obj(obj);
        
    if (game.state == INIT || (c->visibility == HIDDEN && c->owner != game.turn)) {
        return BOARD_HIDDEN_CARD;
    }
    else if (c->type == LINK) {
        return BOARD_LINK_CARD;
    }
    else if (c->type == VIRUS) {
        return BOARD_VIRUS_CARD;
    }
    assert(false && "Unreachable");
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
    for (int i = 0; i < 3; i++) {
        game.cards[i] = (Card){PLAYER1, LINK, HIDDEN, &(game.board[7][i])};
    }
    game.cards[3] = (Card){PLAYER1, LINK, HIDDEN, &(game.board[6][3])};
    game.cards[4] = (Card){PLAYER1, VIRUS, HIDDEN, &(game.board[6][4])};
    for (int i = 0; i < 3; i++) {
        game.cards[i + 5] = (Card){PLAYER1, VIRUS, HIDDEN, &(game.board[7][i + 5])};
    }

    for (int i = 0; i < 3; i++) {
        game.cards[i + 8] = (Card){PLAYER2, LINK, HIDDEN, &(game.board[0][i])};
    }
    game.cards[11] = (Card){PLAYER2, LINK, HIDDEN, &(game.board[1][3])};
    game.cards[12] = (Card){PLAYER2, VIRUS, HIDDEN, &(game.board[1][4])};
    for (int i = 0; i < 3; i++) {
        game.cards[i + 13] = (Card){PLAYER2, VIRUS, HIDDEN, &(game.board[0][i + 5])};
    }
    game.picked_up_card = 0;
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
    game.state = SETUP;
    game.turn = PLAYER1;

}

Game_state get_game_state(void)
{
    return game.state;
}

Success move_card(Game_object* target, Game_object* from)
{
    assert(*target == GAME_EMPTY && get_card_on_obj(target) == 0);
    Card* c = get_card_on_obj(from);
    assert(c != 0);
    c->position = target;
    *from = GAME_EMPTY;
    *target = GAME_CARD;

    return VALID; // Will return valid for now. Error handle later
}

Success setup_player1(int i ,int j)
{
    assert(((i == 7) && ((0 <= j && j < 3) || (5 <= j && j < 8) )) || ((i == 6) && (j == 3 || j == 4)));
    Game_object* obj = &(game.board[i][j]);
    Card* hand = game.picked_up_card;
    if (*obj == GAME_CARD && hand == 0)
    {
        game.picked_up_card = get_card_on_obj(obj);
        *obj = GAME_EMPTY;
        return VALID;
    }
    else if (*obj == GAME_CARD && hand != 0)
    {
        move_card(hand->position, obj);
        *obj = GAME_CARD;
        hand->position = obj;
        game.picked_up_card = 0;
        return VALID;

    }
    assert(false && "Unreachable");
    return INVALID;
}

Success setup_player2(int i, int j)
{
    assert(false && "setup_player2 not implemented");
}

void setup(int i, int j)
{
    switch(game.turn)
    {
        case PLAYER1:
            setup_player1(i, j);
            break;
        case PLAYER2:
            setup_player2(i, j);
            break;
        default:
            assert(false && "Unreachable");
    }
}

Success interact_board(int i, int j)
{
    switch(game.state)
    {
        case INIT:
            return INVALID;
        case SETUP:
            setup(i, j);
            return VALID;
        default:
            assert(false && "Interacting in this game state not Implemented");
            return INVALID;
    }
}

Success next_phase(void)
{
    switch(game.state)
    {
        case INIT:
            start_game();
            return VALID;
        default:
            assert(false && "Switching to next phase not implemented");
    }
}