#include "../../header_files/screens/gameover.h"
#include "../../header_files/logic.h"
#include "../../header_files/draw.h"
#include "../../header_files/core/layout.h"


void gameover_update(struct Game *game) {
    (void)game;
}

void gameover_render(struct Game *game) {
    SDL_Renderer *r = game->renderer;

    draw_layout(game);
    draw_grid(game);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(r, &overlay);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(r, 200, 50, 50, 255);
    SDL_RenderFillRect(r, &game->btn_restart);

    SDL_SetRenderDrawColor(r, 80, 80, 200, 255);
    SDL_RenderFillRect(r, &game->btn_return_menu);
}

static void reset_game_state(struct Game *game){
        game->piece.held_piece = NONE;
        game->piece.hold_used = false;
        game->score.level = 1;
        game->score.points = 0;
        game->score.total_lines = 0;
        game->score.back_to_back = false;
        game->state = STATE_GAME;
        wipe_board(game);
}

void gameover_handle_input(struct Game *game, SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point mouse_point = { event.button.x, event.button.y };

        if (SDL_PointInRect(&mouse_point, &game->btn_restart)) {
            reset_game_state(game);
        } else if (SDL_PointInRect(&mouse_point, &game->btn_return_menu)) {
            game->state = STATE_MENU;
        }
    }
}