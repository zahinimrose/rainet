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
    int ptr;
} Stack;


typedef struct
{
    Game_state state;
    Player turn;
    Board_slot board[BOARD_HEIGHT][BOARD_WIDTH];
    Stack stacks[PLAYER_COUNT];
    Card cards[TOTAL_CARDS];
    Card* picked_up_card; //TODO: Check if this is needed
    Board_slot* picked_from;
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

void place_card(int i, int j, Card* card) //TODO: Explore card by id
{
    game.board[i][j].card = card;
}

void set_up_cards()
{
        place_card(7, 0, &(game.cards[0]));
        place_card(7, 1, &(game.cards[1]));
        place_card(7, 2, &(game.cards[2]));
        place_card(6, 3, &(game.cards[3]));

        place_card(6, 4, &(game.cards[4]));
        place_card(7, 5, &(game.cards[5]));
        place_card(7, 6, &(game.cards[6]));
        place_card(7, 7, &(game.cards[7]));

        place_card(0, 0, &(game.cards[8]));
        place_card(0, 1, &(game.cards[9]));
        place_card(0, 2, &(game.cards[10]));
        place_card(1, 3, &(game.cards[11]));

        place_card(1, 4, &(game.cards[12]));
        place_card(0, 5, &(game.cards[13]));
        place_card(0, 6, &(game.cards[14]));
        place_card(0, 7, &(game.cards[15]));
}

void init_cards(void)
{
    for (int i = 0; i < 4; i++) {
        game.cards[i] = (Card){PLAYER1, LINK, HIDDEN};
    }
    game.cards[3] = (Card){PLAYER1, LINK, HIDDEN};

    for (int i = 0; i < 4; i++) {
        game.cards[i + 4] = (Card){PLAYER1, VIRUS, HIDDEN};
    }

    for (int i = 0; i < 4; i++) {
        game.cards[i + 8] = (Card){PLAYER2, LINK, HIDDEN};
    }
    for (int i = 0; i < 4; i++) {
        game.cards[i + 12] = (Card){PLAYER2, VIRUS, HIDDEN};
    }
    game.picked_up_card = 0;
    set_up_cards();
}

void init_game(void)
{
    game.state = INIT;
    board_empty();
    place_port();
    init_cards();
    game.picked_up_card = 0;
    game.picked_from = 0;
    game.turn = PLAYER1;
}

Game_state get_game_state(void)
{
    return game.state;
}

Success move_card(Board_slot* target, Board_slot* from)
{
    assert(target->card == 0);
    Card* c = from->card;
    assert(c != 0 && c->owner == game.turn);
    target->card = c;
    from->card = 0;
    return VALID; // Will return valid for now. Error handle later
}

Success setup(int i, int j)
{
    Board_slot* slot = &(game.board[i][j]);
    Card* hand = game.picked_up_card;
    if (hand == 0 && game.picked_from == 0)
    {
        game.picked_up_card = slot->card;
        game.picked_from = slot;
        assert(game.picked_up_card->owner == game.turn);
        slot->card = 0;
        return VALID;
    }
    else if (hand != 0 && game.picked_from != 0)
    {
        move_card(game.picked_from, slot);
        slot->card = hand;
        game.picked_up_card = 0;
        game.picked_from = 0;
        return VALID;
    }
    assert(false && "Unreachable");
    return INVALID;
}

Success play(int i, int j)
{
    Board_slot* slot= &(game.board[i][j]);
    Card* hand = game.picked_up_card;
    if (slot->card != 0 && hand == 0 && game.picked_from == 0)
    {
        game.picked_up_card = slot->card;
        game.picked_from = slot;
        assert(game.picked_up_card->owner == game.turn);
        slot->card = 0;
        return VALID;
    }
    assert(slot== (game.picked_from) + 1 ||
           slot== (game.picked_from) - 1 ||
           slot== (game.picked_from) + BOARD_WIDTH ||
           slot== (game.picked_from) - BOARD_WIDTH);
    assert(hand != 0);
    if(slot->card == 0 && hand != 0 && game.picked_from !=0)
    {
        slot->card = hand;
        game.picked_from = 0;
        game.picked_up_card = 0;

        game.turn = game.turn == PLAYER1 ? PLAYER2 : PLAYER1;
        return VALID;
    }
    if (slot->card != 0 && hand != 0 && game.picked_from != 0)
    {
        Card* captured = slot->card;
        assert(captured->owner != game.turn);
        captured->visibility = REVEALED;

        Stack* st = &(game.stacks[game.turn]);
        st->card[st->ptr] = captured;
        (st->ptr)++;

        slot->card = hand;
        game.picked_up_card = 0;
        game.picked_from = 0;
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
            if(claim_victory(game.turn) == VALID)
            {
                game.state = GAME_OVER;
            }
            return VALID;
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
    
    Stack* st = &(game.stacks[player]);

    int i = 0;


    while(i < st->ptr) { //TODO: Check if its enough
        
        buf[i] = get_board_obj_from_card(st->card[i]);
        i++;
        
    }
    while (i < num) {
        buf[i] = BOARD_BLANK;
        i++;
    }
}

Success claim_victory(Player player)
{
    assert(player == game.turn);
    Stack player_stack = game.stacks[player];
    
    int link_card_count = 0;
    for (int i = 0; i < player_stack.ptr; i++)
    {
        if (player_stack.card[i]->type == LINK) {
            link_card_count++;
        }
    }
    if (link_card_count > 3)
    {
        return VALID;
    }

    Player opponent = player == PLAYER1 ? PLAYER2 : PLAYER1;
    Stack opponent_stack = game.stacks[opponent];
    int virus_card_count = 0;

    for (int i = 0; i < opponent_stack.ptr; i++)
    {
        if (opponent_stack.card[i]->type == VIRUS && opponent_stack.card[i]->visibility == REVEALED) {
            virus_card_count++;
        }
    }
    if (virus_card_count > 3)
    {
        return VALID;
    }

    return INVALID;

}

Player get_winner()
{
    assert(game.state == GAME_OVER);
    return game.turn;
}