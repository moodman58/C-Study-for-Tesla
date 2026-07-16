/*
Tesla's Autopilot has a state machine with 4 states:

DISENGAGED — driver has full manual control
ENGAGED — Autopilot active, driver attentive
WARNING — Autopilot active, but driver inattentiveness detected (hands off wheel / eyes off road) — a countdown begins
EMERGENCY_STOP — driver failed to respond during WARNING before the countdown expired; car initiates a controlled stop

Unlike a simple event-driven state machine, WARNING has a timer: if the driver doesn't respond (send DRIVER_RESPONDED) within WARNING_TIMEOUT_TICKS (5 ticks) 
of entering WARNING, the system automatically escalates to EMERGENCY_STOP — even with no new event.
Valid event-driven transitions:

 * Valid transitions:
 *
 * Current State     Event                   Next State
 * ---------------------------------------------------------------
 * DISENGAGED        AP_BUTTON_PRESS      -> ENGAGED
 * ENGAGED           AP_BUTTON_PRESS      -> DISENGAGED
 * ENGAGED           INATTENTION_DETECTED -> WARNING
 * WARNING           DRIVER_RESPONDED     -> ENGAGED
 * WARNING           AP_BUTTON_PRESS      -> DISENGAGED
 * EMERGENCY_STOP    AP_BUTTON_PRESS      -> DISENGAGED
 * any state         OBSTACLE_DETECTED    -> EMERGENCY_STOP
 *
 * Timer-driven (not event-driven):
 * WARNING -> EMERGENCY_STOP  if WARNING_TIMEOUT_TICKS pass with
 *                             no DRIVER_RESPONDED event

Plus the timer-driven transition: WARNING → EMERGENCY_STOP if WARNING_TIMEOUT_TICKS pass with no DRIVER_RESPONDED event.
The system processes one event (or NO_EVENT) per tick. Implement the state machine using a 2D transition table (state x event -> state) for the event-driven part, 
plus separate timer logic layered on top.

*/

#include <stdio.h>
#include <stdbool.h>

#define NUM_STATES            4
#define NUM_EVENTS            5
#define WARNING_TIMEOUT_TICKS 5
#define INVALID_TRANSITION    -1

typedef enum {
    DISENGAGED,
    ENGAGED,
    WARNING,
    EMERGENCY_STOP
} ap_state_t;

typedef enum {
    NO_EVENT,
    AP_BUTTON_PRESS,
    INATTENTION_DETECTED,
    DRIVER_RESPONDED,
    OBSTACLE_DETECTED
} ap_event_t;

typedef struct {
    ap_state_t current_state;
    int        warning_timer;   // ticks spent in WARNING so far; 0 when not in WARNING
} autopilot_t;

const char *state_name(ap_state_t s) {
    static const char *names[NUM_STATES] = {"DISENGAGED", "ENGAGED", "WARNING", "EMERGENCY_STOP"};
    return names[s];
}

const char *event_name(ap_event_t e) {
    static const char *names[NUM_EVENTS] = {
        "NO_EVENT", "AP_BUTTON_PRESS", "INATTENTION_DETECTED",
        "DRIVER_RESPONDED", "OBSTACLE_DETECTED"
    };
    return names[e];
}

ap_state_t next_transition_state(ap_event_t event, ap_state_t current_state){
    switch(current_state){
        
        case DISENGAGED:
            switch(event){
                case AP_BUTTON_PRESS:
                    return ENGAGED;
                case OBSTACLE_DETECTED:
                    return EMERGENCY_STOP;
            }
            break;
        case ENGAGED:
            switch(event){
                case AP_BUTTON_PRESS:
                    return DISENGAGED;
                case INATTENTION_DETECTED:
                    return WARNING;
                case OBSTACLE_DETECTED:
                    return EMERGENCY_STOP;
            }
            break;
        case WARNING:
            switch(event){
                case DRIVER_RESPONDED:
                    return ENGAGED;
                case AP_BUTTON_PRESS:
                    return DISENGAGED;
                case OBSTACLE_DETECTED:
                    return EMERGENCY_STOP;
            }
            break;
        case EMERGENCY_STOP:
            switch(event){
                case AP_BUTTON_PRESS:
                    return DISENGAGED;               
            }
    }
    return current_state;
}

// TODO: Build this transition table such that transition_table[state][event] [["AP_BUTTON_PRESS"][][][]]
// gives the next state, or INVALID_TRANSITION if that event is not valid
// for that state. Remember OBSTACLE_DETECTED is valid from ANY state.
// (Hint: you can build this at startup in a function, or as a static
// initializer -- your choice, just make sure it's correct.)
int transition_table[NUM_STATES][NUM_EVENTS];

void init_transition_table(void) {
    for (ap_state_t state = 0; state<NUM_STATES; ++state){
        for (ap_event_t event = 0; event<NUM_EVENTS; ++event){
            transition_table[state][event] = next_transition_state(event, state);
        }
    }
}



// Processes a single tick: given an event (may be NO_EVENT), updates
// ap->current_state using the transition table, AND handles the
// WARNING timer logic:
//   - if entering WARNING this tick, reset warning_timer to 0
//   - if already in WARNING and no valid transition occurred this tick,
//     increment warning_timer; if it reaches WARNING_TIMEOUT_TICKS,
//     force a transition to EMERGENCY_STOP
//   - if leaving WARNING (to any other state), reset warning_timer to 0
// Should print what happened this tick (transition taken, ignored event,
// or timeout-triggered escalation).
void process_tick(autopilot_t *ap, ap_event_t event) {
    if (ap->warning_timer < 5){
        ap_state_t transition_state = transition_table[ap->current_state][event];
        if (transition_state != ap->current_state){
            printf("transition state: %s\n", state_name(transition_state));
        } else if (transition_state == ap->current_state) {
            printf("Ignored Event\n");
        }
        if (ap->current_state == WARNING && transition_state == WARNING){
            ap->warning_timer++;
        }
        ap->current_state = transition_state;
        return;
    }
    ap->current_state = EMERGENCY_STOP;
    ap->warning_timer = 0;
    printf("Timeout Triggered Escalation\n");
}

int main(void) {
    init_transition_table();

    autopilot_t ap = { .current_state = DISENGAGED, .warning_timer = 0 };

    ap_event_t events[] = {
        AP_BUTTON_PRESS,        // -> ENGAGED
        INATTENTION_DETECTED,   // -> WARNING
        NO_EVENT,                // still WARNING, timer ticks up
        NO_EVENT,
        NO_EVENT,
        NO_EVENT,
        NO_EVENT,
        NO_EVENT                     // should hit timeout here -> EMERGENCY_STOP
    };


    // for (ap_state_t state = 0; state<NUM_STATES; ++state){
    //     for (ap_event_t event = 0; event<NUM_EVENTS; ++event){
    //         ap_state_t table_transition = transition_table[state][event];
    //         printf("state: %s events: %s transition_state: %s \n", state_name(state), event_name(event), state_name(table_transition));
    //     }
    // }

    int num_events = sizeof(events) / sizeof(events[0]);

    for (int i = 0; i < num_events; i++) {
        process_tick(&ap, events[i]);
    }

    printf("\nFinal state: %s\n", state_name(ap.current_state));
    return 0;
}