#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

#include "gameFlow/game.h"

int main(bool resetType)
{
	VDP_setScreenWidth320();
	SPR_init();
	VDP_setPlaneSize(64, 64, true);

	if (!resetType)
	{
		SYS_hardReset();
	}

	player_spawn.initial_x = intToFix16(18 * METATILE_W);
	player_spawn.initial_y = intToFix16(12 * METATILE_W);
	// 2, 12

	SYS_showFrameLoad(true);
	// GAME_menu_init();
	// OBJECT_params();
	// GAME_init();
	BOSS_flux();
	while (true)
	{
		GAME_update();
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}