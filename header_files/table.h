#ifndef TABLE_H
#define TABLE_H

#include "../header_files/tetrominos.h"

/** @brief Represents a point in 2D space (necessary to handle kick data) */
typedef struct {
    int x, y;
} Point;

//Index order: [Actual state][wanted state][Test number]
// Two tables are needed because I and JLTSTZ have different kick tables
extern const Point kicks_jlstz[4][4][5];
extern const Point kicks_i[4][4][5];

#endif