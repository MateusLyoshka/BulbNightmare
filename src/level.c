#include <genesis.h>
#include "level.h"

Map *map;
u8 collision_map[SCREEN_METATILES_W][SCREEN_METATILES_H] = {0};

u8 collision_result = 0;

// Top-Left screen position in map
u16 screen_x = 0;
u16 screen_y = 0;

void LEVEL_generate_screen_collision_map(u8 first_index, u8 last_index)
{
    s16 start_x = screen_x / METATILE_W; // find screen top-left cell in map
    s16 start_y = screen_y / METATILE_W;

    for (u8 x = start_x; x < start_x + SCREEN_METATILES_W; ++x)
    {
        for (u8 y = start_y; y < start_y + SCREEN_METATILES_H; ++y)
        {
            u16 tile_index = MAP_getTile(map, x * (METATILE_W / 8), y * (METATILE_W / 8)) & 0x03FF;
            collision_map[x][y] = 0;

            // if (tile_index != 8) {
            // KLog_U1("No zero tile: ", tile_index);
            // }
            if (tile_index >= first_index && tile_index <= last_index)
            {
                if (tile_index == BLOCKS_LEVEL_INDEX)
                {
                    collision_map[x][y] = 1;
                }
                else if (BOTTOM_SPIKE_LEVEL_INDEX == tile_index || tile_index == TOP_SPIKE_LEVEL_INDEX)
                {
                    collision_map[x][y] = 2;
                }
            }
        }
    }
}

u16 LEVEL_init(u16 ind)
{
    PAL_setPalette(PAL_MAP, levels_pal.data, DMA);
    VDP_loadTileSet(&tiles, ind, DMA);
    map = MAP_create(&level1_map, BG_B, TILE_ATTR_FULL(PAL_MAP, FALSE, FALSE, FALSE, ind));

    MAP_scrollToEx(map, 0, 0, TRUE);

    ind += tiles.numTile;

    return ind;
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
