#ifndef DRAW_H
#define DRAW_H

#include "../Gameconfs.h"
#include "../header_files/tetrominos.h"

/**
 * @brief Draws the main menu
* @param game Pointer to the main Game struct
*/
void draw_menu(struct Game *game);

/**
 * @brief Draws the walls that contain the game grid and the hold panel
 * @param game Pointer to the main Game struct
 */
void draw_layout(struct Game *game);

/** @brief Sets the drawing color based on tetromino type (not used to draw ghost pieces) 
* @param renderer Pointer to the SDL_Renderer
* @param type TetrominoType to set the color for
*/
void color_id(SDL_Renderer *renderer, TetrominoType type);

/** @brief Draws the grid of the game, using the game's grid matrix values to determine filled cells and their colors
 * @param game Pointer to the main Game struct
 */
void draw_grid(struct Game *game);
/** @brief Draws an active tetromino at the specified grid position
* @param renderer Pointer to the SDL_Renderer
* @param type TetrominoType of the piece to draw
* @param rotation Rotation state of the piece
* @param grid_x X position on the grid to draw the piece
* @param grid_y Y position on the grid to draw the piece
*/
void draw_tetro(SDL_Renderer *renderer, TetrominoType type, Rotation rotation, int grid_x, int grid_y);
/** @brief Draws the ghost piece for the current active tetromino (this function does not write values into the game grid)
* @param game Pointer to the main Game struct
*/
void draw_ghost(struct Game *game);
#endif