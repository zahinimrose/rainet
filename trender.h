#define FRAME_HEIGHT 15
#define FRAME_WIDTH  20

void init_display();
void new_frame(void);
void init_cursor();
void set_cursor(int i, int j);
int get_cursor_i();
int get_cursor_j();