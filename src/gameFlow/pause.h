#ifndef _PAUSE
#define _PAUSE

#include <genesis.h>
#include "../screenElements/background.h"
#include "../utils/gameobject.h"

extern u8 pause_proceed;

u16 PAUSE_init(u16 ind);
void PAUSE_update();
void PAUSE_clear();

#endif