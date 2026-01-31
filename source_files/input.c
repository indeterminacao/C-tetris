#include "../header_files/input.h"
#include "../header_files/logic.h"

//Prototypes
/** @brief Auxiliary function for keydown events 
* @param game Pointer to the main Game struct
* @param event SDL_Event structure containing the keydown event
*/
static void keydown_inputs(struct Game *game, SDL_Event event );

/** @brief Auxiliary function for mouse events
* @param game Pointer to the main Game struct
* @param event SDL_Event structure containing the mouse event
*/
static void mouse_inputs(struct Game *game, SDL_Event event);
static void move_left(struct Game *game);
static void move_right(struct Game *game);
static void soft_drop(struct Game *game);
static void try_rotate(struct Game *game, int direction);


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

static void keydown_inputs(struct Game *game, SDL_Event event){
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        if (game->state == STATE_GAME || game->state == STATE_LEADERBOARD || game->state == STATE_OPTIONS) {
            game->state = STATE_MENU;
        } else {
            game->ProgramOn = false;
        }
        return; 
    }

    if (game->state == STATE_GAME) {
        switch(event.key.keysym.scancode){
            case SDL_SCANCODE_LEFT:
                    move_left(game);
                break;
                
            case SDL_SCANCODE_RIGHT:
                    move_right(game);
                break;

            case SDL_SCANCODE_DOWN:
                    soft_drop(game);
                break;

            case SDL_SCANCODE_UP: 
            case SDL_SCANCODE_X:
                    try_rotate(game, 1);
                break;

            case SDL_SCANCODE_Z:
            case SDL_SCANCODE_LCTRL:
                    try_rotate(game, -1);
                break;

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

static void mouse_inputs(struct Game *game, SDL_Event event) {
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

static void move_left(struct Game *game){
    if (!check_collision(game, game->currentX - 1, game->currentY, game->currentRotation)) {
        game->currentX -= 1;
        game->last_move_was_rotate = false;
        EPLD(game);
    }
}

static void move_right(struct Game *game) {
    if (!check_collision(game, game->currentX + 1, game->currentY, game->currentRotation)) {
        game->currentX += 1;
        game->last_move_was_rotate = false;
        EPLD(game);
    }
}

static void soft_drop(struct Game *game){
    if (!check_collision(game, game->currentX, game->currentY + 1, game->currentRotation)) {
        game->currentY += 1;
        game->last_tick = SDL_GetTicks();
        game->last_move_was_rotate = false;
    }
}

static void try_rotate(struct Game *game, int direction) {
    Rotation orot = game->currentRotation;
    spin(game, direction);

    if (game->currentRotation != orot) {
        EPLD(game);
    }
}