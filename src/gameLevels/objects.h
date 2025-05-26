#ifndef OBJECTS
#define OBJECTS

#include <genesis.h>
#include "../utils/globals.h"
#include "map.h"
#include "gameobject.h"
#include "resources.h"
#include "level.h"

typedef struct
{
    u8 level;
    u8 screen;
    u16 x;
    u16 y;
    u8 flip; // 0 = nenhum, 1 = horizontal, 2 = vertical
} ObjectConfig;

u16 OBJECT_door_spawn(u16 ind);
u16 OBJECT_key_spawn(u16 ind);
u16 OBJECT_powerup_spawn(u16 ind);
u16 OBJECT_update(u16 ind);
void OBJECT_clear(GameObject object);
u8 OBJECTS_player_collision(s16 player_x, s16 player_y);

#endif