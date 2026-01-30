#ifndef INIT_H
#define INIT_H

#include "../Gameconfs.h"

/** @brief Initializes SDL and creates the main window and renderer
* @param game Pointer to the main Game struct
* @return true if initialization was successful, false otherwise
*/
bool sdl_initializer(struct Game *game);

/** @brief Cleans up SDL resources and frees allocated memory
* @param game Pointer to the main Game struct
*/
void cleanup(struct Game *game);

#endif