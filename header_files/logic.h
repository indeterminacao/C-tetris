#ifndef LOGIC_H
#define LOGIC_H

#include "../Gameconfs.h"

/** @brief Checks if a tetromino would collide with the grid or walls
* @param game Pointer to the main Game struct
* @param grid_x X position on the grid to check
* @param grid_y Y position on the grid to check
* @param rot Rotation state of the piece
* @return true if there is a collision, false otherwise
*/
bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot);

/** @brief Spawns a new tetromino piece at the top of the grid
* @param game Pointer to the main Game struct
*/
void spawn_piece(struct Game *game);

/** @brief Locks the current piece in place and writes data to the game grid
* @param game Pointer to the main Game struct   
*/
void lock_piece(struct Game *game);

/** @brief Calls lock_piece, checks for line clears and T-Spins, and updates piece data to false (resets active piece and rotation flag)
* @param game Pointer to the main Game struct
*/
void resolve_lock(struct Game *game);

/** @brief Checks for and clears completed lines from the game grid
* @param game Pointer to the main Game struct   
* @return Number of lines cleared
*/
uint8_t clear_lines(struct Game *game);

/** @brief Checks for and clears completed lines from the game grid
* @param game Pointer to the main Game struct   
* @param lines_cleared amount of lines cleared 
* @param tspin type of tspin registered 
* @param b2b Is back-to-back active?
*/
void update_score(struct Game *game, uint8_t lines_cleared, TSpinType tspin);

/** @brief Spins the current piece and apply wall kicks if needed
* @param game Pointer to the main Game struct
* @param direction Spin direction: 1 for clockwise, -1 for counter-clockwise
*/
void spin(struct Game *game, int direction);

/** @brief Checks if the last move was a T-Spin
* @param game Pointer to the main Game struct
* @return TSpinType indicating the type of T-Spin
*/
TSpinType is_t_spin(struct Game *game);

/**
* @brief Hard-drop the active piece to the lowest valid row, lock it, and update state.
* Moves the active piece straight down until it would collide, resets the rotation flag,
* calls resolve_lock() to place the piece and handle line clears/T-spins, and updates
* the game's last tick.
* @pre game->active_piece must be true (an active piece is required)
* @param game Pointer to the main Game struct
*/
void hard_drop(struct Game *game);

/** @brief Extended Placement Lock Down - reduces lock resets, updates lock timer, and prints remaining moves
* @param game Pointer to the main Game struct
*/
void EPLD(struct Game *game);



#endif