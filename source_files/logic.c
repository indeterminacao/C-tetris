#include "../header_files/logic.h"
#include "../header_files/table.h"

bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot) {
    for(int py = 0; py < 4; py++){
        for(int px = 0; px < 4; px++){
            if(TETROMINOS[game->currentType][rot][py][px] != 0){
                int board_x = grid_x + px;
                int board_y = grid_y + py;

                if(board_x < 0 || board_x >= BOARD_WIDTH) { 
                    return true; 
                }
                if(board_y >= TOTAL_ROWS) { 
                    return true; 
                }
                if(board_y >= 0){
                    if(game->grid[board_y][board_x] != 0) {
                    return true; 
                    }
                }
            }
        }
    }
    return false;
}

bool is_blocked(struct Game *game, int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH) return true; 
    if (y >= TOTAL_ROWS) return true;        
    if (y < 0) return false;                    
    return game->grid[y][x] != 0;
}

void EPLD(struct Game *game) {
    if (!check_collision(game,
            game->currentX,
            game->currentY + 1,
            game->currentRotation)) {
        return;
    }

    if (game->lock_resets > 0) {
        game->lock_resets--;
        game->lock_timer = SDL_GetTicks();
    }
}

void spawn_piece(struct Game *game) {
    game->currentType = rand() % 7; 
    game->currentRotation = ROT_0; 
    game->currentX = (BOARD_WIDTH / 2) - 2; 
    game->currentY = 0; 
    game->lock_resets = 15;
    game->active_piece = true;
    game->gravity_timer = game->current_tick;
    game->input.DAS_timer = 0;
    game->input.ARR_timer = 0;
    game->input.soft_drop_timer = 0;
    game->last_move_was_rotate = false; 
    game->input.move_dir = MOVE_NONE;
    game->input.soft_dropping = false;

    if (check_collision(game, game->currentX, game->currentY, game->currentRotation)) {
        printf("GAME OVER\n");
        game->active_piece = false;
    }
}

void lock_piece(struct Game *game) {
    for(int py = 0; py < 4; py++){
        for(int px = 0; px < 4; px++){
            if(TETROMINOS[game->currentType][game->currentRotation][py][px] != 0){
                int board_x = game->currentX + px;
                int board_y = game->currentY + py;

                if(board_y >= 0 && board_y < TOTAL_ROWS && board_x >= 0 && board_x < BOARD_WIDTH){
                    game->grid[board_y][board_x] = game->currentType + 1; 
                }
            }
        }
    }
}

uint8_t clear_lines(struct Game *game) {
    uint8_t lines_cleared = 0; 
    for(int y = 0; y < TOTAL_ROWS; y++){ 
        bool line_full = true;
        for(int x = 0; x < BOARD_WIDTH; x++){
            if(game->grid[y][x] == 0){
                line_full = false;
                break;
            }
        }
        if(line_full){
            lines_cleared++;
            for(int ty = y; ty > 0; ty--){
                for(int tx = 0; tx < BOARD_WIDTH; tx++){
                    game->grid[ty][tx] = game->grid[ty - 1][tx];
                }
            }
            for(int tx = 0; tx < BOARD_WIDTH; tx++){
                game->grid[0][tx] = 0;
            }
            y--; 
        }
    }
    return lines_cleared;
}

TSpinType is_t_spin(struct Game *game) {
    if (game->currentType != T || !game->last_move_was_rotate) {
        return TSPIN_NONE;
    }

    int center_x = game->currentX + 1;
    int center_y = game->currentY + 1;

    int corners[4][2] = {
        {center_x - 1, center_y - 1}, 
        {center_x + 1, center_y - 1}, 
        {center_x - 1, center_y + 1}, 
        {center_x + 1, center_y + 1}  
    };

    int occupied_corners = 0;
    for (int i = 0; i < 4; i++) {
        if (is_blocked(game, corners[i][0], corners[i][1])) {
            occupied_corners++;
        }
    }

    if (occupied_corners < 3) {
        return TSPIN_NONE;
    } 
    

    return TSPIN_NORMAL; 
}

void resolve_lock(struct Game *game) {
    TSpinType tspin = is_t_spin(game);

    lock_piece(game);

    uint8_t lines = clear_lines(game);
    update_score(game, lines, tspin);

    if (tspin == TSPIN_NORMAL) {
        if (lines == 0) printf("T-Spin Zero!\n"); 
        if (lines == 1) printf("T-SPIN SINGLE! \n");
        if (lines == 2) printf("T-SPIN DOUBLE! \n");
        if (lines == 3) printf("T-SPIN TRIPLE! \n");
    } else if (lines > 0) {
        if (lines == 4) printf("TETRIS! \n");
        else printf("Cleared %d lines\n", lines);
    }

    game->active_piece = false;
    game->last_move_was_rotate = false;
}

void update_score(struct Game *game, uint8_t lines_cleared, TSpinType tspin) {
    Uint32 points = 0;
    bool hard_move = false;

    if (tspin == TSPIN_NORMAL) {
        hard_move = true;
        switch (lines_cleared) {
            case 0: points = 400; break; 
            case 1: points = 800; break; 
            case 2: points = 1200; break; 
            case 3: points = 1600; break; 
        }
    } else {
        switch (lines_cleared) {
            case 1: points = 100; break; 
            case 2: points = 300; break; 
            case 3: points = 500; break; 
            case 4: 
                points = 800; 
                hard_move = true;
            break; 
        }
    }
if (hard_move) {
        if (game->B2B) {
            points = (points * 3) / 2; 
            printf("BACK-TO-BACK! \n"); 
        }
        game->B2B = true; 
    } 
    else if (lines_cleared > 0) {
        game->B2B = false; 
    }
    game->score += points * game->level;
    printf("SCORE: %d\n", game->score);

    game->total_linesclr += lines_cleared;
    if (game->total_linesclr >= game->level * 10) {
        game->level++;
        printf("LEVEL UP! %d\n", game->level);
    }
}

void spin(struct Game *game, int direction) {
    Rotation current_rot = game->currentRotation;
    Rotation next_rot;
    
    if (direction == 1) {
        next_rot = (current_rot + 1) % 4;
    } else if (direction == -1) {
        next_rot = (current_rot + 3) % 4;
    } else {
        return; 
    }

    const Point (*kicks)[4][4][5];

    if(game->currentType == O) {
        return;
    } else if (game->currentType == I) {
        kicks = &kicks_i;
    } else {
        kicks = &kicks_jlstz;
    }

    for (int i = 0; i < 5; i++) {
        Point test = (*kicks)[current_rot][next_rot][i];
        int new_x = game->currentX + test.x;
        int new_y = game->currentY + test.y;

        if (!check_collision(game, new_x, new_y, next_rot)) {
            game->currentX = new_x;
            game->currentY = new_y;
            game->currentRotation = next_rot;
            game->last_move_was_rotate = true; 
            return; 
        }
    }
}

void hard_drop(struct Game *game){
    game->last_move_was_rotate = false; 

    while(!check_collision(game, game->currentX, game->currentY + 1, game->currentRotation)){
        game->currentY += 1;
    }
    resolve_lock(game);

    game->gravity_timer = SDL_GetTicks();
}