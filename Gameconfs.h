#ifndef GAMECONFS_H
#define GAMECONFS_H

/**
 * @file Gameconfs.h
 * @brief Umbrella header. Pulls together the split-out definitions in
 * header_files/core/ (layout constants, timing constants, shared types,
 * and the main Game struct) so existing #include "Gameconfs.h" references
 * throughout the project keep working unchanged.
 */

#include <stdio.h>
#include <stdlib.h>

#include "header_files/core/layout.h"
#include "header_files/core/timing.h"
#include "header_files/core/game_types.h"
#include "header_files/core/game_state.h"

#endif