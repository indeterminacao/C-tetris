#ifndef PIECE_H
#define PIECE_H

#include "../tetrominos.h"

typedef struct{
    // --- Active Piece State ---
    bool active;           /**< Is there a piece currently falling? */
    TetrominoType type;   /**< Type of the active piece (I, O, T...) */
    Rotation rotation;    /**< Rotation state (0, 1, 2, 3) */
    int x;                /**< Board X position */
    int y;                /**< Board Y position */
    bool last_move_was_rotate;   /**< For T-Spin detection. Resets on move/tick. */
    bool hold_used;
    TetrominoType held_piece;
} PieceState;

#endif