#include "../../header_files/screens/menu.h"
#include "../../header_files/draw.h"
#include "../../Gameconfs.h"

void menu_handle_input(struct Game *game, SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point mouse_point = { event.button.x, event.button.y };

        if (SDL_PointInRect(&mouse_point, &game->btn_play)) {
            game->state = STATE_GAME;
        } else if (SDL_PointInRect(&mouse_point, &game->btn_leaderboard)) {
            game->state = STATE_LEADERBOARD;
        } else if (SDL_PointInRect(&mouse_point, &game->btn_options)) {
            game->state = STATE_OPTIONS;
        }
    }
}

void menu_update(struct Game *game) {
    (void)game;
}

void menu_render(struct Game *game) {
    draw_menu(game);
}