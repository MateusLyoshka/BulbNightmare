#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/gameobject.h"
#include "../screenElements/level.h"
#include "../enemies/enemies.h"
#include "../screenElements/objects.h"
#include "../screenElements/darkness.h"

#define PLAYER_LIVES 9 // Initial number of lives the player starts with.

typedef struct
{
    fix16 initial_x; // Initial X position (fixed-point).
    fix16 initial_y; // Initial Y position (fixed-point).
} PlayerSpawnPoint;

typedef struct
{
    u16 x;      // Player center X in pixels.
    u16 y;      // Player center Y in pixels.
    u16 tile_x; // Tile X index of the player's center.
    u16 tile_y; // Tile Y index of the player's center.
} PlayerCenter;

extern PlayerCenter player_center;

#define PLAYER_MAX_GRAVITY 300 // Max gravity magnitude applied to the player.

extern GameObject player;
extern f16 player_gravity;
extern f16 player_speed;
extern u8 player_is_alive;
extern u8 player_keys;
extern u8 player_lives;
extern u8 switchs_on;
extern u8 player_can_jump;
extern u8 player_can_walk;
extern PlayerSpawnPoint player_spawn;
extern u8 player_cheat_on;

/**
 * @brief Initializes the player sprite and properties.
 *
 * @param ind Starting VRAM tile index.
 * @return Next available VRAM tile index after loading the player sprite.
 */
u16 PLAYER_init(u16 ind);

/**
 * @brief Updates the player (movement, input, collisions).
 */
void PLAYER_update();

/**
 * @brief Reads player input and sets movement flags.
 */
void PLAYER_get_input();

/**
 * @brief Checks if the player is touching the ground.
 *
 * @return 1 if on ground, 0 otherwise.
 */
u8 PLAYER_on_ground();

/**
 * @brief Checks all types of collisions involving the player.
 */
void PLAYER_check_collisions();

/**
 * @brief Checks for collisions with objects (e.g., keys, switches).
 */
void PLAYER_object_collision();

/**
 * @brief Checks for collisions with spike hazards.
 */
void PLAYER_spike_collision();

/**
 * @brief Checks for collisions with enemies.
 */
void PLAYER_enemy_collision();

/**
 * @brief Plays the player death animation.
 */
void PLAYER_death_anim();

/**
 * @brief Respawns the player at the saved spawn point.
 */
void PLAYER_respawn();

/**
 * @brief Frees player-related sprite resources.
 */
void PLAYER_free();

/**
 * @brief Inverts the direction of gravity affecting the player.
 */
void PLAYER_invert_gravity();

#endif // _PLAYER_H_
