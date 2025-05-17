
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

	// ind += BACKGROUND_show(BG_GAME, ind);
	ind += LEVEL_init(ind);
	PLAYER_init(ind);
	LEVEL_generate_screen_collision_map(0, 5);
	LEVEL_draw_collision_map();
	// LEVEL_draw_map();

	// kprintf("ind: %d\n", ind);
}

// inline void update_camera(GameObject *obj)
// {
// 	if (obj->x > (FIX16(SCREEN_W) - obj->w / 2))
// 	{
// 		obj->x = 0;
// 		screen_x += SCREEN_W;
// 		LEVEL_scroll_update_collision(SCREEN_W, 0);
// 	}
// 	else if (obj->x < (FIX16(-obj->w / 2)))
// 	{
// 		obj->x = FIX16(SCREEN_W - obj->w);
// 		screen_x -= SCREEN_W;
// 		LEVEL_scroll_update_collision(-SCREEN_W, 0);
// 	}

// 	if (obj->y > (FIX16(SCREEN_H) - obj->h / 2))
// 	{
// 		obj->y = 0;
// 		screen_y += SCREEN_H;
// 		LEVEL_scroll_update_collision(0, SCREEN_H);
// 	}
// 	else if (obj->y < (FIX16(-obj->h / 2)))
// 	{
// 		obj->y = FIX16(SCREEN_H - obj->h);
// 		screen_y -= SCREEN_H;
// 		LEVEL_scroll_update_collision(0, -SCREEN_H);
// 	}
// }

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
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}