
#include <genesis.h>
#include <sprite_eng.h>
#include "resources.h"
#include "sprites.h"

u8 gRoom;		  // Sala atual
u32 gFrames;	  // Frame counter
u16 gInd_tileset; // Carrega dados do background
Sprite *gButtonStart;
Sprite *gButtonMenu;
u16 curlInput = 0; // Estado atual do botão
u16 prevInput;	   // Estado anterior
u8 gMainMenuOptions;

int main(bool resetType)
{

	SPR_init();
	gRoom = 1;
	gFrames = 0;
	gInd_tileset = 0;

	// Soft reset doesn't clear RAM. Can lead to bugs.
	if (!resetType)
	{
		SYS_hardReset();
	}

	while (true)
	{
		gFrames++;
		if (gRoom == 1) // Main Menu
		{
			if (gFrames == 1)
			{
				VDP_setScreenWidth320();
				VDP_loadTileSet(img_bg.tileset, gInd_tileset, DMA);
				VDP_setTileMapEx(BG_A, img_bg.tilemap, TILE_ATTR_FULL(PAL0, 0, 0, 0, gInd_tileset), 0, 0, 0, 0, 40, 28, DMA_QUEUE);
				PAL_setPalette(PAL0, img_bg.palette->data, DMA);
				gInd_tileset += img_bg.tileset->numTile;

				gButtonStart = SPR_addSprite(&button_start, 98, 88, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
				PAL_setPalette(PAL1, button_start.palette->data, DMA);
			}
			curlInput = JOY_readJoypad(JOY_1);

			if (((curlInput & BUTTON_START) && !(prevInput & BUTTON_START)))
			{
				gButtonMenu = SPR_addSprite(&buttons_menu, 98, 72, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
				PAL_setPalette(PAL2, buttons_menu.palette->data, DMA);
				SPR_setVisibility(gButtonStart, HIDDEN);
				gMainMenuOptions = 0;
			}
			if (((curlInput & BUTTON_DOWN) && !(prevInput & BUTTON_DOWN)) && gMainMenuOptions == 0)
			{
				gMainMenuOptions = 1;
				SPR_setAnim(gButtonMenu, gMainMenuOptions);
				prevInput = curlInput;
			}
			if (((curlInput & BUTTON_DOWN) && !(prevInput & BUTTON_DOWN)) && gMainMenuOptions == 1)
			{
				gMainMenuOptions = 2;
				SPR_setAnim(gButtonMenu, gMainMenuOptions);
				prevInput = curlInput;
			}
			if (((curlInput & BUTTON_UP) && !(prevInput & BUTTON_UP)) && gMainMenuOptions == 2)
			{
				gMainMenuOptions = 1;
				SPR_setAnim(gButtonMenu, gMainMenuOptions);
				prevInput = curlInput;
			}
			if (((curlInput & BUTTON_UP) && !(prevInput & BUTTON_UP)) && gMainMenuOptions == 1)
			{
				gMainMenuOptions = 0;
				SPR_setAnim(gButtonMenu, gMainMenuOptions);
				prevInput = curlInput;
			}

			prevInput = curlInput;
		}

		if (gRoom == 2) //
		{
		}

		// char str[64];
		// sprintf(str, "tiles usados nos BGs: %d", gInd_tileset);
		// VDP_drawText(str, 1, 1);
		// sprintf(str, "contagem de frames: %ld", gFrames);
		// VDP_drawText(str, 1, 2);

		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}
