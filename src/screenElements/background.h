#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>          // Main SGDK header.
#include "resources.h"        // Game resources like images and sprites.
#include "../utils/globals.h" // Global variables used across the project.
#include "../utils/utils.h"   // General utility functions.

/**
 * @brief Enum for background types used in the game.
 */
typedef enum
{
    ALERT_1,
    ALERT_2,
    ALERT_3,
    ALERT_4,
    ALERT_5,
    ALERT_6,
    BG_LOGO,
    BG_MENU,
    BG_INSTRUCT1,
    BG_PAUSE,
    BG_BOSS,
    BG_DIALOG_1,
    BG_DIALOG_2,
    BG_DIALOG_3,
    FINAL_SCREEN,
    BG_INSTRUCT2,
    BG_CREDITS,
    BG_MAX // Total number of background types.
} BackgroundType;

extern u8 bg_proceed; // Controls background progression (e.g., in cutscenes or menus).

/**
 * @brief Initializes and draws a background image on the specified plane.
 *
 * @param type Background image type from the BackgroundType enum.
 * @param bg Background plane (0 = BG_B, 1 = BG_A).
 * @param pal Palette index to use.
 * @param set_fade Whether to apply a fade-in effect (1 = yes, 0 = no).
 * @param prio Priority of the background plane (1 = high, 0 = low).
 * @param ind Starting VRAM tile index.
 * @return Next available VRAM tile index after loading the background.
 */
u16 BACKGROUND_init_generalized(BackgroundType type, u8 bg, u8 pal, u8 set_fade, u8 prio, u16 ind);

/**
 * @brief Clears a specific background plane.
 *
 * @param bg Background plane to clear (0 = BG_B, 1 = BG_A, 2 = WINDOW).
 * @return Updated VRAM tile index after clearing.
 */
u16 BACKGROUND_clear(u8 bg);

/**
 * @brief Clears both BG_A and BG_B background planes.
 *
 * @param ind Starting VRAM tile index.
 * @return Next available VRAM tile index after clearing both backgrounds.
 */
u16 BACKGROUND_full_clear(u16 ind);

#endif // _BACKGROUND_H_
