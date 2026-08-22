#ifndef CORE_SCORE_STATE_H
#define CORE_SCORE_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// --- Scoring & Progression ---
typedef struct{
    Uint32 points;         /**< Current score */
    Uint32 level;          /**< Current difficulty level */
    Uint32 total_lines;    /**< Total lines cleared (for leveling up) */
    bool back_to_back;     /**< Back-to-Back flag (Tetris or T-Spin streak)*/
    int combo;
} ScoreState;

#endif