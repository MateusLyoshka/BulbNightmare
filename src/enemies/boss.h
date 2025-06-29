#ifndef _BOSS
#define _BOSS

#include "../screenElements/background.h"
#include "../utils/gameobject.h"
#include "../utils/utils.h"
#include "../screenElements/level.h"
#include "../screenElements/hud.h"
#include "../player/player.h"
#include "../screenElements/objects.h"
#include "../gameFlow/menu.h"
#include "../screenElements/darkness.h"
#include "../gameFlow/pause.h"
#include "../gameFlow/game.h"
#include "../utils/globals.h"

extern GameObject enchant;

void BOSS_init();
void BOSS_flux();
void BOSS_flux_update(u8 mask_bool);
void BOSS_speak_anim();
void BOSS_transform_anim();
u16 BOSS_random_crown_anim(u16 min_val, u16 max_val, u16 step);
void BOSS_laugh_anim();
void BOSS_clear();
void BOSS_power(u16 ind);

#endif
