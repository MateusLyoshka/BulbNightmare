#ifndef _ENEMIES
#define _ENEMIES

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../utils/gameobject.h"
#include "../screenElements/level.h"
#include "../player/player.h"

#define GROUND 0
#define FLYING 1
#define MAX_ENEMIES 30

extern u8 enemy_counter;

/**
 * @brief Enemy definition and runtime data.
 */
typedef struct
{
    GameObject firefly;
    u8 type;
    u16 start_x;
    u16 start_y;
    u8 spawn_screen;
    u8 spawn_level;
    u8 on_screen;
} Enemy;

extern Enemy enemy_pool[MAX_ENEMIES];

/**
 * @brief Sets up all predefined enemy parameters.
 */
void ENEMY_params();

/**
 * @brief Initializes a specific enemy in the pool.
 *
 * @param index Index in the enemy_pool.
 * @param type Enemy type (GROUND or FLYING).
 * @param last_x X position in metatile units.
 * @param last_y Y position in metatile units.
 * @param screen Screen ID where it spawns.
 * @param level Level ID where it spawns.
 */
void ENEMY_init(u8 index, u8 type, u16 last_x, u16 last_y, u8 screen, u8 level);

/**
 * @brief Spawns enemies for the current screen and level.
 *
 * @param ind Current VRAM tile index.
 * @return Next available tile index.
 */
u8 ENEMIES_spawn_hub(u8 ind);

/**
 * @brief Spawns a specific enemy by index.
 *
 * @param index Enemy index.
 * @param ind Current VRAM tile index.
 * @return Next available tile index.
 */
u8 ENEMY_spawn(u8 index, u8 ind);

/**
 * @brief Despawns an enemy.
 *
 * @param index Enemy index.
 */
void ENEMY_unspwan(u8 index);

/**
 * @brief Updates all active enemies.
 */
void ENEMIES_update_hub();

/**
 * @brief Updates a ground enemy's logic and movement.
 *
 * @param firefly Pointer to enemy GameObject.
 */
void ENEMIES_g_enemy_update(GameObject *firefly);

/**
 * @brief Despawns all enemies (used during level transitions).
 */
void ENEMIES_level_change_despawn();

// void ENEMIES_f_enemy_update(GameObject *firefly, u8 i); // Flying enemy logic (unused)

#endif // _ENEMIES
