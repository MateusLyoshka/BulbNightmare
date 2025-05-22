# BACKGROUNDS
IMAGE menu_bg "backgrounds\menu_bg.png" BEST 
# IMAGE game_bg "backgrounds\game_bg.png" BEST 
IMAGE white_bg "backgrounds\test_bg.png" BEST 
IMAGE   img_hud           "hud/hud.png" FAST
IMAGE dark_mask "backgrounds/dark_mask.png" FAST 

# SPRITES
SPRITE buttons_menu "sprites/menu_buttons.png" 7 7 FAST 6
SPRITE button_start "sprites/start.png" 7 3 FAST 6
SPRITE spr_player "sprites/player.png" 2 2 BEST 6
SPRITE spr_hud_itens "hud/hud_itens.png" 2 2 FAST 
SPRITE spr_hud_font "hud/hud_font.png" 2 2 FAST 
SPRITE spr_g_enemy "sprites/g_enemy.png" 2 2 FAST 6
SPRITE spr_f_enemy "sprites/f_enemy.png" 2 2 FAST 6
SPRITE spr_door "sprites/door.png" 2 2 FAST
SPRITE spr_key "sprites/key.png" 2 2 FAST 6
SPRITE spr_powerup "sprites/powerup.png" 2 2 FAST 6

# MAPA
TILESET tiles "levels/tiles.png" BEST ALL 
PALETTE levels_pal "levels/level_pal.pal"
MAP level1_map "levels/level1/level1.tmx" tiles_layer BEST 0
MAP level2_map "levels/level2/level2.tmx" tiles_layer BEST 0

# SOUNDS
WAV snd_start "sfx/game_start.wav" XGM 