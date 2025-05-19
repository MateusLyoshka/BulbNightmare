
#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

#include "gameLevels/background.h"
#include "gameLevels/gameobject.h"
#include "utils/utils.h"
#include "gameLevels/level.h"
#include "gameLevels/hud.h"
#include "enemies/enemies.h"
#include "gameLevels/player.h"

u16 ind = TILE_USER_INDEX;

void game_init()
{
	VDP_setScreenWidth320();
	SPR_init();

	// ind += BACKGROUND_show(BG_GAME, ind);
	ind += LEVEL_init(ind);
	ind += HUD_init(ind);
	ENEMIES_init(0);
	ind += ENEMIES_spawn_hub(5, 2, ind);
	PLAYER_init(ind);
	LEVEL_update_camera(&player);
	// LEVEL_draw_collision_map();

	// kprintf("ind: %d\n", ind);
}

int main(bool resetType)
{
	// Soft reset doesn't clear RAM. Can lead to bugs.
	if (!resetType)
	{
		SYS_hardReset();
	}

	game_init();

	SYS_doVBlankProcess();

	while (true)
	{
		if (LEVEL_bool_screen_change)
		{
			ENEMIES_spawn_hub(5, 2, ind);
			LEVEL_bool_screen_change = 0;
		}
		update_input();
		PLAYER_update();
		LEVEL_update_camera(&player);
		ENEMIES_update_hub(5, 2);
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}