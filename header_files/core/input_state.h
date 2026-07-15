#ifndef CORE_INPUT_STATE_H
#define CORE_INPUT_STATE_H

#include "../core/game_types.h"

typedef struct {
    // --- Raw input ---
    const Uint8 *keystate;

    // --- Horizontal movement (DAS / ARR) ---
    MoveDirection move_dir;
    Uint32 DAS_timer;          /**< When horizontal key was pressed */
    Uint32 ARR_timer;          /**< Last horizontal repeat */

    // --- Soft Drop ---
    bool soft_dropping;
    Uint32 soft_drop_timer;    /**< Last soft drop step */

} InputState;

#endif