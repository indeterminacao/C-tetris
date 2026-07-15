#ifndef CORE_PHYSICS_CONFIG_H
#define CORE_PHYSICS_CONFIG_H

#include <SDL2/SDL.h>

typedef struct {
   Uint32 gravity_delay;         /**< ms per automatic fall */
   Uint32 lock_delay;        /**< Time (ms) before locking a piece */
   int max_lock_resets;   /**< Max times lock delay can reset before forced lock (Infinity Rule cap) */
} PhysicsConfig;

#endif