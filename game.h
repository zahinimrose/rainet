#ifndef GAME_H
#define GAME_H

#define HEIGHT 8
#define WIDTH 8

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
    CARD_LINK,
    CARD_VIRUS,
    CORE
} Game_object;

void init_game(void);
Game_object get_board_object(int i, int j);

#endif //GAME_H