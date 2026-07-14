#include "../header_files/input.h"
#include "../Gameconfs.h"
#include "../header_files/screens/menu.h"
#include "../header_files/screens/game.h"
#include "../header_files/screens/leaderboard.h"
#include "../header_files/screens/options.h"

static void dispatch_to_screen(struct Game *game, SDL_Event event){
    switch (game->state)
    {
    case STATE_MENU:
        menu_handle_input(game, event);
        break;
    case STATE_GAME:
        game_screen_handle_input(game, event);
        break;
    case STATE_LEADERBOARD:
        leaderboard_handle_input(game, event);
        break;
    case STATE_OPTIONS:
        options_handle_input(game, event);
        break;
    default:
        break;
    }
}

void event_handling(struct Game *game){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT) {
            game->ProgramOn = false;
            continue;
        }
        
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
            if (game->state == STATE_GAME || game->state == STATE_LEADERBOARD || game->state == STATE_OPTIONS){
                game->state = STATE_MENU;
            } else{
                game->ProgramOn = false;
            }
            continue;
        }

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONDOWN) {
            dispatch_to_screen(game, event);
        }
    }
}