#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <genesis.h>
#include "globals.h"
#include "gameobject.h"
#include "resources.h"

#define NUMBER_OF_LEVELS 5

extern u8 collision_result;

extern Map *map;
extern u8 collision_map[SCREEN_METATILES_W][SCREEN_METATILES_H];

#define COLLISION_LEFT 0b0001
#define COLLISION_RIGHT 0b0010
#define COLLISION_TOP 0b0100
#define COLLISION_BOTTOM 0b1000

#define BLOCKS_LEVEL_INDEX 0
#define BOTTOM_SPIKE_LEVEL_INDEX 2
#define TOP_SPIKE_LEVEL_INDEX 3
#define BACKGROUND_LEVEL_INDEX 4

extern u16 screen_x;
extern u16 screen_y;

u8 LEVEL_wall_at(s16 x, s16 y);
u16 LEVEL_init(u16 ind);
void LEVEL_generate_screen_collision_map(u8 first_index, u8 last_index);
void LEVEL_draw_collision_map();
void LEVEL_move_and_slide(GameObject *obj);

#endif