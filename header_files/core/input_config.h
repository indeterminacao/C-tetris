#ifndef CORE_INPUT_CONFIG_H
#define CORE_INPUT_CONFIG_H

#include <SDL2/SDL.h>

typedef struct {
    Uint32 das_delay;
    Uint32 arr_delay;
    Uint32 soft_drop_arr;
} InputConfig;

#endif