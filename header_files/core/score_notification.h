#ifndef CORE_SCORE_NOTIFICATION_H
#define CORE_SCORE_NOTIFICATION_H

#include "../core/score_state.h"

#define MAX_SCORE_NOTIFICATIONS 6
#define NOTIFICATION_DURATION_MS 2000

typedef enum {
    SCORE_NONE,
    SCORE_SINGLE, SCORE_DOUBLE, SCORE_TRIPLE, SCORE_TETRIS,
    SCORE_TSPIN_MINI, SCORE_TSPIN_ZERO,SCORE_TSPIN_SINGLE, SCORE_TSPIN_DOUBLE, SCORE_TSPIN_TRIPLE,
    SCORE_B2B, SCORE_COMBO, SCORE_PERFECT_CLEAR, SCORE_LEVEL_UP
} ScoreEventType;

/**
 * @brief Notification generated after a scoring event.
 */
typedef struct {
    ScoreEventType type;
    Uint32 points;
    Uint32 expires_at;
} ScoreNotification;

/**
 * @brief FIFO queue of active score notifications.
 */
typedef struct {
    ScoreNotification items[MAX_SCORE_NOTIFICATIONS];
    int count;
} ScoreNotificationQueue;

#endif