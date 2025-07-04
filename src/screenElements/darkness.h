#ifndef _DARKNESS
#define _DARKNESS

#include <genesis.h>          // Main SGDK header.
#include "background.h"       // Background management functions.
#include "../player/player.h" // Player-related definitions and functionality.

extern u8 room_lights[MAP_TOTAL_SCREENS]; // Light status for each screen (0 = dark, 1 = lit).
extern u16 dark_ind;                      // Starting VRAM tile index for the darkness mask.

/**
 * @brief Initializes scroll parameters for the darkness mask.
 */
void MASK_scroll_init();

/**
 * @brief Draws the darkness mask over the screen.
 */
void MASK_draw();

/**
 * @brief Updates the scroll of the darkness mask (e.g., based on player position).
 */
void MASK_scroll_update();

#endif // _DARKNESS
