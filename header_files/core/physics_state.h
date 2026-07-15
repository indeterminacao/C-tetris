#ifndef CORE_PHYSICS_STATE_H
#define CORE_PHYSICS_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
 
   // --- Timing & Physics ---
typedef struct {
 //Gravity
   Uint32 gravity_timer;         /**< Last gravity step tick */
   Uint32 gravity_delay;         /**< ms per automatic fall */
   // --- Lock Delay Mechanics ---
   Uint32 lock_delay;        /**< Time (ms) before locking a piece */
   Uint32 lock_timer;        /**< Timer accumulator for lock delay */
   bool is_locking;          /**< Is the piece currently touching the ground?*/ 
   int lock_resets;          /**< Counter: How many times lock delay was reset (Infinity Rule) */
} PhysicsState;

#endif
