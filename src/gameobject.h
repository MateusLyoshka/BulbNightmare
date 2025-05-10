#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <genesis.h>
#include "sprite_eng.h"
#include "globals.h"

typedef struct
{
    s16 left;
    s16 right;
    s16 top;
    s16 bottom;
} BoundBox;

typedef struct
{
    Sprite *sprite;
    f16 x;
    f16 y;
    f16 next_x;
    f16 next_y;
    f16 speed_x;
    f16 speed_y;
    u8 w;
    u8 h;
    BoundBox box;
    u8 anim;
} GameObject;

u16 GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *const sprite, s16 x, s16 y, u8 pal, u16 ind);
void GAMEOBJECT_update_boundbox(f16 x, f16 y, GameObject *obj);

#endif