/*
Tesla's Autopilot logs every planned maneuver (lane change, brake, accelerate, etc.) as it executes them, 
so that if the safety driver hits override, the system can roll back the last few maneuvers to a known-safe state.
Maneuvers are stored in a fixed-size stack (array-based, no malloc). 
Implement push/pop, and a rollback function that undoes the last n maneuvers, printing each as it's undone.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 20

typedef enum {
    MANEUVER_LANE_CHANGE_LEFT,
    MANEUVER_LANE_CHANGE_RIGHT,
    MANEUVER_BRAKE,
    MANEUVER_ACCELERATE,
    MANEUVER_HOLD_LANE
} maneuver_t;

typedef struct {
    maneuver_t history[MAX_STACK_SIZE];
    int        top;     // index of next free slot; -1 or 0 when empty (your choice, state it)
} maneuver_stack_t;

// Pushes a new maneuver onto the stack. Returns false if stack is full.
bool push_maneuver(maneuver_stack_t *stack, maneuver_t m) {
    // TODO
}

// Pops the most recent maneuver off the stack. Returns false if empty.
bool pop_maneuver(maneuver_stack_t *stack, maneuver_t *out) {
    // TODO
}

// Rolls back the last n maneuvers (pops and prints each one).
// If the stack has fewer than n maneuvers, rolls back everything available.
void rollback(maneuver_stack_t *stack, int n) {
    // TODO
}

int main(void) {
    maneuver_stack_t stack = { .top = 0 };

    push_maneuver(&stack, MANEUVER_HOLD_LANE);
    push_maneuver(&stack, MANEUVER_LANE_CHANGE_LEFT);
    push_maneuver(&stack, MANEUVER_ACCELERATE);
    push_maneuver(&stack, MANEUVER_BRAKE);

    printf("Override triggered, rolling back last 2 maneuvers:\n");
    rollback(&stack, 2);

    return 0;
}