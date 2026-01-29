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
    G = 7  // Ghost piece
} TetrominoType;

typedef enum {
    TSPIN_NONE = 0,
    TSPIN_MINI,
    TSPIN_NORMAL
} TSpinType;

typedef enum {
    ROT_0 = 0, // Spawn
    ROT_R = 1, // Right (Clockwise)
    ROT_2 = 2, // 180
    ROT_L = 3  // Left (Counter-clockwise)
} Rotation;

extern const uint8_t TETROMINOS[TETROMINO_C][ROTATION][MATRIX_SIZE][MATRIX_SIZE];

#endif