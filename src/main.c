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

#define _MASK

// ==============================
// Variáveis globais
// ==============================
u16 ind = TILE_USER_INDEX + 48;
u16 sprites_ind = 0;
u8 enemies_current_level;
u8 enemies_past_level;

// ==============================
// Prototipagem
// ==============================
void game_init();
void game_update();
void menu_init();
void level_change();
void screen_change();
void player_death();

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

	OBJECT_params();
	dark_ind = HUD_background(dark_ind);
	SYS_doVBlankProcess();
	game_init();

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
	if (!player_is_alive)
	{
		player_death();
	}

	if (LEVEL_bool_screen_change)
	{
		screen_change();
	}
	if (LEVEL_bool_level_change)
	{
		level_change();
	}

	MASK_scroll_update();
	update_input();
	PLAYER_update();
	LEVEL_update_camera(&player);

	// ENEMIES_update_hub(enemies_current_level, enemies_past_level);
	HUD_update();
}

void game_init()
{
	// ENEMIES_init();
	ind = LEVEL_alert(ind);
#ifdef _MASK
	MASK_scroll_init();
	MASK_draw(dark_ind);
#endif
	ind = LEVEL_init(ind);
	ind = PLAYER_init(ind);
	sprites_ind = ind; // marca onde começou a somar indices de sprites
	// ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = HUD_init(ind);
	ind = OBJECT_update(ind);
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

void level_change()
{
	player_have_key = 0;
	// ENEMIES_level_change_despawn(enemies_current_level, enemies_past_level);
	if (player_lives)
	{
		LEVEL_current_level += 1;
	}
	else
	{
		LEVEL_current_level = 0;
	}

	enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
	enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];
	SYS_doVBlankProcess();
	game_init();
	PLAYER_respawn();
	player_is_alive = 1;
	LEVEL_bool_level_change = 0;
}

void screen_change()
{
	ind -= (ind - sprites_ind); // ind retornar para onde começaram carregar as sprites
	// ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = OBJECT_update(ind);
#ifdef _MASK
	MASK_draw();
#endif
	LEVEL_bool_screen_change = 0;
}

void player_death()
{
	player_lives -= 1;
	if (!player_lives)
	{
		level_change();
	}
	else
	{
		LEVEL_scroll_update_collision(0, 448);
		PLAYER_respawn();
		player_is_alive = 1;
	}
}
