#ifndef LOGIC_H
#define LOGIC_H

#include "../Gameconfs.h"

bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot);
void lock_piece(struct Game *game);
uint8_t clear_lines(struct Game *game);
Rotation get_next_rotation(struct Game *game, int direction);

#endif