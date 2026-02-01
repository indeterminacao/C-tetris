#include <stdio.h>
#include <time.h>

#include "Gameconfs.h"
#include "header_files/init.h"
#include "header_files/input.h"
#include "header_files/draw.h"
#include "header_files/logic.h"

// TODO: Game Loop - Implement Leveling system (increase gravity speed based on cleared lines)
// TODO: UI/Logic - Scoring system (calculate points based on lines, level and drop type)
// TODO: Polish - Ghost Piece (visual aid for hard drop)

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    srand(time(NULL));

    struct Game game = {
        .ProgramOn = true,
        .state = STATE_MENU,
        .currentType = T,          
        .currentRotation = 0,    
        .currentX = (BOARD_WIDTH / 2) - 2,
        .currentY = 0,            
        .gravity_delay = 500,
        .lock_delay = 500,
        .btn_play = {SCREEN_WIDTH/2 - 100, 150, 200, 50},
        .btn_leaderboard = {SCREEN_WIDTH/2 - 100, 250, 200, 50},
        .btn_options = {SCREEN_WIDTH/2 - 100, 350, 200, 50},
        .active_piece = false,
        .last_move_was_rotate = false,
        .lock_timer = 0,
        .is_locking = false,
        .lock_resets = 15,
        .level = 1,
        .input.keystate = SDL_GetKeyboardState(NULL)
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

        if (game.state == STATE_GAME && !game.active_piece) {
            spawn_piece(&game);

            if (check_collision(&game, game.currentX, game.currentY, game.currentRotation)) {
                printf("GAME OVER!\n");
                game.ProgramOn = false;
            }
        }

        event_handling(&game);
        input_update(&game);

    if (game.state == STATE_GAME && game.active_piece) { 
                
        bool on_ground = check_collision(&game, game.currentX, game.currentY + 1, game.currentRotation);

        if (!on_ground) {
            game.is_locking = false;
            if(!game.input.soft_dropping){
                if(game.current_tick > game.gravity_timer + game.gravity_delay){
                        game.currentY += 1;
                        game.last_move_was_rotate = false;
                        game.gravity_timer = game.current_tick;
                    }
                }
            } else {
                if (!game.is_locking) {
                    game.lock_timer = SDL_GetTicks();
                    game.is_locking = true;
                    }
                if (game.current_tick > game.lock_timer + game.lock_delay || game.lock_resets <= 0) {
                    resolve_lock(&game);
                    game.is_locking = false; 
                    }
                }
            }

        SDL_SetRenderDrawColor(game.renderer, 20, 20, 20, 255);
        SDL_RenderClear(game.renderer);

        if (game.state == STATE_MENU) {
            draw_menu(&game);    

        } else if (game.state == STATE_GAME) {
            draw_layout(&game);  
            draw_grid(&game);

            if (game.active_piece) {
                draw_ghost(&game);
                draw_tetro(game.renderer, game.currentType, game.currentRotation, game.currentX, game.currentY);
            }
        }

        SDL_RenderPresent(game.renderer);
    }

    cleanup(&game);
    return 0;
}