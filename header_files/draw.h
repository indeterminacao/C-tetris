#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "../header_files/tetrominos.h"

struct Game;

/**
 * @brief Draws the main menu UI
 * @param game Pointer to the main Game struct
 */
void draw_menu(struct Game *game);

/**
 * @brief Draws the game layout (walls, floor, hold box)
 * @param game Pointer to the main Game struct
 */
void draw_layout(struct Game *game);

/**
 * @brief Draws the game grid based on the grid matrix
 * @param game Pointer to the main Game struct
 */
void draw_grid(struct Game *game);

/**
 * @brief Draws an active tetromino at the specified grid position
 * @param renderer Pointer to the SDL_Renderer
 * @param type TetrominoType of the piece
 * @param rotation Rotation state of the piece
 * @param grid_x X position on the grid
 * @param grid_y Y position on the grid
 */
void draw_tetro(SDL_Renderer *renderer,
                TetrominoType type,
                Rotation rotation,
                int grid_x,
                int grid_y);

/**
 * @brief Draws the ghost piece for the current active tetromino
 * @param game Pointer to the main Game struct
 */
void draw_ghost(struct Game *game);

#endif
