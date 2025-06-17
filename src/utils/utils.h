#ifndef _UTILS_H_
#define _UTILS_H_

#include <genesis.h>
#include <sprite_eng.h>
#include "globals.h"

extern u8 buttons[NUMBER_OF_JOYPADS];
extern u8 buttons_old[NUMBER_OF_JOYPADS];

static inline u16 getRandomValueBetween(u16 min_val, u16 max_val, u16 step)
{
    if (step == 0 || min_val > max_val)
    {
        return min_val; // Fallback for invalid parameters
    }

    // Calculate the number of possible values
    u16 num_options = ((max_val - min_val) / step) + 1;

    // Generate a random index from 0 to num_options - 1
    u16 random_index = random() % num_options;

    // Calculate the result
    u16 result = min_val + (random_index * step);

    return result;
}
// Função para setar as 4 primeiras cores da paleta 3
static inline void set_black_palletes()
{
    for (u8 i = 0; i < 4; i++)
    {
        PAL_setPalette(i, black_palette, DMA);
    }
}

inline void fadeIn(u16 speed, u16 *target_palette_data, const u16 *black_palette_data, u8 pal_idx)
{
    // Define a paleta de hardware especificada (pal_idx) para as cores de black_palette_data
    // Isso garante que a paleta esteja preta antes do fadeIn.
    PAL_setPalette(pal_idx, black_palette_data, DMA);

    // Faz o fade IN para as cores em target_palette_data.
    // Para afetar apenas a paleta especificada por pal_idx (16 cores):
    u16 start_color_index = pal_idx * 16;
    u16 end_color_index = start_color_index + 15;
    PAL_fadeIn(start_color_index, end_color_index, target_palette_data, speed, FALSE);
}

inline void fadeOut(u16 speed, u8 pal_idx) // Adicionado pal_idx para especificar qual paleta fazer fadeOut
{
    // Faz o fade OUT da paleta especificada por pal_idx (16 cores).
    u16 start_color_index = pal_idx * 16;
    u16 end_color_index = start_color_index + 15;
    PAL_fadeOut(start_color_index, end_color_index, speed, FALSE);
}

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