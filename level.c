const char *map1[] = {
"## #######",
"## #######",
"## #######",
"## #######",
"##   #####",
"#### #####",
"####    S#",
"#### #####",
"#### #####",
"#### #####",
};

typedef struct {
    const char **map;
    int num_rows, num_cols;
} level_t;

char level_get_at(level_t *level, int y, int x) {
    if (y < 0 || y >= level->num_rows) {
        return -1;
    }
    if (x < 0 || x >= level->num_cols) {
        return -1;
    }
    return level->map[y][x];
}
