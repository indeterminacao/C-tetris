#ifndef TETROMINOS_H
#define TETROMINOS_H

#include <stdint.h>

#define TETROMINO_C 7
#define ROTATION 4
#define MATRIX_SIZE 4

typedef enum {
    I = 0,
    O = 1,
    T = 2,
    S = 3,
    Z = 4,
    J = 5,
    L = 6,
} TetrominoType;

typedef enum {
    ROT_NORTH = 0,
    ROT_EAST,
    ROT_SOUTH,
    ROT_WEST
} Rotation;

extern const uint8_t TETROMINOS[TETROMINO_C][ROTATION][MATRIX_SIZE][MATRIX_SIZE];
#endif