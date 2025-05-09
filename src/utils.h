#ifndef _UTILS_H_
#define _UTILS_H_

#include <genesis.h>
#include <sprite_eng.h>
#include "globals.h"

extern u8 buttons[NUMBER_OF_JOYPADS];
extern u8 buttons_old[NUMBER_OF_JOYPADS];

// #define wrap(X, L, H) ((X < L)? H : ((X > H)? : L: X))

#define WRAP(X, L, H) \
    if (X < L)        \
        X = H;        \
    else if (X > H)   \
        X = L;

// #define clamp(X, L, H)   (min(max((X), (L)), (H)))

inline Box UTILS_get_box(int x, int y, int w, int h)
{
    return (Box){x, x + w, y, y + h};
}

#endif