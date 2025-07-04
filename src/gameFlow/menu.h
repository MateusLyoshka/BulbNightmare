#ifndef _MENU
#define _MENU

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../utils/gameobject.h"
#include "../utils/utils.h"
#include "../screenElements/background.h"

/**
 * Initializes the main menu.
 *
 * @param ind Starting VRAM tile index for sprites and background
 * @return Next available VRAM tile index
 */
u16 MENU_init(u16 ind);

/**
 * Updates the main menu (input, selection)
 *
 * @param ind Current VRAM tile index for potential new sprites
 * @return Next available VRAM tile index
 */
u16 MENU_update(u16 ind);

#endif // _MENU
