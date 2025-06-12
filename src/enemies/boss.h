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

u16 BOSS_init(u16 ind);
void BOSS_flux();

#endif
