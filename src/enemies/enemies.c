#include "enemies.h"

Enemy enemy_pool[MAX_ENEMIES];

const f16 g_enemy_speed = 35;
const f16 f_enemy_speed = 28;

u8 enemy_counter = 0;

void ENEMIES_level_change_despawn()
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        if (enemy_pool[i].firefly.sprite != NULL)
        {
            ENEMY_unspwan(i);
        }
    }
}

void ENEMY_params()
{
    // LEVEL 1
    ENEMY_init(0, GROUND, 6, 9, LEVEL_SCREEN_1, 0);
    ENEMY_init(1, FLYING, 2, 5, LEVEL_SCREEN_1, 0);

    // LEVEL 2
    ENEMY_init(2, FLYING, 9, 8, LEVEL_SCREEN_1, 1);
    ENEMY_init(3, GROUND, 4, 9, LEVEL_SCREEN_2, 1);
    ENEMY_init(4, GROUND, 7, 4, LEVEL_SCREEN_2, 1);

    // LEVEL 3
    ENEMY_init(5, GROUND, 14, 12, LEVEL_SCREEN_1, 2);
    ENEMY_init(6, FLYING, 10, 4, LEVEL_SCREEN_2, 2);
    ENEMY_init(7, FLYING, 14, 6, LEVEL_SCREEN_2, 2);
    ENEMY_init(8, FLYING, 11, 8, LEVEL_SCREEN_2, 2);
    ENEMY_init(9, FLYING, 12, 9, LEVEL_SCREEN_2, 2);
    ENEMY_init(10, GROUND, 4, 12, LEVEL_SCREEN_5, 2);

    // LEVEL 4
    ENEMY_init(11, FLYING, 5, 7, LEVEL_SCREEN_1, 3);
    ENEMY_init(12, FLYING, 8, 8, LEVEL_SCREEN_4, 3);
    ENEMY_init(13, FLYING, 12, 8, LEVEL_SCREEN_4, 3);
    ENEMY_init(14, GROUND, 6, 12, LEVEL_SCREEN_4, 3);
    ENEMY_init(15, GROUND, 16, 12, LEVEL_SCREEN_4, 3);
    ENEMY_init(16, GROUND, 6, 5, LEVEL_SCREEN_5, 3);
    ENEMY_init(17, FLYING, 13, 9, LEVEL_SCREEN_5, 3);

    // LEVEL 5
    ENEMY_init(18, GROUND, 2, 9, LEVEL_SCREEN_1, 5);
    ENEMY_init(19, GROUND, 11, 5, LEVEL_SCREEN_1, 5);
    ENEMY_init(20, FLYING, 6, 4, LEVEL_SCREEN_1, 5);
    ENEMY_init(21, FLYING, 3, 5, LEVEL_SCREEN_2, 5);
    ENEMY_init(22, FLYING, 16, 10, LEVEL_SCREEN_2, 5);
    ENEMY_init(23, GROUND, 11, 9, LEVEL_SCREEN_3, 5);
    ENEMY_init(24, GROUND, 2, 9, LEVEL_SCREEN_3, 5);
    ENEMY_init(25, FLYING, 7, 6, LEVEL_SCREEN_3, 5);
}

void ENEMY_init(u8 index, u8 type, u16 start_x, u16 start_y, u8 screen, u8 level)
{

    u16 x_calc = start_x * METATILE_W;
    u16 start_y_calc = start_y * METATILE_W;
    enemy_pool[index].start_x = x_calc;
    enemy_pool[index].start_y = start_y_calc;
    enemy_pool[index].type = type;
    enemy_pool[index].on_screen = 0;
    enemy_pool[index].spawn_level = level;
    enemy_pool[index].spawn_screen = screen;
    // enemy_pool[index].travel_min_range = min_range + x / METATILE_W;
    // enemy_pool[index].travel_max_range = max_range + x / METATILE_W;
    enemy_counter++;
}

u8 ENEMIES_spawn_hub(u8 ind)
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        // kprintf("spawn_screen: %d, screen: %d, spawn_level: %d level: %d", enemy_pool[i].spawn_screen, LEVEL_current_screen, enemy_pool[i].spawn_level, LEVEL_current_level);
        if (enemy_pool[i].spawn_screen == LEVEL_current_screen && enemy_pool[i].spawn_level == LEVEL_current_level)
        {
            ind += ENEMY_spawn(i, ind);
        }
        else if (enemy_pool[i].firefly.sprite != NULL)
        {
            ENEMY_unspwan(i);
        }
    }
    return ind;
}

void ENEMY_unspwan(u8 index)
{
    SPR_releaseSprite(enemy_pool[index].firefly.sprite);
    enemy_pool[index].firefly.sprite = NULL;
    enemy_pool[index].on_screen = 0;
    SPR_update();
}

u8 ENEMY_spawn(u8 index, u8 ind)
{
    switch (enemy_pool[index].type)
    {
    case 0:
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_g_enemy, enemy_pool[index].start_x, enemy_pool[index].start_y, PAL_GAME, false, ind);
        enemy_pool[index].firefly.speed_x = g_enemy_speed;
        break;
    case 1:
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_f_enemy, enemy_pool[index].start_x, enemy_pool[index].start_y, PAL_GAME, false, ind);
        enemy_pool[index].firefly.speed_x = f_enemy_speed;
        break;
    default:
        break;
    }
    enemy_pool[index].on_screen = 1;
    return ind;
}

void ENEMIES_update_hub()
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        if (enemy_pool[i].on_screen)
        {
            switch (enemy_pool[i].type)
            {
            case 0:
                ENEMIES_g_enemy_update(&enemy_pool[i].firefly);
                break;
            case 1:
                ENEMIES_g_enemy_update(&enemy_pool[i].firefly);
                break;
            }
        }
    }
}

void ENEMIES_g_enemy_update(GameObject *firefly)
{
    firefly->next_x = firefly->x + firefly->speed_x;
    GAMEOBJECT_update_boundbox(firefly->next_x, firefly->y, firefly);

    if (firefly->speed_x > 0)
    {
        if (LEVEL_wall_at(firefly->box.right, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.bottom - 1))
        {
            firefly->next_x = FIX16(firefly->box.right / METATILE_W * METATILE_W - firefly->w);
            SPR_setHFlip(firefly->sprite, true);
            firefly->speed_x = -firefly->speed_x;
        }
    }
    else if (firefly->speed_x < 0)
    {
        if (LEVEL_wall_at(firefly->box.left, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.bottom - 1))
        {
            firefly->next_x = FIX16((firefly->box.left / METATILE_W + 1) * METATILE_W);
            SPR_setHFlip(firefly->sprite, false);
            firefly->speed_x = -firefly->speed_x;
        }
    }

    firefly->x = firefly->next_x;
    // kprintf("speed vagalume: %u, g_speed: %u", firefly->speed_x, g_enemy_speed);
    SPR_setPosition(firefly->sprite, fix16ToInt(firefly->x), fix16ToInt(firefly->y));
}

// void ENEMIES_f_enemy_update(GameObject *firefly, u8 i)
// {
//     s16 firefly_center_x = fix16ToInt(firefly->x) + firefly->w / 2;
//     u16 center_x = firefly_center_x / METATILE_W;

//     GAMEOBJECT_update_boundbox(firefly->x, firefly->y, firefly);

//     if (firefly->speed_x > 0)
//     {
//         if (center_x >= enemy_pool[i].travel_max_range)
//         {
//             firefly->speed_x = -firefly->speed_x;
//             SPR_setHFlip(firefly->sprite, false);
//         }
//     }
//     else
//     {
//         if (center_x <= enemy_pool[i].travel_min_range)
//         {
//             firefly->speed_x = -firefly->speed_x;
//             SPR_setHFlip(firefly->sprite, true);
//         }
//     }
//     firefly->x += firefly->speed_x;
//     SPR_setPosition(firefly->sprite, fix16ToInt(firefly->x), fix16ToInt(firefly->y));
// }
