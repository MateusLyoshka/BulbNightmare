#include "boss.h"

u16 BOSS_init(u16 ind)
{
    return ind;
}

void BOSS_flux()
{
    player_can_jump = 0;
    while (true)
    {
        GAME_update();
        SPR_update();
        SYS_doVBlankProcess();
    }
}