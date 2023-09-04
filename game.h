#ifndef GAME_H
#define GAME_H

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
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



void init_game(void);
void start_game(void);
Game_object get_board_object(int i, int j);
Game_state get_state(void);
Success interact_board(int i, int j);

#endif //GAME_H