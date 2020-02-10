#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>

#include "level.c"
#include "item.c"
#include "player.c"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char **argv) {
    int       i, j, new_x, new_y, dir, key, item_slot;
    char      c, new_c;
    player_t  mike;
    item_t    light, snake_snack;
    level_t   level;
    char     *looking_at, *message;

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    mike   = new_player();
    mike.x = 0;
    mike.y = 9;

    light.kind           = ITEM_LIGHT;
    light.light.kind     = LIGHT_LANTERN;
    light.light.strength = 1;

    player_add_item(&mike, light, 0);

/*     level_make(&level, map1, 10, 10); */
/*     level_make(&level, overworld, 36, 40); */
    level_make(&level, map2, 17, 40);

    looking_at = "";
    message    = "";

    refresh();

    while (1) {
        move(0, 0);

        /* Draw the level and Mike. */
        for (i = 0; i < level.num_rows; i += 1) {
            for (j = 0; j < level.num_cols; j += 1) {
                if (i == mike.y && j == mike.x) {
                    switch (mike.facing) {
                        case NORTH: printw("♀"); break;
                        case SOUTH: printw("♀"); break;
                        case EAST:  printw("♀"); break;
                        case WEST:  printw("♀"); break;
/*                         case NORTH: printw("^"); break; */
/*                         case SOUTH: printw("v"); break; */
/*                         case EAST:  printw(">"); break; */
/*                         case WEST:  printw("<"); break; */
                    }
                } else {
                    if (player_can_see(&mike, &level, i, j)) {
                        printw("%c", level_get_at(&level, i, j));
                    } else {
                        printw(" ");
                    }
                }
            }
            printw("\n");
        }
        printw("%s\n", message);
        printw("%s\n", looking_at);
        message = "";


        /* Get the next thing to do. */
        key = getch();
        if (key == 'q') { break; }

        new_x = mike.x;
        new_y = mike.y;
        switch (key) {
            case KEY_DOWN: new_y += 1;
                      mike.facing    = SOUTH;
                      break;
            case KEY_UP: new_y -= 1;
                      mike.facing    = NORTH;
                      break;
            case KEY_RIGHT: new_x += 1;
                      mike.facing    = EAST;
                      break;
            case KEY_LEFT: new_x -= 1;
                      mike.facing    = WEST;
                      break;
        }

        new_c = level_get_at(&level, new_y, new_x);
        if (new_c != '#' && new_c != -1) {
            mike.y = new_y;
            mike.x = new_x;
        }

        switch (level_get_at(&level, new_y, new_x)) {
            case 'S': {
                if ((item_slot = player_item_idx(&mike, ITEM_FOOD, FOOD_SNAKE_SNACK)) != -1) {
                    level_clear_space(&level, new_y, new_x);
                    player_remove_item(&mike, item_slot);
                    message = "You fed the snake a snake snack.";
                } else {
                    printw("A snake killed you.\n");
                    goto out;
                }
                break;
            }
            case 's': {
                snake_snack.kind = ITEM_FOOD;
                snake_snack.food.kind = FOOD_SNAKE_SNACK;
                player_add_item(&mike, snake_snack, player_first_empty_slot(&mike));
                level_clear_space(&level, new_y, new_x);
                message = "You took the snake snack.";
                break;
            }
        }

        switch (player_looking_at(&mike, &level)) {
            case 'S':
                looking_at = "A snaaake!!!";
                break;
            case 's':
                looking_at = "Hey look, a snake snack!";
                break;
            case ' ':
                looking_at = "A clear path";
                break;
            case '#':
                looking_at = "A wall";
                break;
        }

        refresh();
    }

out:
    endwin();
    return 0;
}
