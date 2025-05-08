
#include <genesis.h>
#include <sprite_eng.h>
#include "resources.h"
#include "background.h"

u16 gInd_tileset; // Carrega dados do background
Sprite *gButtonStart;
Sprite *gButtonMenu;
u16 curlInput = 0; // Estado atual do botão
u16 prevInput;	   // Estado anterior
s8 gMainMenuOptions = -1;

u16 ind = TILE_USER_INDEX;

void game_init()
{
	// VDP_setScreenWidth320();
	SPR_init();

#ifndef DEBUG
	ind += BACKGROUND_show(BG_TITLE, ind);

#endif

#ifdef DEBUG
	LEVEL_draw_collision_map();
#endif
}

int main(bool resetType)
{
	// Soft reset doesn't clear RAM. Can lead to bugs.
	if (!resetType)
	{
		SYS_hardReset();
	}

	// VDP_setPlaneSize(64, 64, TRUE);
	game_init();

	SYS_doVBlankProcess();

	while (true)
	{

		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}