#include <genesis.h>
#include <maths.h>

#include "player.h"

GameObject player;
ObjectConfig *collided;

PlayerCenter player_center;
PlayerSpawnPoint player_spawn;
f16 player_gravity = 45;
f16 player_speed = 70;
u8 player_is_alive = 1;
u8 player_keys = 0;
u8 player_lives = 3;
u8 switchs_on = 0;
u8 player_can_jump = 1;

u16 PLAYER_init(u16 ind)
{
    if (player.sprite != NULL)
    {
        SPR_releaseSprite(player.sprite);
    }

    ind += GAMEOBJECT_init(&player, &spr_player,
                           fix16ToInt(player_spawn.initial_x),
                           fix16ToInt(player_spawn.initial_y),
                           PAL_GAME, false, ind);
    player_lives = 3;
    return ind;
}

void PLAYER_center_update()
{

    player_center.x = fix16ToInt(player.x) + player.w / 2;
    player_center.y = fix16ToInt(player.y) + player.h / 2;

    // Centro convertido para tiles
    player_center.tile_x = player_center.x / METATILE_W;
    player_center.tile_y = player_center.y / METATILE_W;
}

void PLAYER_update()
{
    PLAYER_center_update();
    PLAYER_check_collisions();

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
        PLAYER_get_input();

        player.next_x = player.x + player.speed_x;
        player.next_y = player.y + player.speed_y;

        LEVEL_move_and_slide(&player);

        player.x = player.next_x;
        player.y = player.next_y;

        SPR_setPosition(player.sprite, fix16ToInt(player.x), fix16ToInt(player.y));
        SPR_setAnim(player.sprite, player.anim);
    }
}

void PLAYER_get_input()
{
    player.speed_x = 0;
    // player.speed_y = 0;

    if (key_down(0, BUTTON_RIGHT))
    {
        player.speed_x = player_speed;
        SPR_setHFlip(player.sprite, false);
        player.anim = 4;
    }
    else if (key_down(0, BUTTON_LEFT))
    {
        player.speed_x = -player_speed;
        SPR_setHFlip(player.sprite, true);
        player.anim = 4;
    }
    else
    {
        player.anim = 0;
    }
    if (key_pressed(0, BUTTON_A) && PLAYER_on_ground() && player_can_jump)
    {
        player.speed_y = 0;
        player_gravity = -player_gravity;
        SPR_setVFlip(player.sprite, player_gravity < 0);
    }
    if (key_released(0, BUTTON_RIGHT))
    {
        player.anim = 0;
    }
    if (key_released(0, BUTTON_LEFT))
    {
        player.anim = 0;
    }
    if (!PLAYER_on_ground())
    {
        player.anim = 2;
    }
#ifdef _FLY
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
#endif
}

void PLAYER_object_collision()
{
    collided = OBJECT_check_collision(player_center.x, player_center.y);

    if (collided)
    {
        if (collided->type == 2)
        {
            kprintf("Colidiu com a CHAVE!");
            player_keys += 1;
            OBJECT_clear(collided, true);
            // collided = NULL;
        }
        else if (collided->type == 0 && player_keys == keys_on_level[LEVEL_current_level])
        {
            kprintf("Colidiu com a PORTA!");

            SPR_setVisibility(player.sprite, HIDDEN);
            SPR_setAnim(collided->obj.sprite, 1);
            for (u16 i = 0; i < 40; i++)
            {
                SPR_update();
                SYS_doVBlankProcess();
            }

            PLAYER_respawn();
            LEVEL_bool_level_change = 1;
        }
        else if (collided->type == 1)
        {
            if (key_down(0, BUTTON_B) && room_lights[LEVEL_current_screen] == 0)
            {
                switchs_on += 1;
                room_lights[LEVEL_current_screen] = 1;
                SPR_setAnim(collided->obj.sprite, 1);
                collided->obj.anim = 1;
            }

            // OBJECT_collect(collided);
            // collided = NULL;
        }
        else
        {
            kprintf("Colidiu com objeto desconhecido!");
        }
    }
}

void PLAYER_spike_collision()
{
    if (collision_map[player_center.tile_x][player_center.tile_y] == TOP_SPIKE_LEVEL_INDEX || collision_map[player_center.tile_x][player_center.tile_y] == BOTTOM_SPIKE_LEVEL_INDEX)
    {
        // kprintf("Espinho no centro do player! (%d, %d)", player_center.tile_x, player_center.tile_y)
        kprintf("colidiu com espinho");
        player_is_alive = 0;
        return;
    }
}

void PLAYER_enemy_collision()
{

    for (u8 i = ENEMIES_enemies_on_level[LEVEL_current_level]; i < ENEMIES_enemies_on_level[LEVEL_current_level + 1]; i++)
    {
        if (enemy_pool[i].firefly.sprite == NULL)
            continue; // inimigo inexistente

        GameObject *e = &enemy_pool[i].firefly;

        if (player_center.x >= e->box.left && player_center.x <= e->box.right &&
            player_center.y >= e->box.top && player_center.y <= e->box.bottom)
        {
            // kprintf("Colidiu com inimigo %d!", i);
            player_is_alive = 0;
            return;
        }
    }
}

void PLAYER_check_collisions()
{
    PLAYER_enemy_collision();
    PLAYER_spike_collision();
    PLAYER_object_collision();
}

void PLAYER_respawn()
{
    if (!player_is_alive)
    {
        SPR_setAnim(player.sprite, 8);
        for (u16 i = 0; i < 20; i++)
        {
            SPR_update();
            SYS_doVBlankProcess();
        }
    }

    // Reposiciona o player
    player.x = player_spawn.initial_x;
    player.y = player_spawn.initial_y;

    // Reseta velocidades
    player.speed_x = 0;
    player.speed_y = 0;

    // player_keys = 0;

    // Atualiza a posição do sprite
    SPR_setPosition(player.sprite,
                    fix16ToInt(player.x),
                    fix16ToInt(player.y));

    player_gravity = abs(player_gravity); // Garante gravidade para baixo
    SPR_setVFlip(player.sprite, FALSE);

    SPR_setAnim(player.sprite, 9);
    for (u16 i = 0; i < 36; i++)
    {
        SPR_update();
        SYS_doVBlankProcess();
    }
    player.anim = 0;
    SPR_setAnim(player.sprite, player.anim);
}

void PLAYER_free()
{
    SPR_releaseSprite(player.sprite);
    player.sprite = NULL;
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