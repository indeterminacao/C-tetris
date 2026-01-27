#ifndef TABLE_H
#define TABLE_H

#include "../header_files/tetrominos.h"

typedef struct {
    int x, y;
} Point;

//Index order: [Actual state][wanted state][Test number]
extern const Point kicks_jlstz[4][4][5];
extern const Point kicks_i[4][4][5];

#endif