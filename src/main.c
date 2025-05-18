
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

u16 gInd_tileset; // Carrega dados do background
Sprite *gButtonStart;
Sprite *gButtonMenu;
u16 curlInput = 0; // Estado atual do botão
u16 prevInput;	   // Estado anterior
s8 gMainMenuOptions = -1;

u16 ind = TILE_USER_INDEX;

void game_init()
{
	VDP_setScreenWidth320();
	SPR_init();

	// ind += BACKGROUND_show(BG_GAME, ind);
	ind += LEVEL_init(ind);
	ind += HUD_init(ind);
	ind += ENEMIES_init(ind, 0);
	PLAYER_init(ind);
	LEVEL_generate_screen_collision_map(0, 5);
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
		update_input();
		PLAYER_update();
		LEVEL_update_camera(&player);
		ENEMIES_update_hub(2, 0);
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}