#ifndef INPUT_H
#define INPUT_H

#include "../Gameconfs.h"

/** @brief Master function that handles all input events
* @param game Pointer to the main Game struct
 */
void event_handling(struct Game *game);

/** @brief Auxiliary function for keydown events 
* @param game Pointer to the main Game struct
* @param event SDL_Event structure containing the keydown event
*/
void keydown_inputs(struct Game *game, SDL_Event event );

/** @brief Auxiliary function for mouse events
* @param game Pointer to the main Game struct
* @param event SDL_Event structure containing the mouse event
*/
void mouse_inputs(struct Game *game, SDL_Event event);

#endif