#define NORTH (1)
#define SOUTH (2)
#define EAST  (3)
#define WEST  (4)

typedef struct {
    int x, y;
    int facing;
    item_t inventory[5];
    light_t *light;
} player_t;

player_t new_player(void) {
    player_t p;

    memset(&p, 0, sizeof(p));

    p.light  = NULL;
    p.facing = SOUTH;

    return p;
}

void player_add_item(player_t *player, item_t item, int slot) {
    if (slot < 0 || slot > 5) {
        return;
    }

    player->inventory[slot] = item;
    if (item.kind == ITEM_LIGHT) {
        player->light = &(player->inventory[slot].light);
    }
}

void player_remove_item(player_t *player, int slot) {
    if (slot < 0 || slot > 5) {
        return;
    }

    player->inventory[slot].kind = ITEM_EMPTY;
}

int player_first_empty_slot(player_t *player) {
    int i;

    for (i = 0; i < 5; i += 1) {
        if (player->inventory[i].kind == ITEM_EMPTY) {
            return i;
        }
    }

    return -1;
}

int player_item_idx(player_t *player, int item_kind, int item_kind_kind) {
    int i;

    for (i = 0; i < 5; i += 1) {
        if (player->inventory[i].kind == item_kind) {
            switch (player->inventory[i].kind) {
                case ITEM_EMPTY: continue;
                case ITEM_LIGHT: {
                    if (player->inventory[i].light.kind == item_kind_kind) {
                        return i;
                    }
                    break;
                }
                case ITEM_FOOD: {
                    if (player->inventory[i].food.kind == item_kind_kind) {
                        return i;
                    }
                    break;
                }
                case ITEM_TOOL: {
                    if (player->inventory[i].tool.kind == item_kind_kind) {
                        return i;
                    }
                    break;
                }
                case ITEM_TREASURE: {
                    if (player->inventory[i].treasure.kind == item_kind_kind) {
                        return i;
                    }
                    break;
                }
            }
        }
    }

    return -1;
}

char player_looking_at(player_t *player, level_t *level) {
    int y, x;

    y = player->y;
    x = player->x;

    switch (player->facing) {
        case NORTH: y -= 1; break;
        case SOUTH: y += 1; break;
        case EAST:  x += 1; break;
        case WEST:  x -= 1; break;
    }

    return level_get_at(level, y, x);
}

int player_can_see_with_flashlight(player_t *player, int y, int x) {
    switch (player->facing) {
        case NORTH: {
            if (x != player->x) {
                return 0;
            }
            if (y > player->y
            ||  y < player->y - player->light->strength) {
                return 0;
            }
            break;
        }
        case SOUTH: {
            if (x != player->x) {
                return 0;
            }
            if (y <= player->y
            ||  y > player->y + player->light->strength) {
                return 0;
            }
            break;
        }
        case EAST: {
            if (y != player->y) {
                return 0;
            }
            if (x <= player->x
            ||  x > player->x + player->light->strength) {
                return 0;
            }
            break;
        }
        case WEST: {
            if (y != player->y) {
                return 0;
            }
            if (x > player->x
            ||  x < player->x - player->light->strength) {
                return 0;
            }
            break;
        }
    }
    return 1;
}

int player_can_see_with_lantern(player_t *player, int y, int x) {
    if (x > player->x + player->light->strength
    ||  x < player->x - player->light->strength
    ||  y > player->y + player->light->strength
    ||  y < player->y - player->light->strength) {
        return 0;
    }

    return 1;
}

int player_can_see(player_t *player, level_t *level, int y, int x) {
    if (level->is_overworld) {
        return 1;
    }

    if (!player->light) {
        return 0;
    }

    switch (player->light->kind) {
        case LIGHT_FLASHLIGHT:
            return player_can_see_with_flashlight(player, y, x);
        case LIGHT_LANTERN:
            return player_can_see_with_lantern(player, y, x);
        default:
            return 0;
    }
}
