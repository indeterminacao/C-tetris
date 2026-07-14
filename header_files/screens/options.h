#ifndef OPTIONS_SCREEN_H
#define OPTIONS_SCREEN_H

#include <SDL2/SDL.h>
struct Game;

/** @brief Handles a single input event while the options screen is active
* @param game Pointer to the main Game struct
* @param event SDL_Event to process
*/
void options_handle_input(struct Game *game, SDL_Event event);

/** @brief Per-frame update for the options screen
* @param game Pointer to the main Game struct
*/
void options_update(struct Game *game);

/** @brief Renders the options screen
* @param game Pointer to the main Game struct
*/
void options_render(struct Game *game);

#endif