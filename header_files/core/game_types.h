#ifndef CORE_GAME_TYPES_H
#define CORE_GAME_TYPES_H

#include <stdbool.h>
#include <SDL2/SDL.h>

/** @brief High-level game states */
typedef enum {
    STATE_MENU,        /**< Main menu */
    STATE_GAME,        /**< Gameplay running */
    STATE_LEADERBOARD, /**< Leaderboard / scores */
    STATE_OPTIONS      /**< Options / settings */
} GameState;

typedef enum {
    MOVE_NONE,
    MOVE_LEFT,
    MOVE_RIGHT
} MoveDirection;

#endif