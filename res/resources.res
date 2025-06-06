# BACKGROUNDS
IMAGE menu_bg "backgrounds\menu_bg.png" BEST 
# IMAGE game_bg "backgrounds\game_bg.png" BEST 
IMAGE white_bg "backgrounds\test_bg.png" BEST 
IMAGE   img_hud           "hud/hud.png" FAST
IMAGE dark_mask "backgrounds/dark_mask.png" FAST 
IMAGE dark_mask_mid "backgrounds/dark_mask_mid.png" FAST 
IMAGE dark_mask_max "backgrounds/dark_mask_max.png" FAST 
IMAGE utf_logo "backgrounds/utfpr.png" FAST 
IMAGE instructions "backgrounds/instructions.png" FAST 
IMAGE alert_1 "backgrounds/level_alert/level1.png" FAST
IMAGE alert_2 "backgrounds/level_alert/level2.png" FAST
IMAGE alert_3 "backgrounds/level_alert/level3.png" FAST
IMAGE alert_4 "backgrounds/level_alert/level4.png" FAST
IMAGE alert_5 "backgrounds/level_alert/level5.png" FAST

# SPRITES
SPRITE spr_menu "sprites/menu_buttons.png" 7 7 FAST 8
SPRITE spr_start "sprites/start.png" 7 3 FAST 8
SPRITE spr_player "sprites/player.png" 2 2 BEST 3
SPRITE spr_hud_itens "hud/hud_itens.png" 2 2 FAST 
SPRITE spr_hud_font "hud/hud_font.png" 2 2 FAST 
SPRITE spr_g_enemy "sprites/g_enemy.png" 2 2 FAST 3
SPRITE spr_f_enemy "sprites/f_enemy.png" 2 2 FAST 3
SPRITE spr_door "sprites/door.png" 2 2 FAST
SPRITE spr_key "sprites/key.png" 2 2 FAST 8
SPRITE spr_powerup "sprites/powerup.png" 2 2 FAST 8

# MAPA
TILESET tiles "levels/tiles.png" BEST ALL 
PALETTE game_pal "levels/game.pal"
PALETTE level_pal "levels/level.pal"
MAP level1_map "levels/level1/level1.tmx" tiles_layer BEST 0
MAP level2_map "levels/level2/level2.tmx" tiles_layer BEST 0
MAP level3_map "levels/level3/level3.tmx" tiles_layer BEST 0
MAP level4_map "levels/level4/level4.tmx" tiles_layer BEST 0

# SOUNDS
WAV snd_start "sfx/game_start.wav" XGM 