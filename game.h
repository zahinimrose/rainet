#ifndef GAME_H
#define GAME_H

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
#define PLAYER_COUNT 2
#define CARDS_PER_PLAYER 8
#define CARD_TYPE_COUNT 4
#define TOTAL_CARDS (PLAYER_COUNT * CARDS_PER_PLAYER)

typedef bool Success; //TODO: Determine if success variable is needed
#define VALID 1
#define INVALID 0

typedef enum
{
    PLAYER1 = 0,
    PLAYER2 = 1,
    PLAYER_NONE
} Player;

typedef enum
{
    INIT,
    SETUP,
    PLAY,
    GAME_OVER
} Game_state;

typedef enum
{
    BOARD_BLANK,
    BOARD_PORT,
    BOARD_HIDDEN_CARD,
    BOARD_LINK_CARD,
    BOARD_VIRUS_CARD
} Board_object;

void init_game(void);
Board_object get_board_object(int i, int j);
Game_state get_game_state(void);
Player get_turn(void);
Success interact_board(int i, int j);
Success next_phase(void);
void get_stack(Board_object* buf, int num, Player player);
Player get_winner();

#endif //GAME_H