#include <genesis.h>
#include <maths.h>

#include "player.h"

GameObject player;
GameObject *collided;

PlayerCenter player_center;
PlayerSpawnPoint player_spawn;
f16 player_gravity = 30;
f16 player_speed = 70;
u8 player_is_alive = 1;
u8 player_have_key = 0;

u16 PLAYER_init(u16 ind)
{
    if (player.sprite != NULL)
    {
        SPR_releaseSprite(player.sprite);
    }

    player_spawn.initial_x = intToFix16(2 * METATILE_W);
    player_spawn.initial_y = intToFix16(12 * METATILE_W);

    ind += GAMEOBJECT_init(&player, &spr_player,
                           fix16ToInt(player_spawn.initial_x),
                           fix16ToInt(player_spawn.initial_y),
                           PAL_GAME, ind);
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
    if (key_pressed(0, BUTTON_A) && PLAYER_on_ground())
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
        if (collided == &key)
        {
            kprintf("Colidiu com a CHAVE!");
            player_have_key = 1;
            OBJECT_collect(collided);
            // collided = NULL;
        }
        else if (collided == &door && player_have_key)
        {
            // kprintf("Colidiu com a PORTA!");
            PLAYER_respawn();
            LEVEL_bool_level_change = 1;
        }
        else if (collided == &powerup)
        {
            // kprintf("Colidiu com o POWERUP!");
            if (key_down(0, BUTTON_B))
            {
                room_lights[LEVEL_current_screen] = 1;
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
        // kprintf("Espinho no centro do player! (%d, %d)", player_center.tile_x, player_center.tile_y);
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
    if (!player_is_alive)
    {
        PLAYER_respawn();
    }
}

void PLAYER_respawn()
{
    // Reposiciona o player
    player.x = player_spawn.initial_x;
    player.y = player_spawn.initial_y;

    // Reseta velocidades
    player.speed_x = 0;
    player.speed_y = 0;

    // player_have_key = 0;

    // Atualiza a posição do sprite
    SPR_setPosition(player.sprite,
                    fix16ToInt(player.x),
                    fix16ToInt(player.y));

    // Reseta animação se necessário
    player.anim = 0;
    SPR_setAnim(player.sprite, player.anim);

    // Reseta gravidade se necessário
    player_gravity = abs(player_gravity); // Garante gravidade para baixo
    SPR_setVFlip(player.sprite, FALSE);
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