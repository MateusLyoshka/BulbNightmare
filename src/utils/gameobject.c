#include "gameobject.h"

u16 GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *const sprite, s16 x, s16 y, u8 pal, u8 prio, u16 ind)
{
    obj->x = FIX16(x);
    obj->y = FIX16(y);
    obj->next_x = obj->x;
    obj->next_y = obj->y;
    obj->speed_x = 0;
    obj->speed_y = 0;
    obj->anim = 0;

    if (sprite != &spr_boss && sprite != &spr_face && sprite != &spr_dialog)
    {
        PAL_setPalette(pal, sprite->palette->data, DMA);
    }

    obj->sprite = SPR_addSprite(sprite, x, y, TILE_ATTR_FULL(pal, prio, FALSE, 0, ind));
    obj->w = obj->sprite->definition->w;
    obj->h = obj->sprite->definition->h;
    SPR_setDepth(obj->sprite, 0);

    return obj->sprite->definition->maxNumTile;
}

void GAMEOBJECT_update_boundbox(f16 x, f16 y, GameObject *obj)
{
    obj->box.left = fix16ToInt(x);
    obj->box.top = fix16ToInt(y);
    obj->box.right = fix16ToInt(x) + obj->w;  // - 1;
    obj->box.bottom = fix16ToInt(y) + obj->h; // - 1;
}