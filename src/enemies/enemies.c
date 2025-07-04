#include "enemies.h" // Includes the header for enemy management.

Enemy enemy_pool[MAX_ENEMIES]; // Array to store all enemy configurations.

const f16 g_enemy_speed = 35; // Speed for ground enemies.
const f16 f_enemy_speed = 28; // Speed for flying enemies.

u8 enemy_counter = 0; // Counts the total number of enemies defined in the game.

/**
 * @brief Despawns all enemies currently active in the game.
 * Releases their sprite resources, typically called during level changes or resets.
 */
void ENEMIES_level_change_despawn()
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        if (enemy_pool[i].firefly.sprite != NULL) // Check if the enemy has an active sprite.
        {
            ENEMY_unspwan(i); // Despawn the enemy.
        }
    }
}

/**
 * @brief Defines the parameters for all enemies in the game.
 * Initializes each enemy's type, starting position, spawn screen, and level.
 */
void ENEMY_params()
{
    // LEVEL 1 enemies
    ENEMY_init(0, GROUND, 6, 9, LEVEL_SCREEN_1, 0); // Ground enemy on screen 1, level 0.
    ENEMY_init(1, FLYING, 2, 5, LEVEL_SCREEN_1, 0); // Flying enemy on screen 1, level 0.

    // LEVEL 2 enemies
    ENEMY_init(2, FLYING, 9, 8, LEVEL_SCREEN_1, 1); // Flying enemy on screen 1, level 1.
    ENEMY_init(3, GROUND, 4, 9, LEVEL_SCREEN_2, 1); // Ground enemy on screen 2, level 1.
    ENEMY_init(4, GROUND, 7, 4, LEVEL_SCREEN_2, 1); // Ground enemy on screen 2, level 1.

    // LEVEL 3 enemies
    ENEMY_init(5, GROUND, 14, 12, LEVEL_SCREEN_1, 2); // Ground enemy on screen 1, level 2.
    ENEMY_init(6, FLYING, 10, 4, LEVEL_SCREEN_2, 2);  // Flying enemy on screen 2, level 2.
    ENEMY_init(7, FLYING, 14, 6, LEVEL_SCREEN_2, 2);  // Flying enemy on screen 2, level 2.
    ENEMY_init(8, FLYING, 11, 8, LEVEL_SCREEN_2, 2);  // Flying enemy on screen 2, level 2.
    ENEMY_init(9, FLYING, 12, 9, LEVEL_SCREEN_2, 2);  // Flying enemy on screen 2, level 2.
    ENEMY_init(10, GROUND, 4, 12, LEVEL_SCREEN_5, 2); // Ground enemy on screen 5, level 2.

    // LEVEL 4 enemies
    ENEMY_init(11, FLYING, 5, 7, LEVEL_SCREEN_1, 3);   // Flying enemy on screen 1, level 3.
    ENEMY_init(12, FLYING, 8, 8, LEVEL_SCREEN_4, 3);   // Flying enemy on screen 4, level 3.
    ENEMY_init(13, FLYING, 12, 8, LEVEL_SCREEN_4, 3);  // Flying enemy on screen 4, level 3.
    ENEMY_init(14, GROUND, 6, 12, LEVEL_SCREEN_4, 3);  // Ground enemy on screen 4, level 3.
    ENEMY_init(15, GROUND, 16, 12, LEVEL_SCREEN_4, 3); // Ground enemy on screen 4, level 3.
    ENEMY_init(16, GROUND, 6, 5, LEVEL_SCREEN_5, 3);   // Ground enemy on screen 5, level 3.
    ENEMY_init(17, FLYING, 13, 9, LEVEL_SCREEN_5, 3);  // Flying enemy on screen 5, level 3.

    // LEVEL 5 enemies
    ENEMY_init(18, GROUND, 2, 9, LEVEL_SCREEN_1, 5);   // Ground enemy on screen 1, level 5.
    ENEMY_init(19, GROUND, 11, 5, LEVEL_SCREEN_1, 5);  // Ground enemy on screen 1, level 5.
    ENEMY_init(20, FLYING, 6, 4, LEVEL_SCREEN_1, 5);   // Flying enemy on screen 1, level 5.
    ENEMY_init(21, FLYING, 3, 5, LEVEL_SCREEN_2, 5);   // Flying enemy on screen 2, level 5.
    ENEMY_init(22, FLYING, 16, 10, LEVEL_SCREEN_2, 5); // Flying enemy on screen 2, level 5.
    ENEMY_init(23, GROUND, 11, 9, LEVEL_SCREEN_3, 5);  // Ground enemy on screen 3, level 5.
    ENEMY_init(24, GROUND, 2, 9, LEVEL_SCREEN_3, 5);   // Ground enemy on screen 3, level 5.
    ENEMY_init(25, FLYING, 7, 6, LEVEL_SCREEN_3, 5);   // Flying enemy on screen 3, level 5.
}

/**
 * @brief Initializes a single enemy's configuration within the enemy_pool.
 * @param index The index in the enemy_pool array.
 * @param type The enemy's movement type (GROUND/FLYING).
 * @param start_x Initial X-coordinate (in metatile units).
 * @param start_y Initial Y-coordinate (in metatile units).
 * @param screen The screen where this enemy spawns.
 * @param level The level where this enemy spawns.
 */
void ENEMY_init(u8 index, u8 type, u16 start_x, u16 start_y, u8 screen, u8 level)
{
    // Calculate initial pixel coordinates from metatile units.
    u16 x_calc = start_x * METATILE_W;
    u16 start_y_calc = start_y * METATILE_W;

    enemy_pool[index].start_x = x_calc;       // Store calculated start X.
    enemy_pool[index].start_y = start_y_calc; // Store calculated start Y.
    enemy_pool[index].type = type;            // Set enemy type.
    enemy_pool[index].on_screen = 0;          // Mark as not on screen initially.
    enemy_pool[index].spawn_level = level;    // Set spawn level.
    enemy_pool[index].spawn_screen = screen;  // Set spawn screen.
    enemy_counter++;                          // Increment total enemy count.
}

/**
 * @brief Manages spawning and despawning of enemies based on the current screen and level.
 * @param ind The current VRAM tile index for sprite allocation.
 * @return The next available VRAM tile index after managing enemies.
 */
u8 ENEMIES_spawn_hub(u8 ind)
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        // Check if the enemy should be on the current screen and level.
        if (enemy_pool[i].spawn_screen == LEVEL_current_screen && enemy_pool[i].spawn_level == LEVEL_current_level)
        {
            ind += ENEMY_spawn(i, ind); // Spawn the enemy.
        }
        else if (enemy_pool[i].firefly.sprite != NULL) // If enemy is active but not on current screen/level.
        {
            ENEMY_unspwan(i); // Despawn the enemy.
        }
    }
    return ind; // Return updated VRAM index.
}

/**
 * @brief Despawns a specific enemy by releasing its sprite resources.
 * @param index The index of the enemy to despawn in the enemy_pool.
 */
void ENEMY_unspwan(u8 index)
{
    SPR_releaseSprite(enemy_pool[index].firefly.sprite); // Release the enemy's sprite.
    enemy_pool[index].firefly.sprite = NULL;             // Nullify the sprite pointer.
    enemy_pool[index].on_screen = 0;                     // Mark as not on screen.
    SPR_update();                                        // Update the sprite engine to reflect changes.
}

/**
 * @brief Spawns a specific enemy by its index, initializing its GameObject and speed.
 * @param index The index of the enemy in the enemy_pool to spawn.
 * @param ind The current VRAM tile index for sprite allocation.
 * @return The next available VRAM tile index after spawning the enemy.
 */
u8 ENEMY_spawn(u8 index, u8 ind)
{
    switch (enemy_pool[index].type)
    {
    case 0: // Ground enemy.
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_g_enemy, enemy_pool[index].start_x, enemy_pool[index].start_y, PAL_GAME, false, ind);
        enemy_pool[index].firefly.speed_x = g_enemy_speed; // Set ground enemy speed.
        break;
    case 1: // Flying enemy.
        ind += GAMEOBJECT_init(&enemy_pool[index].firefly, &spr_f_enemy, enemy_pool[index].start_x, enemy_pool[index].start_y, PAL_GAME, false, ind);
        enemy_pool[index].firefly.speed_x = f_enemy_speed; // Set flying enemy speed.
        break;
    default:
        break;
    }
    enemy_pool[index].on_screen = 1; // Mark enemy as on screen.
    return ind;                      // Return updated VRAM index.
}

/**
 * @brief Updates all currently active enemies.
 * Iterates through the enemy pool and calls the appropriate update function based on enemy type.
 */
void ENEMIES_update_hub()
{
    for (u8 i = 0; i < enemy_counter; i++)
    {
        if (enemy_pool[i].on_screen) // Only update enemies that are currently on screen.
        {
            switch (enemy_pool[i].type)
            {
            case 0: // Ground enemy.
                ENEMIES_g_enemy_update(&enemy_pool[i].firefly);
                break;
            case 1: // Flying enemy (currently using same update as ground enemy in this code).
                ENEMIES_g_enemy_update(&enemy_pool[i].firefly);
                break;
            }
        }
    }
}

/**
 * @brief Updates a ground-bound (or general) enemy's movement and collision with walls.
 * Reverses direction and flips sprite if a wall is encountered.
 * @param firefly Pointer to the GameObject of the enemy to update.
 */
void ENEMIES_g_enemy_update(GameObject *firefly)
{
    firefly->next_x = firefly->x + firefly->speed_x;                  // Calculate next X position.
    GAMEOBJECT_update_boundbox(firefly->next_x, firefly->y, firefly); // Update bounding box for next position.

    if (firefly->speed_x > 0) // Moving right.
    {
        // Check for wall collision at multiple points on the right side.
        if (LEVEL_wall_at(firefly->box.right, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.right, firefly->box.bottom - 1))
        {
            // If collision, snap to wall, flip sprite, and reverse direction.
            firefly->next_x = FIX16(firefly->box.right / METATILE_W * METATILE_W - firefly->w);
            SPR_setHFlip(firefly->sprite, true);  // Flip horizontally.
            firefly->speed_x = -firefly->speed_x; // Reverse direction.
        }
    }
    else if (firefly->speed_x < 0) // Moving left.
    {
        // Check for wall collision at multiple points on the left side.
        if (LEVEL_wall_at(firefly->box.left, firefly->box.top) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.top + firefly->h / 2) ||
            LEVEL_wall_at(firefly->box.left, firefly->box.bottom - 1))
        {
            // If collision, snap to wall, flip sprite, and reverse direction.
            firefly->next_x = FIX16((firefly->box.left / METATILE_W + 1) * METATILE_W);
            SPR_setHFlip(firefly->sprite, false); // No horizontal flip.
            firefly->speed_x = -firefly->speed_x; // Reverse direction.
        }
    }

    firefly->x = firefly->next_x; // Apply the calculated next X position.
    // kprintf("speed vagalume: %u, g_speed: %u", firefly->speed_x, g_enemy_speed); // Debug print.
    SPR_setPosition(firefly->sprite, fix16ToInt(firefly->x), fix16ToInt(firefly->y)); // Update sprite position.
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