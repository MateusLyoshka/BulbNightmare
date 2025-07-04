#include "game.h" // Includes the main game header.

u16 ind = TILE_USER_INDEX + 32; // Global VRAM tile index, initialized after reserved system tiles.
u8 game_initiated;              // Flag indicating if the game has been initialized.
u8 game_ended = 0;              // Flag indicating if the game has ended.

const u16 *game_palettes[6]; // Array to store pointers to palette data for different levels.

/**
 * @brief Initializes the array of game palettes.
 * Assigns specific palette data to each level index.
 */
void GAME_init_palettes()
{
    game_palettes[0] = level1_pal.data; // Palette for Level 1.
    game_palettes[1] = level2_pal.data; // Palette for Level 2.
    game_palettes[2] = level3_pal.data; // Palette for Level 3.
    game_palettes[3] = level4_pal.data; // Palette for Level 4.
    game_palettes[4] = level4_pal.data; // Palette for Level 5 (reusing Level 4 palette).
    game_palettes[5] = boss_pal.data;   // Palette for Boss Level.
}

/**
 * @brief Initializes game sound effects.
 * Sets up PCM samples for various in-game events.
 */
void GAME_init_sounds()
{
    XGM_setPCM(65, snd_start, sizeof(snd_start));                 // Sound for start/selection.
    XGM_setPCM(66, snd_death, sizeof(snd_death));                 // Sound for player death.
    XGM_setPCM(67, snd_key, sizeof(snd_key));                     // Sound for collecting a key.
    XGM_setPCM(68, snd_switch, sizeof(snd_switch));               // Sound for activating a switch.
    XGM_setPCM(69, snd_jump, sizeof(snd_jump));                   // Sound for player jump.
    XGM_setPCM(70, snd_button_change, sizeof(snd_button_change)); // Sound for menu navigation.
    XGM_setPCM(71, snd_boss_power, sizeof(snd_boss_power));       // Sound for boss power-up.
    XGM_setPCM(72, snd_door, sizeof(snd_door));                   // Sound for opening a door.
    XGM_setPCM(73, snd_cheat_on, sizeof(snd_cheat_on));           // Sound for turning cheat on.
    XGM_setPCM(74, snd_cheat_off, sizeof(snd_cheat_off));         // Sound for turning cheat off.
}

/**
 * @brief Main game update loop, executed every frame.
 * Handles player status, screen/level changes, boss logic, input, and updates
 * all active game elements.
 */
void GAME_update()
{
    if (!player_is_alive) // Check if player is dead.
    {
        GAME_player_death(); // Handle player death sequence.
    }
    if (LEVEL_bool_screen_change) // Check for screen change flag.
    {
        GAME_screen_change(); // Handle screen transition.
    }
    if (LEVEL_bool_level_change) // Check for level change flag.
    {
        GAME_level_change(); // Handle level transition.
    }
    if (LEVEL_current_level == 5) // Special logic for boss level.
    {
        BOSS_power(ind); // Activate boss power-up.
    }
    if (key_pressed(0, BUTTON_C) && player_is_alive) // Toggle cheat mode with C button if player is alive.
    {
        if (!player_cheat_on) // If cheats are off.
        {
            XGM_startPlayPCM(73, 1, SOUND_PCM_CH1); // Play cheat on sound.
            player_cheat_on = 1;                    // Enable cheats.
        }
        else // If cheats are on.
        {
            XGM_startPlayPCM(74, 1, SOUND_PCM_CH1); // Play cheat off sound.
            player_cheat_on = 0;                    // Disable cheats.
        }
    }

    GAME_pause_init();            // Check for and handle pause menu activation.
    MASK_scroll_update();         // Update the darkness mask scrolling.
    update_input();               // Update controller input states.
    PLAYER_update();              // Update player logic (movement, collisions, animation).
    LEVEL_update_camera(&player); // Update camera position based on player.
    ENEMIES_update_hub();         // Update all active enemies.
    HUD_update();                 // Update the Head-Up Display.
}

/**
 * @brief Initializes the game state at the start of a level or new game.
 * Sets up palettes, HUD, level, player, enemies, and object states.
 * @param ind The current VRAM tile index.
 * @return The updated VRAM tile index.
 */
u16 GAME_init()
{
    set_black_palletes(); // Set all palettes to black for a fade-in effect.
    if (!game_initiated)  // On first game initiation.
    {
        OBJECT_key_reset(); // Reset collected keys.
    }
    ind = LEVEL_alert(ind);      // Display level alert screen with fade-in/out.
    if (LEVEL_current_level < 4) // For earlier levels, initialize HUD normally.
    {
        ind = HUD_background(ind); // Draw HUD background.
        HUD_init(ind);             // Initialize HUD elements.
    }
    else if (LEVEL_current_level == 4) // Special handling for level 4 (boss level setup).
    {
        HUD_clear();                 // Clear HUD.
        VDP_setWindowVPos(FALSE, 0); // Disable window for HUD.
    }
    ind = LEVEL_init(ind);        // Initialize the current level (map, tiles).
    ind = PLAYER_init(ind);       // Initialize the player.
    ind = ENEMIES_spawn_hub(ind); // Spawn enemies for the current screen/level.

    GAME_mask_init(); // Initialize the darkness mask.

    OBJECT_update(ind);           // Update and spawn objects for the current screen.
    LEVEL_update_camera(&player); // Ensure camera is correctly positioned.
    PLAYER_respawn();             // Respawn player at designated point.
    game_initiated = 1;           // Mark game as initiated.
    SPR_update();                 // Update sprites on screen.
    SYS_doVBlankProcess();        // Wait for V-blank.
    GAME_update_palletes();       // Apply game palettes.
    return ind;                   // Return final VRAM index.
}

/**
 * @brief Updates the game's palettes.
 * Sets the main game palette and the background B palette based on the current level.
 */
void GAME_update_palletes()
{
    PAL_setPalette(PAL_GAME, game_pal.data, DMA);                              // Set game sprite palette.
    PAL_setPalette(PAL_BACKGROUND_B, game_palettes[LEVEL_current_level], DMA); // Set background palette based on current level.
}

/**
 * @brief Initializes the darkness mask system.
 * Configures scrolling, draws the mask, and updates it.
 */
void GAME_mask_init()
{
    MASK_scroll_init();   // Initialize mask scrolling.
    MASK_draw(dark_ind);  // Draw the darkness mask.
    MASK_scroll_update(); // Update mask scrolling.
    SPR_update();         // Update sprites.
}

/**
 * @brief Handles the consequences of player death.
 * Decrements lives, manages game over, or respawns player.
 */
void GAME_player_death()
{
    if (LEVEL_current_level < 5) // If not in boss level.
    {
        player_lives -= 1; // Decrement player lives.
    }

    if (!player_lives) // If no lives left, game over.
    {
        game_initiated = 0;  // Reset game initiation flag.
        GAME_level_change(); // Trigger level change (to menu/game over).
    }
    else // If lives remain, respawn player.
    {
        PLAYER_death_anim();                   // Play player death animation.
        LEVEL_scroll_update_collision(0, 448); // Reset camera/scroll to a default spawn screen.
        GAME_screen_change();                  // Re-initialize screen elements.
        PLAYER_respawn();                      // Respawn the player.
        player_is_alive = 1;                   // Mark player as alive again.
    }
}

/**
 * @brief Manages the end game sequence.
 * Displays final screens with fades and resets game state.
 */
void GAME_end_game()
{
    set_black_palletes(); // Set palettes to black.
    // Display various end-game/dialog screens with fade effects.
    ind = BACKGROUND_init_generalized(11, 1, PAL0, true, true, ind); // Dialog 1.
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(12, 1, PAL0, true, true, ind); // Dialog 2.
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(13, 1, PAL0, true, true, ind); // Dialog 3.
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(14, 1, PAL0, true, true, ind); // Final screen.
    fadeIn(120, target_palette, black_palette, PAL0);
    waitMs(4000);
    fadeOut(120, PAL0);
    ind = BACKGROUND_clear(1);   // Clear background.
    game_ended = 1;              // Mark game as ended.
    LEVEL_bool_level_change = 0; // Reset level change flag.
    GAME_reset();                // Reset game to main menu.
}

/**
 * @brief Handles level transitions.
 * Despawns enemies, updates level index, and re-initializes game for the new level.
 */
void GAME_level_change()
{
    ENEMIES_level_change_despawn();     // Despawn enemies from previous level.
    if (player_lives && !pause_proceed) // If player has lives and not exiting from pause.
    {
        LEVEL_current_level += 1; // Advance to next level.
    }
    else // If no lives or exiting from pause.
    {
        LEVEL_current_level = 0; // Go back to level 0.
    }
    if (LEVEL_current_level == 6) // Check if all levels completed.
    {
        kprintf("level: %d", LEVEL_current_level); // Debug print.
        GAME_end_game();                           // Trigger end game sequence.
        return;
    }
    ind = TILE_USER_INDEX + 32;   // Reset VRAM index.
    player_keys = 0;              // Reset collected keys.
    switchs_on = 0;               // Reset activated switches.
    player_is_alive = 1;          // Player is alive for new level.
    LEVEL_bool_level_change = 0;  // Reset level change flag.
    if (LEVEL_current_level == 4) // Special handling for boss level (level 4).
    {
        BOSS_flux(); // Activate boss flux.
    }
    else // For regular levels.
    {
        GAME_init(); // Re-initialize game for the new level.
    }
}

/**
 * @brief Handles screen transitions within a level.
 * Spawns enemies and updates objects for the new screen.
 */
void GAME_screen_change()
{
    if (player_is_alive || (!player_is_alive && LEVEL_last_screen != 6)) // Spawn enemies if player is alive or if not a special last screen.
    {
        ind = ENEMIES_spawn_hub(ind); // Spawn enemies relevant to the new screen.
    }
    OBJECT_update(ind);           // Update and spawn objects for the new screen.
    MASK_draw();                  // Redraw the darkness mask for the new screen.
    LEVEL_bool_screen_change = 0; // Reset screen change flag.
}

/**
 * @brief Initializes and manages the main menu sequence.
 * Displays logo, menu background, and handles menu interaction until game starts.
 */
void GAME_menu_init()
{
    // Display logo with fade-in/out.
    ind = BACKGROUND_init_generalized(6, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_clear(1); // Clear background.

    // Display menu background with fade-in.
    ind = BACKGROUND_init_generalized(7, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    ind = MENU_init(ind); // Initialize menu elements.

    while (!bg_proceed) // Loop while menu is active (until "Start Game" is selected).
    {
        ind = MENU_update(ind); // Update menu logic.
        update_input();         // Update input.
        SPR_update();           // Update sprites.
        SYS_doVBlankProcess();  // Wait for V-blank.
    }
    ind = TILE_USER_INDEX + 32; // Reset VRAM index after menu exit.
}

/**
 * @brief Handles activation and management of the pause menu.
 * When 'Start' is pressed, enters the pause loop, processes options, and resumes/restarts/exits.
 */
void GAME_pause_init()
{
    if (key_pressed(0, BUTTON_START)) // If Start button is pressed.
    {
        ind = PAUSE_init(ind); // Initialize pause menu.
        while (!pause_proceed) // Loop while in pause menu.
        {
            PAUSE_update();        // Update pause menu logic.
            update_input();        // Update input.
            SPR_update();          // Update sprites.
            SYS_doVBlankProcess(); // Wait for V-blank.
        }
        PAUSE_clear();          // Clear pause menu resources.
        SPR_update();           // Update sprites after clearing.
        if (pause_proceed == 1) // If "Resume" selected.
        {
            ind = TILE_USER_INDEX + 32; // Reset VRAM index.
            ind = HUD_background(ind);  // Redraw HUD background.
            ind = LEVEL_init(ind);      // Re-initialize level (for consistent state after pause).
            MASK_scroll_init();         // Re-initialize mask scrolling.
            MASK_draw(dark_ind);        // Redraw darkness mask.
        }
        else if (pause_proceed == 2) // If "Restart Level" selected.
        {
            GAME_level_change(); // Trigger a level change (effectively restarting current level).
        }
        else if (pause_proceed == 3) // If "Exit to Menu" selected.
        {
            GAME_reset(); // Reset the entire game.
        }
        pause_proceed = 0; // Reset pause proceed flag.
    }
}

/**
 * @brief Resets the entire game to its initial state.
 * Clears all game elements, resets player stats, and returns to the main menu.
 */
void GAME_reset()
{
    player_keys = 0;    // Reset player keys.
    switchs_on = 0;     // Reset activated switches.
    game_initiated = 0; // Mark game as not initiated.
    hud_initiated = 0;  // Mark HUD as not initiated.

    if (LEVEL_current_level < 5) // If not in boss level.
    {
        HUD_clear(); // Clear HUD.
    }

    if (enchant.sprite != NULL) // If boss enchantment sprite exists.
    {
        SPR_releaseSprite(enchant.sprite); // Release it.
        enchant.sprite = NULL;
    }
    LEVEL_current_level = 0; // Reset current level to 0.
    // Clear all background planes.
    ind = BACKGROUND_clear(0);
    ind = BACKGROUND_clear(1);
    ind = BACKGROUND_clear(2);
    OBJECT_update(ind);             // Update objects (will clear non-existent ones).
    ENEMIES_update_hub();           // Update enemies (will clear non-existent ones).
    OBJECT_clear_all();             // Explicitly clear all objects.
    ENEMIES_level_change_despawn(); // Despawn all enemies.
    VDP_setWindowVPos(FALSE, 0);    // Reset window position.
    PLAYER_free();                  // Free player sprite resources.
    SPR_update();                   // Update sprites.
    SYS_doVBlankProcess();          // Wait for V-blank.
    GAME_menu_init();               // Return to main menu.
    GAME_init();                    // Initialize game from start (for consistent state after menu).
}