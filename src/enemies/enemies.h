#ifndef _G_ENEMMIES
#define _G_ENEMMIES

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../gameLevels/gameobject.h"

#define MAX_ENEMIES 20
#define FLYING 10
#define GROUND 10
#define LEVEL_1 2
#define LEVEL_2 3
#define LEVEL_3 4
#define LEVEL_4 5
#define LEVEL_5 6

typedef struct
{
    GameObject firefly;
    u8 type; // ground:0 flying:1
    u8 level_id;
    u16 last_x;
    u16 last_y;
} Enemy;

extern Enemy enemy_pool[MAX_ENEMIES];

u8 ENEMIES_init(u16 ind);

#endif