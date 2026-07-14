#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SDL2/SDL.h>
struct Game;

/** @brief Handles a single input event while the menu screen is active
* @param game Pointer to the main Game struct
* @param event SDL_Event to process (mouse clicks)
*/
void menu_handle_input(struct Game *game, SDL_Event event);

/** @brief Per-frame update for the menu screen (currently nothing to do)
* @param game Pointer to the main Game struct
*/
void menu_update(struct Game *game);

/** @brief Renders the menu screen
* @param game Pointer to the main Game struct
*/
void menu_render(struct Game *game);

#endif