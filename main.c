#include <stdio.h>
#include <stdbool.h>

#include "trender.h"
#include "game.h"
#include "control.h"

int main(void)
{
    init_game();
    set_cursor(0, 0);
    bool running = true;

    while(running) {
        new_frame();
        while(!input()) {}
    }

    return 0;
}