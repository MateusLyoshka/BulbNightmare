#include <genesis.h>
#include "level.h"
// #define DEBUG

Map *map;
u8 collision_map[SCREEN_METATILES_W + OFFSCREEN_TILES * 2][SCREEN_METATILES_H + OFFSCREEN_TILES * 2] = {0};

u8 collision_result = 0;
u8 LEVEL_current_level = 4;
u8 LEVEL_current_screen = 0;
u8 LEVEL_bool_screen_change = 0;
u8 LEVEL_bool_level_change = 0;

// Top-Left screen position in map
u16 screen_x = 0;
u16 screen_y = 0;

void LEVEL_generate_screen_collision_map(u8 first_index, u8 last_index)
{
    // Calcula a posição inicial em tiles no mapa geral
    s16 map_start_x = (screen_x / METATILE_W) - OFFSCREEN_TILES;
    s16 map_start_y = (screen_y / METATILE_W) - OFFSCREEN_TILES;

    memset(collision_map, 0, sizeof(collision_map));
    // Preenche apenas a área visível (não as bordas offscreen)
    for (u8 x = 0; x < SCREEN_METATILES_W; ++x)
    {
        for (u8 y = 0; y < SCREEN_METATILES_H; ++y)
        {
            // Calcula a posição real no mapa
            u16 tile_pos_x = map_start_x + x + OFFSCREEN_TILES;
            u16 tile_pos_y = map_start_y + y + OFFSCREEN_TILES;

            u16 tile_index = MAP_getTile(map, tile_pos_x * (METATILE_W / 8), tile_pos_y * (METATILE_W / 8)) & 0x03FF;

            if (tile_index >= first_index && tile_index <= last_index)
            {
                if (tile_index == BLOCKS_LEVEL_INDEX)
                    collision_map[x][y] = 1;
                else if (tile_index == BOTTOM_SPIKE_LEVEL_INDEX || tile_index == TOP_SPIKE_LEVEL_INDEX)
                    collision_map[x][y] = 2;
            }
            else
            {
                collision_map[x + OFFSCREEN_TILES][y + OFFSCREEN_TILES] = 0;
            }
        }
    }
}

u16 screen_x_control = 0;
u16 screen_y_control = 448;

u16 LEVEL_init(u16 ind)
{
    screen_x_control = 0;
    screen_y_control = 448;
    screen_x = 0;
    screen_y = 448;
    u8 col = screen_x / SCREEN_W;
    u8 row = screen_y / SCREEN_H;
    LEVEL_current_screen = row * MAP_X_SCREENS + col;

    PAL_setPalette(PAL_BACKGROUND_B, level_pal.data, DMA);
    VDP_loadTileSet(&tiles, ind, DMA);

    // kprintf("current level %d", LEVEL_current_level);
    LEVEL_map_clear();
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
    MAP_scrollToEx(map, 0, screen_y, TRUE);

    ind += tiles.numTile;
    LEVEL_generate_screen_collision_map(0, 5);

    return ind;
}

void LEVEL_map_clear()
{
    MAP_release(map);
    VDP_clearPlane(BG_B, true);
}

void LEVEL_move_and_slide(GameObject *obj)
{
    collision_result = 0;
    GAMEOBJECT_update_boundbox(obj->next_x, obj->y, obj);

    if (obj->speed_x > 0)
    { // moving right
        if (LEVEL_wall_at(obj->box.right, obj->box.top) ||
            LEVEL_wall_at(obj->box.right, obj->box.top + obj->h / 2) ||
            LEVEL_wall_at(obj->box.right, obj->box.bottom - 1))
        {
            obj->next_x = FIX16(obj->box.right / METATILE_W * METATILE_W - obj->w);
            collision_result |= COLLISION_RIGHT;
        }
    }

    if (obj->speed_x < 0)
    { // moving left
        if (LEVEL_wall_at(obj->box.left, obj->box.top) ||
            LEVEL_wall_at(obj->box.left, obj->box.top + obj->h / 2) ||
            LEVEL_wall_at(obj->box.left, obj->box.bottom - 1))
        {
            obj->next_x = FIX16((obj->box.left / METATILE_W + 1) * METATILE_W);
            collision_result |= COLLISION_LEFT;
        }
    }

    GAMEOBJECT_update_boundbox(obj->next_x, obj->next_y, obj);

    if (obj->speed_y < 0)
    { // moving up
        if (LEVEL_wall_at(obj->box.left, obj->box.top) ||
            LEVEL_wall_at(obj->box.left + obj->w / 2, obj->box.top) ||
            LEVEL_wall_at(obj->box.right - 1, obj->box.top))
        {
            obj->next_y = FIX16((obj->box.top / METATILE_W + 1) * METATILE_W);
            collision_result |= COLLISION_TOP;
        }
    }

    else if (obj->speed_y > 0)
    { // moving down
        if (LEVEL_wall_at(obj->box.left, obj->box.bottom) ||
            LEVEL_wall_at(obj->box.left + obj->w / 2, obj->box.bottom) ||
            LEVEL_wall_at(obj->box.right - 1, obj->box.bottom))
        {
            obj->next_y = FIX16((obj->box.bottom / METATILE_W) * METATILE_W - obj->h);
            collision_result |= COLLISION_BOTTOM;
        }
    }
}

void LEVEL_scroll_update_collision(s16 offset_x, s16 offset_y)
{
    // kprintf("screen x %d", screen_x);
    // kprintf("screen y %d", screen_y);
    // kprintf("actual screen %d", LEVEL_current_screen);
    screen_x = offset_x;
    screen_y = offset_y;

    // Receive the offset again in case the function was called from another file (allows teleporting the player anywhere, anytime)
    screen_x_control = offset_x;
    screen_y_control = offset_y;
    kprintf("screen x: %d, screen y: %d", screen_x_control, screen_y_control);

    u8 col = screen_x / SCREEN_W;
    u8 row = screen_y / SCREEN_H;

    if (player_is_alive)
    {
        LEVEL_bool_screen_change = 1;
    }

    LEVEL_current_screen = row * MAP_X_SCREENS + col;

    MAP_scrollTo(map, screen_x, screen_y);
    LEVEL_generate_screen_collision_map(0, 5);

#ifdef DEBUG
    LEVEL_draw_collision_map();
#endif
}

void LEVEL_update_camera(GameObject *obj)

{
    if (obj->x > (FIX16(SCREEN_W) - obj->w / 2))
    {
        obj->x = 0;
        screen_x_control += SCREEN_W;
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control);
    }
    else if (obj->x < (FIX16(-obj->w / 2)))
    {
        obj->x = FIX16(SCREEN_W - obj->w);
        screen_x_control -= SCREEN_W;
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control);
    }

    if (obj->y > (FIX16(SCREEN_H) - obj->h / 2))
    {
        obj->y = 0;
        screen_y_control += SCREEN_H;
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control);
    }
    else if (obj->y < (FIX16(-obj->h / 2)))
    {
        obj->y = FIX16(SCREEN_H - obj->h);
        screen_y_control -= SCREEN_H;
        LEVEL_scroll_update_collision(screen_x_control, screen_y_control);
    }
}

void LEVEL_draw_collision_map()
{
    VDP_setTextPlane(BG_A);
    PAL_setColor(15, RGB24_TO_VDPCOLOR(0xFFFFFF));
    for (u8 x = 0; x < SCREEN_METATILES_W; ++x)
    {
        for (u8 y = 0; y < SCREEN_METATILES_H; ++y)
        {
            intToStr(collision_map[x][y], text, 1);
            VDP_drawText(text, x * METATILE_W / 8, y * METATILE_W / 8);
        }
    }
}

u8 LEVEL_wall_at(s16 x, s16 y)
{
    u8 value = collision_map[x / METATILE_W][y / METATILE_W];
    if (value == 1)
    {
        return 1;
    }
    else
        return 0;
}

u16 black_palette[64] = {0};
u16 target_palette[64];
u16 LEVEL_alert(u16 ind)
{
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
    ind = BACKGROUND_init_generalized(LEVEL_current_level, 1, PAL0, TRUE, false, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60);
    ind = BACKGROUND_full_clear(ind);
    return ind;
}
