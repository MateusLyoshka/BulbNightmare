#include <genesis.h>
#include <maths.h>

#include "player.h"
// #include "level.h"
#include "utils.h"

GameObject player;

u16 PLAYER_init(u16 ind)
{
    ind += GAMEOBJECT_init(&player, &spr_player, SCREEN_W / 2 - 12, SCREEN_H / 2 - 12, PAL_PLAYER, ind);
    return ind;
}