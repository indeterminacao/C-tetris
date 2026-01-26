#ifndef GAMECONFS_H
#define GAMECONFS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "tetrominos.h" 

#define TITLE "C-tetris"

#define BLOCK_SIZE 30
#define WALL_THICKNESS 1 

#define HOLD_PANEL_WIDTH 6 
#define BOARD_WIDTH 10
#define RIGHT_PANEL_WIDTH 6

#define VISIBLE_HEIGHT 20
#define BUFFER_ZONE 2   
#define BOARD_HEIGHT 20 
#define TOTAL_ROWS (VISIBLE_HEIGHT + BUFFER_ZONE) 
#define SCREEN_WIDTH (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH + WALL_THICKNESS + BOARD_WIDTH + WALL_THICKNESS + RIGHT_PANEL_WIDTH))
#define SCREEN_HEIGHT (BLOCK_SIZE * (VISIBLE_HEIGHT + WALL_THICKNESS))
#define GAME_OFFSET_X (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH))

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_LEADERBOARD,
    STATE_OPTIONS
} GameState;

struct Game {
    uint8_t grid[TOTAL_ROWS][BOARD_WIDTH];
    
    bool ProgramOn;
    SDL_Window *window;
    SDL_Renderer *renderer;

    //Time
    Uint32 current_tick;
    Uint32 last_tick;
    int drop_speed;

    GameState state;

    int currentX; 
    int currentY; 
    bool active_piece;
    
    TetrominoType currentType;      // Piece (I, O, T...)
    Rotation currentRotation;       // Rotation (0, 1, 2, 3)

    SDL_Rect r; 

    //Menu buttons
    SDL_Rect btn_play;
    SDL_Rect btn_leaderboard;
    SDL_Rect btn_options;
};

#endif