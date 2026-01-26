#include "../header_files/draw.h"

static bool is_mouse_over(SDL_Rect rect) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_Point pt = {x, y};
    return SDL_PointInRect(&pt, &rect);
}

typedef struct {
    Uint8 r, g, b, a;
} Color;

static const Color TETROMINO_COLORS[7] = {
    [I] = { 0, 255, 255, 255 },   // I - Cyan
    [O] = { 255, 255, 0, 255 },   // O - Yellow
    [T] = { 128, 0, 128, 255 },   // T - Purple
    [S] = { 0, 255, 0, 255 },     // S - Green
    [Z] = { 255, 0, 0, 255 },     // Z - Red
    [J] = { 0, 0, 255, 255 },     // J - Blue
    [L] = { 255, 165, 0, 255 },   // L - Orange
};

void color_id(SDL_Renderer *renderer, TetrominoType type) {
    Color c = TETROMINO_COLORS[type];
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void draw_menu(struct Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(game->renderer, &game->btn_play);
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(game->renderer, &game->btn_leaderboard);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(game->renderer, &game->btn_options);
}

void draw_layout(struct Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
    SDL_Rect wall_block = {0, 0, BLOCK_SIZE, BLOCK_SIZE};

    wall_block.x = GAME_OFFSET_X - BLOCK_SIZE;
    for (int y = 0; y < BOARD_HEIGHT + 1; y++) {
        wall_block.y = y * BLOCK_SIZE;
        SDL_RenderFillRect(game->renderer, &wall_block);
        
        SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255); 
        SDL_RenderDrawRect(game->renderer, &wall_block);
        SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255); 
    }

    wall_block.x = GAME_OFFSET_X + (BOARD_WIDTH * BLOCK_SIZE);
    for (int y = 0; y < BOARD_HEIGHT + 1; y++) {
        wall_block.y = y * BLOCK_SIZE;
        SDL_RenderFillRect(game->renderer, &wall_block);
        
        SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
        SDL_RenderDrawRect(game->renderer, &wall_block);
        SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
    }

    wall_block.y = BOARD_HEIGHT * BLOCK_SIZE;
    int start_x = GAME_OFFSET_X - BLOCK_SIZE;
for (int i = 0; i < BOARD_WIDTH + 2; i++) { 
        wall_block.x = start_x + (i * BLOCK_SIZE);
        
        SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(game->renderer, &wall_block);
        
        SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
        SDL_RenderDrawRect(game->renderer, &wall_block);
    }

    SDL_Rect hold_box;

    hold_box.x = BLOCK_SIZE; 
    hold_box.y = BLOCK_SIZE * 2; 
    
    hold_box.w = BLOCK_SIZE * 4;
    hold_box.h = BLOCK_SIZE * 4;

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &hold_box);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(game->renderer, &hold_box);
}

void draw_grid(struct Game *game) {
    for(int y = 0; y < TOTAL_ROWS; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            
            if(game->grid[y][x] != 0){
                SDL_Rect block_rect;
                block_rect.w = BLOCK_SIZE;
                block_rect.h = BLOCK_SIZE;
                block_rect.x = GAME_OFFSET_X + (x * BLOCK_SIZE);
                
                int screen_y = (y - BUFFER_ZONE) * BLOCK_SIZE;
                block_rect.y = screen_y;

                if (screen_y >= 0) {
                    color_id(game->renderer, game->grid[y][x] - 1);
                    SDL_RenderFillRect(game->renderer, &block_rect);
                    
                    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 100);
                    SDL_RenderDrawRect(game->renderer, &block_rect);
                }
            }
        }
    }
}

void draw_tetro(struct Game *game, SDL_Renderer *renderer, TetrominoType type, Rotation rotation, int grid_x, int grid_y) {
    
    for(int py = 0; py < 4; py++){
        for(int px = 0; px < 4; px++){
            if(TETROMINOS[type][rotation][py][px] != 0){
                
                int FinalX = grid_x + px;
                int FinalY = grid_y + py;
        
                SDL_Rect block_rect;
                block_rect.w = BLOCK_SIZE;
                block_rect.h = BLOCK_SIZE;
                block_rect.x = GAME_OFFSET_X + (FinalX * BLOCK_SIZE);
                block_rect.y = (FinalY - BUFFER_ZONE) * BLOCK_SIZE;

                if((FinalY - BUFFER_ZONE) >= 0){
                
                    color_id(renderer, type);
                    SDL_RenderFillRect(renderer, &block_rect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                    SDL_RenderDrawRect(renderer, &block_rect);
                }
            }
        }
    }
}