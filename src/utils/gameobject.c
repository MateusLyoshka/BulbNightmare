#include "gameobject.h"

// Initializes a GameObject: sets its position, velocity, sprite and other attributes
u16 GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *const sprite, s16 x, s16 y, u8 pal, u8 prio, u16 ind)
{
    // Set initial fixed-point position
    obj->x = FIX16(x);
    obj->y = FIX16(y);

    // Prepare next position (used for logic updates)
    obj->next_x = obj->x;
    obj->next_y = obj->y;

    // Zero movement speed and animation state
    obj->speed_x = 0;
    obj->speed_y = 0;
    obj->anim = 0;

    // Add sprite with appropriate tile attributes
    obj->sprite = SPR_addSprite(sprite, x, y, TILE_ATTR_FULL(pal, prio, FALSE, 0, ind));

    // Store sprite dimensions
    obj->w = obj->sprite->definition->w;
    obj->h = obj->sprite->definition->h;

    // Set sprite depth (rendering order)
    SPR_setDepth(obj->sprite, 0);

    // Return the number of tiles used by the sprite
    return obj->sprite->definition->maxNumTile;
}

// Updates the bounding box based on the current fixed-point position
void GAMEOBJECT_update_boundbox(f16 x, f16 y, GameObject *obj)
{
    obj->box.left = fix16ToInt(x);
    obj->box.top = fix16ToInt(y);
    obj->box.right = fix16ToInt(x) + obj->w;
    obj->box.bottom = fix16ToInt(y) + obj->h;
}
