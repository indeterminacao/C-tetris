#ifndef CORE_LAYOUT_H
#define CORE_LAYOUT_H

/** @brief Application title used for the window */
#define TITLE "C-tetris"

/** @name Layout constants
 * @brief Dimensions calculated in pixels based on block size
 */
/** @{ */
#define BLOCK_SIZE 30             /**< Size (px) of one block */
#define WALL_THICKNESS 1          /**< Thickness of side walls in blocks */

#define HOLD_PANEL_WIDTH 6        /**< Width (in blocks) of the hold panel */
#define BOARD_WIDTH 10            /**< Visible board width (in blocks) */
#define RIGHT_PANEL_WIDTH 6       /**< Right-side UI panel width */

#define VISIBLE_HEIGHT 20         /**< Number of visible rows shown to the player */
#define BUFFER_ZONE 2             /**< Invisible buffer rows above the visible board */
#define BOARD_HEIGHT VISIBLE_HEIGHT          /**< Visible board height (duplicate of VISIBLE_HEIGHT) */
#define TOTAL_ROWS (VISIBLE_HEIGHT + BUFFER_ZONE)

// Calculated Screen Dimensions
#define SCREEN_WIDTH (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH + WALL_THICKNESS + BOARD_WIDTH + WALL_THICKNESS + RIGHT_PANEL_WIDTH))
#define SCREEN_HEIGHT (BLOCK_SIZE * (VISIBLE_HEIGHT + WALL_THICKNESS))
#define GAME_OFFSET_X (BLOCK_SIZE * (WALL_THICKNESS + HOLD_PANEL_WIDTH))
/** @} */

#endif