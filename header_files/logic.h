#ifndef LOGIC_H
#define LOGIC_H

#include "../Gameconfs.h"


bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot);
void spawn_piece(struct Game *game);
void lock_piece(struct Game *game);
uint8_t clear_lines(struct Game *game);
void spin(struct Game *game, int direction);
void hard_drop(struct Game *game);

#endif