#include "enemies.h"

Enemy enemy_pool[MAX_ENEMIES];

const f16 g_enemy_speed = 35;
const f16 f_enemy_speed = 20;

u8 ENEMY_spawn(u8 index, u8 type, u16 last_x, u16 last_y, u8 min_range, u8 max_range, u16 ind)
{

    enemy_pool[index].last_x = last_x;
    enemy_pool[index].last_y = last_y;
    enemy_pool[index].type = type;
    enemy_pool[index].travel_min_range = min_range + last_x / METATILE_W;
    enemy_pool[index].travel_max_range = max_range + last_x / METATILE_W;

    if (enemy_pool[index].type == 0)
    {
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_g_enemy, enemy_pool[index].last_x, enemy_pool[index].last_y, PAL_NPC, ind);
        enemy_pool[index].firefly.speed_x = g_enemy_speed;
    }
    else if (enemy_pool[index].type == 1)
    {
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_f_enemy, enemy_pool[index].last_x, enemy_pool[index].last_y, PAL_NPC, ind);
        enemy_pool[index].firefly.speed_x = f_enemy_speed;
    }

    return ind;
}

u8 ENEMIES_init(u16 ind, u8 level)
{
    if (level == 0)
    {
        ind += ENEMY_spawn(LEVEL_1_ENEMY_1, 0, 7 * METATILE_W, 9 * METATILE_W, false, false, ind);
        ind += ENEMY_spawn(LEVEL_1_ENEMY_2, 1, 7 * METATILE_W, 7 * METATILE_W, 0, 5, ind);
    }
    return ind;
}

void ENEMIES_update_hub(u8 actual_level_enemies, u8 last_level_enemies)
{
    for (u8 i = last_level_enemies; i < actual_level_enemies; i++)
    {
        switch (enemy_pool[i].type)
        {
        case 0:
            ENEMIES_g_enemy_update(&enemy_pool[i].firefly);
            break;
        case 1:
            ENEMIES_f_enemy_update(&enemy_pool[i].firefly, i);
            break;
        default:
            break;
        }
    }
}

void ENEMIES_g_enemy_update(GameObject *firefly)
{
    firefly->next_x = firefly->x + firefly->speed_x;
    GAMEOBJECT_update_boundbox(firefly->next_x, firefly->y, firefly);

    if (firefly->speed_x > 0)
    { // moving right
        if (LEVEL_wall_at(firefly->box.right, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.bottom - 1))
        {
            firefly->next_x = FIX16(firefly->box.right / METATILE_W * METATILE_W - firefly->w);
            SPR_setHFlip(firefly->sprite, false);
            firefly->speed_x = -firefly->speed_x;
        }
    }
    else if (firefly->speed_x < 0)
    { // moving left
        if (LEVEL_wall_at(firefly->box.left, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.bottom - 1))
        {
            firefly->next_x = FIX16((firefly->box.left / METATILE_W + 1) * METATILE_W);
            SPR_setHFlip(firefly->sprite, true);
            firefly->speed_x = -firefly->speed_x;
        }
    }

    firefly->x = firefly->next_x;
    SPR_setPosition(firefly->sprite, fix16ToInt(firefly->x), fix16ToInt(firefly->y));
}

void ENEMIES_f_enemy_update(GameObject *firefly, u8 i)
{
    s16 firefly_center_x = fix16ToInt(firefly->x) + firefly->w / 2;
    u16 center_x = firefly_center_x / METATILE_W;

    GAMEOBJECT_update_boundbox(firefly->x, firefly->y, firefly);

    if (firefly->speed_x > 0)
    {
        if (center_x >= enemy_pool[i].travel_max_range)
        {
            firefly->speed_x = -firefly->speed_x;
            SPR_setHFlip(firefly->sprite, false);
        }
    }
    else
    {
        if (center_x <= enemy_pool[i].travel_min_range)
        {
            firefly->speed_x = -firefly->speed_x;
            SPR_setHFlip(firefly->sprite, true);
        }
    }
    firefly->x += firefly->speed_x;
    SPR_setPosition(firefly->sprite, fix16ToInt(firefly->x), fix16ToInt(firefly->y));
}
