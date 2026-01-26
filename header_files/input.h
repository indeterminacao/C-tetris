#ifndef INPUT_H
#define INPUT_H

#include "../Gameconfs.h"

void event_handling(struct Game *game);
void keydown_inputs(struct Game *game, SDL_Event event );
void mouse_inputs(struct Game *game, SDL_Event event);

#endif