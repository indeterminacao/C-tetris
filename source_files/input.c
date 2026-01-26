#include "../header_files/input.h"
#include "../header_files/logic.h"

void event_handling(struct Game *game){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                game->ProgramOn = false;
                    break;
                case SDL_KEYDOWN: 
                    keydown_inputs(game, event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_inputs(game, event);
                default:
                    break;
            }
    }
}

void keydown_inputs(struct Game *game, SDL_Event event ){
    switch(event.key.keysym.scancode){
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (game->state == STATE_GAME || game->state == STATE_LEADERBOARD || game->state == STATE_OPTIONS) {
                    game->state = STATE_MENU;
                } else {
                    game->ProgramOn = false;
                }
            return;
        }
    }
    if (game->state == STATE_GAME) {
        switch(event.key.keysym.scancode){
            case SDL_SCANCODE_LEFT:
                if (!check_collision(game, game->currentX - 1, game->currentY, game->currentRotation)) {
                    game->currentX -= 1;
                }
                break;
                
            case SDL_SCANCODE_RIGHT:
                if (!check_collision(game, game->currentX + 1, game->currentY, game->currentRotation)) {
                    game->currentX += 1;
                }
                break;

            case SDL_SCANCODE_DOWN:
                if (!check_collision(game, game->currentX, game->currentY + 1, game->currentRotation)) {
                    game->currentY += 1;
                    game->last_tick = SDL_GetTicks();
                }
                break;

            case SDL_SCANCODE_UP: 
            case SDL_SCANCODE_X:
                {
                Rotation next_rot = get_next_rotation(game, 1);
                if (!check_collision(game, game->currentX, game->currentY, next_rot)) {
                    game->currentRotation = next_rot;
                }
                break;
            }
            case SDL_SCANCODE_Z:
            case SDL_SCANCODE_LCTRL:
            {
                Rotation prev_rot = get_next_rotation(game, -1);
                if (!check_collision(game, game->currentX, game->currentY, prev_rot)) {
                    game->currentRotation = prev_rot;
                }
                break;
            }
            case SDL_SCANCODE_SPACE:
            if(event.key.repeat == 0){
                hard_drop(game);
            }
                break;
            default: 
                break;
        }
    }
}

void mouse_inputs(struct Game *game, SDL_Event event) {
    if (game->state == STATE_MENU && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point mouse_point = { event.button.x, event.button.y };

        if (SDL_PointInRect(&mouse_point, &game->btn_play)) {
            game->state = STATE_GAME;
        }
        else if (SDL_PointInRect(&mouse_point, &game->btn_leaderboard)) {
            game->state = STATE_LEADERBOARD;
        }
        else if (SDL_PointInRect(&mouse_point, &game->btn_options)) {
            game->state = STATE_OPTIONS;
        }
    }
}

bool is_mouse_over(SDL_Rect rect) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_Point pt = {x, y};
    return SDL_PointInRect(&pt, &rect);
}