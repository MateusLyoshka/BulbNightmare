#ifndef OBJECTS
#define OBJECTS

#include <genesis.h>
#include "../utils/globals.h"
#include "map.h"
#include "gameobject.h"
#include "resources.h"
#include "level.h"

extern GameObject door;
extern GameObject key;
extern GameObject powerup;

u16 OBJECT_door_spawn(u16 ind);
u16 OBJECT_key_spawn(u16 ind);
u16 OBJECT_powerup_spawn(u16 ind);
u16 OBJECT_update(u16 ind);
void OBJECT_clear(GameObject object);

#endif