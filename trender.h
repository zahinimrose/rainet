#ifndef TRENDER_H
#define TRENDER_H

#define FRAME_HEIGHT 15
#define FRAME_WIDTH  20

typedef enum
{
    SEL_EMPTY,
    SEL_BOARD,
    SEL_STACK_PLAYER1,
    SEL_STACK_PLAYER2
    
} Selected;

void init_display();
void new_frame(void);
void init_cursor();
void set_cursor(int i, int j);
int get_cursor_i();
int get_cursor_j();
Selected get_selected(void);\

#endif //TRENDER_H