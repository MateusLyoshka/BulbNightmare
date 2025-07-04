#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <genesis.h>
#include "../utils/globals.h"
#include "map.h"
#include "../utils/gameobject.h"
#include "resources.h"
#include "background.h"
#include "../player/player.h"

#define NUMBER_OF_LEVELS 5 // Total number of levels in the game.

// Global state variables for level and screen.
extern u8 LEVEL_current_level;      // Current level index.
extern u8 LEVEL_current_screen;     // Current screen index within the level.
extern u8 LEVEL_bool_level_change;  // Flag indicating if the level changed.
extern u8 LEVEL_bool_screen_change; // Flag indicating if the screen changed.
extern u8 LEVEL_last_screen;        // Index of the last visited screen.

extern u8 collision_result; // Result of latest collision check.
extern Map *map;            // Pointer to current map instance.

#define OFFSCREEN_TILES 3                                                                                    // Number of metatiles added around the screen for smoother collisions.
extern u8 collision_map[SCREEN_METATILES_W + OFFSCREEN_TILES * 2][SCREEN_METATILES_H + OFFSCREEN_TILES * 2]; // Collision map with padding.

// Bitmasks for collision detection.
#define COLLISION_LEFT 0b0001
#define COLLISION_RIGHT 0b0010
#define COLLISION_TOP 0b0100
#define COLLISION_BOTTOM 0b1000

// Tile indices for map block types.
#define BLOCKS_LEVEL_INDEX 0
#define BOTTOM_SPIKE_LEVEL_INDEX 2
#define TOP_SPIKE_LEVEL_INDEX 3
#define BACKGROUND_LEVEL_INDEX 4

// Screen scroll positions.
extern u16 screen_x;
extern u16 screen_y;

/**
 * @brief Checks whether there is a wall at a given position.
 *
 * @param x X-coordinate in pixels.
 * @param y Y-coordinate in pixels.
 * @return 1 if a wall is present, 0 otherwise.
 */
u8 LEVEL_wall_at(s16 x, s16 y);

/**
 * @brief Initializes a level: loads map, sets scroll and state.
 *
 * @param ind Starting VRAM tile index.
 * @return Next available VRAM tile index.
 */
u16 LEVEL_init(u16 ind);

/**
 * @brief Generates the collision map for the current screen.
 *
 * @param first_index First tile index considered solid.
 * @param last_index Last tile index considered solid.
 */
void LEVEL_generate_screen_collision_map(u8 first_index, u8 last_index);

/**
 * @brief Draws the collision map on screen (for debugging).
 */
void LEVEL_draw_collision_map();

/**
 * @brief Moves a GameObject while handling collisions.
 *
 * @param obj Pointer to the GameObject to move.
 */
void LEVEL_move_and_slide(GameObject *obj);

/**
 * @brief Updates scroll position and regenerates collision map.
 *
 * @param offset_x New X offset for scroll.
 * @param offset_y New Y offset for scroll.
 */
void LEVEL_scroll_update_collision(s16 offset_x, s16 offset_y);

/**
 * @brief Updates the camera position based on a GameObject's position.
 *
 * @param obj Pointer to the object (e.g., player) to center the camera on.
 */
void LEVEL_update_camera(GameObject *obj);

/**
 * @brief Clears the current level map from VRAM.
 */
void LEVEL_map_clear();

/**
 * @brief Displays a level alert or transition animation.
 *
 * @param ind Current VRAM tile index.
 * @return Next available VRAM tile index.
 */
u16 LEVEL_alert(u16 ind);

#endif // _LEVEL_H_
