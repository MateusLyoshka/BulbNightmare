#ifndef _BOSS
#define _BOSS

#include "../screenElements/background.h"
#include "../utils/gameobject.h"
#include "../utils/utils.h"
#include "../screenElements/level.h"
#include "../screenElements/hud.h"
#include "../player/player.h"
#include "../screenElements/objects.h"
#include "../gameFlow/menu.h"
#include "../screenElements/darkness.h"
#include "../gameFlow/pause.h"
#include "../gameFlow/game.h"
#include "../utils/globals.h"

extern GameObject enchant;

/**
 * @brief Initializes the boss fight.
 */
void BOSS_init();

/**
 * @brief Manages the full boss fight sequence.
 */
void BOSS_flux();

/**
 * @brief Updates boss-related logic and optionally updates the darkness mask.
 *
 * @param mask_bool Whether to update the darkness mask.
 */
void BOSS_flux_update(u8 mask_bool);

/**
 * @brief Plays the boss's speaking animation.
 */
void BOSS_speak_anim();

/**
 * @brief Plays the boss's transformation animation.
 */
void BOSS_transform_anim();

/**
 * @brief Generates a random value for boss animation behavior.
 *
 * @param min_val Minimum value.
 * @param max_val Maximum value.
 * @param step Value increment.
 * @return Randomized result.
 */
u16 BOSS_random_crown_anim(u16 min_val, u16 max_val, u16 step);

/**
 * @brief Plays the boss's laughing animation.
 */
void BOSS_laugh_anim();

/**
 * @brief Frees boss-related resources.
 */
void BOSS_clear();

/**
 * @brief Activates the boss's special ability (e.g., power effect).
 *
 * @param ind Current VRAM tile index.
 */
void BOSS_power(u16 ind);

#endif // _BOSS
