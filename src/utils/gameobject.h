#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <genesis.h>
#include "resources.h"
#include "sprite_eng.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

// Defines a rectangular bounding box for collision or positioning
typedef struct
{
    s16 left;   // Left boundary (X)
    s16 right;  // Right boundary (X + width)
    s16 top;    // Top boundary (Y)
    s16 bottom; // Bottom boundary (Y + height)
} BoundBox;

// Represents a general game object with position, velocity, sprite and bounding box
typedef struct
{
    Sprite *sprite; // Pointer to the object's sprite
    f16 x;          // Current X position (fixed-point)
    f16 y;          // Current Y position (fixed-point)
    f16 next_x;     // Next X position (used for logic before rendering)
    f16 next_y;     // Next Y position
    f16 speed_x;    // Horizontal movement speed
    f16 speed_y;    // Vertical movement speed
    u8 w;           // Width in pixels
    u8 h;           // Height in pixels
    BoundBox box;   // Bounding box for collision detection
    u8 anim;        // Current animation frame/state
    u8 depth;       // Rendering depth or draw order
} GameObject;

/**
 * Initializes a GameObject instance.
 *
 * @param obj     Pointer to the GameObject to initialize
 * @param sprite  Pointer to the sprite definition
 * @param x       Initial X position (in pixels)
 * @param y       Initial Y position
 * @param pal     Palette index (0–3)
 * @param prio    Sprite priority (0–3, lower draws on top)
 * @param ind     VRAM index for sprite tiles
 * @return        VRAM index offset after sprite allocation
 */
u16 GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *const sprite, s16 x, s16 y, u8 pal, u8 prio, u16 ind);

/**
 * Updates the bounding box of a GameObject based on its current position.
 *
 * @param x    X position of the object
 * @param y    Y position of the object
 * @param obj  Pointer to the GameObject to update
 */
void GAMEOBJECT_update_boundbox(f16 x, f16 y, GameObject *obj);

#endif
