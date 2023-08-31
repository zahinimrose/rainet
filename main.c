#include <stdio.h>
#include <stdbool.h>

#include "trender.h"
#include "game.h"
#include "control.h"

int main(void)
{
    init_game();
    bool running = true;

    while(running) {
        new_frame();
        input();
    }

    return 0;
}