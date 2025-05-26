#include "objects.h"

GameObject door;
GameObject key;
GameObject powerup;

u8 door_on_screen = 0;
u8 key_on_screen = 0;
u8 powerup_on_screen = 0;

const ObjectConfig door_configs[] = {
    {0, 6, 16 * METATILE_W, 6 * METATILE_W, 0}, // Level 0
    {1, 6, 17 * METATILE_W, 5 * METATILE_W, 0}, // Level 1
    {2, 3, 11 * METATILE_W, 4 * METATILE_W, 0}, // Level 2
    {3, 8, 2 * METATILE_W, 3 * METATILE_W, 0}   // Level 3
};

const ObjectConfig key_configs[] = {
    {0, 6, 2 * METATILE_W, 5 * METATILE_W, 0},  // Level 0
    {1, 3, 17 * METATILE_W, 4 * METATILE_W, 0}, // Level 1
    {2, 7, 13 * METATILE_W, 8 * METATILE_W, 0}, // Level 2
    {3, 4, 3 * METATILE_W, 6 * METATILE_W, 0}   // Level 3
};

const ObjectConfig powerup_configs[] = {
    {0, 6, 17 * METATILE_W, 12 * METATILE_W, 0},
    {1, 6, 2 * METATILE_W, 5 * METATILE_W, 2},
};

u16 OBJECT_door_spawn(u16 ind)
{
    u8 door_should_be_visible = 0;

    // Verifica todas as configurações
    for (u8 i = 0; i < sizeof(door_configs) / sizeof(ObjectConfig); i++)
    {
        if (door_configs[i].level == LEVEL_current_level &&
            door_configs[i].screen == LEVEL_current_screen)
        {
            // Condições para mostrar a porta
            if (!door_on_screen)
            {
                ind += GAMEOBJECT_init(&door, &spr_door, door_configs[i].x, door_configs[i].y, PAL_GAME, ind);
            }
            door_should_be_visible = 1;
            break;
        }
    }

    // Se a porta está visível mas não deveria estar
    if (door_on_screen && !door_should_be_visible)
    {
        OBJECT_clear(door);
    }

    door_on_screen = door_should_be_visible;
    return ind;
}

u16 OBJECT_key_spawn(u16 ind)
{
    u8 key_should_be_visible = 0;

    for (u8 i = 0; i < sizeof(key_configs) / sizeof(ObjectConfig); i++)
    {
        if (key_configs[i].level == LEVEL_current_level &&
            key_configs[i].screen == LEVEL_current_screen)
        {

            if (!key_on_screen)
            {
                ind += GAMEOBJECT_init(&key, &spr_key, key_configs[i].x, key_configs[i].y, PAL_GAME, ind);

                // Aplica flip se necessário
                if (key_configs[i].flip == 1)
                    SPR_setHFlip(key.sprite, TRUE);
                else if (key_configs[i].flip == 2)
                    SPR_setVFlip(key.sprite, TRUE);
            }
            key_should_be_visible = 1;
            break;
        }
    }

    if (key_on_screen && !key_should_be_visible)
    {
        OBJECT_clear(key);
    }

    key_on_screen = key_should_be_visible;
    return ind;
}
u16 OBJECT_powerup_spawn(u16 ind)
{
    u8 powerup_should_be_visible = 0;

    for (u8 i = 0; i < sizeof(powerup_configs) / sizeof(ObjectConfig); i++)
    {
        if (powerup_configs[i].level == LEVEL_current_level && powerup_configs[i].screen == LEVEL_current_screen)
        {

            if (!powerup_on_screen)
            {
                ind += GAMEOBJECT_init(&powerup, &spr_powerup,
                                       powerup_configs[i].x, powerup_configs[i].y,
                                       PAL_GAME, ind);

                // Aplica flip
                if (powerup_configs[i].flip == 1)
                    SPR_setHFlip(powerup.sprite, TRUE);
                else if (powerup_configs[i].flip == 2)
                    SPR_setVFlip(powerup.sprite, FALSE);
            }
            powerup_should_be_visible = 1;
            break;
        }
    }

    if (powerup_on_screen && !powerup_should_be_visible)
    {
        OBJECT_clear(powerup);
    }

    powerup_on_screen = powerup_should_be_visible;
    return ind;
}

u16 OBJECT_update(u16 ind)
{
    ind += OBJECT_door_spawn(ind);
    ind += OBJECT_key_spawn(ind);
    ind += OBJECT_powerup_spawn(ind);
    return ind;
}

void OBJECT_clear(GameObject object)
{
    SPR_releaseSprite(object.sprite);
    object.sprite = NULL;
}