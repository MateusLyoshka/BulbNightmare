#include "objects.h"

u8 objects_spawned = 0;                   // Counter for the total number of objects initialized in the game.
ObjectConfig objects_config[MAX_OBJECTS]; // Array to store configuration for all game objects.
u8 objects_initiated = 0;                 // Flag to track if objects have been globally initiated (not currently used in logic after assignment).

// Arrays storing the total number of keys and switches present on each level.
u16 keys_on_level[] = {1, 2, 3, 4, 1, 1};    // Keys for levels 0 to 5.
u16 switchs_on_level[] = {1, 2, 3, 4, 0, 0}; // Switches for levels 0 to 5.

/**
 * @brief Defines the parameters and configurations for all game objects.
 *
 * This function is responsible for calling OBJECT_init for each object,
 * setting its type, initial position, level, screen, and priority.
 */
void OBJECT_params()
{
    // Level 1 (index 0) objects
    OBJECT_init(0, 0, 17, 4, 0, 6, false); // Door
    OBJECT_init(1, 1, 2, 8, 0, 6, false);  // Switch
    OBJECT_init(2, 3, 2, 8, 0, 6, true);   // Spark (high priority)
    OBJECT_init(3, 2, 17, 8, 0, 6, false); // Key

    // Level 2 (index 1) objects
    OBJECT_init(4, 2, 17, 4, 1, 6, false); // Key
    OBJECT_init(5, 1, 17, 9, 1, 6, false); // Switch
    OBJECT_init(6, 3, 17, 9, 1, 6, true);  // Spark
    OBJECT_init(7, 0, 2, 3, 1, 7, false);  // Door
    OBJECT_init(8, 2, 15, 4, 1, 7, false); // Key
    OBJECT_init(9, 1, 3, 8, 1, 7, false);  // Switch
    OBJECT_init(10, 3, 3, 8, 1, 7, true);  // Spark

    // Level 3 (index 2) objects
    OBJECT_init(11, 2, 10, 3, 2, 6, false);  // Key
    OBJECT_init(12, 1, 15, 12, 2, 6, false); // Switch
    OBJECT_init(13, 3, 15, 12, 2, 6, true);  // Spark
    OBJECT_init(14, 2, 5, 7, 2, 7, false);   // Key
    OBJECT_init(15, 1, 17, 12, 2, 7, false); // Switch
    OBJECT_init(16, 3, 17, 12, 2, 7, true);  // Spark
    OBJECT_init(17, 0, 14, 12, 2, 4, false); // Door
    OBJECT_init(18, 2, 14, 3, 2, 4, false);  // Key
    OBJECT_init(19, 1, 2, 5, 2, 4, false);   // Switch
    OBJECT_init(20, 3, 2, 5, 2, 4, true);    // Spark

    // Level 4 (index 3) objects
    OBJECT_init(21, 2, 3, 8, 3, 6, false);  // Key
    OBJECT_init(22, 1, 17, 4, 3, 6, false); // Switch
    OBJECT_init(23, 3, 17, 4, 3, 6, true);  // Spark
    OBJECT_init(24, 1, 12, 3, 3, 3, false); // Switch
    OBJECT_init(25, 3, 12, 3, 3, 3, true);  // Spark
    OBJECT_init(26, 0, 3, 12, 3, 6, false); // Door
    OBJECT_init(27, 2, 16, 8, 3, 4, false); // Key
    OBJECT_init(28, 1, 14, 4, 3, 4, false); // Switch
    OBJECT_init(29, 3, 14, 4, 3, 4, true);  // Spark
    OBJECT_init(30, 2, 1, 9, 3, 7, false);  // Key
    OBJECT_init(31, 2, 16, 9, 3, 7, false); // Key
    OBJECT_init(32, 1, 11, 3, 3, 7, false); // Switch
    OBJECT_init(33, 3, 11, 3, 3, 7, true);  // Spark

    // Level 5 (index 4) objects
    OBJECT_init(34, 0, 15, 12, 4, 8, false); // Door
    OBJECT_init(35, 2, 11, 12, 4, 8, false); // Key

    // Level 6 (index 5) objects
    OBJECT_init(36, 0, 1, 1, 5, 1, false); // Door
    OBJECT_init(37, 2, 5, 6, 5, 8, false); // Key
}

/**
 * @brief Initializes the configuration for a single game object.
 *
 * This function sets the initial state, type, position, level, screen,
 * and priority for an object within the `objects_config` array.
 *
 * @param i Index of the object in the `objects_config` array.
 * @param type Type of the object (0=door, 1=switch, 2=key, 3=spark).
 * @param x X-coordinate in metatiles (16-pixel units).
 * @param y Y-coordinate in metatiles.
 * @param level Level index where the object belongs.
 * @param screen Screen index within the level where the object belongs.
 * @param prio Priority for the object's sprite (true for high, false for low).
 */
void OBJECT_init(u8 i, u8 type, u16 x, u16 y, u8 level, u8 screen, u8 prio)
{
    objects_config[i].collected = 0;   // Initialize as not collected/activated.
    objects_config[i].level = level;   // Set the level the object belongs to.
    objects_config[i].on_screen = 0;   // Initialize as not on screen.
    objects_config[i].screen = screen; // Set the screen the object belongs to.
    objects_config[i].type = type;     // Set the object type.

    // Adjust object position based on type for specific sprites.
    if (type == 3) // If it's a spark
    {
        objects_config[i].x = x * METATILE_W - 8; // Adjust X for spark sprite offset.
        objects_config[i].y = y * METATILE_W - 8; // Adjust Y for spark sprite offset.
    }
    else // For other object types
    {
        objects_config[i].x = x * METATILE_W; // Use direct metatile to pixel conversion for X.
        objects_config[i].y = y * METATILE_W; // Use direct metatile to pixel conversion for Y.
    }

    objects_config[i].prio = prio; // Set the sprite priority.
    objects_spawned++;             // Increment the count of initialized objects.
}

/**
 * @brief Updates and manages the visibility and spawning of game objects.
 *
 * This function iterates through all configured objects. It clears objects
 * that are no longer on the current screen or have been collected, and then
 * spawns objects that should be visible on the current screen and are not yet collected.
 *
 * @param ind The current VRAM tile index for allocating sprite data.
 * @return The next available VRAM tile index after all updates and spawns.
 */
u16 OBJECT_update(u16 ind)
{
    // First pass: clear objects that are no longer relevant to the current screen.
    for (u8 i = 0; i < objects_spawned; i++)
    {
        // If object belongs to current level/screen AND is not collected, but should NOT be on screen: clear it.
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            OBJECT_clear(&objects_config[i], false);
        }
    }

    // Second pass: spawn objects that should be on the current screen and are not collected.
    for (u8 i = 0; i < objects_spawned; i++)
    {
        // If object belongs to current level/screen AND is not collected, then spawn it.
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            ind += OBJECT_spawn(i, ind); // Spawn the object and update VRAM index.
        }
        else
        {
            OBJECT_clear(&objects_config[i], false);
        }
    }
    // objects_initiated = 1; // Commented out: This line would set a flag that the objects are initiated.
    return ind; // Return the final VRAM index.
}

/**
 * @brief Resets the 'collected' status for all key objects.
 *
 * This function is typically called when the player loses a life or the level
 * restarts, ensuring that collected keys are available again.
 */
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

/**
 * @brief Spawns a specific game object by initializing its sprite.
 *
 * This function takes an object's index and its VRAM starting index,
 * then initializes the appropriate sprite based on the object's type.
 *
 * @param i Index of the object in the `objects_config` array.
 * @param ind The starting VRAM tile index for the object's sprite.
 * @return The next available VRAM tile index after the object's sprite has been loaded.
 */
u16 OBJECT_spawn(u8 i, u16 ind)
{
    // Use a switch statement to handle different object types.
    switch (objects_config[i].type)
    {
    case 0: // Door
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_door, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    case 1: // Light switch
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_light_switch, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        if (room_lights[LEVEL_current_screen] != 0)
        {
            SPR_setAnim(objects_config[i].obj.sprite, 1);
        }
        break;
    case 2: // Key
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_key, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    case 3: // Spark
        ind += GAMEOBJECT_init(&objects_config[i].obj, &spr_sparks, objects_config[i].x, objects_config[i].y, PAL_GAME, objects_config[i].prio, ind);
        break;
    default:
        break;
    }
    objects_config[i].on_screen = 1;
    // Update the object's bounding box after its sprite has been initialized.
    GAMEOBJECT_update_boundbox(objects_config[i].obj.x, objects_config[i].obj.y, &objects_config[i].obj);

    return ind;
}

/**
 * @brief Checks for collision between the player's center and all active objects.
 *
 * This function iterates through all objects that are currently on screen and
 * checks if the player's center coordinates fall within any object's bounding box.
 *
 * @param player_center_x The X-coordinate of the player's center.
 * @param player_center_y The Y-coordinate of the player's center.
 * @return A pointer to the `ObjectConfig` of the first collided object found, or NULL if no collision.
 */
ObjectConfig *OBJECT_check_collision(u16 player_center_x, u16 player_center_y)
{
    for (u8 i = 0; i < objects_spawned; i++)
    {
        // Check if the object is currently on screen and if the player's center is within its bounding box.
        if (objects_config[i].on_screen &&
            player_center_x >= objects_config[i].obj.box.left &&
            player_center_x <= objects_config[i].obj.box.right &&
            player_center_y >= objects_config[i].obj.box.top &&
            player_center_y <= objects_config[i].obj.box.bottom)
        {
            return &objects_config[i]; // Return a pointer to the collided object's configuration.
        }
    }
    return NULL; // No collision detected.
}

/**
 * @brief Clears an object from the screen and optionally marks it as collected.
 *
 * This function releases the object's sprite from VRAM and updates its status.
 *
 * @param config Pointer to the `ObjectConfig` of the object to clear.
 * @param collect Boolean flag: if true, also marks the object as collected.
 */
void OBJECT_clear(ObjectConfig *config, u8 collect)
{
    if (collect)
    {
        config->collected = 1;
    }
    config->on_screen = 0;
    if (config->obj.sprite != NULL)
    {
        SPR_releaseSprite(config->obj.sprite);
        config->obj.sprite = NULL; // Crucial: set sprite pointer to NULL to prevent dangling pointers.
    }
}

/**
 * @brief Clears all objects belonging to the current level and screen that are not collected.
 *
 * This function is useful for situations like screen transitions where objects on the
 * previous screen need to be removed.
 */
void OBJECT_clear_all()
{
    for (u8 i = 0; i < objects_spawned; i++)
    {
        // If object belongs to current level/screen AND is not collected, clear it.
        if (objects_config[i].level == LEVEL_current_level && objects_config[i].screen == LEVEL_current_screen && !(objects_config[i].collected))
        {
            OBJECT_clear(&objects_config[i], false);
        }
    }
}