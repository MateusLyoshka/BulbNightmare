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
#include "gameLevels/objects.h"
#include "gameLevels/drawtile.h"

// ==============================
// Variáveis globais
// ==============================
u16 ind = TILE_USER_INDEX;
u8 enemies_current_level;
u8 enemies_past_level;

// ==============================
// Prototipagem
// ==============================
void game_init();
void game_update();

int main(bool resetType)
{
	VDP_setScreenWidth320();
	SPR_init();

	enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
	enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];

	PAL_setPalette(PAL_GAME, game_pal.data, DMA);

	if (!resetType)
	{
		SYS_hardReset();
	}
	SYS_showFrameLoad(true);
	game_init();
	SYS_doVBlankProcess();
	while (true)
	{
		game_update();

		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}

void game_update()
{
	if (LEVEL_bool_screen_change)
	{
		ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
		ind = OBJECT_update(ind, TRUE);
		LEVEL_bool_screen_change = 0;
	}
	if (LEVEL_bool_level_change)
	{
		player_have_key = 0;
		ENEMIES_level_change_despawn(enemies_current_level, enemies_past_level);
		LEVEL_current_level += 1;

		enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
		enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];

		ind = TILE_USER_INDEX;
		game_init();

		LEVEL_bool_level_change = 0;
	}
	update_input();
	PLAYER_update();
	LEVEL_update_camera(&player);
	// kprintf("past: %d", enemies_past_level);
	// kprintf("current: %d", enemies_current_level);
	ENEMIES_update_hub(enemies_current_level, enemies_past_level);
	HUD_update();
}

void game_init()
{
	ind += LEVEL_init(ind);
	ind += PLAYER_init(ind);
	ind += HUD_init(ind);
	ENEMIES_init();
	ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = OBJECT_update(ind, TRUE);
	LEVEL_update_camera(&player);
}
