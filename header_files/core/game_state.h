#ifndef CORE_GAME_STATE_H
#define CORE_GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../core/layout.h"
#include "../core/game_types.h"
#include "../core/game_state.h"
#include "../core/input_state.h"
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
    //Gravity
    Uint32 gravity_timer;         /**< Last gravity step tick */
    Uint32 gravity_delay;         /**< ms per automatic fall */

    // --- Lock Delay Mechanics ---
    Uint32 lock_delay;        /**< Time (ms) before locking a piece */
    Uint32 lock_timer;        /**< Timer accumulator for lock delay */
    bool is_locking;          /**< Is the piece currently touching the ground? */
    int lock_resets;          /**< Counter: How many times lock delay was reset (Infinity Rule) */

    // --- Active Piece State ---
    bool active_piece;           /**< Is there a piece currently falling? */
    TetrominoType currentType;   /**< Type of the active piece (I, O, T...) */
    Rotation currentRotation;    /**< Rotation state (0, 1, 2, 3) */
    int currentX;                /**< Board X position */
    int currentY;                /**< Board Y position */
    bool last_move_was_rotate;   /**< For T-Spin detection. Resets on move/tick. */
    bool hold_used;
    TetrominoType held_piece;

    // --- Scoring & Progression ---
    Uint32 score;             /**< Current score */
    Uint32 level;             /**< Current difficulty level */
    Uint32 total_linesclr;    /**< Total lines cleared (for leveling up) */
    bool B2B;                 /**< Back-to-Back flag (Tetris or T-Spin streak) */

    // --- UI & Inputs ---
    SDL_Rect btn_play;        /**< Menu: Play button area */
    SDL_Rect btn_leaderboard; /**< Menu: Leaderboard button area */
    SDL_Rect btn_options;     /**< Menu: Options button area */
    InputState input;         /**< Current input state */
};

#endif