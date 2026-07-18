/*
 * ============================================================
 * Adaptive Cruise Control — Following Gap Controller
 * ============================================================
 *
 * States:
 *   SAFE             - gap and closing rate both within comfortable bounds
 *   CAUTION          - gap closing at a concerning rate, or distance
 *                       getting tight (not yet dangerous) -> light braking
 *   EMERGENCY_BRAKE  - gap is dangerously small AND still closing
 *                       -> hard braking
 *   FAULT            - radar has given no valid reading for too many
 *                       consecutive ticks
 *
 * --- Constants ---
 *   HISTORY_SIZE            = 4      (rolling window of distance readings)
 *   TICK_INTERVAL_SEC       = 0.5f   (time between radar readings)
 *
 *   SAFE_DISTANCE_M         = 30.0f  (>= this and not closing fast -> SAFE)
 *   CAUTION_DISTANCE_M      = 15.0f  (below this -> at least CAUTION)
 *   DANGER_DISTANCE_M       = 8.0f   (below this AND closing -> EMERGENCY)
 *
 *   CONCERNING_CLOSE_RATE   = -3.0f  (m/s; negative = closing. Rate more
 *                                     negative than this -> at least CAUTION,
 *                                     even if distance alone looks OK)
 *   DANGEROUS_CLOSE_RATE    = -6.0f  (m/s; more negative than this, combined
 *                                     with DANGER_DISTANCE_M -> EMERGENCY)
 *
 *   MAX_FAULT_TICKS         = 3      (consecutive invalid readings -> FAULT)
 *
 * --- What to implement ---
 *   1. record_reading()        - pushes a new distance reading into the
 *                                 rolling history (circular buffer). If
 *                                 `valid` is false, don't push a distance --
 *                                 instead increment a fault counter; a
 *                                 valid reading resets that counter to 0.
 *
 *   2. compute_closing_rate()  - using the OLDEST and NEWEST readings
 *                                 currently in the history (not just two
 *                                 adjacent samples -- use the full window
 *                                 for a smoother/less noisy estimate),
 *                                 compute rate of change in m/s.
 *                                 Return 0.0f if fewer than 2 readings
 *                                 are available yet.
 *
 *   3. process_tick()          - given a new (distance, valid) radar
 *                                 reading:
 *                                   - record it
 *                                   - if fault counter >= MAX_FAULT_TICKS,
 *                                     go to FAULT (sticky until a valid
 *                                     reading arrives again)
 *                                   - otherwise compute closing rate and
 *                                     current distance, and decide the
 *                                     new state using BOTH values together
 *                                     per the rules above
 *                                   - print state, distance, computed rate
 * ============================================================
 */

#include <stdio.h>
#include <stdbool.h>

#define HISTORY_SIZE       4
#define TICK_INTERVAL_SEC  0.5f

#define SAFE_DISTANCE_M       30.0f
#define CAUTION_DISTANCE_M    15.0f
#define DANGER_DISTANCE_M     8.0f

#define CONCERNING_CLOSE_RATE  -3.0f
#define DANGEROUS_CLOSE_RATE   -6.0f

#define MAX_FAULT_TICKS    3

typedef enum {
    SAFE,
    CAUTION,
    EMERGENCY_BRAKE,
    FAULT
} acc_state_t;

typedef struct {
    float distance_history[HISTORY_SIZE];
    int   write_index;
    int   count;             // readings currently stored (caps at HISTORY_SIZE)
    int   consecutive_fault_ticks;

    acc_state_t state;
} acc_controller_t;

const char *state_name(acc_state_t s) {
    static const char *names[] = {"SAFE", "CAUTION", "EMERGENCY_BRAKE", "FAULT"};
    return names[s];
}

// Pushes a new distance reading into the circular history buffer.
// If valid == false, do NOT push a distance -- just increment
// consecutive_fault_ticks. If valid == true, push the distance
// and reset consecutive_fault_ticks to 0.
void record_reading(acc_controller_t *c, float distance_m, bool valid) { 
    if (!valid){
      c->consecutive_fault_ticks++;
      return;
    }
    c->distance_history[c->write_index] = distance_m;
    c->consecutive_fault_ticks = 0;
    c->write_index = (c->write_index+1) % HISTORY_SIZE;
    if(c->count < HISTORY_SIZE){
      c->count++;
    }
    return;
}

// Computes closing rate (m/s) using the OLDEST and NEWEST samples
// currently stored in the history (full window, not adjacent samples).
// Negative = closing, positive = opening.
// Return 0.0f if fewer than 2 readings are available.
float compute_closing_rate(const acc_controller_t *c) {
    int oldest_idx;
    int newest_idx;
  
    if(c->count < 2){
      return 0.0f;
    }
    if (c->count < HISTORY_SIZE){
      oldest_idx = 0;
      newest_idx = c->write_index - 1;
      float old_distance = c->distance_history[oldest_idx];
      float new_distance = c->distance_history[newest_idx];
    
      float closing_rate = (new_distance - old_distance) / (c->count - 1 * TICK_INTERVAL_SEC);
      return closing_rate;
    }

    oldest_idx = c->write_index;
    if (c->write_index == 0){
      newest_idx = HISTORY_SIZE - 1;
    } else {
      newest_idx = c->write_index - 1;
    }
    float old_distance = c->distance_history[oldest_idx]; // [1, 2, 3, 4, 5] -> [2, 2, 3, 4, 5]
    float new_distance = c->distance_history[newest_idx];
    float closing_rate = (new_distance - old_distance) / (c->count * TICK_INTERVAL_SEC);
    return closing_rate;
    
    // TODO
    // Hint: you'll need to figure out which buffer slot holds the
    // oldest reading and which holds the newest, given write_index
    // and count -- this is the trickiest part of the whole problem.
}

// Processes one radar tick and updates c->state accordingly.
void process_tick(acc_controller_t *c, float distance_m, bool valid) {
    // TODO
}

int main(void) {
    acc_controller_t acc = {0};
    acc.state = SAFE;

    // Car ahead starts steady, then closes distance rapidly
    float distances[] = {35.0f, 34.5f, 34.0f, 33.8f, 25.0f, 18.0f, 10.0f, 6.0f};
    bool  valid_flags[] = {true, true, true, true, true, true, true, true};

    for (int i = 0; i < 8; i++) {
        process_tick(&acc, distances[i], valid_flags[i]);
    }

    // Simulate a radar fault
    process_tick(&acc, 0.0f, false);
    process_tick(&acc, 0.0f, false);
    process_tick(&acc, 0.0f, false);

    printf("\nFinal state: %s\n", state_name(acc.state));
    return 0;
}