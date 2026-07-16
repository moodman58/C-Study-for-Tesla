/*
Tesla's charge port door is controlled by a state machine with 4 states:

CLOSED — default resting state
OPENING — door is actuating open
OPEN — door fully open, ready for cable insertion
CHARGING — cable inserted, actively charging

Transitions are driven by discrete events (button press, cable inserted, cable removed, charge complete, obstruction detected). 
Not every event is valid in every state — invalid events for the current state should be ignored (log it, but don't change state).
Valid transitions:

 * c   + BUTTON_PRESS         -> OPENING
 * OPENING  + DOOR_ACTUATED        -> OPEN
 * OPEN     + CABLE_INSERTED       -> CHARGING
 * OPEN     + BUTTON_PRESS         -> CLOSED
 * CHARGING + CABLE_REMOVED        -> OPEN
 * CHARGING + CHARGE_COMPLETE      -> OPEN
 * any      + OBSTRUCTION_DETECTED -> CLOSED

Implement the state machine to process a queue of incoming events in order, printing each transition (or ignored event) as it's processed.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAX_EVENTS 20

typedef enum {
    CLOSED,
    OPENING,
    OPEN,
    CHARGING,
    NUM_STATES
} port_state_t;

typedef enum {
    BUTTON_PRESS,
    DOOR_ACTUATED,
    CABLE_INSERTED,
    CABLE_REMOVED,
    CHARGE_COMPLETE,
    OBSTRUCTION_DETECTED,
    NUM_EVENTS
} port_event_t;

typedef struct {
    port_event_t queue[MAX_EVENTS];
    int          count;
} event_queue_t;

const char *state_name(port_state_t s) {
    static const char *names[NUM_STATES] = {"CLOSED", "OPENING", "OPEN", "CHARGING"};
    return names[s];
}

const char *event_name(port_event_t e) {
    static const char *names[NUM_EVENTS] = {
        "BUTTON_PRESS", "DOOR_ACTUATED", "CABLE_INSERTED",
        "CABLE_REMOVED", "CHARGE_COMPLETE", "OBSTRUCTION_DETECTED"
    };
    return names[e];
}

// Given the current state and an incoming event, returns the next state.
// If the event is invalid for the current state, returns current_state unchanged.
// Sets *transitioned to true/false accordingly.
port_state_t get_next_state(port_state_t current_state, port_event_t event, bool *transitioned) {
    switch(current_state){
        case CLOSED:
            if(event == BUTTON_PRESS){
                *transitioned = true;
                return OPENING;
            }
            break;
        case OPENING:
            if(event == DOOR_ACTUATED){
                *transitioned = true;
                return OPEN;
            }
            if(event == OBSTRUCTION_DETECTED){
                *transitioned = true;
                return CLOSED;
            }
            break;
        case OPEN:
            if(event == CABLE_INSERTED){
                *transitioned = true;
                return CHARGING;
            }
            if(event == BUTTON_PRESS){
                *transitioned = true;
                return CLOSED;
            }
            if(event == OBSTRUCTION_DETECTED){
                *transitioned = true;
                return CLOSED;
            }
            break;
        case CHARGING:
            if(event == CABLE_REMOVED || event == CHARGE_COMPLETE){
                *transitioned = true;
                return OPEN;
            }
            else if(event == OBSTRUCTION_DETECTED){
                *transitioned = true;
                return CLOSED;
            }
            else{
                *transitioned = false;
                return current_state;
            }
            break;
    }
    *transitioned = false;
    return current_state;
    // TODO
}

// Processes every event in the queue, in order, updating state and
// printing either a transition or an "ignored" message for each.
void process_events(port_state_t *state, const event_queue_t *events) {
    port_event_t current_event;
    port_state_t current_state = *state;
    bool transitioned;
    for(int i=0; i<events->count; i++){
        current_event = events->queue[i];
        printf("event: %s\n", event_name(current_event));
        current_state = get_next_state(current_state, current_event, &transitioned);
        if(transitioned){
            printf("transistioned state: %s\n", state_name(current_state));
        }
        else{
            printf("ignored\n");
        }
    }
    // TODO
}

int main(void) {
    port_state_t state = CLOSED;

    event_queue_t events = {
        .queue = {
            BUTTON_PRESS,
            DOOR_ACTUATED,
            CABLE_INSERTED,
            CABLE_REMOVED,
            BUTTON_PRESS,        // invalid here (state is OPEN, but wait - this one IS valid)
            OBSTRUCTION_DETECTED // valid from any state
        },
        .count = 6
    };

    process_events(&state, &events);

    printf("\nFinal state: %s\n", state_name(state));
    return 0;
}