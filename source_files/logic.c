#include "../header_files/logic.h"
#include "../header_files/table.h"

bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot) {
    for(int py = 0; py < 4; py++){
        for(int px =0; px < 4; px++){
            if(TETROMINOS[game->currentType][rot][py][px] != 0){
            int board_x = grid_x + px;
            int board_y = grid_y + py;

            if(board_x < 0 || board_x >= BOARD_WIDTH){
                return true; 
            }
            if(board_y >= TOTAL_ROWS){
                return true; 
            }
            if(board_y >= 0){
                if(game->grid[board_y][board_x] != 0){
                    return true; 
                }
            }
            }
        }
    }
    return false;
}

void spawn_piece(struct Game *game) {
    game->currentType = rand() % 7; 
    game->currentRotation = 0; 
    game->currentX = (BOARD_WIDTH / 2) - 2; 
    game->currentY = 0; 
    game->active_piece = true;
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
            return; 
        }
    }
}

void hard_drop(struct Game *game){
    while(!check_collision(game, game->currentX, game->currentY + 1, game->currentRotation)){
        game->currentY += 1;
    }

    lock_piece(game);

    uint8_t lines = clear_lines(game);
    if (lines > 0) {    
        printf("Cleared %d lines!\n", lines);
    }   

    game->active_piece = false;

    game->last_tick = SDL_GetTicks();
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