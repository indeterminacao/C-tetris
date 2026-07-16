#include "../header_files/logic.h"
#include "../header_files/table.h"

static bool is_blocked(struct Game *game, int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH) return true; 
    if (y >= TOTAL_ROWS) return true;        
    if (y < 0) return false;                    
    return game->grid[y][x] != 0;
}

bool check_collision(struct Game *game, int grid_x, int grid_y, Rotation rot) {
    for(int py = 0; py < 4; py++){
        for(int px = 0; px < 4; px++){
            if(TETROMINOS[game->piece.type][rot][py][px] != 0){
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

/** @brief Returns the next piece from a shuffled 7-bag.
 * Guarantees every tetromino type appears exactly once every 7 spawns,
 * avoiding the long droughts or streaks that plain rand() % 7 can produce.
 */
static TetrominoType next_bag_piece(void) {
    static TetrominoType bag[TETROMINO_C];
    static int bag_index = TETROMINO_C; 

    if (bag_index >= TETROMINO_C) {
        for (int i = 0; i < TETROMINO_C; i++) {
            bag[i] = (TetrominoType)i;
        }
        for (int i = TETROMINO_C - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            TetrominoType tmp = bag[i];
            bag[i] = bag[j];
            bag[j] = tmp;
        }
        bag_index = 0;
    }

    return bag[bag_index++];
}

static void initialize_piece_state(struct Game *game){
    game->piece.rotation = ROT_0; 
    game->piece.x = (BOARD_WIDTH / 2) - 2; 
    game->piece.y = 0; 
    game->physics.lock_resets = game->physics_config.max_lock_resets;
    game->piece.active = true;
    game->physics.gravity_timer = game->current_tick;
    game->input.das_timer = 0;
    game->input.arr_timer = 0;
    game->input.soft_drop_timer = 0;
    game->piece.last_move_was_rotate = false; 
    game->input.move_dir = MOVE_NONE;
    game->input.soft_dropping = false;
}

static void check_spawn_collision(struct Game *game){
    if (check_collision(game, game->piece.x, game->piece.y, game->piece.rotation)) {
        printf("GAME OVER\n");
        game->piece.active = false;
    }
}

static void activate_current_piece(struct Game *game){
    initialize_piece_state(game);
    check_spawn_collision(game);
}

void spawn_piece(struct Game *game) {
    game->piece.type = next_bag_piece(); 
    activate_current_piece(game);
}

void spin(struct Game *game, int direction) {
    Rotation current_rot = game->piece.rotation;
    Rotation next_rot;
    
    if (direction == 1) {
        next_rot = (current_rot + 1) % 4;
    } else if (direction == -1) {
        next_rot = (current_rot + 3) % 4;
    } else {
        return; 
    }

    const Point (*kicks)[4][4][5];

    if(game->piece.type == O) {
        return;
    } else if (game->piece.type == I) {
        kicks = &kicks_i;
    } else {
        kicks = &kicks_jlstz;
    }

    for (int i = 0; i < 5; i++) {
        Point test = (*kicks)[current_rot][next_rot][i];
        int new_x = game->piece.x + test.x;
        int new_y = game->piece.y + test.y;

        if (!check_collision(game, new_x, new_y, next_rot)) {
            game->piece.x = new_x;
            game->piece.y = new_y;
            game->piece.rotation = next_rot;
            game->piece.last_move_was_rotate = true; 
            return; 
        }
    }
}

void hold_piece(struct Game *game){
    if(game->piece.hold_used || !game->piece.active){
        return;
    }
    if(game->piece.held_piece == NONE){
        game->piece.held_piece = game->piece.type;
        spawn_piece(game);
    } else{
        TetrominoType tmp = game->piece.held_piece;
        game->piece.held_piece = game->piece.type;
        game->piece.type = tmp;
        activate_current_piece(game);
    }
    if(game->piece.active){
        game->piece.hold_used = true;
    }
}

void hard_drop(struct Game *game){
    if (!game->piece.active) {
        return;
    }
    game->piece.last_move_was_rotate = false; 

    while(!check_collision(game, game->piece.x, game->piece.y + 1, game->piece.rotation)){
        game->piece.y += 1;
    }
    resolve_lock(game);

    game->physics.gravity_timer = game->current_tick;
}

void lock_piece(struct Game *game) {
    for(int py = 0; py < 4; py++){
        for(int px = 0; px < 4; px++){
            if(TETROMINOS[game->piece.type][game->piece.rotation][py][px] != 0){
                int board_x = game->piece.x + px;
                int board_y = game->piece.y + py;

                if(board_y >= 0 && board_y < TOTAL_ROWS && board_x >= 0 && board_x < BOARD_WIDTH){
                    game->grid[board_y][board_x] = (uint8_t)(game->piece.type + 1); 
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

TSpinType detect_t_spin(struct Game *game) {
    if (game->piece.type != T || !game->piece.last_move_was_rotate) {
        return TSPIN_NONE;
    }

    int center_x = game->piece.x + 1;
    int center_y = game->piece.y + 1;

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
    TSpinType tspin = detect_t_spin(game);

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

    game->piece.active = false;
    game->piece.last_move_was_rotate = false;
    game->piece.hold_used = false;
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
        if (game->score.back_to_back) {
            points = (points * 3) / 2; 
            printf("BACK-TO-BACK! \n"); 
        }
        game->score.back_to_back = true; 
    } 
    else if (lines_cleared > 0) {
        game->score.back_to_back = false; 
    }
    game->score.points += points * game->score.level;
    printf("SCORE: %u\n", game->score.points);

    game->score.total_lines += lines_cleared;
    if (game->score.total_lines >= game->score.level * 10) {
        game->score.level++;
        printf("LEVEL UP! %u\n", game->score.level);
    }
}

void extend_lock_delay(struct Game *game) {
    if (!check_collision(game,
            game->piece.x,
            game->piece.y + 1,
            game->piece.rotation)) {
        return;
    }

    if (game->physics.lock_resets > 0) {
        game->physics.lock_resets--;
        game->physics.lock_timer = game->current_tick;
    }
}