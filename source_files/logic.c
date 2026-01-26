#include "../header_files/logic.h"

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

Rotation get_next_rotation(struct Game *game, int direction) {
    if (direction == 1) {
        return (game->currentRotation + 1) % 4;
    } 
    else if (direction == -1) {
        return (game->currentRotation + 3) % 4;
    }
    return game->currentRotation;
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