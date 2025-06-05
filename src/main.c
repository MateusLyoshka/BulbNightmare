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
#include "screenElements/darkness.h"

// ==============================
// Variáveis globais
// ==============================
u16 ind = TILE_USER_INDEX + 32;
u8 enemies_current_level;
u8 enemies_past_level;

// ==============================
// Prototipagem
// ==============================
void game_init();
void game_update();
void menu_init();

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
	PAL_setColor(1, RGB24_TO_VDPCOLOR(0xFFFFFF));

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
		// MASK_draw();
		kprintf("%d aaaa", ind);
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

	MASK_scroll_update();
	update_input();
	PLAYER_update();
	LEVEL_update_camera(&player);

	ENEMIES_update_hub(enemies_current_level, enemies_past_level);
	HUD_update();
}

void game_init()
{
	// ind = LEVEL_alert(ind);
	MASK_scroll_init();
	MASK_draw(dark_ind);
	kprintf("%d a", ind);
	ind = LEVEL_init(ind);
	ind = PLAYER_init(ind);
	ind = HUD_init(ind);
	ENEMIES_init();
	ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = OBJECT_update(ind, TRUE);
	LEVEL_update_camera(&player);
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
