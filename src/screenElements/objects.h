#ifndef OBJECTS
#define OBJECTS

#include <genesis.h>
#include "resources.h"
#include "map.h"
#include "../utils/gameobject.h"
#include "level.h"
#include "../utils/globals.h"

#define MAX_OBJECTS 50 // Maximum number of game objects that can be managed.

// External declarations for specific game objects.
extern GameObject door;
extern GameObject key;
extern GameObject light_switch;

extern u16 keys_on_level[6];    // Expected number of keys in each of the 6 levels.
extern u16 switchs_on_level[6]; // Expected number of switches in each of the 6 levels.

// Configuration structure for a game object.
typedef struct
{
    GameObject obj; // Base GameObject (sprite, position, etc.).
    u8 type;        // Object type: 0 = door, 1 = switch, 2 = key, 3 = spark.
    u16 x;          // X position in pixels.
    u16 y;          // Y position in pixels.
    u8 collected;   // 1 if collected/activated, 0 otherwise.
    u8 level;       // Level index where the object appears.
    u8 screen;      // Screen index within the level.
    u8 on_screen;   // 1 if currently spawned on screen, 0 otherwise.
    u8 prio;        // Sprite priority: 1 = high, 0 = low.
} ObjectConfig;

// Global array of object configurations.
extern ObjectConfig objects_config[MAX_OBJECTS];

/**
 * @brief Checks for collision between the player and any game object.
 *
 * @param player_center_x Player's center X position.
 * @param player_center_y Player's center Y position.
 * @return Pointer to the collided ObjectConfig, or NULL if no collision.
 */
ObjectConfig *OBJECT_check_collision(u16 player_center_x, u16 player_center_y);

/**
 * @brief Sets all object parameters for the current level.
 */
void OBJECT_params();

/**
 * @brief Initializes a game object configuration.
 *
 * @param i Index in the objects_config array.
 * @param type Object type (0 = door, 1 = switch, 2 = key, 3 = spark).
 * @param x Initial X position (in metatile units).
 * @param y Initial Y position (in metatile units).
 * @param level Level index.
 * @param screen Screen index.
 * @param prio Sprite priority (1 = high, 0 = low).
 */
void OBJECT_init(u8 i, u8 type, u16 x, u16 y, u8 level, u8 screen, u8 prio);

/**
 * @brief Spawns an object on screen.
 *
 * @param i Index in the objects_config array.
 * @param ind Starting VRAM tile index for the sprite.
 * @return Next available VRAM tile index after the object's sprite.
 */
u16 OBJECT_spawn(u8 i, u16 ind);

/**
 * @brief Updates object visibility and state for the current screen.
 *
 * @param ind Current VRAM tile index.
 * @return Next available VRAM tile index after updates.
 */
u16 OBJECT_update(u16 ind);

/**
 * @brief Clears an object from the screen and optionally marks it as collected.
 *
 * @param config Pointer to the object configuration.
 * @param collect If 1, marks the object as collected.
 */
void OBJECT_clear(ObjectConfig *config, u8 collect);

/**
 * @brief Resets the 'collected' status for all key objects.
 */
void OBJECT_key_reset();

/**
 * @brief Clears all objects from the current screen.
 */
void OBJECT_clear_all();

#endif // OBJECTS
