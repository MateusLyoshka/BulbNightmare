#include <genesis.h>
#include <maths.h>

#include "player.h"
#include "level.h"
#include "utils.h"

GameObject player;
f16 player_gravity = 2;

u16 PLAYER_init(u16 ind)
{
    ind += GAMEOBJECT_init(&player, &spr_player, SCREEN_W / 2 - 12, SCREEN_H / 2 + 10, PAL_PLAYER, ind);
    return ind;
}

void PLAYER_update()
{
    player.speed_y += 2;
    player.next_y = player.y + player.speed_y;
    PLAYER_get_input_lr();

    player.next_x = player.x + player.speed_x;
    // player.next_y = player.y + player.speed_y;

    LEVEL_move_and_slide(&player);

    player.x = player.next_x;
    player.y = player.next_y;

    SPR_setPosition(player.sprite, fix16ToInt(player.x), fix16ToInt(player.y));
    SPR_setAnim(player.sprite, player.anim);
}

void PLAYER_get_input_lr()
{
    player.speed_x = 0;
    player.speed_y = 0;

    if (key_down(0, BUTTON_RIGHT))
    {
        player.speed_x = PLAYER_SPEED;
        player.anim = 0;
    }
    if (key_down(0, BUTTON_LEFT))
    {
        player.speed_x = -PLAYER_SPEED;
        player.anim = 0;
    }
    if (key_pressed(0, BUTTON_A))
    {
        player_gravity = -player_gravity;
        SPR_setVFlip(player.sprite, player_gravity < 0);
    }
    // if (key_down(0, BUTTON_UP))
    // {
    //     player.speed_y = -PLAYER_SPEED;
    //     player.anim = 0;
    // }
    // if (key_down(0, BUTTON_DOWN))
    // {
    //     player.speed_y = PLAYER_SPEED;
    //     player.anim = 0;
    // }
    if (key_released(0, BUTTON_RIGHT))
    {
        player.anim = 0;
    }
    if (key_released(0, BUTTON_LEFT))
    {
        player.anim = 0;
    }
}