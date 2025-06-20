#include "objects.h"

u8 objects_spawned = 0;
ObjectConfig objects_config[MAX_OBJECTS];
u8 objects_initiated = 0;

u16 keys_on_level[] = {1, 2, 3, 4, 1, 1};
u16 switchs_on_level[] = {1, 2, 3, 4, 0, 0};

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
    OBJECT_init(11, 2, 10, 3, 2, 6, false);
    OBJECT_init(12, 1, 15, 12, 2, 6, false);
    OBJECT_init(13, 3, 15, 12, 2, 6, true);

    OBJECT_init(14, 2, 5, 7, 2, 7, false);
    OBJECT_init(15, 1, 17, 12, 2, 7, false);
    OBJECT_init(16, 3, 17, 12, 2, 7, true);

    OBJECT_init(17, 0, 14, 12, 2, 4, false);
    OBJECT_init(18, 2, 14, 3, 2, 4, false);
    OBJECT_init(19, 1, 2, 5, 2, 4, false);
    OBJECT_init(20, 3, 2, 5, 2, 4, true);

    // Level 4
    OBJECT_init(21, 2, 3, 8, 3, 6, false);
    OBJECT_init(22, 1, 17, 4, 3, 6, false);
    OBJECT_init(23, 3, 17, 4, 3, 6, true);

    OBJECT_init(24, 1, 12, 3, 3, 3, false);
    OBJECT_init(25, 3, 12, 3, 3, 3, true);

    OBJECT_init(26, 0, 3, 12, 3, 6, false); // 11, 12, tela 4
    OBJECT_init(27, 2, 16, 8, 3, 4, false);
    OBJECT_init(28, 1, 14, 4, 3, 4, false);
    OBJECT_init(29, 3, 14, 4, 3, 4, true);

    OBJECT_init(30, 2, 1, 9, 3, 7, false);
    OBJECT_init(31, 2, 16, 9, 3, 7, false);
    OBJECT_init(32, 1, 11, 3, 3, 7, false);
    OBJECT_init(33, 3, 11, 3, 3, 7, true);

    OBJECT_init(34, 0, 15, 12, 4, 8, false);
    OBJECT_init(35, 2, 11, 12, 4, 8, false);

    OBJECT_init(36, 0, 1, 1, 5, 1, false);
    OBJECT_init(37, 2, 5, 6, 5, 8, false);
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
    objects_spawned++;
}

u16 OBJECT_update(u16 ind)
{

    for (u8 i = 0; i < objects_spawned; i++)
    {
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            OBJECT_clear(&objects_config[i], false);
        }
    }

    for (u8 i = 0; i < objects_spawned; i++)
    {
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            kprintf("object update");
            ind += OBJECT_spawn(i, ind);
        }
        else
        {

            OBJECT_clear(&objects_config[i], false);
        }
    }
    // objects_initiated = 1;
    return ind;
}

void OBJECT_key_reset()
{
    for (u8 i = 0; i < objects_spawned; i++)
    {
        if (objects_config[i].type == 2)
        {
            objects_config[i].collected = 0;
        }
    }
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
        if (room_lights[LEVEL_current_screen] != 0)
        {
            SPR_setAnim(objects_config[i].obj.sprite, 1);
        }

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
    GAMEOBJECT_update_boundbox(objects_config[i].obj.x, objects_config[i].obj.y, &objects_config[i].obj);

    return ind;
}

ObjectConfig *OBJECT_check_collision(u16 player_center_x, u16 player_center_y)
{
    for (u8 i = 0; i < objects_spawned; i++)
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
    config->on_screen = 0;          // Marca que não está mais ativo na tela
    if (config->obj.sprite != NULL) // Adiciona verificação para segurança
    {
        SPR_releaseSprite(config->obj.sprite);
        config->obj.sprite = NULL; // Crucial: marca que não há mais sprite associado
    }
}

void OBJECT_clear_all()
{
    for (u8 i = 0; i < objects_spawned; i++)
    {
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            OBJECT_clear(&objects_config[i], false);
        }
    }
}
