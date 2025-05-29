#include "objects.h"

// Objetos do jogo
GameObject door;
GameObject key;
GameObject powerup;

// Flags de visibilidade
u8 door_on_screen = 0;
u8 key_on_screen = 0;
u8 powerup_on_screen = 0;

// Configurações dos objetos
ObjectConfig door_configs[] = {
    {0, 6, 16 * METATILE_W, 6 * METATILE_W, 0, 0},
    {1, 6, 17 * METATILE_W, 5 * METATILE_W, 0, 0},
    {2, 3, 11 * METATILE_W, 4 * METATILE_W, 0, 0},
    {3, 8, 2 * METATILE_W, 3 * METATILE_W, 0, 0}};

ObjectConfig key_configs[] = {
    {0, 6, 2 * METATILE_W, 5 * METATILE_W, 0, 0},
    {1, 3, 17 * METATILE_W, 4 * METATILE_W, 0, 0},
    {2, 7, 13 * METATILE_W, 8 * METATILE_W, 0, 0},
    {3, 4, 3 * METATILE_W, 6 * METATILE_W, 0, 0}};

ObjectConfig powerup_configs[] = {
    {0, 6, 17 * METATILE_W, 12 * METATILE_W, 0, 0},
    {1, 6, 2 * METATILE_W, 5 * METATILE_W, 0, 0},
    // {1, 6, 3 * METATILE_W, 7 * METATILE_W, 0, 0},
};

// Tabela de tipos de objetos
const ObjectType object_types[] = {
    {&door, &spr_door, door_configs, sizeof(door_configs) / sizeof(ObjectConfig), &door_on_screen},
    {&key, &spr_key, key_configs, sizeof(key_configs) / sizeof(ObjectConfig), &key_on_screen},
    {&powerup, &spr_powerup, powerup_configs, sizeof(powerup_configs) / sizeof(ObjectConfig), &powerup_on_screen}};

u16 OBJECT_spawn_type(const ObjectType *type, u16 ind, u8 force_respawn)
{
    u8 should_be_visible = 0;

    for (u8 i = 0; i < type->config_count; i++)
    {
        if (type->configs[i].level == LEVEL_current_level &&
            (type->configs[i].screen == LEVEL_current_screen) &&
            type->configs[i].collected == 0)
        {

            if (!*(type->on_screen_flag) || force_respawn)
            {
                // Sempre limpa antes, se for respawn forçado
                if (force_respawn && *(type->on_screen_flag))
                {
                    OBJECT_clear(type->obj);
                }

                ind += GAMEOBJECT_init(type->obj, type->sprite,
                                       type->configs[i].x, type->configs[i].y,
                                       PAL_GAME, ind);

                if (type->configs[i].flip == 1)
                    SPR_setHFlip(type->obj->sprite, TRUE);
                else if (type->configs[i].flip == 2)
                    SPR_setVFlip(type->obj->sprite, TRUE);
            }

            GAMEOBJECT_update_boundbox(type->obj->x, type->obj->y, type->obj);
            should_be_visible = 1;
            break;
        }
    }

    if (*(type->on_screen_flag) && !should_be_visible)
    {
        OBJECT_clear(type->obj);
    }

    *(type->on_screen_flag) = should_be_visible;
    return ind;
}

u16 OBJECT_update(u16 ind, u8 force_respawn)
{
    for (u8 i = 0; i < sizeof(object_types) / sizeof(ObjectType); i++)
    {
        ind = OBJECT_spawn_type(&object_types[i], ind, force_respawn);
    }
    return ind;
}

GameObject *OBJECT_check_collision(u16 player_center_x, u16 player_center_y)
{
    for (u8 i = 0; i < sizeof(object_types) / sizeof(ObjectType); i++)
    {
        if (*(object_types[i].on_screen_flag) &&
            player_center_x >= object_types[i].obj->box.left &&
            player_center_x <= object_types[i].obj->box.right &&
            player_center_y >= object_types[i].obj->box.top &&
            player_center_y <= object_types[i].obj->box.bottom)
        {
            return object_types[i].obj;
        }
    }
    return NULL;
}

void OBJECT_collect(GameObject *object)
{
    if (object == NULL)
        return;

    for (u8 i = 0; i < sizeof(object_types) / sizeof(ObjectType); i++)
    {
        for (u8 j = 0; j < object_types[i].config_count; j++)
        {
            if (object_types[i].configs[j].level == LEVEL_current_level &&
                object_types[i].configs[j].screen == LEVEL_current_screen &&
                object_types[i].obj == object)
            {
                object_types[i].configs[j].collected = 1;
                break;
            }
        }

        if (object_types[i].obj == object)
        {
            if (object->sprite != NULL)
            {
                SPR_releaseSprite(object->sprite);
                object->sprite = NULL;
            }

            *(object_types[i].on_screen_flag) = 0;
            break;
        }
    }
}
void OBJECT_clear(GameObject *object)
{
    if (object == NULL)
        return;

    for (u8 i = 0; i < sizeof(object_types) / sizeof(ObjectType); i++)
    {
        if (object_types[i].obj == object)
        {
            if (object->sprite != NULL)
            {
                SPR_releaseSprite(object->sprite);
                object->sprite = NULL;
            }

            *(object_types[i].on_screen_flag) = 0;
            break;
        }
    }
}
