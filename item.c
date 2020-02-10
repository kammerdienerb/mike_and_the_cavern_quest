#define ITEM_EMPTY    (0)
#define ITEM_LIGHT    (1)
#define ITEM_FOOD     (2)
#define ITEM_TOOL     (3)
#define ITEM_TREASURE (4)

#define LIGHT_FLASHLIGHT (1)
#define LIGHT_LANTERN    (2)

typedef struct {
    int kind;
    int strength;
} light_t;

#define FOOD_SNAKE_SNACK (1)
#define FOOD_PIZZA       (2)

typedef struct {
    int kind;
} food_t;

#define TOOL_ROPE    (1)
#define TOOL_SHOVEL  (2)
#define TOOL_SNORKEL (3)

typedef struct {
    int kind;
    int rope_length;
} tool_t;

#define TREASURE_TRUE_LOVE (1)
#define TREASURE_GOLD      (2)
#define TREASURE_RUBY      (3)
#define TREASURE_DIAMOND   (4)
#define TREASURE_FOSSIL    (5)

typedef struct {
    int kind;
    int amount;
} treasure_t;

typedef struct {
    int kind;

    union {
        light_t    light;
        food_t     food;
        tool_t     tool;
        treasure_t treasure;
    };
} item_t;
