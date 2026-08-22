#include <stdio.h>
#include <time.h>

#include "Gameconfs.h"
#include "header_files/init.h"
#include "header_files/input.h"
#include "header_files/logic.h"
#include "header_files/screens/menu.h"
#include "header_files/screens/game.h"
#include "header_files/screens/leaderboard.h"
#include "header_files/screens/options.h"
#include "header_files/screens/gameover.h"

/**
 * TODO LIST - (GAMEPLAY)
 *
 * HOLD 
 * [x] Hold logic
 * [X] Hold rendering
 *  
 * GAME OVER 
 * [X] Add STATE_GAMEOVER to GameState
 * [X] Create screens/gameover.c + .h, same pattern as the other 3 screens
 * [X] Wire into main.c's two switches + input.c's dispatcher
 * [X] Return to Menu 
 * [X] Restart option
 *   
 * SCORING LOGIC 
 * [X] Track hard drop distance, add points for it
 * [X] Track soft drop distance, add points for it
 * [X] Implement Combo
 * [X] Implement Perfect Clear
 *
 * IMPLEMENT LEVEL PROGRESSION
 * 
 * Create the scoring event system
 *  
 * TEXT RENDERING FOUNDATION 
 * [ ] Load a font via SDL_ttf
 * [ ] A draw_text() helper in draw.c
 * [ ] Free the font in cleanup()
 *
 * NEXT QUEUE 
 * [ ] Add peek-ahead to piece generation
 * [ ] Show next piece
 * [ ] Show 5-piece queue
 *
 * HUD 
 * [ ] Score display
 * [ ] Level display
 * [ ] Lines display
 * [ ] Back-to-Back display
 * [ ] Combo display
 */

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    srand((unsigned int)time(NULL));

    struct Game game = {
        .ProgramOn = true,
        .state = STATE_MENU,
        .piece.type = T,          
        .piece.rotation = 0,    
        .piece.x = (BOARD_WIDTH / 2) - 2,
        .piece.y = 0,            
        .physics_config.gravity_delay = DEFAULT_GRAVITY_DELAY,
        .physics_config.lock_delay = DEFAULT_LOCK_DELAY,
        .physics_config.max_lock_resets = DEFAULT_MAX_LOCK_RESETS,
        .btn_play = {SCREEN_WIDTH/2 - 100, 150, 200, 50},
        .btn_leaderboard = {SCREEN_WIDTH/2 - 100, 250, 200, 50},
        .btn_options = {SCREEN_WIDTH/2 - 100, 350, 200, 50},
        .btn_restart = {SCREEN_WIDTH/2 - 100, 250, 200, 50},
        .btn_return_menu = {SCREEN_WIDTH/2 - 100, 350, 200, 50},
        .piece.active = false,
        .piece.last_move_was_rotate = false,
        .physics.lock_timer = 0,
        .physics.is_locking = false,
        .score.level = 1,
        .input.keystate = SDL_GetKeyboardState(NULL),
        .input_config.das_delay = DEFAULT_DAS_DELAY,
        .input_config.arr_delay = DEFAULT_ARR_DELAY,
        .input_config.soft_drop_arr = DEFAULT_SOFT_DROP_ARR,
        .piece.held_piece = NONE,
        .piece.hold_used = false,
        .score.combo = -1,
        .block_count = 0
    };

    wipe_board(&game);

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
            case STATE_GAMEOVER:
            gameover_update(&game);
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
            case STATE_GAMEOVER:
            gameover_render(&game);
            break;
        }

        SDL_RenderPresent(game.renderer);
    }

    cleanup(&game);
    return 0;
}