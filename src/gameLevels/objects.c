#include "objects.h"

GameObject door;
GameObject key;
GameObject powerup;

u8 door_on_screen = 0;
u8 key_on_screen = 0;
u8 powerup_on_screen = 0;

u16 OBJECT_door_spawn(u16 ind)
{
    if (LEVEL_current_level == 0)
    {
        ind += GAMEOBJECT_init(&door, &spr_door, 16 * METATILE_W, 6 * METATILE_W, PAL_PLAYER, ind);
    }
    else if (LEVEL_current_level == 1)
    {
        if (LEVEL_current_screen == 6)
        {
            ind += GAMEOBJECT_init(&door, &spr_door, 17 * METATILE_W, 5 * METATILE_W, PAL_PLAYER, ind);
            door_on_screen = 1;
        }
        else if (door_on_screen)
        {
            OBJECT_clear(door);
            door_on_screen = 0;
        }
    }

    return ind;
}

u16 OBJECT_key_spawn(u16 ind)
{
    if (LEVEL_current_level == 0)
    {
        ind += GAMEOBJECT_init(&key, &spr_key, 5 * METATILE_W, 6 * METATILE_W, PAL_PLAYER, ind);
    }
    else if (LEVEL_current_level == 1)
    {
        if (LEVEL_current_screen == 3)
        {
            ind += GAMEOBJECT_init(&key, &spr_key, 17 * METATILE_W, 4 * METATILE_W, PAL_PLAYER, ind);
            key_on_screen = 1;
        }
        else if (key_on_screen)
        {
            OBJECT_clear(key);
            key_on_screen = 0;
        }
    }
    return ind;
}

u16 OBJECT_powerup_spawn(u16 ind)
{
    if (LEVEL_current_level == 0)
    {
        ind += GAMEOBJECT_init(&powerup, &spr_powerup, 5 * METATILE_W, 6 * METATILE_W, PAL_PLAYER, ind);
    }
    else if (LEVEL_current_level == 1)
    {
        if (LEVEL_current_screen == 6)
        {
            ind += GAMEOBJECT_init(&powerup, &spr_powerup, 2 * METATILE_W, 5 * METATILE_W, PAL_PLAYER, ind);
            SPR_setVFlip(powerup.sprite, FALSE);
            powerup_on_screen = 1;
        }
        else if (powerup_on_screen)
        {
            OBJECT_clear(powerup);
            powerup_on_screen = 0;
        }
    }
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