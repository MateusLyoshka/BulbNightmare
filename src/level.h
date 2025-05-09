#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <genesis.h>
#include "globals.h"
#include "gameobject.h"
#include "resources.h"

#define NUMBER_OF_LEVELS 5

extern Map *map;

extern u16 screen_x;
extern u16 screen_y;

u16 LEVEL_init(u16 ind);

#endif