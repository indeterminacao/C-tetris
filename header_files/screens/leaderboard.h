#ifndef LEADERBOARD_SCREEN_H
#define LEADERBOARD_SCREEN_H

#include <SDL2/SDL.h>
struct Game;

/** @brief Handles a single input event while the leaderboard screen is active
* @param game Pointer to the main Game struct
* @param event SDL_Event to process
*/
void leaderboard_handle_input(struct Game *game, SDL_Event event);

/** @brief Per-frame update for the leaderboard screen
* @param game Pointer to the main Game struct
*/
void leaderboard_update(struct Game *game);

/** @brief Renders the leaderboard screen
* @param game Pointer to the main Game struct
*/
void leaderboard_render(struct Game *game);

#endif