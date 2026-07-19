#include "../../header_files/screens/game.h"
#include "../../header_files/logic.h"
#include "../../header_files/draw.h"

/**
 * @brief Moves the active piece horizontally.
 * @param game Pointer to the main Game structure.
 * @param dx Horizontal movement direction. Valid values are:
 *            `-1` for left.
 *            `+1` for right.
 */
static void move_horizontal(struct Game *game, int dx) {
    if (!game->piece.active) return;
    if (!check_collision(game, game->piece.x + dx, game->piece.y, game->piece.rotation)) {
        game->piece.x += dx;
        game->piece.last_move_was_rotate = false;
        extend_lock_delay(game);
    }
}

static void add_soft_drop_points(struct Game *game){
    game->score.points++;
}

static void soft_drop(struct Game *game){
    if (! game->piece.active) {
        return;
    }
    if (!check_collision(game,  game->piece.x,  game->piece.y + 1,  game->piece.rotation)) {
        game->piece.y += 1;
        add_soft_drop_points(game);
        game->piece.last_move_was_rotate = false;
    }
}

static void try_rotate(struct Game *game, int direction) {
    if (! game->piece.active) {
        return;
    }
    Rotation orot =  game->piece.rotation;
    spin(game, direction);

    if ( game->piece.rotation != orot) {
        extend_lock_delay(game);
    }
}

void game_screen_handle_input(struct Game *game, SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.repeat != 0) {
            return;
        }
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                    game->input.move_dir = MOVE_LEFT;
                    game->input.das_timer = game->current_tick;
                    game->input.arr_timer = game->current_tick;
                    move_horizontal(game, -1);
                break;

            case SDL_SCANCODE_RIGHT:
                    game->input.move_dir = MOVE_RIGHT;
                    game->input.das_timer = game->current_tick;
                    game->input.arr_timer = game->current_tick;
                    move_horizontal(game, 1);
                break;

            case SDL_SCANCODE_DOWN:
                    game->input.soft_dropping = true;
                    soft_drop(game);
                    game->input.soft_drop_timer = game->current_tick;
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
                    hard_drop(game);
                break;

            case SDL_SCANCODE_C:
                    hold_piece(game);
                break;

            default:
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                if (game->input.move_dir == MOVE_LEFT) {
                    game->input.move_dir = MOVE_NONE;
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (game->input.move_dir == MOVE_RIGHT) {
                    game->input.move_dir = MOVE_NONE;
                }
                break;
            case SDL_SCANCODE_DOWN:
                game->input.soft_dropping = false;
                break;
            default:
                break;
        }
    }
}

static void update_das_arr(struct Game *game, Uint32 now) {
    if (game->input.move_dir == MOVE_NONE) return;
    if (now - game->input.das_timer < game->input_config.das_delay) return;
    if (now - game->input.arr_timer < game->input_config.arr_delay) return;

    if (game->input.move_dir == MOVE_LEFT) {
        move_horizontal(game, -1);
    } else if (game->input.move_dir == MOVE_RIGHT) {
        move_horizontal(game, 1);
    }
    game->input.arr_timer = now;
}

static void update_soft_drop_repeat(struct Game *game, Uint32 now) {
    if (game->input.soft_dropping) {
        if (now - game->input.soft_drop_timer >= game->input_config.soft_drop_arr) {
            soft_drop(game);
            game->input.soft_drop_timer = now;
        }
    }
}

static bool spawn_if_needed(struct Game *game) {
    if (game->piece.active) {
        return false;
    }

    spawn_piece(game);
    if (!game->piece.active) {
        game->state = STATE_GAMEOVER;
    }
    return true;
}

static void update_gravity_and_lock(struct Game *game, Uint32 now){
    bool on_ground = check_collision(game,  game->piece.x,  game->piece.y + 1,  game->piece.rotation);
    if (!on_ground) {
        game->physics.is_locking = false;
        if (!game->input.soft_dropping && now > game->physics.gravity_timer + game->physics_config.gravity_delay) {
             game->piece.y += 1;
            game->piece.last_move_was_rotate = false;
            game->physics.gravity_timer = now;
        }
    } else {
        if (!game->physics.is_locking) {
            game->physics.lock_timer = now;
            game->physics.is_locking = true;
        }
        if (now > game->physics.lock_timer + game->physics_config.lock_delay || game->physics.lock_resets <= 0) {
            resolve_lock(game);
            game->physics.is_locking = false;
        }
    }
}


void game_screen_update(struct Game *game) {
    Uint32 now = game->current_tick;

    // Continuous DAS/ARR horizontal movement
    update_das_arr(game, now);

    // Continuous soft drop
    update_soft_drop_repeat(game, now);

    // Spawn a new piece if none is active
    if(spawn_if_needed(game)){
        return;
    }

    // Gravity and lock delay
    update_gravity_and_lock(game, now);
}

void game_screen_render(struct Game *game) {
    draw_layout(game);
    draw_grid(game);
    if (game->piece.active) {
        draw_ghost(game);
        draw_tetro(game->renderer, game->piece.type,  game->piece.rotation,  game->piece.x,  game->piece.y);
    }
}