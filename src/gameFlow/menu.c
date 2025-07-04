#include "menu.h" // Includes the header file for menu functionalities.

GameObject start; // GameObject for the "Press Start" visual element.
GameObject menu;  // GameObject for the main menu options.

u8 room = 0;         // Tracks the current 'room' or state within the menu system (e.g., main menu, instructions).
u8 current_room = 0; // Another variable tracking the current room/state.
s8 menu_option = -1; // Current selected menu option: -1 for "Press Start", 0 for "Start Game", 1 for "Instructions", 2 for "Credits".

/**
 * @brief Initializes the main menu.
 *
 * Resets menu state variables, sets the background palette, and initializes
 * the "Press Start" visual element.
 *
 * @param ind The starting VRAM tile index for loading graphics.
 * @return The next available VRAM tile index after initialization.
 */
u16 MENU_init(u16 ind)
{
    room = 0;                                                       // Reset menu 'room' to 0.
    current_room = 0;                                               // Reset current room state to 0.
    menu_option = -1;                                               // Set initial menu option to -1 (indicating "Press Start" screen).
    bg_proceed = 0;                                                 // Reset background proceed flag.
    PAL_setPalette(PAL_BACKGROUND_A, spr_start.palette->data, DMA); // Set palette for the start screen sprite.
    // Initialize the 'start' GameObject (spr_start sprite) at position (97, 86) on PAL_BACKGROUND_A with high priority.
    ind += GAMEOBJECT_init(&start, &spr_start, 97, 86, PAL_BACKGROUND_A, true, ind);
    return ind; // Return the updated VRAM index.
}

/**
 * @brief Updates the main menu based on user input.
 *
 * Handles navigation between menu options, sound effects, and transitions
 * to game start, instructions, or credits screens.
 *
 * @param ind The current VRAM tile index for loading graphics if new sprites are needed.
 * @return The next available VRAM tile index after updates.
 */
u16 MENU_update(u16 ind)
{
    // If 'Start' button is pressed and currently on "Press Start" screen.
    if (key_pressed(0, BUTTON_START) && menu_option == -1)
    {
        menu_option++; // Change menu option to 0 (Start Game).
        SPR_releaseSprite(start.sprite);
        start.sprite = NULL;
        // Initialize the 'menu' GameObject (spr_menu sprite) at position (96, 72) on PAL_BACKGROUND_A.
        ind += GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_BACKGROUND_A, true, ind);

        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
    }
    // If 'Down' button is pressed and current option is 0 (Start Game).
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 0)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1); // Play menu navigation sound.
        menu_option++;                          // Move to next option (1: Instructions).
        SPR_setAnim(menu.sprite, menu_option);  // Update menu sprite animation to reflect selection.
    }
    // If 'Down' button is pressed and current option is 1 (Instructions).
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1); // Play menu navigation sound.
        menu_option++;                          // Move to next option (2: Credits).
        SPR_setAnim(menu.sprite, menu_option);  // Update menu sprite animation.
    }
    // If 'Up' button is pressed and current option is 2 (Credits).
    else if (key_pressed(0, BUTTON_UP) && menu_option == 2)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1); // Play menu navigation sound.
        menu_option--;                          // Move to previous option (1: Instructions).
        SPR_setAnim(menu.sprite, menu_option);  // Update menu sprite animation.
    }
    // If 'Up' button is pressed and current option is 1 (Instructions).
    else if (key_pressed(0, BUTTON_UP) && menu_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1); // Play menu navigation sound.
        menu_option--;                          // Move to previous option (0: Start Game).
        SPR_setAnim(menu.sprite, menu_option);  // Update menu sprite animation.
    }
    // If 'A' button is pressed and current option is 0 (Start Game).
    else if (key_pressed(0, BUTTON_A) && menu_option == 0)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
        SPR_releaseSprite(menu.sprite);         // Release the menu sprite.
        SPR_update();                           // Update sprites.
        fadeOut(60, PAL0);                      // Fade out palette 0 over 60 frames.
        waitMs(400);
        ind = BACKGROUND_clear(1); // Clear background plane 1.
        bg_proceed = 1;            // Set flag to proceed to game.
    }
    // If 'A' button is pressed, current option is 1 (Instructions), and not currently in another room.
    else if (key_pressed(0, BUTTON_A) && menu_option == 1 && current_room == 0)
    {
        u8 proceed = 0;                         // Local flag for instruction screen progression.
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
        current_room = 1;                       // Set current room to 1 (Instructions).
        // Initialize instruction background (type 8, BG_B, no fade, low priority).
        ind = BACKGROUND_init_generalized(8, 1, PAL_BACKGROUND_B, false, false, ind);
        SPR_releaseSprite(menu.sprite); // Release the menu sprite.
        update_input();                 // Update input state.

        while (!proceed) // Loop while on instruction screen.
        {
            if (key_pressed(0, BUTTON_A)) // If 'A' is pressed, proceed.
            {
                proceed = 1;
            }
            update_input();
            SPR_update();
            SYS_doVBlankProcess();
        }

        BACKGROUND_clear(1);
        // Initialize next instruction background (type 15, BG_B, no fade, low priority).
        ind = BACKGROUND_init_generalized(15, 1, PAL_BACKGROUND_B, false, false, ind);
    }
    // If 'A' button is pressed, current option is 2 (Credits), and not currently in another room.
    else if (key_pressed(0, BUTTON_A) && menu_option == 2 && current_room == 0)
    {
        // Initialize credits background (type 16, BG_B, no fade, low priority).
        ind = BACKGROUND_init_generalized(16, 1, PAL_BACKGROUND_B, false, false, ind);
        SPR_releaseSprite(menu.sprite);
        current_room = 2; // Set current room to 2 (Credits).
    }
    // If 'A' button is pressed and currently in instruction or credits room.
    else if (key_pressed(0, BUTTON_A) && (current_room == 1 || current_room == 2))
    {
        ind = BACKGROUND_clear(1);
        current_room = 0;
        // Initialize main menu background (type 7, BG_B, no fade, high priority).
        ind = BACKGROUND_init_generalized(7, 1, PAL_BACKGROUND_B, false, true, ind);
        // Re-initialize the menu sprite for the main menu.
        ind = GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_BACKGROUND_A, true, ind);
        SPR_setAnim(menu.sprite, menu_option); // Set menu sprite animation to the last selected option.
    }

    return ind;
}