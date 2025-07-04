#include "background.h"

// Array of pointers to Image structures, mapping BackgroundType enum values to actual image data.
const Image *background_images[BG_MAX] = {
    [ALERT_1] = &alert_1,
    [ALERT_2] = &alert_2,
    [ALERT_3] = &alert_3,
    [ALERT_4] = &alert_4,
    [ALERT_6] = &alert_6,
    [ALERT_5] = &alert_5,
    [BG_LOGO] = &utf_logo,
    [BG_MENU] = &menu_bg,
    [BG_PAUSE] = &pause_bg,
    [BG_BOSS] = &boss_bg,
    [BG_INSTRUCT1] = &instructions1,
    [BG_INSTRUCT2] = &instructions2,
    [BG_CREDITS] = &credits,
    [BG_DIALOG_1] = &dialog_1,
    [BG_DIALOG_2] = &dialog_2,
    [BG_DIALOG_3] = &dialog_3,
    [FINAL_SCREEN] = &final_screen};
u8 bg_proceed = 0; // Global variable to control background progression, initialized to 0.

/**
 * @brief Initializes and draws a background image on a specified plane.
 *
 * This function handles the loading of palette data, optional fade effects,
 * and drawing the image to the VDP plane.
 *
 * @param type The type of background to display (from BackgroundType enum).
 * @param bg The VDP plane to draw on (0 for BG_B, 1 for BG_A).
 * @param pal The palette index to use for the image.
 * @param set_fade Boolean flag: if true, the palette will be set to black before drawing
 * and the `target_palette` will be prepared for a fade-in.
 * @param prio The priority of the background plane (true for high, false for low).
 * @param ind The starting VRAM tile index for the image data.
 * @return The next available VRAM tile index after the image has been loaded.
 */
u16 BACKGROUND_init_generalized(BackgroundType type, u8 bg, u8 pal, u8 set_fade, u8 prio, u16 ind)
{
    // Clear both background planes and update the VRAM index.
    ind = BACKGROUND_full_clear(ind);
    // If the background type is out of bounds or the image pointer is NULL, return current index.
    if (type >= BG_MAX || background_images[type] == NULL)
        return ind;

    const Image *img = background_images[type];

    PAL_setPalette(pal, img->palette->data, DMA);
    // If fade is required, store the target palette and set the VDP palette to black.
    if (set_fade)
    {
        PAL_getPalette(pal, target_palette);     // Store the current palette as the target.
        PAL_setPalette(pal, black_palette, DMA); // Set the palette to black immediately.
    }

    // Draw the image to the specified VDP plane (BG_B or BG_A).
    // TILE_ATTR_FULL sets up tile attributes including palette, priority, flip flags, and tile index.
    VDP_drawImageEx(bg == 0 ? BG_B : BG_A, img,
                    TILE_ATTR_FULL(pal, prio, 0, 0, ind),
                    0, 0, false, DMA);

    // Return the next available tile index, which is the current index plus the number of tiles in the image.
    return ind + img->tileset->numTile;
}

/**
 * @brief Clears a specified VDP background plane.
 *
 * This function clears the tile map of the given background plane.
 *
 * @param bg The VDP plane to clear (0 for BG_B, 1 for BG_A, 2 for WINDOW).
 * @return The new starting tile index in VRAM (typically TILE_USER_INDEX + 32).
 */
u16 BACKGROUND_clear(u8 bg)
{
    // Clear BG_A plane if bg is 1.
    if (bg == 1)
    {
        VDP_clearPlane(BG_A, false);
    }
    // Clear BG_B plane if bg is 0.
    else if (bg == 0)
    {
        VDP_clearPlane(BG_B, false);
    }
    // Clear WINDOW plane if bg is 2.
    else if (bg == 2)
    {
        VDP_clearPlane(WINDOW, false);
    }
    // Log an error if an invalid background plane is specified.
    else
    {
        kprintf("BACKGROUND_clear invalid");
    }

    // Return a default starting tile index.
    return TILE_USER_INDEX + 32;
}

/**
 * @brief Clears both main VDP background planes (BG_A and BG_B).
 *
 * This function calls `BACKGROUND_clear` for both BG_B (0) and BG_A (1).
 *
 * @param ind The current starting tile index in VRAM (unused in this implementation, but for consistency).
 * @return The new starting tile index in VRAM after clearing both planes.
 */
u16 BACKGROUND_full_clear(u16 ind)
{
    ind = BACKGROUND_clear(0);
    ind = BACKGROUND_clear(1);
    return ind;
}