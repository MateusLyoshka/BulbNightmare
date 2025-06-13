#include "boss.h"

u16 boss_ind = TILE_USER_INDEX;

u16 BOSS_init(u16 ind)
{
    return ind;
}

void BOSS_flux()
{
    // boss_ind = GAME_init();
    boss_ind = LEVEL_init(boss_ind);
    boss_ind = PLAYER_init(boss_ind);

    LEVEL_update_camera(&player);
    // boss_ind = LEVEL_init(boss_ind);
    // BACKGROUND_init_generalized(10, 1, PAL0, false, false, boss_ind);

    SPR_update();
    SYS_doVBlankProcess();
    PLAYER_respawn();

    player_can_jump = 0;
    while (true)
    {
        update_input();
        PLAYER_update();
        LEVEL_update_camera(&player);
        SPR_update();
        SYS_doVBlankProcess();
    }
}