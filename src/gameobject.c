#include "gameobject.h"
#include "utils.h"

u16 GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *const sprite, s16 x, s16 y, u8 pal, u16 ind)
{
    obj->x = FIX16(x);
    obj->y = FIX16(y);
    obj->next_x = obj->x;
    obj->next_y = obj->y;
    obj->speed_x = 0;
    obj->speed_y = 0;
    obj->gravity = FIX16(2);
    obj->anim = 0;
    PAL_setPalette(pal, sprite->palette->data, DMA);

    obj->sprite = SPR_addSprite(sprite, x, y, TILE_ATTR_FULL(pal, FALSE, FALSE, 0, ind));
    obj->w = obj->sprite->definition->w;
    obj->h = obj->sprite->definition->h;

    return obj->sprite->definition->maxNumTile;
}