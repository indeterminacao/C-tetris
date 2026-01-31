#ifndef GAMECONFS_H
#define GAMECONFS_H

/**
 * @file Gameconfs.h
 * @brief Core game configuration, layout constants and the main Game struct.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "tetrominos.h" 

/** @brief Application title used for the window */
#define TITLE "C-tetris"

/** @name Layout constants 
 * @brief Dimensions calculated in pixels based on block size
 */
/** @{ */
#define BLOCK_SIZE 30             /**< Size (px) of one block */
#define WALL_THICKNESS 1          /**< Thickness of side walls in blocks */

#define HOLD_PANEL_WIDTH 6        /**< Width (in blocks) of the hold panel */
#define BOARD_WIDTH 10            /**< Visible board width (in blocks) */
#define RIGHT_PANEL_WIDTH 6       /**< Right-side UI panel width */

#define VISIBLE_HEIGHT 20         /**< Number of visible rows shown to the player */
#define BUFFER_ZONE 2             /**< Invisible buffer rows above the visible board */
#define BOARD_HEIGHT 20           /**< Visible board height (duplicate of VISIBLE_HEIGHT) */
#define TOTAL_ROWS (VISIBLE_HEIGHT + BUFFER_ZONE) 

// Calculated Screen Dimensions
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
    Uint32 current_tick;      /**< Current SDL tick */
    Uint32 last_tick;         /**< Last tick when gravity advanced */
    int drop_speed;           /**< Gravity: Milliseconds per automatic drop */
    
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

    // --- Scoring & Progression ---
    Uint32 score;             /**< Current score */
    Uint32 level;             /**< Current difficulty level */
    Uint32 total_linesclr;    /**< Total lines cleared (for leveling up) */
    bool B2B;                 /**< Back-to-Back flag (Tetris or T-Spin streak) */

    // --- UI & Inputs ---
    SDL_Rect r;               /**< Generic temporary rect (Debug/Helper) */
    SDL_Rect btn_play;        /**< Menu: Play button area */
    SDL_Rect btn_leaderboard; /**< Menu: Leaderboard button area */
    SDL_Rect btn_options;     /**< Menu: Options button area */
};

#endif