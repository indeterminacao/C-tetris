#include <stdio.h>
#include <time.h>

#include "Gameconfs.h"
#include "header_files/init.h"
#include "header_files/input.h"
#include "../header_files/screens/menu.h"
#include "../header_files/screens/game.h"
#include "../header_files/screens/leaderboard.h"
#include "../header_files/screens/options.h"

/**
 * TODO (Architecture Refactoring)
 *
 * [X] Game State Management
 * [ ] Header Organization (split Gameconfs.h)
 * --[ ] Move into sub-struct organization
 * [ ] Configurable Settings (DAS, ARR, Soft Drop, Gravity, etc.)
 * [ ] Internal Code Organization
 *      - Function names
 *      - Variable names
 *      - Function responsibilities
 */

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    srand((unsigned int)time(NULL));

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
        .input.keystate = SDL_GetKeyboardState(NULL),
        .held_piece = NONE,
        .hold_used = false
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

        switch (game.state) {
            case STATE_MENU:
                menu_update(&game);
                break;
            case STATE_GAME:
                game_screen_update(&game);
                break;
            case STATE_LEADERBOARD:
                leaderboard_update(&game);
                break;
            case STATE_OPTIONS:
                options_update(&game);
                break;
        }

        SDL_SetRenderDrawColor(game.renderer, 20, 20, 20, 255);
        SDL_RenderClear(game.renderer);

        switch (game.state) {
            case STATE_MENU:
                menu_render(&game);
                break;
            case STATE_GAME:
                game_screen_render(&game);
                break;
            case STATE_LEADERBOARD:
                leaderboard_render(&game);
                break;
            case STATE_OPTIONS:
                options_render(&game);
                break;
        }

        SDL_RenderPresent(game.renderer);
    }

    cleanup(&game);
    return 0;
}