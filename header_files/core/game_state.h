#ifndef CORE_GAME_STATE_H
#define CORE_GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../core/layout.h"
#include "../core/game_types.h"
#include "../core/input_state.h"
#include "../core/score_state.h"
#include "../core/piece_state.h"
#include "../core/physics_state.h"
#include "../tetrominos.h"


/**
 * @brief Main game state structure
 * Holds the global game state used across the engine.
 */
struct Game {
    // --- Core System ---
    bool ProgramOn;           /**< Main loop flag */
    GameState state;          /**< Current high-level state (Menu, Game, etc) */
    SDL_Window *window;       /**< SDL window pointer */
    SDL_Renderer *renderer;   /**< SDL renderer pointer */

    // --- Grid & Board ---
    uint8_t grid[TOTAL_ROWS][BOARD_WIDTH]; /**< Board grid: 0=empty, >0=piece_id+1 */

    // --- Timing & Physics ---
    Uint32 current_tick;          /**< Current SDL tick (global timing) */
    PhysicsState physics;

    // --- Active Piece State ---
    PieceState piece;         /**< Current piece state */

    ScoreState score;         /**< Current score state */

    // --- UI & Inputs ---
    SDL_Rect btn_play;        /**< Menu: Play button area */
    SDL_Rect btn_leaderboard; /**< Menu: Leaderboard button area */
    SDL_Rect btn_options;     /**< Menu: Options button area */
    InputState input;         /**< Current input state */
};

#endif