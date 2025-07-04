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
u8 player_lives = 0;
u8 switchs_on = 0;
u8 player_cheat_on = 0;
u8 player_can_jump = 1;
u8 player_can_walk = 1;

/**
 * @brief Initializes the player.
 * @param ind VRAM tile index.
 * @return Next available VRAM tile index.
 */
u16 PLAYER_init(u16 ind)
{
    if (player.sprite != NULL)
        SPR_releaseSprite(player.sprite);

    ind += GAMEOBJECT_init(&player, &spr_player,
                           fix16ToInt(player_spawn.initial_x),
                           fix16ToInt(player_spawn.initial_y),
                           PAL_GAME, false, ind);
    player_lives = PLAYER_LIVES;
    return ind;
}

/**
 * @brief Updates player's center position and tile.
 */
void PLAYER_center_update()
{
    player_center.x = fix16ToInt(player.x) + player.w / 2;
    player_center.y = fix16ToInt(player.y) + player.h / 2;
    player_center.tile_x = player_center.x / METATILE_W;
    player_center.tile_y = player_center.y / METATILE_W;
}

/**
 * @brief Player main update logic.
 */
void PLAYER_update()
{
    PLAYER_center_update();
    PLAYER_check_collisions();

    if (player_is_alive)
    {
        if (player_gravity > 0)
        {
            if (player.speed_y < PLAYER_MAX_GRAVITY)
                player.speed_y += player_gravity;
        }
        else
        {
            if (player.speed_y > -PLAYER_MAX_GRAVITY)
                player.speed_y += player_gravity;
        }

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

/**
 * @brief Handles input and movement.
 */
void PLAYER_get_input()
{
    player.speed_x = 0;

    if (key_down(0, BUTTON_RIGHT) && player_can_walk)
    {
        player.speed_x = player_speed;
        SPR_setHFlip(player.sprite, false);
        player.anim = 4;
    }
    else if (key_down(0, BUTTON_LEFT) && player_can_walk)
    {
        player.speed_x = -player_speed;
        SPR_setHFlip(player.sprite, true);
        player.anim = 4;
    }
    else
        player.anim = 0;

    if (key_pressed(0, BUTTON_A) && PLAYER_on_ground() && player_can_jump)
    {
        XGM_startPlayPCM(69, 1, SOUND_PCM_CH1);
        player.speed_y = 0;
        player_gravity = -player_gravity;
        SPR_setVFlip(player.sprite, player_gravity < 0);
    }

    if (key_released(0, BUTTON_RIGHT) || key_released(0, BUTTON_LEFT))
        player.anim = 0;

    if (!PLAYER_on_ground())
        player.anim = 2;

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

/**
 * @brief Inverts gravity.
 */
void PLAYER_invert_gravity()
{
    player.speed_y = 0;
    player_gravity = -player_gravity;
    SPR_setVFlip(player.sprite, player_gravity < 0);
}

/**
 * @brief Handles collisions with objects.
 */
void PLAYER_object_collision()
{
    collided = OBJECT_check_collision(player_center.x, player_center.y);

    if (collided)
    {
        if (collided->type == 2)
        {
            XGM_startPlayPCM(67, 1, SOUND_PCM_CH1);
            player_keys++;
            OBJECT_clear(collided, true);
        }
        else if (collided->type == 0 && player_keys == keys_on_level[LEVEL_current_level])
        {
            XGM_startPlayPCM(72, 1, SOUND_PCM_CH1);
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
                XGM_startPlayPCM(68, 1, SOUND_PCM_CH1);
                switchs_on++;
                room_lights[LEVEL_current_screen] = 1;
                SPR_setAnim(collided->obj.sprite, 1);
                collided->obj.anim = 1;
            }
        }
        else
        {
            kprintf("Unknown object collision!");
        }
    }
}

/**
 * @brief Handles spike collisions.
 */
void PLAYER_spike_collision()
{
    if (collision_map[player_center.tile_x][player_center.tile_y] == TOP_SPIKE_LEVEL_INDEX ||
        collision_map[player_center.tile_x][player_center.tile_y] == BOTTOM_SPIKE_LEVEL_INDEX)
    {
        if (!player_cheat_on)
            player_is_alive = 0;
    }
}

/**
 * @brief Handles enemy collisions.
 */
void PLAYER_enemy_collision()
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        if (enemy_pool[i].firefly.sprite == NULL)
            continue;

        GameObject *e = &enemy_pool[i].firefly;

        if (player_center.x >= e->box.left && player_center.x <= e->box.right &&
            player_center.y >= e->box.top && player_center.y <= e->box.bottom)
        {
            if (!player_cheat_on)
                player_is_alive = 0;
            return;
        }
    }
}

/**
 * @brief Checks all player collisions.
 */
void PLAYER_check_collisions()
{
    PLAYER_enemy_collision();
    PLAYER_spike_collision();
    PLAYER_object_collision();
}

/**
 * @brief Plays death animation.
 */
void PLAYER_death_anim()
{
    XGM_startPlayPCM(66, 1, SOUND_PCM_CH1);
    if (!player_is_alive)
    {
        SPR_setAnim(player.sprite, 8);
        for (u16 i = 0; i < 20; i++)
        {
            SPR_update();
            SYS_doVBlankProcess();
        }
    }
}

/**
 * @brief Respawns the player.
 */
void PLAYER_respawn()
{
    player.x = player_spawn.initial_x;
    player.y = player_spawn.initial_y;
    player.speed_x = 0;
    player.speed_y = 0;

    SPR_setPosition(player.sprite, fix16ToInt(player.x), fix16ToInt(player.y));
    player_gravity = abs(player_gravity);
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

/**
 * @brief Frees the player sprite.
 */
void PLAYER_free()
{
    SPR_releaseSprite(player.sprite);
    player.sprite = NULL;
}

/**
 * @brief Checks if the player is on the ground.
 * @return 1 if grounded, 0 otherwise.
 */
u8 PLAYER_on_ground()
{
    return ((LEVEL_wall_at(player.box.left, player.box.top) ||
             LEVEL_wall_at(player.box.left + player.w / 2, player.box.top) ||
             LEVEL_wall_at(player.box.right - 1, player.box.top)) ||
            (LEVEL_wall_at(player.box.left, player.box.bottom) ||
             LEVEL_wall_at(player.box.left + player.w / 2, player.box.bottom) ||
             LEVEL_wall_at(player.box.right - 1, player.box.bottom)));
}