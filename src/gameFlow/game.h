#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"
#include "../screenElements/background.h"
#include "../utils/gameobject.h"
#include "../utils/utils.h"
#include "../screenElements/level.h"
#include "../screenElements/hud.h"
#include "../enemies/enemies.h"
#include "../player/player.h"
#include "../screenElements/objects.h"
#include "menu.h"
#include "../screenElements/darkness.h"
#include "pause.h"
#include "../enemies/boss.h"

extern const u16 *game_palettes[6];

/**
 * Ends the current game session (game over or final screen).
 */
void GAME_end_game();

/**
 * Initializes all sound effects used in the game.
 */
void GAME_init_sounds();

/**
 * Loads palettes for each level.
 */
void GAME_init_palettes();

/**
 * Updates the current game palettes.
 */
void GAME_update_palletes();

/**
 * Initializes the core game state (level, player, enemies, HUD).
 *
 * @param ind Starting VRAM tile index
 * @return Next available VRAM tile index
 */
u16 GAME_init();

/**
 * Main game update loop (called every frame).
 */
void GAME_update();

/**
 * Initializes and displays the main menu.
 */
void GAME_menu_init();

/**
 * Handles setup when changing levels.
 */
void GAME_level_change();

/**
 * Handles setup when changing screens within a level.
 */
void GAME_screen_change();

/**
 * Manages the player’s death sequence and effects.
 */
void GAME_player_death();

/**
 * Initializes and displays the pause menu.
 */
void GAME_pause_init();

/**
 * Resets the entire game state.
 */
void GAME_reset();

/**
 * Initializes the darkness mask system.
 */
void GAME_mask_init();
