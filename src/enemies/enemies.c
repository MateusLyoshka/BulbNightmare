#include "enemies.h"

Enemy enemy_pool[MAX_ENEMIES];

u8 ENEMY_spawn(u8 actual_level, u8 last_level, u8 type, u16 last_x, u16 last_y, u16 ind)
{
    for (u8 i = last_level; i < actual_level; i++)
    {
        enemy_pool[i].last_x = last_x;
        enemy_pool[i].last_y = last_y;
        enemy_pool[i].level_id = actual_level;
        enemy_pool[i].type = type;

        if (enemy_pool[i].type == 0)
        {
            ind += GAMEOBJECT_init(&enemy_pool[i].firefly, &spr_g_enemy, enemy_pool[i].last_x, enemy_pool[i].last_y, PAL_NPC, ind);
        }
        else if (enemy_pool[i].type == 1)
        {
            ind += GAMEOBJECT_init(&enemy_pool[i].firefly, &spr_f_enemy, enemy_pool[i].last_x, enemy_pool[i].last_y, PAL_NPC, ind);
        }
    }
    return ind;
}

u8 ENEMIES_init(u16 ind)
{
    ind += ENEMY_spawn(LEVEL_1, 0, 0, 7 * METATILE_W, 9 * METATILE_W, ind);
    ind += ENEMY_spawn(LEVEL_1, 0, 1, 12 * METATILE_W, 11 * METATILE_W, ind);
    return ind;
}
