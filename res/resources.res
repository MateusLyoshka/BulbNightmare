# BACKGROUNDS
IMAGE menu_bg "backgrounds\menu_bg.png" BEST 
IMAGE game_bg "backgrounds\game_bg.png" BEST 
IMAGE white_bg "backgrounds\test_bg.png" BEST 

# SPRITES
SPRITE buttons_menu "sprites/menu_buttons.png" 7 7 FAST 6
SPRITE button_start "sprites/start.png" 7 3 FAST 6
SPRITE spr_player "sprites/player.png" 2 2 NONE 6

# MAPA
TILESET tiles "levels/tiles.png" BEST ALL 
PALETTE levels_pal "levels/level_pal.pal"
MAP level1_map "levels/level1/level1_map.tmx" map_layer BEST 0

# SOUNDS
WAV snd_start "sfx/game_start.wav" XGM 