#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
struct Game;

/** @brief Master function that handles all input events
* @param game Pointer to the main Game struct
 */
void event_handling(struct Game *game);

#endif