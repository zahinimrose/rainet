#ifndef GAME_H
#define GAME_H

#define HEIGHT 8
#define WIDTH 8
#define PLAYER_COUNT 2
#define CARDS_PER_PLAYER 8

typedef bool Success;
#define VALID 1
#define INVALID 0

typedef enum
{
    INIT,
    SETUP_PLAYER1,
    SETUP_PLAYER2,
    TURN_PLAYER1,
    TURN_PLAYER2,
    GAME_OVER
} Game_state;

typedef enum
{
    EMPTY,
    PORT,
    CARD,
    STACK_PLAYER1,
    STACK_PLAYER2
} Game_object;

typedef struct
{
    enum {PLAYER1, PLAYER2} owner;
    enum {LINK, VIRUS} type;
    enum {HIDDEN, REVEALED} visibility;
    Game_object position;
} Card;

void init_game(void);
void start_game(void);
Game_object get_board_object(int i, int j);
Game_state get_state(void);
Success place_card(Game_object obj, Card card);

#endif //GAME_H