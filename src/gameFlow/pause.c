#include "pause.h"

GameObject pause;     // Declare a GameObject for the pause menu visual elements.
s8 pause_option = 0;  // Variable to track the currently selected option in the pause menu (0: Resume, 1: Restart Level, 2: Exit to Menu).
u8 pause_proceed = 0; // Flag to indicate how the user proceeded from the pause menu (0: not proceeded, 1: Resume, 2: Restart Level, 3: Exit to Menu).

/**
 * @brief Initializes the pause menu.
 *
 * Clears the background, sets up the pause background image, and initializes the pause menu sprite.
 * Resets the pause option and proceed flag.
 *
 * @param ind The starting VRAM tile index for loading graphics.
 * @return The next available VRAM tile index after initialization.
 */
u16 PAUSE_init(u16 ind)
{
    BACKGROUND_clear(1); // Clear background plane 1 (BG_A), typically where the game plays.
    // Reset vertical scroll for BG_A and BG_B.
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
    // Initialize the pause background (type 9 for BG_PAUSE, on BG_A, PAL_BACKGROUND_A, no fade, high priority).
    ind = BACKGROUND_init_generalized(9, 1, PAL_BACKGROUND_A, false, true, ind);
    // Initialize the 'pause' GameObject with its sprite (spr_pause), position, palette, priority, and VRAM index.
    ind = GAMEOBJECT_init(&pause, &spr_pause, 128, 80, PAL_GAME, true, ind);
    pause_option = 0;
    pause_proceed = 0;
    return ind;
}

/**
 * @brief Updates the pause menu based on user input.
 *
 * Handles navigation through pause menu options and sets the `pause_proceed` flag
 * when an option is selected. Plays sound effects for navigation and selection.
 */
void PAUSE_update()
{
    // Handle 'Down' button press to navigate menu.
    if (key_pressed(0, BUTTON_DOWN) && pause_option == 0) // If 'Down' pressed and current option is 0 (Resume).
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);  // Play button change sound.
        pause_option++;                          // Move to next option (1: Restart Level).
        SPR_setAnim(pause.sprite, pause_option); // Update sprite animation to reflect new option.
    }
    else if (key_pressed(0, BUTTON_DOWN) && pause_option == 1) // If 'Down' pressed and current option is 1 (Restart Level).
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);  // Play button change sound.
        pause_option++;                          // Move to next option (2: Exit to Menu).
        SPR_setAnim(pause.sprite, pause_option); // Update sprite animation.
    }
    // Handle 'Up' button press to navigate menu.
    else if (key_pressed(0, BUTTON_UP) && pause_option == 2) // If 'Up' pressed and current option is 2 (Exit to Menu).
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);  // Play button change sound.
        pause_option--;                          // Move to previous option (1: Restart Level).
        SPR_setAnim(pause.sprite, pause_option); // Update sprite animation.
    }
    else if (key_pressed(0, BUTTON_UP) && pause_option == 1) // If 'Up' pressed and current option is 1 (Restart Level).
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);  // Play button change sound.
        pause_option--;                          // Move to previous option (0: Resume).
        SPR_setAnim(pause.sprite, pause_option); // Update sprite animation.
    }
    // Handle 'A' button press to select option.
    else if (key_pressed(0, BUTTON_A) && pause_option == 0) // If 'A' pressed and current option is 0 (Resume).
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
        pause_proceed = 1;                      // Set proceed flag to 1 (Resume).
    }
    else if (key_pressed(0, BUTTON_A) && pause_option == 1) // If 'A' pressed and current option is 1 (Restart Level).
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
        pause_proceed = 2;                      // Set proceed flag to 2 (Restart Level).
        set_black_palletes();                   // Set all palettes to black (for transition).
    }
    else if (key_pressed(0, BUTTON_A) && pause_option == 2) // If 'A' pressed and current option is 2 (Exit to Menu).
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1); // Play selection sound.
        pause_proceed = 3;                      // Set proceed flag to 3 (Exit to Menu).
        set_black_palletes();                   // Set all palettes to black (for transition).
    }
}

/**
 * @brief Clears the pause menu by releasing its sprite resources.
 *
 * Ensures that the sprite memory is freed and the sprite pointer is nulled.
 */
void PAUSE_clear()
{
    SPR_releaseSprite(pause.sprite);
    pause.sprite = NULL;
}