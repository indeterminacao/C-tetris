#include <stdio.h>
#include <time.h>
#include "Gameconfs.h"
#include "header_files/init.h"
#include "header_files/input.h"
#include "header_files/draw.h"
#include "header_files/logic.h"

//TODO: Input Mechanics - Add mappings for Clockwise (Up/X) and Counter-Clockwise (Z/Ctrl)
//TODO: Rotation Logic - Implement SRS (Super Rotation System) standard wall kick tables
//TODO: Game Loop - Implement Leveling system (increase gravity speed based on cleared lines)
//TODO: UI/Logic - Scoring system (calculate points based on lines, level and drop type)
//TODO: Polish - Ghost Piece (visual aid for hard drop)

int main(int argc, char *argv[]) {
    srand(time(NULL));

    struct Game game = {
        .ProgramOn = true,
        .state = STATE_MENU,
        .currentType = T,           
        .currentRotation = 0,    
        .currentX = (BOARD_WIDTH / 2) - 2, 
        .currentY = 0,             
        .drop_speed = 500,
        .btn_play = {SCREEN_WIDTH/2 - 100, 150, 200, 50},
        .btn_leaderboard = {SCREEN_WIDTH/2 - 100, 250, 200, 50},
        .btn_options = {SCREEN_WIDTH/2 - 100, 350, 200, 50},
    };

    for(int y=0; y<TOTAL_ROWS; y++) {
        for(int x=0; x<BOARD_WIDTH; x++) {
            game.grid[y][x] = 0;
        }
    }

    if(sdl_initializer(&game)){
        return 1;
    }
while(game.ProgramOn){
        game.current_tick = SDL_GetTicks();
        
        event_handling(&game); 

if (game.state == STATE_GAME) {
    if(game.current_tick > game.last_tick + game.drop_speed){
if (!check_collision(&game, game.currentX, game.currentY + 1, game.currentRotation)) {
    game.currentY += 1; 
} else {
    lock_piece(&game); 
    uint8_t lines = clear_lines(&game);
    if (lines > 0) {    
        printf("Cleared %d lines!\n", lines);
    }

    
    game.currentType = rand() % 7;
    game.currentRotation = 0;
    game.currentX = (BOARD_WIDTH / 2) - 2;
    game.currentY = 0;

    if (check_collision(&game, game.currentX, game.currentY, game.currentRotation)) {
        printf("GAME OVER!\n");
        game.ProgramOn = false; 
    }
}
        
        game.last_tick = game.current_tick;
    }
}

        SDL_SetRenderDrawColor(game.renderer, 20, 20, 20, 255);
        SDL_RenderClear(game.renderer);

        if (game.state == STATE_MENU) {
            draw_menu(&game);     
        } else if (game.state == STATE_GAME) {
            draw_layout(&game);   
            draw_grid(&game); 
            draw_tetro(&game, game.renderer, game.currentType, game.currentRotation, game.currentX, game.currentY);
        }

        SDL_RenderPresent(game.renderer); 
    }

    cleanup(&game);
    return 0;
}