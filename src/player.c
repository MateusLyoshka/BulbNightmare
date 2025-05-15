#include <genesis.h>
#include <maths.h>

#include "player.h"
#include "level.h"
#include "utils.h"

GameObject player;
f16 player_gravity = 30;
f16 player_speed = 70;

u16 PLAYER_init(u16 ind)
{
    ind += GAMEOBJECT_init(&player, &spr_player, SCREEN_W / 2 - 12, SCREEN_H / 2 + 10, PAL_PLAYER, ind);
    return ind;
}

void PLAYER_update()
{
    if (collision_map[player.x][player.y] == 2)
    {
        kprintf("caiu no espingas");
    }

    if (PLAYER_on_ground())
    {
        kprintf("oi blocos");
    }

    if (player_gravity > 0)
    {
        if (player.speed_y < PLAYER_MAX_GRAVITY)
        {
            player.speed_y += player_gravity;
        }
    }
    else
    {
        if (player.speed_y > -PLAYER_MAX_GRAVITY)
        {
            player.speed_y += player_gravity;
        }
    }
    // kprintf("speed: %i", player.speed_y);
    PLAYER_get_input_lr();

    player.next_x = player.x + player.speed_x;
    player.next_y = player.y + player.speed_y;

    LEVEL_move_and_slide(&player);

    player.x = player.next_x;
    player.y = player.next_y;

    SPR_setPosition(player.sprite, fix16ToInt(player.x), fix16ToInt(player.y));
    SPR_setAnim(player.sprite, player.anim);
}

void PLAYER_get_input_lr()
{
    player.speed_x = 0;
    // player.speed_y = 0;

    if (key_down(0, BUTTON_RIGHT))
    {
        player.speed_x = player_speed;
        player.anim = 2;
    }
    if (key_down(0, BUTTON_LEFT))
    {
        player.speed_x = -player_speed;
        player.anim = 2;
    }
    if (key_pressed(0, BUTTON_A) && PLAYER_on_ground())
    {
        player.speed_y = 0;
        player_gravity = -player_gravity;
        SPR_setVFlip(player.sprite, player_gravity < 0);
    }
    // if (key_down(0, BUTTON_UP))
    // {
    //     player.speed_y = -player_speed;
    //     player.anim = 0;
    // }
    // if (key_down(0, BUTTON_DOWN))
    // {
    //     player.speed_y = player_speed;
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

u8 PLAYER_on_ground()
{
    return ((LEVEL_wall_at(player.box.left, player.box.top) ||
             LEVEL_wall_at(player.box.left + player.w / 2, player.box.top) ||
             LEVEL_wall_at(player.box.right - 1, player.box.top)) ||
            (LEVEL_wall_at(player.box.left, player.box.bottom) ||
             LEVEL_wall_at(player.box.left + player.w / 2, player.box.bottom) ||
             LEVEL_wall_at(player.box.right - 1, player.box.bottom)));
}