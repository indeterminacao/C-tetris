#ifndef GAMEOVER_SCREEN_H
#define GAMEOVER_SCREEN_H

#include <SDL2/SDL.h>

struct Game;
void gameover_handle_input(struct Game *game, SDL_Event event);

void gameover_update(struct Game *game);

void gameover_render(struct Game *game);
#endif