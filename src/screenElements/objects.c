#include "objects.h"

u8 objects_initiated = 0;
ObjectConfig objects_config[MAX_OBJECTS];

void OBJECT_params()
{
    // Level 1
    OBJECT_init(0, 0, 17, 4, 0, 6, false);
    OBJECT_init(1, 1, 2, 8, 0, 6, false);
    OBJECT_init(2, 3, 2, 8, 0, 6, true);
    OBJECT_init(3, 2, 17, 8, 0, 6, false);

    // Level 2
    OBJECT_init(4, 2, 17, 4, 1, 6, false);
    OBJECT_init(5, 1, 17, 9, 1, 6, false);
    OBJECT_init(6, 3, 17, 9, 1, 6, true);

    OBJECT_init(7, 0, 2, 3, 1, 7, false);
    OBJECT_init(8, 2, 15, 4, 1, 7, false);
    OBJECT_init(9, 1, 3, 8, 1, 7, false);
    OBJECT_init(10, 3, 3, 8, 1, 7, true);

    // Level 3
    OBJECT_init(11, 2, 2, 3, 2, 6, false);
    OBJECT_init(12, 1, 17, 12, 2, 6, false);
    OBJECT_init(13, 3, 17, 12, 2, 6, true);

    OBJECT_init(14, 2, 5, 7, 2, 7, false);
    OBJECT_init(15, 1, 17, 12, 2, 7, false);
    OBJECT_init(16, 3, 17, 12, 2, 7, true);

    OBJECT_init(17, 0, 14, 12, 2, 4, false);
    OBJECT_init(18, 2, 14, 3, 2, 4, false);
    OBJECT_init(19, 1, 2, 5, 2, 4, false);
    OBJECT_init(20, 3, 2, 5, 2, 4, true);
}

void OBJECT_init(u8 i, u8 type, u16 x, u16 y, u8 level, u8 screen, u8 prio)
{
    objects_config[i].collected = 0;
    objects_config[i].level = level;
    objects_config[i].on_screen = 0;
    objects_config[i].screen = screen;
    objects_config[i].type = type;
    if (type == 3)
    {
        objects_config[i].x = x * METATILE_W - 8;
        objects_config[i].y = y * METATILE_W - 8;
    }
    else
    {
        objects_config[i].x = x * METATILE_W;
        objects_config[i].y = y * METATILE_W;
    }

    objects_config[i].prio = prio;
    objects_initiated++;
}

u16 OBJECT_update(u16 ind)
{
    for (u8 i = 0; i < objects_initiated; i++)
    {
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            ind += OBJECT_spawn(i, ind);
        }
        else
        {
            if (objects_config[i].obj.sprite != NULL)
            {
                OBJECT_clear(&objects_config[i], false);
            }
        }
    }
    return ind;
}

u16 OBJECT_spawn(u8 i, u16 ind)
{
    switch (objects_config[i].type)
    {
    case 0:
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_door, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    case 1:
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_light_switch, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    case 2:
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_key, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    case 3:
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_sparks, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    default:
        break;
    }
    objects_config[i].on_screen = 1;
    GAMEOBJECT_update_boundbox(objects_config[i].obj.x, objects_config[i].obj.y, &objects_config[i].obj); // FIXED

    return ind;
}

ObjectConfig *OBJECT_check_collision(u16 player_center_x, u16 player_center_y)
{
    for (u8 i = 0; i < objects_initiated; i++)
    {
        if (objects_config[i].on_screen &&
            player_center_x >= objects_config[i].obj.box.left &&
            player_center_x <= objects_config[i].obj.box.right &&
            player_center_y >= objects_config[i].obj.box.top &&
            player_center_y <= objects_config[i].obj.box.bottom)
        {
            return &objects_config[i];
        }
    }
    return NULL;
}

void OBJECT_clear(ObjectConfig *config, u8 collect)
{
    if (collect)
    {
        config->collected = 1;
    }
    config->on_screen = 0;
    SPR_releaseSprite(config->obj.sprite);
    config->obj.sprite = NULL;
}
