#include "pause.h"

GameObject pause;
s8 pause_option = 0;
u8 pause_proceed = 0;

u16 PAUSE_init(u16 ind)
{
    BACKGROUND_clear(1);
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
    ind = BACKGROUND_init_generalized(9, 1, PAL_BACKGROUND_A, false, true, ind);
    ind = GAMEOBJECT_init(&pause, &spr_pause, 128, 80, PAL_GAME, true, ind);
    pause_option = 0;
    pause_proceed = 0;
    return ind;
}

void PAUSE_update()
{
    if (key_pressed(0, BUTTON_DOWN) && pause_option == 0)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        pause_option++;
        SPR_setAnim(pause.sprite, pause_option);
    }
    else if (key_pressed(0, BUTTON_DOWN) && pause_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        pause_option++;
        SPR_setAnim(pause.sprite, pause_option);
    }
    else if (key_pressed(0, BUTTON_UP) && pause_option == 2)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        pause_option--;
        SPR_setAnim(pause.sprite, pause_option);
    }
    else if (key_pressed(0, BUTTON_UP) && pause_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        pause_option--;
        SPR_setAnim(pause.sprite, pause_option);
    }
    else if (key_pressed(0, BUTTON_A) && pause_option == 0)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
        pause_proceed = 1;
    }
    else if (key_pressed(0, BUTTON_A) && pause_option == 1)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
        pause_proceed = 2;
        set_black_palletes();
    }
    else if (key_pressed(0, BUTTON_A) && pause_option == 2)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
        pause_proceed = 3;
        set_black_palletes();
    }
}

void PAUSE_clear()
{
    SPR_releaseSprite(pause.sprite);
    pause.sprite = NULL;
}