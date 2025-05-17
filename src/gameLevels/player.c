#include <genesis.h>
#include <maths.h>

#include "player.h"

GameObject player;
f16 player_gravity = 30;
f16 player_speed = 70;
u8 player_is_alive = 1;

u16 PLAYER_init(u16 ind)
{
    ind += GAMEOBJECT_init(&player, &spr_player, 2 * METATILE_W, 12 * METATILE_W, PAL_PLAYER, ind);
    return ind;
}

void PLAYER_update()
{
    s16 player_center_x = fix16ToInt(player.x) + player.w / 2;
    s16 player_center_y = fix16ToInt(player.y) + player.h / 2;

    u16 center_x = player_center_x / METATILE_W;
    u16 center_y = player_center_y / METATILE_W;

    if (collision_map[center_x][center_y] == TOP_SPIKE_LEVEL_INDEX || collision_map[center_x][center_y] == BOTTOM_SPIKE_LEVEL_INDEX)
    {
        kprintf("Espinho no centro do player! (%d, %d)", center_x, center_y);
        // player_is_alive = 0;
    }

    if (player_is_alive)
    {
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
}

void PLAYER_get_input_lr()
{
    player.speed_x = 0;
    // player.speed_y = 0;

    if (key_down(0, BUTTON_RIGHT))
    {
        player.speed_x = player_speed;
        player.anim = 0;
    }
    if (key_down(0, BUTTON_LEFT))
    {
        player.speed_x = -player_speed;
        player.anim = 0;
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