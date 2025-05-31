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

typedef struct
{
	u16 *glow;
	u8 glow_elems;
	u8 glow_current_elem;
	u16 color_ind;
} ColorParams;

ColorParams color_params_vec[2];

// ==============================
// Variáveis globais
// ==============================
u16 ind = TILE_USER_INDEX;
u8 enemies_current_level;
u8 enemies_past_level;

u8 proceed = 0;

// ==============================
// Prototipagem
// ==============================
void game_init();
void game_update();

const u16 logo_color_glow_0[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x0D2A3C),
	RGB24_TO_VDPCOLOR(0x1A5579),
	RGB24_TO_VDPCOLOR(0x277FB5),
	RGB24_TO_VDPCOLOR(0x35AAF2)};

// logo_color_glow_2: de preto até azul profundo #020659
const u16 logo_color_glow_1[] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x010116),
	RGB24_TO_VDPCOLOR(0x01032B),
	RGB24_TO_VDPCOLOR(0x010442),
	RGB24_TO_VDPCOLOR(0x020659)};

u8 color_delay = 5;

static inline void glow_color(u16 color_index, const u16 *const color_vector, u8 n, u8 *current_elem, u8 fade_type)
{
	static u8 inc = 1;

	// kprintf("%d", *current_elem);
	if (fade_type)
	{
		*current_elem += inc;
	}
	else
	{
		*current_elem -= inc;
	}
	PAL_setColor(color_index, color_vector[*current_elem]);
	kprintf("%d", *current_elem);
	if (*current_elem == 0 || *current_elem == n - 1)
	{
		proceed = 1;
		return;
	}
}

static inline void color_effects(ColorParams *color_params, u8 max_elems, u8 fade_type)
{
	--color_delay;
	if (color_delay == 0)
	{
		for (u8 i = 0; i < max_elems; i++)
		{
			glow_color(color_params[i].color_ind, color_params_vec[i].glow, color_params_vec[i].glow_elems, &color_params_vec[i].glow_current_elem, fade_type);
		}
		color_delay = 20;
	}
}

void Color_init(ColorParams *color_params, u8 pos, u16 *glow, u8 glow_elems, u16 color_ind)
{
	color_params[pos].color_ind = color_ind;
	color_params[pos].glow_elems = glow_elems;
	color_params[pos].glow_current_elem = 0;
	color_params[pos].glow = glow;
}

int main(bool resetType)
{
	VDP_setScreenWidth320();
	SPR_init();

	Color_init(color_params_vec, 0, (u16 *)logo_color_glow_0, 5, PAL1 * 16 + 1);
	Color_init(color_params_vec, 1, (u16 *)logo_color_glow_1, 5, PAL1 * 16 + 2);

	if (!resetType)
	{
		SYS_hardReset();
	}

	SYS_showFrameLoad(true);

	for (int i = 0; i < 60; i++)
	{
		SYS_doVBlankProcess();
	}
	// Inicializa o fundo azul escuro
	// PAL_setPalette(PAL_GAME, game_pal.data, DMA);
	ind = BACKGROUND_init(ind);
	ind = BACKGROUND_logo_init(ind);

	// Desenha a logo com cores zeradas (preto)

	// Espera um tempo antes de iniciar o fade (opcional)

	while (!proceed)
	{
		color_effects(color_params_vec, 2, 1);
		SYS_doVBlankProcess();
	}
	proceed = 0;
	for (int i = 0; i < 60; i++)
	{
		SYS_doVBlankProcess();
	}
	while (!proceed)
	{
		color_effects(color_params_vec, 2, 0);
		SYS_doVBlankProcess();
	}
	while (true)
	{
		/* code */
	}

	// Seta a paleta principal do jogo

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
