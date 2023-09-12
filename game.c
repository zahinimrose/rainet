#include <assert.h>
#include <stdbool.h>

#include "game.h"

typedef struct
{
    Player owner;
    enum {LINK, VIRUS} type;
    enum {HIDDEN, REVEALED} visibility; // Visibility to all player
} Card;

typedef struct
{
    Card* card;
    bool is_port;
} Board_slot;

typedef struct
{
    Card* card[TOTAL_CARDS]; // TODO: check if maximum number of card slot is needed
} Stack;


typedef struct
{
    Game_state state;
    Player turn;
    Board_slot board[BOARD_HEIGHT][BOARD_WIDTH];
    Stack stacks[PLAYER_COUNT];
    Card cards[TOTAL_CARDS];
    Card* picked_up_card; //TODO: Check if this is needed
} Game;

static Game game; //Global game state

void board_empty(void)
{
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            game.board[i][j].card = 0;
            game.board[i][j].is_port = false;
        }
    }
}

void place_port(void)
{
    game.board[0][3].is_port = true;
    game.board[0][4].is_port = true;
    game.board[7][3].is_port = true;
    game.board[7][4].is_port = true;

}

Board_object get_board_obj_from_card(Card* c)
{
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
    Board_slot* slot = &(game.board[i][j]);

    if (slot->card == 0) {
        return slot->is_port ? BOARD_PORT : BOARD_BLANK;
    }
    else {
        return get_board_obj_from_card(slot->card);
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
    game.picked_up_card = 0;
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
    assert(c != 0 && c->owner == game.turn);
    c->position = target;
    *from = GAME_EMPTY;
    *target = GAME_CARD;

    return VALID; // Will return valid for now. Error handle later
}

Success setup(int i, int j)
{
    Game_object* obj = &(game.board[i][j]);
    assert(*obj == GAME_CARD);
    Card* hand = game.picked_up_card;
    if (hand == 0)
    {
        game.picked_up_card = get_card_on_obj(obj);
        assert(game.picked_up_card->owner == game.turn);
        *obj = GAME_EMPTY;
        return VALID;
    }
    else if (hand != 0)
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

Success play(int i, int j)
{
    Game_object* obj = &(game.board[i][j]);
    assert(*obj == GAME_CARD || *obj == GAME_EMPTY);
    Card* hand = game.picked_up_card;
    if (*obj == GAME_CARD && hand == 0)
    {
        game.picked_up_card = get_card_on_obj(obj);
        assert(game.picked_up_card->owner == game.turn);
        *obj = GAME_EMPTY;
        return VALID;
    }
    assert(obj == (hand->position) + 1 ||
           obj == (hand->position) - 1 ||
           obj == (hand->position) + BOARD_WIDTH ||
           obj == (hand->position) - BOARD_WIDTH);
    assert(hand != 0);
    if(*obj == GAME_EMPTY && hand != 0)
    {
        *obj = GAME_CARD;
        hand->position = obj;
        game.picked_up_card = 0;
        game.turn = game.turn == PLAYER1 ? PLAYER2 : PLAYER1;
        return VALID;
    }
    if (*obj == GAME_CARD && hand != 0)
    {
        Card* captured = get_card_on_obj(obj);
        assert(captured->owner != game.turn);
        captured->visibility = REVEALED;
        captured->position = &(game.stacks[game.turn]);
        *(hand->position) = GAME_EMPTY;
        hand->position = obj;
        game.picked_up_card = 0;
        game.turn = game.turn == PLAYER1 ? PLAYER2 : PLAYER1;
        return VALID;
    }
    
    assert(false && "Unreachable");
    return INVALID;
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
        case PLAY:
            play(i, j);
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
            game.state = SETUP;
            game.turn = PLAYER1;
            return VALID;
        case SETUP:
            assert(game.picked_up_card == 0);
            if (game.turn == PLAYER1) {
                game.turn = PLAYER2;

                return VALID;
            }
            else if (game.turn == PLAYER2) {
                game.state = PLAY;
                game.turn = PLAYER1;

                return VALID;
            }
            assert(false && "Unreachable");
        case PLAY:
            assert(false && "Next_phase of PLAY not Implemented");
        default:
            assert(false && "Switching to next phase not implemented");
    }
}

Player get_turn(void)
{
    return game.turn;
}

void get_stack(Board_object* buf, int num, Player player)
{
    assert(num >= 8);
    Game_object* obj;
    switch(player)
    {
        case PLAYER1:
            obj = &(game.stacks[0]);
            break;
        case PLAYER2:
            obj = &(game.stacks[1]);
            break;
    }
    int i = 0;
    int j = 0;
    while(j < TOTAL_CARDS) {
        Card* c = &(game.cards[j]);
        if (c->position == obj) {
            buf[i] = get_board_obj_from_card(c);
            i++;
        }
        j++;
    }
    while (i < num) {
        buf[i] = BOARD_BLANK;
        i++;
    }
}