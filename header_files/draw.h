#ifndef DRAW_H
#define DRAW_H

#include "../Gameconfs.h"
#include "../header_files/tetrominos.h"

void draw_menu(struct Game *game);
void draw_layout(struct Game *game);
void color_id(SDL_Renderer *renderer, TetrominoType type);
void draw_grid(struct Game *game);
void draw_tetro(SDL_Renderer *renderer, TetrominoType type, Rotation rotation, int grid_x, int grid_y);

#endif