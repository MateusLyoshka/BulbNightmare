#ifndef _PAUSE
#define _PAUSE

#include <genesis.h>
#include "../screenElements/background.h"
#include "../utils/gameobject.h"

// Controls progression from pause menu
extern u8 pause_proceed;

/**
 * Initializes the pause menu.
 *
 * @param ind Starting VRAM tile index for sprites and background
 * @return Next available VRAM tile index
 */
u16 PAUSE_init(u16 ind);

/**
 * Updates the pause menu state (input, selection)
 */
void PAUSE_update();

/**
 * Clears resources used by the pause menu
 */
void PAUSE_clear();

#endif // _PAUSE
