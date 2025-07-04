# IMAGE backgrounds
IMAGE menu_bg "backgrounds\menu_bg.png" BEST 
IMAGE pause_bg "backgrounds/pause_background.png" FAST 
IMAGE img_hud  "hud/hud.png" FAST
IMAGE dark_mask "backgrounds/dark_mask.png" FAST 
IMAGE utf_logo "backgrounds/utfpr.png" FAST 
IMAGE instructions1 "backgrounds/controls1.png" FAST 
IMAGE instructions2 "backgrounds/controls2.png" FAST 
IMAGE credits "backgrounds/credits.png" FAST 

IMAGE boss_bg "backgrounds/boss/boss2.png" FAST 

# IMAGE backgrounds end game
IMAGE dialog_1 "backgroundS/end_game/dialog_1.png" FAST  
IMAGE dialog_2 "backgroundS/end_game/dialog_2.png" FAST  
IMAGE dialog_3 "backgroundS/end_game/dialog_3.png" FAST  
IMAGE final_screen "backgroundS/end_game/final_screen.png" FAST  

# IMAGE bg_alerts
IMAGE alert_1 "backgrounds/level_alert/level1.png" FAST
IMAGE alert_2 "backgrounds/level_alert/level2.png" FAST
IMAGE alert_3 "backgrounds/level_alert/level3.png" FAST
IMAGE alert_4 "backgrounds/level_alert/level4.png" FAST
IMAGE alert_5 "backgrounds/level_alert/level5.png" FAST
IMAGE alert_6 "backgrounds/level_alert/level6.png" FAST


# SPRITES hud
SPRITE spr_font_x "hud/font.png" 2 2 FAST 
SPRITE spr_font_L "hud/font2.png" 2 2 FAST 
SPRITE spr_font_N "hud/font_numbers.png" 2 2 FAST 

# SPRITES level
SPRITE spr_player "sprites/player.png" 2 2 BEST 4
SPRITE spr_hud_itens "hud/hud_itens.png" 2 2 FAST 
SPRITE spr_g_enemy "sprites/g_enemy.png" 2 2 FAST 3
SPRITE spr_f_enemy "sprites/f_enemy.png" 2 2 FAST 3
SPRITE spr_door "sprites/door.png" 2 2 BEST  15
SPRITE spr_key "sprites/key.png" 2 2 FAST 8
SPRITE spr_light_switch "sprites/switch.png" 2 2 FAST 
SPRITE spr_sparks "sprites/sparks.png" 4 4 FAST 8
SPRITE spr_enchant "sprites/enchant.png" 2 2 FAST 8
SPRITE spr_boss "sprites/enemies/boss/boss.png" 12 24 FAST 8
SPRITE spr_face "sprites/enemies/boss/face.png" 4 4 FAST 10
SPRITE spr_dialog "sprites/enemies/boss/dialog.png" 16 4 FAST

# SPRITES menu
SPRITE spr_menu "sprites/menu_buttons.png" 7 7 FAST 8
SPRITE spr_pause "sprites/pause.png" 7 7 FAST 8
SPRITE spr_start "sprites/start.png" 7 3 FAST 8

# MAPA
TILESET tiles "levels/tiles.png" BEST ALL 
MAP level1_map "levels/level1/level1.tmx" tiles_layer BEST 0
MAP level2_map "levels/level2/level2.tmx" tiles_layer BEST 0
MAP level3_map "levels/level3/level3.tmx" tiles_layer BEST 0
MAP level4_map "levels/level4/level4.tmx" tiles_layer BEST 0
MAP level5_map "levels/level5/level5.tmx" tiles_layer BEST 0
MAP level6_map "levels/level6/level6.tmx" tiles_layer BEST 0

# PALETTES
PALETTE game_pal "levels/game.pal"

PALETTE level1_pal "palettes/level1.pal"
PALETTE level2_pal "palettes/level2.pal"
PALETTE level3_pal "palettes/level3.pal"
PALETTE level4_pal "palettes/level4.pal"
PALETTE boss_pal "palettes/level5.pal"

# SOUNDS
WAV snd_boss_power "sfx/boss_power.wav" XGM 
WAV snd_button_change "sfx/button_change.wav" XGM 
WAV snd_death "sfx/death.wav" XGM 
WAV snd_jump "sfx/jump.wav" XGM 
WAV snd_key "sfx/key.wav" XGM 
WAV snd_switch "sfx/switch.wav" XGM 
WAV snd_door "sfx/door.wav" XGM 
WAV snd_start "sfx/start.wav" XGM 
WAV snd_cheat_on "sfx/cheat_on.wav" XGM 
WAV snd_cheat_off "sfx/cheat_off.wav" XGM 