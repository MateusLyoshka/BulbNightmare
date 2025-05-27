#include <genesis.h>
#include <maths.h>

#include "player.h"

GameObject player;
f16 player_gravity = 30;
f16 player_speed = 70;
u8 player_is_alive = 1;
u8 player_have_key = 0;
GameObject *collided;

u16 PLAYER_init(u16 ind)
{
    ind += GAMEOBJECT_init(&player, &spr_player, 2 * METATILE_W, 12 * METATILE_W, PAL_GAME, ind);
    return ind;
}

void PLAYER_update()
{
    PLAYER_check_death();

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
        player.anim = 8;
    }
    if (key_down(0, BUTTON_LEFT))
    {
        player.speed_x = -player_speed;
        player.anim = 8;
    }
    if (key_pressed(0, BUTTON_A) && PLAYER_on_ground())
    {
        player.speed_y = 0;
        player_gravity = -player_gravity;
        SPR_setVFlip(player.sprite, player_gravity < 0);
    }
    if (key_down(0, BUTTON_UP))
    {
        player.speed_y = -player_speed;
        player.anim = 8;
    }
    if (key_down(0, BUTTON_DOWN))
    {
        player.speed_y = player_speed;
        player.anim = 8;
    }
    if (key_released(0, BUTTON_RIGHT))
    {
        player.anim = 8;
    }
    if (key_released(0, BUTTON_LEFT))
    {
        player.anim = 8;
    }
}

void PLAYER_check_death()
{
    // Centro do jogador
    s16 center_x_px = fix16ToInt(player.x) + player.w / 2;
    s16 center_y_px = fix16ToInt(player.y) + player.h / 2;

    // Centro convertido para tiles
    u16 tile_x = center_x_px / METATILE_W;
    u16 tile_y = center_y_px / METATILE_W;

    // Verifica espinhos
    if (collision_map[tile_x][tile_y] == TOP_SPIKE_LEVEL_INDEX || collision_map[tile_x][tile_y] == BOTTOM_SPIKE_LEVEL_INDEX)
    {
        kprintf("Espinho no centro do player! (%d, %d)", tile_x, tile_y);
        player_is_alive = 1;
        return;
    }

    collided = OBJECT_check_collision(center_x_px, center_y_px);

    if (collided)
    {
        if (collided == &key)
        {
            kprintf("Colidiu com a CHAVE!");
            player_have_key = 1;
            OBJECT_clear(collided);
            // collided = NULL;
        }
        else if (collided == &door && player_have_key)
        {
            kprintf("Colidiu com a PORTA!");
        }
        else if (collided == &powerup)
        {
            kprintf("Colidiu com o POWERUP!");
            OBJECT_clear(collided);
            // collided = NULL;
        }
        else
        {
            kprintf("Colidiu com objeto desconhecido!");
        }
    }

    // Verifica inimigos
    // for (u8 i = ENEMIES_level_enemies[LEVEL_current_level]; i < ENEMIES_level_enemies[LEVEL_current_level + 1]; i++)
    // {
    //     if (enemy_pool[i].firefly.sprite == NULL)
    //         continue; // inimigo inexistente

    //     GameObject *e = &enemy_pool[i].firefly;

    //     if (center_x_px >= e->box.left && center_x_px <= e->box.right &&
    //         center_y_px >= e->box.top && center_y_px <= e->box.bottom)
    //     {
    //         kprintf("Colidiu com inimigo %d!", i);
    //         player_is_alive = 0;
    //         return;
    //     }
    // }
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