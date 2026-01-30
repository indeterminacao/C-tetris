#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <stdint.h>

#define TETROMINO_C 7
#define ROTATION 4
#define MATRIX_SIZE 4

/** @brief Tetromino shape identifier
* Used to index into TETROMINOS and TETROMINO_COLORS
* G does not represent a real tetromino, but is used for the ghost piece color
*/
typedef enum {
    I = 0,
    O = 1,
    T = 2,
    S = 3,
    Z = 4,
    J = 5,
    L = 6,
    G = 7 
} TetrominoType;

/** @brief T-Spin type identifier */
typedef enum {
    TSPIN_NONE = 0,
    TSPIN_MINI,
    TSPIN_NORMAL
} TSpinType;

/** @brief Rotation states for tetrominos 
* Made using notation used in the hard drop site: https://harddrop.com/wiki/SRS
*/
typedef enum {
    ROT_0 = 0, // Spawn
    ROT_R = 1, // Right (Clockwise)
    ROT_2 = 2, // 180
    ROT_L = 3  // Left (Counter-clockwise)
} Rotation;

/** @brief 4x4 Matrices defining tetromino shapes in all rotation states
* Each tetromino is represented in a 4x4 grid for each of its 4 rotation states.
* A value of 0 indicates an empty cell, while a non-zero value indicates a filled
*/
extern const uint8_t TETROMINOS[TETROMINO_C][ROTATION][MATRIX_SIZE][MATRIX_SIZE];

#endif