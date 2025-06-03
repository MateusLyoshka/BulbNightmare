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

const u16 glow_light_blue_1[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x030E1E),
	RGB24_TO_VDPCOLOR(0x071D3D),
	RGB24_TO_VDPCOLOR(0x0A2B5C),
	RGB24_TO_VDPCOLOR(0x0E3A7B),
	RGB24_TO_VDPCOLOR(0x12489A),
	RGB24_TO_VDPCOLOR(0x1D66B8),
	RGB24_TO_VDPCOLOR(0x35AAF2)};

const u16 glow_light_blue_2[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x001113),
	RGB24_TO_VDPCOLOR(0x002226),
	RGB24_TO_VDPCOLOR(0x003439),
	RGB24_TO_VDPCOLOR(0x01454C),
	RGB24_TO_VDPCOLOR(0x02575F),
	RGB24_TO_VDPCOLOR(0x026872),
	RGB24_TO_VDPCOLOR(0x037A85),
	RGB24_TO_VDPCOLOR(0x038B98),
	RGB24_TO_VDPCOLOR(0x0396A6)};

const u16 glow_medium_blue[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x00020B),
	RGB24_TO_VDPCOLOR(0x000516),
	RGB24_TO_VDPCOLOR(0x000722),
	RGB24_TO_VDPCOLOR(0x010A2D),
	RGB24_TO_VDPCOLOR(0x010C39),
	RGB24_TO_VDPCOLOR(0x010F44),
	RGB24_TO_VDPCOLOR(0x020659)};

const u16 glow_dark_blue[] = {
	RGB24_TO_VDPCOLOR(0x000000), // Preto
	RGB24_TO_VDPCOLOR(0x00020B),
	RGB24_TO_VDPCOLOR(0x000516),
	RGB24_TO_VDPCOLOR(0x000722),
	RGB24_TO_VDPCOLOR(0x010A2D),
	RGB24_TO_VDPCOLOR(0x010C39),
	RGB24_TO_VDPCOLOR(0x010F44),
	RGB24_TO_VDPCOLOR(0x011150),
	RGB24_TO_VDPCOLOR(0x01145B),
	RGB24_TO_VDPCOLOR(0x020659) // Azul profundo final
};

const u16 glow_olive_medium[] = {
	RGB24_TO_VDPCOLOR(0x000000), // Preto
	RGB24_TO_VDPCOLOR(0x121105),
	RGB24_TO_VDPCOLOR(0x24220A),
	RGB24_TO_VDPCOLOR(0x363410),
	RGB24_TO_VDPCOLOR(0x484515),
	RGB24_TO_VDPCOLOR(0x5A561A),
	RGB24_TO_VDPCOLOR(0x646322),
	RGB24_TO_VDPCOLOR(0x6B692A),
	RGB24_TO_VDPCOLOR(0x8C8C34),
	RGB24_TO_VDPCOLOR(0x8C8C34) // Amarelo-esverdeado final
};

const u16 glow_olive_dark[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x111009),
	RGB24_TO_VDPCOLOR(0x221E12),
	RGB24_TO_VDPCOLOR(0x332C1B),
	RGB24_TO_VDPCOLOR(0x443B24),
	RGB24_TO_VDPCOLOR(0x55492D),
	RGB24_TO_VDPCOLOR(0x665736),
	RGB24_TO_VDPCOLOR(0x595736), // Final exato
	RGB24_TO_VDPCOLOR(0x595736), // Repetido para consistência
	RGB24_TO_VDPCOLOR(0x595736)};

const u16 glow_olive_bright[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x1A1A09),
	RGB24_TO_VDPCOLOR(0x333312),
	RGB24_TO_VDPCOLOR(0x4D4D1B),
	RGB24_TO_VDPCOLOR(0x666624),
	RGB24_TO_VDPCOLOR(0x80802D),
	RGB24_TO_VDPCOLOR(0x8A8A30),
	RGB24_TO_VDPCOLOR(0x909034), // Final exato
	RGB24_TO_VDPCOLOR(0x909034),
	RGB24_TO_VDPCOLOR(0x909034)};

ColorParams color_params_vec[6];

static inline void color_effects(ColorParams *color_params, u8 max_elems, u8 fade_type)
{
	--bg_color_delay;
	if (bg_color_delay == 0)
	{
		for (u8 i = 0; i < max_elems; i++)
		{
			glow_color(color_params[i].color_ind, color_params_vec[i].glow, color_params_vec[i].glow_elems, &color_params_vec[i].glow_current_elem, fade_type);
		}
		bg_color_delay = 8;
	}
}

void Color_init(ColorParams *color_params, u8 pos, u16 *glow, u8 glow_elems, u16 color_ind)
{
	color_params[pos].color_ind = color_ind;
	color_params[pos].glow_elems = glow_elems;
	color_params[pos].glow_current_elem = 0;
	color_params[pos].glow = glow;
}

u16 black_palette[64] = {0};
u16 target_palette[64];

static inline void fade_proceed(u8 fade_type, u8 color_elems)
{
	while (!bg_proceed)
	{
		color_effects(color_params_vec, color_elems, fade_type);
		SYS_doVBlankProcess();
	}
	bg_proceed = 0;
}

void fadeIn(u16 speed)
{
	// Versão correta do PAL_fadeIn no SGDK atual:
	// void PAL_fadeIn(u16 fromCol, u16 toCol, const u16* pal, u16 numFrame, bool async);

	// Primeiro definimos a paleta preta
	PAL_setPalette(PAL0, black_palette, DMA);

	// Depois fazemos o fade in para a paleta alvo
	PAL_fadeIn(0, 63, target_palette, speed, FALSE);
}

void fadeOut(u16 speed)
{
	// Versão correta do PAL_fadeOut no SGDK atual:
	// void PAL_fadeOut(u16 fromCol, u16 toCol, u16 numFrame, bool async);

	PAL_fadeOut(0, 63, speed, FALSE);
}

int main(bool resetType)
{
	VDP_setScreenWidth320();
	SPR_init();
	VDP_setPlaneSize(64, 64, true);

	if (!resetType)
	{
		SYS_hardReset();
	}

	SYS_showFrameLoad(true);

	ind = BACKGROUND_init_generalized(2, 0, PAL0, ind);

	// Obtém a paleta atual (para o fade in) - versão correta
	PAL_getPalette(PAL0, target_palette);

	// Define a paleta como preta inicialmente - versão correta
	PAL_setPalette(PAL0, black_palette, DMA);

	// Faz fade in
	fadeIn(60); // 60 frames para o fade in

	// Aguarda 2 segundos
	for (u16 i = 0; i < 120; i++)
		SYS_doVBlankProcess();

	// Faz fade out
	fadeOut(60); // 60 frames para o fade out

	while (1)
	{
		SYS_doVBlankProcess();
	}

	// Incia o vetor com o vetor das cores de brilho da logo
	// Color_init(color_params_vec, 0, (u16 *)glow_light_blue_1, 8, PAL_BACKGROUND_B * 16 + 1);
	// Color_init(color_params_vec, 1, (u16 *)glow_medium_blue, 8, PAL_BACKGROUND_B * 16 + 2);

	// Mostra logo da utf, fade in e fade out
	// ind = BACKGROUND_init_generalized(0, 0, PAL_BACKGROUND_B, true, ind);

	// fade_proceed(1, 2);
	// bg_proceed = 0;
	// waitMs(1000);

	// fade_proceed(0, 2);

	// bg_proceed = 0;
	// waitMs(1000);

	// Color_init(color_params_vec, 0, (u16 *)glow_dark_blue, 10, PAL_BACKGROUND_B * 16 + 1);
	// Color_init(color_params_vec, 1, (u16 *)glow_medium_blue, 10, PAL_BACKGROUND_B * 16 + 2);
	// Color_init(color_params_vec, 2, (u16 *)glow_light_blue_2, 10, PAL_BACKGROUND_B * 16 + 3);
	// Color_init(color_params_vec, 3, (u16 *)glow_olive_dark, 10, PAL_BACKGROUND_B * 16 + 4);
	// Color_init(color_params_vec, 4, (u16 *)glow_olive_medium, 10, PAL_BACKGROUND_B * 16 + 5);

	// ind = BACKGROUND_init_generalized(1, 0, PAL_BACKGROUND_B, true, ind);

	// fade_proceed(1, 5);
	// // ind = BACKGROUND_clean(0);
	// bg_proceed = 0;
	// ind = MENU_init(ind);
	// while (!bg_proceed)
	// {
	// 	ind = MENU_update(ind);
	// 	update_input();
	// 	SPR_update();
	// 	SYS_doVBlankProcess();
	// 	/* code */
	// }
	// fade_proceed(0, 5);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	VDP_drawImageEx(BG_A, &dark_mask, TILE_ATTR_FULL(PAL_BACKGROUND_A, 1, 0, 0, ind), 0, 0, true, DMA);
	ind += dark_mask.tileset->numTile;
	// Seta a paleta principal do jogo
	enemies_current_level = ENEMIES_enemies_on_level[LEVEL_current_level + 1];
	enemies_past_level = ENEMIES_enemies_on_level[LEVEL_current_level];

	game_init();

	SYS_doVBlankProcess();

	while (true)
	{
		game_update();
		VDP_setHorizontalScroll(BG_A, player_center.x - 34);
		VDP_setVerticalScroll(BG_A, -player_center.y + 34);
		kprintf("%d, %d", player_center.x, player_center.y);
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

		ind = TILE_USER_INDEX;
		game_init();
		player_is_alive = 1;
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
	ind = LEVEL_init(ind);
	ind = PLAYER_init(ind);
	ind = HUD_init(ind);
	ENEMIES_init();
	ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
	ind = OBJECT_update(ind, TRUE);
	LEVEL_update_camera(&player);
}
