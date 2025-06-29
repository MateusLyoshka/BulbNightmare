
#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

#include "../screenElements/background.h"
#include "../utils/gameobject.h"
#include "../utils/utils.h"
#include "../screenElements/level.h"
#include "../screenElements/hud.h"
#include "../enemies/enemies.h"
#include "../player/player.h"
#include "../screenElements/objects.h"
#include "menu.h"
#include "../screenElements/darkness.h"
#include "pause.h"
#include "../enemies/boss.h"

extern const u16 *game_palettes[6];

void GAME_end_game();
void GAME_init_sounds();
void GAME_init_palettes();
void GAME_update_palletes();
u16 GAME_init();
void GAME_update();
void GAME_menu_init();
void GAME_level_change();
void GAME_screen_change();
void GAME_player_death();
void GAME_pause_init();
void GAME_reset();
void GAME_mask_init();