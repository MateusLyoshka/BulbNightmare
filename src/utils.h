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

// GAMEPAD INPUT

inline void update_input()
{
    for (int i = NUMBER_OF_JOYPADS - 1; i >= 0; i--)
    {
        buttons_old[i] = buttons[i];
        buttons[i] = JOY_readJoypad(i);
    }
}

inline bool is_bitset(u8 value, u8 bit)
{
    return ((value & bit) == bit);
}

inline bool key_down(u8 joy_id, u8 key)
{
    return is_bitset(buttons[joy_id], key);
}

inline bool key_pressed(u8 joy_id, u8 key)
{
    return is_bitset(buttons[joy_id], key) && !is_bitset(buttons_old[joy_id], key);
}

inline bool key_released(u8 joy_id, u8 key)
{
    return !is_bitset(buttons[joy_id], key) && is_bitset(buttons_old[joy_id], key);
}

#endif