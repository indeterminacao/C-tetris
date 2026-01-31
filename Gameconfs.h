#ifndef GAMECONFS_H
#define GAMECONFS_H

/**
 * @file Gameconfs.h
 * @brief Core game configuration, layout constants and the main Game struct.
 *
 * This header defines layout constants (block size, board width/height), the
 * `GameState` enum and the primary `Game` struct which holds the global
 * game state used across the engine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "tetrominos.h"

/** @brief Application title used for the window */
#define TITLE "C-tetris"

/** @name Layout constants */
/** @{ */
#define BLOCK_SIZE 30              /**< Size (px) of one block */
#define WALL_THICKNESS 1          /**< Thickness of side walls in blocks */

#define HOLD_PANEL_WIDTH 6        /**< Width (in blocks) of the hold panel */
#define BOARD_WIDTH 10            /**< Visible board width (in blocks) */
#define RIGHT_PANEL_WIDTH 6       /**< Right-side UI panel width */

#define VISIBLE_HEIGHT 20         /**< Number of visible rows shown to the player */
#define BUFFER_ZONE 2             /**< Invisible buffer rows above the visible board */
#define BOARD_HEIGHT 20           /**< Visible board height (duplicate of VISIBLE_HEIGHT) */
#define TOTAL_ROWS (VISIBLE_HEIGHT + BUFFER_ZONE) /**< Total rows including buffer */

#define SCREEN_WIDTH (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH + WALL_THICKNESS + BOARD_WIDTH + WALL_THICKNESS + RIGHT_PANEL_WIDTH))
#define SCREEN_HEIGHT (BLOCK_SIZE * (VISIBLE_HEIGHT + WALL_THICKNESS))
#define GAME_OFFSET_X (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH))
/** @} */

/**
 * @brief High-level game states
 */
typedef enum {
    STATE_MENU,        /**< Main menu */
    STATE_GAME,        /**< Gameplay running */
    STATE_LEADERBOARD, /**< Leaderboard / scores */
    STATE_OPTIONS      /**< Options / settings */
} GameState;

/**
 * @brief Main game state structure
 *
 * - `grid` stores the board cells as values: 0 == empty, where non-zero values
 *   encode a placed piece (piece_id + 1). The grid includes a buffer above
 *   the visible rows to allow spawning pieces out of view.
 * - Timing fields keep track of ticks and automatic drop speed.
 * - Piece state fields describe the currently active piece when `active_piece`
 *   is true.
 */
struct Game {
    uint8_t grid[TOTAL_ROWS][BOARD_WIDTH]; /**< Board grid data */

    bool ProgramOn;           /**< Whether the program loop should continue */
    SDL_Window *window;       /**< SDL window pointer */
    SDL_Renderer *renderer;   /**< SDL renderer pointer */

    /* Timing */
    Uint32 current_tick;      /**< Current SDL tick */
    Uint32 last_tick;         /**< Last tick when gravity advanced */
    int drop_speed;           /**< Milliseconds per automatic drop */
    Uint32 lock_delay;        /**< Milliseconds before locking a piece */
    Uint32 lock_timer;        /**< Timer for lock delay */
    bool is_locking;

    GameState state;          /**< Current high-level state */

    int currentX;             /**< Active piece X coordinate on board */
    int currentY;             /**< Active piece Y coordinate on board */
    bool active_piece;        /**< True when a piece is falling */
    bool last_move_was_rotate;/**< True if the last action was a rotation. Critical for T-Spin detection; resets on move/tick. */
    int Lock_resets;            /**< Number of times the lock delay has been reset for the current piece */

    Uint32 score;            /**< Player score */
    Uint32 level;            /**< Current level */
    Uint32 total_linesclr;    /**< Total lines cleared */
    bool B2B;                 /**< Back-to-back Tetris/T-Spin flag */

    TetrominoType currentType;      /**< Active piece type (I/O/T/...) */
    Rotation currentRotation;       /**< Active piece rotation */

    SDL_Rect r;              /**< Placeholder rect */

    /* Menu buttons */
    SDL_Rect btn_play;       /**< Play button rectangle */
    SDL_Rect btn_leaderboard;/**< Leaderboard button rectangle */
    SDL_Rect btn_options;    /**< Options button rectangle */
};

#endif