#ifndef _UTILS_H_
#define _UTILS_H_

#include <genesis.h>
#include <sprite_eng.h>
#include "globals.h"

// Stores current and previous button states for each joypad
extern u8 buttons[NUMBER_OF_JOYPADS];
extern u8 buttons_old[NUMBER_OF_JOYPADS];

// Returns a random value between min_val and max_val with a defined step size
static inline u16 getRandomValueBetween(u16 min_val, u16 max_val, u16 step)
{
    if (step == 0 || min_val > max_val)
    {
        return min_val; // Return minimum value on invalid input
    }

    u16 num_options = ((max_val - min_val) / step) + 1;
    u16 random_index = random() % num_options;
    u16 result = min_val + (random_index * step);

    return result;
}

// Sets the first four colors of palettes 0–3 to black
static inline void set_black_palletes()
{
    for (u8 i = 0; i < 4; i++)
    {
        PAL_setPalette(i, black_palette, DMA);
    }
}

// Performs a fade-in from black to the specified palette at a given speed
inline void fadeIn(u16 speed, u16 *target_palette_data, const u16 *black_palette_data, u8 pal_idx)
{
    PAL_setPalette(pal_idx, black_palette_data, DMA);

    u16 start_color_index = pal_idx * 16;
    u16 end_color_index = start_color_index + 15;

    PAL_fadeIn(start_color_index, end_color_index, target_palette_data, speed, FALSE);
}

// Performs a fade-out of the specified palette to black at a given speed
inline void fadeOut(u16 speed, u8 pal_idx)
{
    u16 start_color_index = pal_idx * 16;
    u16 end_color_index = start_color_index + 15;

    PAL_fadeOut(start_color_index, end_color_index, speed, FALSE);
}

// Wraps value X between bounds L and H
#define WRAP(X, L, H) \
    if (X < L)        \
        X = H;        \
    else if (X > H)   \
        X = L;

// Constructs a Box struct from coordinates and dimensions
inline Box UTILS_get_box(int x, int y, int w, int h)
{
    return (Box){x, x + w, y, y + h};
}

// ---------------------------------------------
// INPUT HANDLING
// ---------------------------------------------

// Updates button states (current and previous)
inline void update_input()
{
    for (int i = NUMBER_OF_JOYPADS - 1; i >= 0; i--)
    {
        buttons_old[i] = buttons[i];
        buttons[i] = JOY_readJoypad(i);
    }
}

// Returns true if the specified bit is set
inline bool is_bitset(u8 value, u8 bit)
{
    return ((value & bit) == bit);
}

// Returns true if a key is being held down
inline bool key_down(u8 joy_id, u8 key)
{
    return is_bitset(buttons[joy_id], key);
}

// Returns true if a key was just pressed
inline bool key_pressed(u8 joy_id, u8 key)
{
    return is_bitset(buttons[joy_id], key) && !is_bitset(buttons_old[joy_id], key);
}

// Returns true if a key was just released
inline bool key_released(u8 joy_id, u8 key)
{
    return !is_bitset(buttons[joy_id], key) && is_bitset(buttons_old[joy_id], key);
}

#endif
