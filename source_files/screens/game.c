#include "../../header_files/screens/game.h"
#include "../../header_files/logic.h"
#include "../../header_files/draw.h"

static void move_left(struct Game *game){
    if (!game->active_piece) {
        return;
    }
    if (!check_collision(game, game->currentX - 1, game->currentY, game->currentRotation)) {
        game->currentX -= 1;
        game->last_move_was_rotate = false;
        EPLD(game);
    }
}

static void move_right(struct Game *game) {
    if (!game->active_piece) {
        return;
    }
    if (!check_collision(game, game->currentX + 1, game->currentY, game->currentRotation)) {
        game->currentX += 1;
        game->last_move_was_rotate = false;
        EPLD(game);
    }
}

static void soft_drop(struct Game *game){
    if (!game->active_piece) {
        return;
    }
    if (!check_collision(game, game->currentX, game->currentY + 1, game->currentRotation)) {
        game->currentY += 1;
        game->last_move_was_rotate = false;
    }
}

static void try_rotate(struct Game *game, int direction) {
    if (!game->active_piece) {
        return;
    }
    Rotation orot = game->currentRotation;
    spin(game, direction);

    if (game->currentRotation != orot) {
        EPLD(game);
    }
}

void game_screen_handle_input(struct Game *game, SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                if (event.key.repeat == 0) {
                    game->input.move_dir = MOVE_LEFT;
                    game->input.DAS_timer = game->current_tick;
                    game->input.ARR_timer = game->current_tick;
                    move_left(game);
                }
                break;

            case SDL_SCANCODE_RIGHT:
                if (event.key.repeat == 0) {
                    game->input.move_dir = MOVE_RIGHT;
                    game->input.DAS_timer = game->current_tick;
                    game->input.ARR_timer = game->current_tick;
                    move_right(game);
                }
                break;

            case SDL_SCANCODE_DOWN:
                if (event.key.repeat == 0) {
                    game->input.soft_dropping = true;
                    soft_drop(game);
                    game->input.soft_drop_timer = game->current_tick;
                }
                break;

            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_X:
                if (event.key.repeat == 0) {
                    try_rotate(game, 1);
                }
                break;

            case SDL_SCANCODE_Z:
            case SDL_SCANCODE_LCTRL:
                if (event.key.repeat == 0) {
                    try_rotate(game, -1);
                }
                break;

            case SDL_SCANCODE_SPACE:
                if (event.key.repeat == 0) {
                    hard_drop(game);
                }
                break;

            case SDL_SCANCODE_C:
                if (event.key.repeat == 0) {
                    hold_piece(game);
                }
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

void game_screen_update(struct Game *game) {
    Uint32 now = game->current_tick;

    // Continuous DAS/ARR horizontal movement
    if (game->input.move_dir != MOVE_NONE) {
        if (now - game->input.DAS_timer >= DAS_TIME) {
            if (now - game->input.ARR_timer >= ARR_TIME) {
                if (game->input.move_dir == MOVE_LEFT) {
                    move_left(game);
                } else if (game->input.move_dir == MOVE_RIGHT) {
                    move_right(game);
                }
                game->input.ARR_timer = now;
            }
        }
    }

    // Continuous soft drop
    if (game->input.soft_dropping) {
        if (now - game->input.soft_drop_timer >= SOFT_DROP_ARR) {
            soft_drop(game);
            game->input.soft_drop_timer = now;
        }
    }

    // Spawn a new piece if none is active
    if (!game->active_piece) {
        spawn_piece(game);
        if (!game->active_piece) {
            game->ProgramOn = false;
        }
        return;
    }

    // Gravity and lock delay
    bool on_ground = check_collision(game, game->currentX, game->currentY + 1, game->currentRotation);
    if (!on_ground) {
        game->is_locking = false;
        if (!game->input.soft_dropping && now > game->gravity_timer + game->gravity_delay) {
            game->currentY += 1;
            game->last_move_was_rotate = false;
            game->gravity_timer = now;
        }
    } else {
        if (!game->is_locking) {
            game->lock_timer = now;
            game->is_locking = true;
        }
        if (now > game->lock_timer + game->lock_delay || game->lock_resets <= 0) {
            resolve_lock(game);
            game->is_locking = false;
        }
    }
}

void game_screen_render(struct Game *game) {
    draw_layout(game);
    draw_grid(game);
    if (game->active_piece) {
        draw_ghost(game);
        draw_tetro(game->renderer, game->currentType, game->currentRotation, game->currentX, game->currentY);
    }
}