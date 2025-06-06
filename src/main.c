#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

#include "screenElements/background.h"
#include "utils/gameobject.h"
#include "utils/utils.h"
#include "screenElements/level.h"
#include "screenElements/hud.h"
#include "enemies/enemies.h"
#include "player/player.h"
#include "screenElements/objects.h"
#include "screenElements/menu.h"

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
void menu_init();
void mask_scroll_init();
void mask_scroll_update();
void level_alert();

int main(bool resetType)
{
	VDP_setScreenWidth320();
	SPR_init();
	VDP_setPlaneSize(64, 64, true);

	if (!resetType)
	{
		SYS_hardReset();
	}

	// menu_init();

	SYS_showFrameLoad(true);

	enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
	enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];

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
	if (LEVEL_bool_screen_change || !player_is_alive)
	{
		if (!player_is_alive)
		{
			ENEMIES_level_change_despawn(enemies_current_level, enemies_past_level);
			LEVEL_scroll_update_collision(0, 448);
		}

		ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
		ind = OBJECT_update(ind, TRUE);
		player_is_alive = 1;
		LEVEL_bool_screen_change = 0;
	}
	if (LEVEL_bool_level_change)
	{
		player_have_key = 0;
		ENEMIES_level_change_despawn(enemies_current_level, enemies_past_level);
		LEVEL_current_level += 1;

		enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
		enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];
		SYS_doVBlankProcess();
		game_init();
		player_is_alive = 1;
		LEVEL_bool_level_change = 0;
	}

	mask_scroll_update();
	update_input();
	PLAYER_update();
	LEVEL_update_camera(&player);

	ENEMIES_update_hub(enemies_current_level, enemies_past_level);
	HUD_update();
}

void mask_scroll_update()
{
	VDP_setHorizontalScroll(BG_A, player_center.x - 42);
	VDP_setVerticalScroll(BG_A, -player_center.y + 42);
	// normal 34/34
}

void game_init()
{
	// level_alert();
	mask_scroll_init();
	ind = LEVEL_init(ind);
	ind = PLAYER_init(ind);
	ind = HUD_init(ind);
	ENEMIES_init();
	ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = OBJECT_update(ind, TRUE);
	LEVEL_update_camera(&player);
}

void mask_scroll_init()
{
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	VDP_drawImageEx(BG_A, &dark_mask_max, TILE_ATTR_FULL(PAL_BACKGROUND_A, 1, 0, 0, ind), 0, 0, true, DMA);
	ind += dark_mask.tileset->numTile;
}

void menu_init()
{
	ind = BACKGROUND_init_generalized(5, 0, PAL0, true, ind);
	fadeIn(60, target_palette, black_palette, PAL0);
	waitMs(1000);
	fadeOut(60);
	ind = BACKGROUND_clear(0);

	ind = BACKGROUND_init_generalized(6, 0, PAL0, true, ind);
	fadeIn(60, target_palette, black_palette, PAL0);
	ind = MENU_init(ind);

	while (!bg_proceed)
	{
		ind = MENU_update(ind);
		update_input();
		SPR_update();
		SYS_doVBlankProcess();
	}
}

void level_alert()
{
	ind = BACKGROUND_init_generalized(LEVEL_current_level, 0, PAL0, TRUE, ind);
	fadeIn(60, target_palette, black_palette, PAL0);
	waitMs(1000);
	fadeOut(60);
	ind = BACKGROUND_full_clear(ind);
}
