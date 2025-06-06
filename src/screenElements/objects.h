#ifndef OBJECTS
#define OBJECTS

#include <genesis.h>
#include "resources.h"
#include "map.h"
#include "../utils/gameobject.h"
#include "level.h"
#include "../utils/globals.h"

extern GameObject door;
extern GameObject key;
extern GameObject light_switch;

typedef struct
{
    u8 level;
    u8 screen;
    u16 x;
    u16 y;
    u8 flip;
    u8 collected;
} ObjectConfig;

typedef struct
{
    GameObject *obj;
    const SpriteDefinition *sprite;
    ObjectConfig *configs;
    u8 config_count;
    u8 *on_screen_flag;
} ObjectType;

// Adiciona esta função ao header
GameObject *OBJECT_check_collision(u16 player_center_x, u16 player_center_y);
u16 OBJECT_spawn_type(const ObjectType *type, u16 ind, u8 force_respawn);
u16 OBJECT_update(u16 ind, u8 force_respawn);
void OBJECT_collect(GameObject *object);
void OBJECT_clear(GameObject *object);

#endif