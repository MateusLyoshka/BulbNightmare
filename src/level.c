#include <genesis.h>
#include "level.h"

Map *map;

// Top-Left screen position in map
u16 screen_x = 0;
u16 screen_y = 0;

u16 LEVEL_init(u16 ind)
{
    PAL_setPalette(PAL_MAP, levels_pal.data, DMA);
    VDP_loadTileSet(&tiles, ind, DMA);
    map = MAP_create(&level1_map, BG_A, TILE_ATTR_FULL(PAL_MAP, FALSE, FALSE, FALSE, ind));

    MAP_scrollToEx(map, 0, 0, TRUE);

    ind += tiles.numTile;

    return ind;
}