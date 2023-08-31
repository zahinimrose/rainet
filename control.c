#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "game.h"

char take_input(void)
{
    char buf[2];
    printf("Input:");
    fgets(buf, sizeof(buf), stdin);

    return buf[0];
}

int input(void)
{
    char ch = take_input();

    switch (ch) {
        case 'q':
            printf("Exiting Game\n");
            
            exit(0);
        default:
            printf("Input %c is not recognized\n", ch);
            return 0;
    }
}
