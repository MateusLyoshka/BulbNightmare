#include <genesis.h>
#include "level.h"
// #define DEBUG // Commented out: Uncomment this line to enable debug features like drawing the collision map.

Map *map; // Pointer to the current game map structure.
// 2D array representing the collision map for the current screen,
// including off-screen areas for smoother collision detection.
u8 collision_map[SCREEN_METATILES_W + OFFSCREEN_TILES * 2][SCREEN_METATILES_H + OFFSCREEN_TILES * 2] = {0};

u8 collision_result = 0;         // Stores a bitmask of collision directions (left, right, top, bottom).
u8 LEVEL_current_level = 0;      // The index of the current game level, initialized to 0.
u8 LEVEL_current_screen = 0;     // The index of the current screen within the level, initialized to 0.
u8 LEVEL_bool_screen_change = 0; // Flag to indicate if a screen change has occurred, initialized to 0.
u8 LEVEL_bool_level_change = 0;  // Flag to indicate if a level change has occurred, initialized to 0.
u8 LEVEL_last_screen = 6;        // Stores the index of the last visited screen, initialized to 6.

// Top-Left screen position in the overall map, in pixels.
u16 screen_x = 0; // Current X-coordinate of the camera
u16 screen_y = 0; // Current Y-coordinate of the camera

/**
 * @brief Generates the collision map for the current screen.
 *
 * This function populates the `collision_map` array based on the tiles
 * in the currently loaded map, marking areas as collidable (1) or hazard (2).
 *
 * @param first_index The minimum tile index that is considered for collision.
 * @param last_index The maximum tile index that is considered for collision.
 */
void LEVEL_generate_screen_collision_map(u8 first_index, u8 last_index)
{
    // Calculate the starting tile position in the overall map, adjusted for off-screen tiles.
    s16 map_start_x = (screen_x / METATILE_W) - OFFSCREEN_TILES;
    s16 map_start_y = (screen_y / METATILE_W) - OFFSCREEN_TILES;

    // Clear the entire collision map before generating new data.
    memset(collision_map, 0, sizeof(collision_map));
    // Iterate over the visible screen area (excluding off-screen borders initially).
    for (u8 x = 0; x < SCREEN_METATILES_W; ++x)
    {
        for (u8 y = 0; y < SCREEN_METATILES_H; ++y)
        {
            // Calculate the actual tile position in the map data, including off-screen buffer.
            u16 tile_pos_x = map_start_x + x + OFFSCREEN_TILES;
            u16 tile_pos_y = map_start_y + y + OFFSCREEN_TILES;

            // Get the tile index from the map data, masking to get the base tile ID.
            u16 tile_index = MAP_getTile(map, tile_pos_x * (METATILE_W / 8), tile_pos_y * (METATILE_W / 8)) & 0x03FF;

            // Check if the tile index falls within the specified range for collision detection.
            if (tile_index >= first_index && tile_index <= last_index)
            {
                // Assign collision type based on tile index.
                if (tile_index == BLOCKS_LEVEL_INDEX)
                    collision_map[x][y] = 1; // Solid block.
                else if (tile_index == BOTTOM_SPIKE_LEVEL_INDEX || tile_index == TOP_SPIKE_LEVEL_INDEX)
                    collision_map[x][y] = 2; // Spike.
                else
                    collision_map[x][y] = 0; // No collision.
            }
            else
            {
                // If tile is outside the collision range, ensure it's marked as non-collidable.
                collision_map[x + OFFSCREEN_TILES][y + OFFSCREEN_TILES] = 0;
            }
        }
    }
}

u16 screen_x_control = 0;   // Internal control variable for X-scrolling.
u16 screen_y_control = 448; // Internal control variable for Y-scrolling.

/**
 * @brief Initializes the current game level.
 *
 * This function sets up the initial screen coordinates, determines the current screen,
 * loads the appropriate map, and generates the initial collision map.
 *
 * @param ind The starting VRAM tile index for the level's tileset.
 * @return The next available VRAM tile index after loading the tileset.
 */
u16 LEVEL_init(u16 ind)
{
    screen_x_control = 0;   // Reset X scroll control.
    screen_y_control = 448; // Reset Y scroll control.
    screen_x = 0;           // Reset current screen X.
    screen_y = 448;         // Reset current screen Y.

    // Calculate current screen column and row based on pixel coordinates.
    u8 col = screen_x / SCREEN_W;
    u8 row = screen_y / SCREEN_H;
    // Determine the current screen index.
    LEVEL_current_screen = row * MAP_X_SCREENS + col;
    LEVEL_last_screen = LEVEL_current_screen; // Store the initial screen as the last screen.

    // Load the main tileset for the level into VRAM.
    VDP_loadTileSet(&tiles, ind, DMA);

    LEVEL_map_clear(); // Clear any previously loaded map.

    // Load the appropriate map based on the current level.
    if (LEVEL_current_level == 0)
    {
        map = MAP_create(&level1_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    else if (LEVEL_current_level == 1)
    {
        map = MAP_create(&level2_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    else if (LEVEL_current_level == 2)
    {
        map = MAP_create(&level3_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    else if (LEVEL_current_level == 3)
    {
        map = MAP_create(&level4_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    else if (LEVEL_current_level == 4)
    {
        map = MAP_create(&level5_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    else if (LEVEL_current_level == 5)
    {
        map = MAP_create(&level6_map, BG_B, TILE_ATTR_FULL(PAL_BACKGROUND_B, FALSE, FALSE, FALSE, ind));
    }
    // Scroll the map to the initial position.
    MAP_scrollToEx(map, 0, screen_y, TRUE);

    // Update the VRAM index to reflect the space taken by the tileset.
    ind += tiles.numTile;
    // Generate the collision map for the loaded screen.
    LEVEL_generate_screen_collision_map(0, 5);

    return ind; // Return the new VRAM index.
}

/**
 * @brief Clears the current map from the VDP and releases its resources.
 */
void LEVEL_map_clear()
{
    MAP_release(map);
    VDP_clearPlane(BG_B, true);
}

/**
 * @brief Handles object movement and collision detection/resolution with the level geometry.
 *
 * This function checks for collisions in X and Y directions and adjusts the
 * object's `next_x` and `next_y` positions to prevent penetration.
 * It updates `collision_result` with the detected collision directions.
 *
 * @param obj Pointer to the GameObject to be moved and checked for collisions.
 */
void LEVEL_move_and_slide(GameObject *obj)
{
    collision_result = 0; // Reset collision result.
    // Update the object's bounding box based on its potential next X position and current Y.
    GAMEOBJECT_update_boundbox(obj->next_x, obj->y, obj);

    if (obj->speed_x > 0)
    { // Moving right
        // Check for collision on the right side at top, middle, and bottom points.
        if (LEVEL_wall_at(obj->box.right, obj->box.top) ||
            LEVEL_wall_at(obj->box.right, obj->box.top + obj->h / 2) ||
            LEVEL_wall_at(obj->box.right, obj->box.bottom - 1))
        {
            // If collision, snap the object to the left edge of the colliding tile.
            obj->next_x = FIX16(obj->box.right / METATILE_W * METATILE_W - obj->w);
            collision_result |= COLLISION_RIGHT; // Set right collision flag.
        }
    }

    if (obj->speed_x < 0)
    { // Moving left
        // Check for collision on the left side at top, middle, and bottom points.
        if (LEVEL_wall_at(obj->box.left, obj->box.top) ||
            LEVEL_wall_at(obj->box.left, obj->box.top + obj->h / 2) ||
            LEVEL_wall_at(obj->box.left, obj->box.bottom - 1))
        {
            // If collision, snap the object to the right edge of the colliding tile.
            obj->next_x = FIX16((obj->box.left / METATILE_W + 1) * METATILE_W);
            collision_result |= COLLISION_LEFT; // Set left collision flag.
        }
    }

    // Update the object's bounding box based on its potential next X and next Y positions.
    GAMEOBJECT_update_boundbox(obj->next_x, obj->next_y, obj);

    if (obj->speed_y < 0)
    { // Moving up
        // Check for collision on the top side at left, middle, and right points.
        if (LEVEL_wall_at(obj->box.left, obj->box.top) ||
            LEVEL_wall_at(obj->box.left + obj->w / 2, obj->box.top) ||
            LEVEL_wall_at(obj->box.right - 1, obj->box.top))
        {
            // If collision, snap the object to the bottom edge of the colliding tile.
            obj->next_y = FIX16((obj->box.top / METATILE_W + 1) * METATILE_W);
            collision_result |= COLLISION_TOP; // Set top collision flag.
        }
    }

    else if (obj->speed_y > 0)
    { // Moving down
        // Check for collision on the bottom side at left, middle, and right points.
        if (LEVEL_wall_at(obj->box.left, obj->box.bottom) ||
            LEVEL_wall_at(obj->box.left + obj->w / 2, obj->box.bottom) ||
            LEVEL_wall_at(obj->box.right - 1, obj->box.bottom))
        {
            // If collision, snap the object to the top edge of the colliding tile.
            obj->next_y = FIX16((obj->box.bottom / METATILE_W) * METATILE_W - obj->h);
            collision_result |= COLLISION_BOTTOM; // Set bottom collision flag.
        }
    }
}

/**
 * @brief Updates the screen scroll position and regenerates the collision map.
 *
 * This function is called when the camera needs to move, typically due to
 * player movement across screen boundaries. It updates global screen coordinates
 * and triggers collision map regeneration.
 *
 * @param offset_x The new X-coordinate for the top-left of the screen.
 * @param offset_y The new Y-coordinate for the top-left of the screen.
 */
void LEVEL_scroll_update_collision(s16 offset_x, s16 offset_y)
{

    screen_x = offset_x; // Update global screen X.
    screen_y = offset_y; // Update global screen Y.

    // Receive the offset again in case the function was called from another file (allows teleporting the player anywhere, anytime)
    screen_x_control = offset_x; // Update X scroll control.
    screen_y_control = offset_y; // Update Y scroll control.

    // Calculate current screen column and row.
    u8 col = screen_x / SCREEN_W;
    u8 row = screen_y / SCREEN_H;

    // If the player is alive, mark that a screen change has occurred.
    if (player_is_alive)
    {
        LEVEL_bool_screen_change = 1;
    }
    LEVEL_last_screen = LEVEL_current_screen;         // Store the current screen as the last screen.
    LEVEL_current_screen = row * MAP_X_SCREENS + col; // Update the current screen index.

    MAP_scrollTo(map, screen_x, screen_y);     // Scroll the map to the new position.
    LEVEL_generate_screen_collision_map(0, 5); // Regenerate collision map for the new screen.

#ifdef DEBUG
    LEVEL_draw_collision_map(); // Draw collision map for debugging if DEBUG is defined.
#endif
}

/**
 * @brief Updates the camera position based on the player's position, handling screen transitions.
 *
 * This function checks if the player has moved beyond the current screen boundaries
 * and, if so, adjusts the player's position to the opposite side of the new screen
 * and triggers a screen scroll update.
 *
 * @param obj Pointer to the GameObject (player) whose position determines camera updates.
 */
void LEVEL_update_camera(GameObject *obj)
{
    if (obj->x > (FIX16(SCREEN_W) - obj->w / 2))
    {                                                                      // Player moved past right boundary
        obj->x = 0;                                                        // Wrap player to left side of new screen.
        screen_x_control += SCREEN_W;                                      // Increment screen X control by one screen width.
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control); // Trigger screen change.
    }
    else if (obj->x < (FIX16(-obj->w / 2)))
    {                                                                      // Player moved past left boundary
        obj->x = FIX16(SCREEN_W - obj->w);                                 // Wrap player to right side of new screen.
        screen_x_control -= SCREEN_W;                                      // Decrement screen X control by one screen width.
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control); // Trigger screen change.
    }

    if (obj->y > (FIX16(SCREEN_H) - obj->h / 2))
    {                                                                      // Player moved past bottom boundary
        obj->y = 0;                                                        // Wrap player to top side of new screen.
        screen_y_control += SCREEN_H;                                      // Increment screen Y control by one screen height.
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control); // Trigger screen change.
    }
    else if (obj->y < (FIX16(-obj->h / 2)))
    {                                                                      // Player moved past top boundary
        obj->y = FIX16(SCREEN_H - obj->h);                                 // Wrap player to bottom side of new screen.
        screen_y_control -= SCREEN_H;                                      // Decrement screen Y control by one screen height.
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control); // Trigger screen change.
    }
}

/**
 * @brief Draws the collision map onto BG_A for debugging purposes.
 *
 * This function displays the collision values (1 for wall, 2 for spike, 0 for empty)
 * as text on the screen, useful for visualizing the collision grid.
 */
void LEVEL_draw_collision_map()
{
    VDP_setTextPlane(BG_A);
    PAL_setColor(15, RGB24_TO_VDPCOLOR(0xFFFFFF)); // Set color 15 to white for text.
    // Iterate through the collision map and draw each value.
    for (u8 x = 0; x < SCREEN_METATILES_W; ++x)
    {
        for (u8 y = 0; y < SCREEN_METATILES_H; ++y)
        {
            char text[2];                                               // Buffer for converting int to string.
            intToStr(collision_map[x][y], text, 1);                     // Convert collision value to string.
            VDP_drawText(text, x * METATILE_W / 8, y * METATILE_W / 8); // Draw text at tile position.
        }
    }
}

/**
 * @brief Checks if a wall tile exists at the given pixel coordinates.
 *
 * This function queries the collision map to determine if the specified
 * location corresponds to a solid wall.
 *
 * @param x X-coordinate in pixels.
 * @param y Y-coordinate in pixels.
 * @return 1 if a wall tile (collision_map value 1) is found, 0 otherwise.
 */
u8 LEVEL_wall_at(s16 x, s16 y)
{
    // Get the collision value from the map based on the tile coordinates.
    u8 value = collision_map[x / METATILE_W][y / METATILE_W];
    // Return 1 if it's a wall, otherwise 0.
    if (value == 1)
    {
        return 1;
    }
    else
        return 0;
}

/**
 * @brief Handles an "alert" sequence for the current level.
 *
 * This typically involves displaying a full-screen image with a fade-in/fade-out effect,
 * often used for level transitions, boss introductions, or important events.
 * The background plane is changed to BG_A and then cleared after the alert.
 *
 * @param ind The current VRAM tile index for loading background images.
 * @return The next available VRAM tile index after the alert sequence is complete.
 */
u16 LEVEL_alert(u16 ind)
{
    // Reset horizontal and vertical scroll for BG_A to 0 (fixed position).
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);

    ind = BACKGROUND_init_generalized(LEVEL_current_level, 1, PAL0, TRUE, false, ind);

    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);

    fadeOut(60, PAL0);

    ind = BACKGROUND_full_clear(ind);
    return ind;
}