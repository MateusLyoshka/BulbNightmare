#ifndef _HUD_H
#define _HUD_H

#include <genesis.h>
#include "resources.h"
#include "../utils/gameobject.h"
#include "../utils/globals.h"
#include "../player/player.h"
#include "objects.h"

extern u8 hud_initiated;

/**
 * @brief Draws the HUD background layer.
 *
 * @param ind Starting VRAM tile index.
 * @return Next available VRAM tile index after drawing.
 */
u16 HUD_background(u16 ind);

/**
 * @brief Initializes HUD elements such as icons and numbers.
 *
 * @param ind Starting VRAM tile index for sprite allocation.
 * @return Next available VRAM tile index after initialization.
 */
u16 HUD_init(u16 ind);

/**
 * @brief Updates the HUD with current game state (e.g., lives, keys, gems).
 */
void HUD_update();

/**
 * @brief Clears HUD elements from the screen and VRAM.
 */
void HUD_clear();

#endif // _HUD_H
