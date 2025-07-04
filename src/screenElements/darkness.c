#include "darkness.h"

u8 room_lights[MAP_TOTAL_SCREENS]; // Array to store the light status for each screen/room.
u16 dark_ind = TILE_USER_INDEX;    // Global variable to hold the VRAM tile index for the darkness mask, initialized to TILE_USER_INDEX.

/**
 * @brief Initializes the darkness mask and scrolling mode.
 *
 * This function sets the initial light state for all rooms and configures
 * the VDP scrolling mode for horizontal and vertical plane scrolling.
 */
void MASK_scroll_init()
{
    // Initialize room_lights array:
    for (u8 i = 0; i < MAP_TOTAL_SCREENS; i++)
    {
        // If the current level is 5, all rooms are initially lit.
        if (LEVEL_current_level == 5)
            room_lights[i] = 1;
        // Otherwise, all rooms are initially dark (or set to a default state, e.g., 0).
        else
            room_lights[i] = 0;
    }
    // Set color 33 in the palette to black (RGB 0,0,0) for the darkness effect.
    PAL_setColor(33, RGB24_TO_VDPCOLOR(0));
    // Set the VDP scrolling mode to enable horizontal and vertical scrolling for both planes.
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
}

/**
 * @brief Draws or updates the darkness mask based on the current room's light status.
 *
 * If the current room is dark, a dark mask image is drawn on BG_A.
 * If the room is lit, BG_A is cleared.
 */
void MASK_draw()
{
    // Reset the darkness mask's VRAM index to TILE_USER_INDEX.
    dark_ind = TILE_USER_INDEX;

    // Check if the current screen is dark (room_lights[LEVEL_current_screen] is 0).
    if (room_lights[LEVEL_current_screen] == 0)
    {
        // Draw the 'dark_mask' image onto BG_A with specified attributes.
        VDP_drawImageEx(BG_A, &dark_mask, TILE_ATTR_FULL(PAL_BACKGROUND_A, 1, 0, 0, dark_ind), 0, 0, false, DMA);
    }
    else
    {
        // If the room is not dark, clear BG_A (background plane 1).
        BACKGROUND_clear(1);
    }
}

/**
 * @brief Updates the scrolling of the darkness mask based on player position.
 *
 * This function also handles the state transition for room lighting.
 */
void MASK_scroll_update()
{
    // If the current room has just been lit (status 1).
    if (room_lights[LEVEL_current_screen] == 1)
    {
        // Clear BG_A to remove the darkness mask.
        BACKGROUND_clear(1);
        // Change the room light status to 2, indicating it's now fully lit and processed.
        room_lights[LEVEL_current_screen] = 2;
    }
    // If the current room is dark (status 0).
    else if (room_lights[LEVEL_current_screen] == 0)
    {
        // Set horizontal scroll for BG_A relative to the player's center X position.
        VDP_setHorizontalScroll(BG_A, player_center.x - 34);
        // Set vertical scroll for BG_A relative to the player's center Y position.
        VDP_setVerticalScroll(BG_A, -player_center.y + 34);
    }
}