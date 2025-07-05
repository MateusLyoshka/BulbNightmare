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

	GAME_init_sounds();
	GAME_init_palettes();
	ENEMY_params();
	GAME_update_palletes();
	OBJECT_params();
	GAME_menu_init();
	GAME_init();

	while (true) // Main game loop, continues indefinitely.
	{
		GAME_update();
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}