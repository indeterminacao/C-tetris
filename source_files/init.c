#include "../Gameconfs.h"
#include "../header_files/init.h"

static bool initialize_sdl(void){
    if (!SDL_Init(SDL_INIT_EVERYTHING)) return false;

    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return true;
}

static bool create_window(struct Game *game){
    game->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, 0);
    if(game->window) return false;

    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit(); 
    return true;
}

static bool create_renderer(struct Game *game){
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(game->renderer) return false;

    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    return true;
}

bool sdl_initializer(struct Game *game){
    if (initialize_sdl()) return true;
    if (create_window(game)) return true;
    if (create_renderer(game)) return true;
    
    return false;
}

void cleanup(struct Game *game){
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = NULL; 
    }
    if (game->window) {
        SDL_DestroyWindow(game->window);
        game->window = NULL;
    }
    SDL_Quit();
}