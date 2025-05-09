
#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

#include "background.h"
#include "player.h"
#include "utils.h"
#include "level.h"

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

	ind += BACKGROUND_show(BG_GAME, ind);
	ind += LEVEL_init(ind);
	PLAYER_init(ind);
	kprintf("ind: %d\n", ind);
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
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}