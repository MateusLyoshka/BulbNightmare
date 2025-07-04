#include "boss.h"

u16 random_timer = 0; // Timer for random boss actions/animations.
u8 timer_defined = 0; // Flag to indicate if the random timer has been set.

GameObject boss;      // Main boss sprite.
GameObject face;      // Boss's face sprite (for expressions).
GameObject dialog;    // Dialog box sprite.
GameObject boss_key;  // Boss key sprite (not explicitly used in provided code, but declared).
GameObject boss_door; // Boss door sprite (not explicitly used in provided code, but declared).
GameObject enchant;   // Enchantment effect sprite.

u16 boss_ind = TILE_USER_INDEX + 32; // VRAM tile index for boss-related assets.
u8 boss_proceed = 0;                 // Flag to control progression through boss phases.
u8 dialog_next = 0;                  // Index for the next dialog frame.
u8 transformed = 0;                  // Flag: 1 if boss has transformed.
u8 laughing = 0;                     // Flag: 1 if boss is laughing.
u8 room_black_bool = 0;              // Flag: 1 if the room's palette has been set to black.

/**
 * @brief Initializes the boss fight.
 * Resets boss-related flags and variables, initializes the main game, and sets player abilities.
 */
void BOSS_init()
{
    boss_ind = TILE_USER_INDEX + 32; // Reset VRAM index.
    boss_proceed = 0;                // Reset boss progression flag.
    dialog_next = 0;                 // Reset dialog index.
    transformed = 0;                 // Reset transformation flag.
    laughing = 0;                    // Reset laughing flag.

    boss_ind = GAME_init(); // Initialize the main game, loading level, player, etc.

    player_can_jump = 0;    // Disable player jumping.
    player_can_walk = 1;    // Enable player walking.
    BOSS_flux_update(true); // Initial update, potentially with mask.
}

/**
 * @brief Manages the boss fight's "flux" or introductory sequence.
 * Handles camera movement, transitions, initial dialog, and gravity inversion events.
 */
void BOSS_flux()
{
    BOSS_init(); // Initialize the boss fight state.

    while (!boss_proceed) // Loop until the initial boss sequence is complete.
    {
        BOSS_flux_update(true); // Update game elements, including darkness mask.

        if (LEVEL_current_screen == 7) // Specific logic for screen 7 (boss entrance).
        {
            if (!room_black_bool) // If room hasn't been blacked out yet.
            {
                PAL_setPalette(PAL_BACKGROUND_B, black_palette, DMA); // Set background palette to black.
                room_black_bool = 1;                                  // Mark room as blacked out.
            }

            if (player_center.tile_x == 4) // If player reaches a specific tile X.
            {
                player_can_walk = 0;                     // Disable player walking.
                LEVEL_scroll_update_collision(640, 448); // Force camera scroll to a specific position.
                LEVEL_bool_screen_change = 0;            // Reset screen change flag.
                LEVEL_bool_level_change = 0;             // Reset level change flag.
                for (u8 i = 0; i < 120; i++)             // Wait for a short duration.
                {
                    SYS_doVBlankProcess();
                }
                BACKGROUND_clear(1); // Clear background plane 1.
                boss_proceed = 1;    // Set boss progression flag to proceed.
            }
        }
    }

    boss_proceed = 0;                 // Reset boss progression flag.
    VDP_setHorizontalScroll(BG_A, 0); // Reset horizontal scroll for BG_A.
    VDP_setVerticalScroll(BG_A, 0);   // Reset vertical scroll for BG_A.
    // Initialize boss arena background.
    boss_ind = BACKGROUND_init_generalized(10, 1, PAL0, true, false, boss_ind);
    // Initialize boss face and body sprites.
    boss_ind = GAMEOBJECT_init(&face, &spr_face, 145, 42, PAL0, false, boss_ind);
    boss_ind = GAMEOBJECT_init(&boss, &spr_boss, 96, 0, PAL0, false, boss_ind);
    SPR_update(); // Update sprites.

    for (u8 i = 0; i < 60; i++) // Short wait period.
    {
        BOSS_flux_update(false); // Update without mask.
    }
    player_can_walk = 0;                             // Disable player walking.
    fadeIn(20, target_palette, black_palette, PAL0); // Fade in palette.
    waitMs(2000);                                    // Wait for 2 seconds.
    BOSS_flux_update(false);                         // Update.
    waitMs(500);                                     // Wait for 0.5 seconds.
    // Initialize dialog box.
    boss_ind = GAMEOBJECT_init(&dialog, &spr_dialog, 96, 168, PAL0, false, boss_ind);
    BOSS_speak_anim(false); // Start boss speaking animation.

    while (!boss_proceed) // Loop through boss dialogue/events.
    {
        if (key_pressed(0, BUTTON_B)) // If 'B' button is pressed.
        {
            dialog_next += 1;      // Advance dialog.
            if (dialog_next == 12) // If all dialog is complete.
            {
                boss_proceed = 1; // Proceed from boss sequence.
            }
            else
            {
                SPR_setAnim(dialog.sprite, dialog_next); // Set next dialog animation frame.
            }
            if (dialog_next < 10) // For initial dialog frames.
            {
                BOSS_speak_anim(dialog_next > 8); // Play speaking animation, with a change after dialog 8.
            }
        }
        if (dialog_next == 8 && !transformed) // At specific dialog and if boss hasn't transformed.
        {
            BOSS_transform_anim();                  // Play boss transformation animation.
            XGM_startPlayPCM(71, 1, SOUND_PCM_CH2); // Play boss power sound.
            // Spawn enchantment effect at player's center.
            GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, boss_ind);
            for (u8 i = 0; i < 60; i++) // Short wait for effect.
            {
                BOSS_flux_update(false);
            }
            SPR_releaseSprite(enchant.sprite); // Release enchantment sprite.
            PLAYER_invert_gravity();           // Invert player's gravity.
            BOSS_flux_update(false);           // Update.
            while (!PLAYER_on_ground())        // Wait until player lands.
            {
                BOSS_flux_update(false);
            }
            XGM_startPlayPCM(71, 1, SOUND_PCM_CH2); // Play power sound again.
            // Spawn enchantment effect again.
            GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, boss_ind);
            for (u8 i = 0; i < 60; i++) // Short wait for effect.
            {
                BOSS_flux_update(false);
            }
            SPR_releaseSprite(enchant.sprite); // Release enchantment sprite.
            PLAYER_invert_gravity();           // Invert player's gravity again.
            BOSS_flux_update(false);           // Update.

            while (!PLAYER_on_ground()) // Wait until player lands.
            {
                BOSS_flux_update(false);
            }

            transformed = 1; // Mark boss as transformed.
        }
        if (dialog_next == 10 && !laughing) // At specific dialog and if boss isn't laughing.
        {
            BOSS_laugh_anim(); // Play boss laughing animation.
            laughing = 1;      // Mark boss as laughing.
        }

        BOSS_flux_update(false); // Update without mask.
    }
    BOSS_clear(); // Clear boss elements.

    OBJECT_update(boss_ind);         // Update objects in the scene.
    player_can_walk = 1;             // Re-enable player walking.
    player_can_jump = 1;             // Re-enable player jumping.
    while (!LEVEL_bool_level_change) // Wait for level change.
    {
        BOSS_flux_update(false); // Update.
    }

    return;
}

/**
 * @brief Clears boss-related sprites and backgrounds.
 * Releases memory and nullifies pointers.
 */
void BOSS_clear()
{
    BACKGROUND_clear(1);              // Clear background plane 1.
    SPR_releaseSprite(boss.sprite);   // Release boss sprite.
    SPR_releaseSprite(face.sprite);   // Release face sprite.
    SPR_releaseSprite(dialog.sprite); // Release dialog sprite.
    boss.sprite = NULL;               // Nullify boss sprite pointer.
    face.sprite = NULL;               // Nullify face sprite pointer.
    dialog.sprite = NULL;             // Nullify dialog sprite pointer.
    SPR_update();                     // Update sprites.
}

/**
 * @brief Plays the boss's speaking animation.
 * Sets facial expressions for speaking, with some random variations.
 */
void BOSS_speak_anim()
{
    // Get a random timer value for speaking animation variations.
    u16 random_timer = BOSS_random_crown_anim(90, 150, 30);

    // Set face animation based on whether boss is transformed.
    SPR_setAnim(face.sprite, transformed ? 4 : 1);
    for (u8 i = 0; i < random_timer; i++) // Loop for animation duration.
    {
        BOSS_flux_update(false); // Update without mask.
    }
    SPR_setAnim(boss.sprite, 0);                   // Set boss body animation to idle.
    SPR_setAnim(face.sprite, transformed ? 3 : 0); // Set face animation back to idle.
}

/**
 * @brief Plays the boss's transformation animation.
 * Changes the boss's face sprite to a transformation state.
 */
void BOSS_transform_anim()
{
    SPR_setAnim(face.sprite, 2); // Set face animation to transformation frame.
    for (u8 i = 0; i < 80; i++)  // Loop for animation duration.
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(face.sprite, 3); // Set face animation to transformed idle.
    return;
}

/**
 * @brief Plays the boss's laughing animation.
 * Changes the boss's face sprite through laughing frames.
 */
void BOSS_laugh_anim()
{
    SPR_setAnim(face.sprite, 5); // Set face animation to start laughing frame.
    for (u8 i = 0; i < 70; i++)  // Loop for animation duration.
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(face.sprite, 6); // Set face animation to end laughing frame.
}

/**
 * @brief Generates a random value for boss crown animation timing or behavior.
 * @param min_val Minimum possible random value.
 * @param max_val Maximum possible random value.
 * @param step Step increment for random value generation.
 * @return A calculated random animation duration or behavior indicator.
 */
u16 BOSS_random_crown_anim(u16 min_val, u16 max_val, u16 step)
{
    s16 random_c = random() % 3; // Get a random number (0, 1, or 2).
    u16 timer_value = 0;
    if (!random_c) // If random_c is 0.
    {
        timer_value = getRandomValueBetween(min_val, max_val, step); // Get a random timer value.
    }
    if (random_c == 1) // If random_c is 1.
    {
        SPR_setAnim(boss.sprite, 1); // Set boss body animation to a specific frame.
    }

    return random_c ? 120 : timer_value; // If random_c is not 0, return 120, otherwise return timer_value.
}

/**
 * @brief General update function for boss fight sequences.
 * Handles common updates like input, player, camera, and optional mask updates.
 * @param mask_bool Boolean flag indicating whether the darkness mask should be updated.
 */
void BOSS_flux_update(u8 mask_bool)
{
    update_input();  // Update controller input.
    PLAYER_update(); // Update player logic.
    if (mask_bool)   // If mask update is requested.
    {
        MASK_scroll_update(); // Update darkness mask scrolling.
    }

    LEVEL_update_camera(&player); // Update camera based on player position.
    SPR_update();                 // Update sprites.
    SYS_doVBlankProcess();        // Wait for V-blank.
}

/**
 * @brief Manages the boss's "power" ability, involving gravity inversion.
 * Triggers gravity inversion at specific timer intervals and displays an enchantment effect.
 * @param ind The current VRAM tile index for sprite allocation.
 */
void BOSS_power(u16 ind)
{
    if (!timer_defined || !player_is_alive) // If timer not defined or player is dead.
    {
        random_timer = getRandomValueBetween(180, 300, 60); // Set new random timer.
        timer_defined = 1;                                  // Mark timer as defined.
    }
    if (random_timer == 180) // At a specific timer value.
    {
        XGM_startPlayPCM(71, 1, SOUND_PCM_CH2); // Play boss power sound.
        // Initialize enchantment effect sprite at player's center.
        GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, ind);
    }
    if (enchant.sprite != NULL) // If enchantment sprite is active.
    {
        s16 enchant_pos_x = player_center.x - (enchant.w / 2); // Calculate enchantment X position.
        s16 enchant_pos_y = player_center.y - (enchant.h / 2); // Calculate enchantment Y position.

        SPR_setPosition(enchant.sprite, enchant_pos_x, enchant_pos_y); // Set enchantment sprite position.
    }
    if (random_timer == 0) // When timer runs out.
    {
        PLAYER_invert_gravity();           // Invert player's gravity.
        timer_defined = 0;                 // Reset timer defined flag.
        SPR_releaseSprite(enchant.sprite); // Release enchantment sprite.
    }
    random_timer--; // Decrement timer.
}