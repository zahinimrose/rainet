#include <stdio.h>
#include <stdbool.h>

#include "trender.h"
#include "game.h"
#include "control.h"

int main(void)
{
    init_game();
    init_display();
    init_cursor();
    bool running = true;

    while(running) {
        new_frame();
        while(!input()) {}
        
    }

    return 0;
}