#include "../header_files/draw.h"
#include "../header_files/logic.h"

// Types and Colors
typedef struct {
    Uint8 r, g, b, a;
} Color;

static const Color TETROMINO_COLORS[8] = {
    [I] = { 0,   255, 255, 255 },
    [O] = { 255, 255,   0, 255 },
    [T] = { 128,   0, 128, 255 },
    [S] = { 0,   255,   0, 255 },
    [Z] = { 255,   0,   0, 255 },
    [J] = { 0,     0, 255, 255 },
    [L] = { 255, 165,   0, 255 },
    [G] = { 192, 192, 192, 100 }   // Ghost
};

static const Color COLOR_WALL    = { 100, 100, 100, 255 };
static const Color COLOR_BORDER  = {  50,  50,  50, 255 };
static const Color COLOR_OUTLINE = { 255, 255, 255, 100 };

// Basic drawing utilities

static inline void set_color(SDL_Renderer *r, Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

static void draw_rect(SDL_Renderer *r, SDL_Rect *rect, Color fill, Color border) {
    set_color(r, fill);
    SDL_RenderFillRect(r, rect);

    set_color(r, border);
    SDL_RenderDrawRect(r, rect);
}

// Block rendering (from grid to screen)

static void draw_block_at(SDL_Renderer *r, int grid_x, int grid_y, Color color){
    SDL_Rect rect = {
        .x = grid_x,
        .y = grid_y,
        .w = BLOCK_SIZE,
        .h = BLOCK_SIZE
    };
    set_color(r, color);
    SDL_RenderFillRect(r, &rect);

    set_color(r, COLOR_OUTLINE);
    SDL_RenderDrawRect(r, &rect);
}

static void draw_block_grid(SDL_Renderer *r, int grid_x, int grid_y, Color color) {
    int screen_y = (grid_y - BUFFER_ZONE) * BLOCK_SIZE;

    if (screen_y < 0) { // Don't draw blocks hidden in buffer zone
        return;
    }
    draw_block_at(r, GAME_OFFSET_X + grid_x * BLOCK_SIZE, screen_y, color);
}

// On-board piece: grid coordinates, board offset + buffer-zone culling apply.
// Used for the active piece and the ghost piece.
static void draw_piece_shape_on_board(SDL_Renderer *r, TetrominoType type,
                            Rotation rotation, int grid_x,
                            int grid_y, Color color) {

    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (TETROMINOS[type][rotation][py][px]) {
                draw_block_grid(r, grid_x + px, grid_y + py, color);
            }
        }
    }
}

// Off-board piece: absolute pixel origin, no board offset, no culling.
// Used for the hold box and (later) the next-queue preview.
static void draw_piece_shape_at(SDL_Renderer *r, TetrominoType type,
                            Rotation rotation, int origin_x,
                            int origin_y, Color color) {

    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (TETROMINOS[type][rotation][py][px]) {
                draw_block_at(r, origin_x + px * BLOCK_SIZE, origin_y + py * BLOCK_SIZE, color);
            }
        }
    }
}


// Ui / menu

void draw_menu(struct Game *game) {
    SDL_Renderer *r = game->renderer;

    SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
    SDL_RenderFillRect(r, &game->btn_play);

    SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    SDL_RenderFillRect(r, &game->btn_leaderboard);

    SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
    SDL_RenderFillRect(r, &game->btn_options);
}

// Layout (walls, floor, hold box)

static void draw_walls(SDL_Rect block, SDL_Renderer *r, 
                        int left_wall_x, int right_wall_x){

    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        block.y = y * BLOCK_SIZE;

        block.x = left_wall_x;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);

        block.x = right_wall_x;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);
    }
}

static void draw_floor(SDL_Rect block, SDL_Renderer *r, int floor_y){

    block.y = floor_y;
    for (int x = -1; x <= BOARD_WIDTH; x++) {
        block.x = GAME_OFFSET_X + x * BLOCK_SIZE;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);
    }
}

static void draw_hold_box(struct Game *game){
    SDL_Renderer *r = game->renderer;
    SDL_Rect hold_box = {
        .x = BLOCK_SIZE / 2,
        .y = BLOCK_SIZE * 2,
        .w = BLOCK_SIZE * 4 + 1,
        .h = BLOCK_SIZE * 4 + 1
    };

    draw_rect(r, &hold_box,
              (Color){0, 0, 0, 255},
              (Color){255, 255, 255, 255});

    if(game->piece.held_piece != NONE){
        draw_piece_shape_at(r, game->piece.held_piece, ROT_0,
                            hold_box.x,
                            hold_box.y,
                            TETROMINO_COLORS[game->piece.held_piece]);
    }
}

void draw_layout(struct Game *game) {
    SDL_Renderer *r = game->renderer;
    SDL_Rect block = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

    const int left_wall_x  = GAME_OFFSET_X - BLOCK_SIZE;
    const int right_wall_x = GAME_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE;
    const int floor_y      = BOARD_HEIGHT * BLOCK_SIZE;

    // walls
    draw_walls(block, r, left_wall_x, right_wall_x);

    //floor
    draw_floor(block, r, floor_y);

    // Hold box
    draw_hold_box(game);
}

// Grid and pieces

void draw_grid(struct Game *game) {
    for (int y = 0; y < TOTAL_ROWS; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int cell = game->grid[y][x];
            if (cell != 0) {
                draw_block_grid(
                    game->renderer,
                    x,
                    y,
                    TETROMINO_COLORS[cell - 1]
                );
            }
        }
    }
}

void draw_tetro(SDL_Renderer *r, TetrominoType type,
                Rotation rotation,int grid_x,
                                int grid_y){

    draw_piece_shape_on_board(r, type, rotation, grid_x, grid_y, TETROMINO_COLORS[type]);
}

//Ghost piece

static int compute_ghost_y(struct Game *game) {
    int y = game->piece.y;
    while (!check_collision(game, game->piece.x, y + 1,  game->piece.rotation)) {
        y++;
    }
    return y;
}

void draw_ghost(struct Game *game) {
    SDL_Renderer *r = game->renderer;
    int ghost_y = compute_ghost_y(game);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    draw_piece_shape_on_board(r, game->piece.type, game->piece.rotation, game->piece.x, ghost_y, TETROMINO_COLORS[G]);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}