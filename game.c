#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "level.c"
#include "player.c"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char **argv) {
    int      i, j, new_x, new_y;
    char     c;
    player_t mike;
    level_t  level;

    mike.x = 2;
    mike.y = 0;

    level.map      = map1;
    level.num_rows = 8;
    level.num_cols = 10;

    system("clear");

    while (1) {
        /* Draw the level and Mike. */
        for (i = 0; i < level.num_rows; i += 1) {
            for (j = 0; j < level.num_cols; j += 1) {
                if (i == mike.y && j == mike.x) {
                    printf("^");
                } else {
                    printf("%c", level_get_at(&level, i, j));
                }
            }
            printf("\n");
        }

        /* Get the next thing to do. */
        c = getchar();
        if (c == 'q') { break; }
        getchar(); /* Get the newline character. Ignore it. */

        new_x = mike.x;
        new_y = mike.y;
        switch (c) {
            case 's': new_y += 1; break;
            case 'n': new_y -= 1; break;
            case 'e': new_x += 1; break;
            case 'w': new_x -= 1; break;
        }

        switch (level_get_at(&level, new_y, new_x)) {
            case ' ': {
                mike.y = new_y;
                mike.x = new_x;
                break;
            }
            case 'S': {
                printf("A snake!!!\n");
                break;
            }
        }

        system("clear");
    }

    return 0;
}
