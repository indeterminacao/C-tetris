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

static void draw_block(SDL_Renderer *r, int grid_x, int grid_y, Color color) {
    int screen_y = (grid_y - BUFFER_ZONE) * BLOCK_SIZE;

    if (screen_y < 0) { // Don't draw blocks hidden in buffer zone
        return;
    }

    SDL_Rect rect = {
        .x = GAME_OFFSET_X + grid_x * BLOCK_SIZE,
        .y = screen_y,
        .w = BLOCK_SIZE,
        .h = BLOCK_SIZE
    };

    set_color(r, color);
    SDL_RenderFillRect(r, &rect);

    set_color(r, COLOR_OUTLINE);
    SDL_RenderDrawRect(r, &rect);
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

void draw_layout(struct Game *game) {
    SDL_Renderer *r = game->renderer;
    SDL_Rect block = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

    const int left_wall_x  = GAME_OFFSET_X - BLOCK_SIZE;
    const int right_wall_x = GAME_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE;
    const int floor_y      = BOARD_HEIGHT * BLOCK_SIZE;

    // walls
    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        block.y = y * BLOCK_SIZE;

        block.x = left_wall_x;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);

        block.x = right_wall_x;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);
    }

    //floor
    block.y = floor_y;
    for (int x = -1; x <= BOARD_WIDTH; x++) {
        block.x = GAME_OFFSET_X + x * BLOCK_SIZE;
        draw_rect(r, &block, COLOR_WALL, COLOR_BORDER);
    }

    // Hold box
    SDL_Rect hold_box = {
        .x = BLOCK_SIZE,
        .y = BLOCK_SIZE * 2,
        .w = BLOCK_SIZE * 4,
        .h = BLOCK_SIZE * 4
    };

    draw_rect(r, &hold_box,
              (Color){0, 0, 0, 255},
              (Color){255, 255, 255, 255});
}

// Grid and pieces

void draw_grid(struct Game *game) {
    for (int y = 0; y < TOTAL_ROWS; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int cell = game->grid[y][x];
            if (cell != 0) {
                draw_block(
                    game->renderer,
                    x,
                    y,
                    TETROMINO_COLORS[cell - 1]
                );
            }
        }
    }
}

void draw_tetro(SDL_Renderer *r,
                TetrominoType type,
                Rotation rotation,
                int grid_x,
                int grid_y)
{
    Color color = TETROMINO_COLORS[type];

    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (TETROMINOS[type][rotation][py][px]) {
                draw_block(
                    r,
                    grid_x + px,
                    grid_y + py,
                    color
                );
            }
        }
    }
}

//Ghost piece

static int compute_ghost_y(struct Game *game) {
    int y = game->currentY;
    while (!check_collision(game, game->currentX, y + 1, game->currentRotation)) {
        y++;
    }
    return y;
}

void draw_ghost(struct Game *game) {
    SDL_Renderer *r = game->renderer;
    int ghost_y = compute_ghost_y(game);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    Color ghost_color = TETROMINO_COLORS[G];

    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (TETROMINOS[game->currentType][game->currentRotation][py][px]) {
                draw_block(
                    r,
                    game->currentX + px,
                    ghost_y + py,
                    ghost_color
                );
            }
        }
    }

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}