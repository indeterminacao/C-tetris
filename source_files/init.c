#include "../header_files/init.h"

bool sdl_initializer(struct Game *game){
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;
    }

    game->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, 0);
    if(!(game->window)){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit(); 
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!(game->renderer)){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return true;
    }
    
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