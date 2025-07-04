#include <genesis.h>	// Includes the main SGDK library for Sega Genesis development.
#include <sprite_eng.h> // Includes the sprite engine library for managing sprites.

#include "resources.h"	   // Includes game resources like sprites, sounds, and palettes.
#include "gameFlow/game.h" // Includes the main game flow functions.

/**
 * @brief The main entry point of the game application.
 * This function initializes the Sega Genesis hardware, sets up game resources,
 * and runs the primary game loop.
 * @param resetType A boolean indicating the type of reset (true for soft reset, false for hard reset).
 * @return Returns 0 upon successful execution.
 */
int main(bool resetType)
{
	VDP_setScreenWidth320();		// Sets the VDP (Video Display Processor) to 320-pixel wide screen mode.
	SPR_init();						// Initializes the sprite engine.
	VDP_setPlaneSize(64, 64, true); // Sets the size of background planes A and B to 64x64 tiles.
	// SYS_showFrameLoad(true); // Commented out: Enables displaying frame load (for debugging performance).

	if (!resetType) // If it's not a soft reset (i.e., a power-on or hard reset).
	{
		SYS_hardReset(); // Performs a hard reset of the system.
	}

	// Sets the player's initial spawn point coordinates in fixed-point format.
	player_spawn.initial_x = intToFix16(2 * METATILE_W);  // X-coordinate: 2 metatiles.
	player_spawn.initial_y = intToFix16(12 * METATILE_W); // Y-coordinate: 12 metatiles.

	GAME_init_sounds();		// Initializes all sound effects for the game.
	GAME_init_palettes();	// Initializes the game's palettes.
	ENEMY_params();			// Sets up the parameters for all enemies.
	GAME_update_palletes(); // Applies the initial game palettes.
	OBJECT_params();		// Sets up the parameters for all interactive objects.
	GAME_menu_init();		// Initializes and displays the main menu.
	GAME_init();			// Initializes the game state (levels, player, enemies, etc.).

	while (true) // Main game loop, continues indefinitely.
	{
		GAME_update();		   // Updates all game logic (player, enemies, collisions, etc.).
		SPR_update();		   // Updates the sprite engine, sending sprite data to VDP.
		SYS_doVBlankProcess(); // Waits for the V-blank period, synchronizing with screen refresh.
	}

	return 0; // Should theoretically never be reached in a continuous game loop.
}