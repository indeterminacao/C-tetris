#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SDL2/SDL.h>
struct Game;

/** @brief Handles a single input event while the game screen is active
* @param game Pointer to the main Game struct
* @param event SDL_Event to process (keydown or keyup)
*/
void game_screen_handle_input(struct Game *game, SDL_Event event);

/** @brief Per-frame update for the game screen: spawning, gravity, lock delay,
* and continuous DAS/ARR movement and soft drop
* @param game Pointer to the main Game struct
*/
void game_screen_update(struct Game *game);

/** @brief Renders the game screen: board, grid, ghost piece, active piece
* @param game Pointer to the main Game struct
*/
void game_screen_render(struct Game *game);

#endif