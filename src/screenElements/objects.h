#ifndef OBJECTS
#define OBJECTS

#include <genesis.h>
#include "resources.h"
#include "map.h"
#include "../utils/gameobject.h"
#include "level.h"
#include "../utils/globals.h"

#define MAX_OBJECTS 50

extern GameObject door;
extern GameObject key;
extern GameObject light_switch;

extern u8 keys_on_level[5];
extern u8 switchs_on_level[5];
extern u8 objects_initiated;

typedef struct
{
    GameObject obj;
    u8 type; // 0 door, 1 switch, 2 key, 3 spark
    u16 x;
    u16 y;
    u8 collected;
    u8 level;
    u8 screen;
    u8 on_screen;
    u8 prio;
} ObjectConfig;

extern ObjectConfig objects_config[MAX_OBJECTS];

ObjectConfig *OBJECT_check_collision(u16 player_center_x, u16 player_center_y);
void OBJECT_params();
void OBJECT_init(u8 i, u8 type, u16 x, u16 y, u8 level, u8 screen, u8 prio);
u16 OBJECT_spawn(u8 i, u16 ind);
u16 OBJECT_update(u16 ind);
void OBJECT_clear(ObjectConfig *config, u8 collect);

#endif