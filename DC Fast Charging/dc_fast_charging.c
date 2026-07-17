/*
 * ============================================================
 * DC Fast Charging Controller — Precharge / CC / CV State Machine
 * ============================================================
 *
 * States:
 *   IDLE              - no session active
 *   PRECHARGE         - small test current verifies contact resistance
 *                        is safe (P = I^2 * R) before full power is applied
 *   CONSTANT_CURRENT  - fixed current applied; voltage ramps up each tick
 *   CONSTANT_VOLTAGE  - voltage held fixed; current decays each tick
 *   COMPLETE          - current has tapered below cutoff; done
 *   FAULT             - entered from ANY state on safety violation or
 *                        precharge timeout; terminal until RESET event
 *
 * --- Constants / thresholds ---
 *   PRECHARGE_TEST_CURRENT_A   = 1.0f    (small test current during precharge)
 *   MAX_PRECHARGE_DISSIPATION_W = 50.0f  (I^2 * R must stay below this)
 *   PRECHARGE_STABLE_TICKS_REQUIRED = 3  (consecutive safe ticks needed)
 *   PRECHARGE_TIMEOUT_TICKS    = 8       (fault if not stable by then)
 *
 *   CC_CURRENT_A               = 250.0f  (fixed current during CC phase)
 *   VOLTAGE_RAMP_PER_TICK_V    = 20.0f   (voltage rise per tick during CC)
 *   TARGET_VOLTAGE_V           = 400.0f  (CC -> CV transition point)
 *
 *   CV_CURRENT_DECAY_FACTOR    = 0.7f    (current *= this each CV tick)
 *   CURRENT_CUTOFF_A           = 5.0f    (below this in CV -> COMPLETE)
 *
 *   MAX_SAFE_CURRENT_A         = 300.0f  (overcurrent fault threshold, any state)
 *   MAX_SAFE_VOLTAGE_V         = 450.0f  (overvoltage fault threshold, any state)
 *
 * --- What to implement ---
 *   1. compute_dissipation_w()   - P = I^2 * R
 *   2. exceeds_safety_limits()   - checks current/voltage against the
 *                                  MAX_SAFE_* thresholds (any state)
 *   3. process_tick()            - the core state machine + physics sim:
 *        - runs different behavior depending on current state
 *        - updates voltage/current/timers as described above
 *        - checks safety limits FIRST, every tick, regardless of state
 *        - transitions states according to the rules above
 *        - prints what happened this tick (state, voltage, current,
 *          any transition taken)
 *   4. reset_charger()           - returns charger to IDLE, clears
 *                                  all counters/timers/measurements
 * ============================================================
 */

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    IDLE,
    PRECHARGE,
    CONSTANT_CURRENT,
    CONSTANT_VOLTAGE,
    COMPLETE,
    FAULT
} charge_state_t;

typedef enum {
    START_CHARGE,
    TICK,
    RESET
} charge_event_t;

typedef struct {
    charge_state_t state;

    float voltage;              // current pack voltage
    float current;              // current charging current
    float contact_resistance;   // ohms, measured/simulated during precharge

    int   precharge_stable_count;  // consecutive safe precharge ticks so far
    int   precharge_tick_count;    // total ticks spent in precharge so far

    char  last_fault_reason[64];
} charger_t;

const char *state_name(charge_state_t s) {
    static const char *names[] = {
        "IDLE", "PRECHARGE", "CONSTANT_CURRENT",
        "CONSTANT_VOLTAGE", "COMPLETE", "FAULT"
    };
    return names[s];
}

// P = I^2 * R
float compute_dissipation_w(float current, float resistance) {
    // TODO
}

// Returns true if current or voltage exceeds the MAX_SAFE_* thresholds
bool exceeds_safety_limits(float current, float voltage) {
    // TODO
}

void reset_charger(charger_t *c) {
    // TODO
}

// Processes one tick of the charging session.
// `contact_resistance_reading` is only meaningful during PRECHARGE
// (simulates a fresh resistance measurement each precharge tick).
void process_tick(charger_t *c, charge_event_t event, float contact_resistance_reading) {
    // TODO
}

int main(void) {
    charger_t charger;
    reset_charger(&charger);

    process_tick(&charger, START_CHARGE, 0.0f);

    // Precharge: resistance readings for each tick (simulate a connector
    // that stabilizes after a couple of noisy readings)
    float precharge_readings[] = {0.15f, 0.08f, 0.05f, 0.05f, 0.05f};
    for (int i = 0; i < 5; i++) {
        process_tick(&charger, TICK, precharge_readings[i]);
    }

    // CC phase ticks (resistance reading irrelevant here, pass 0)
    for (int i = 0; i < 12; i++) {
        process_tick(&charger, TICK, 0.0f);
    }

    // CV phase ticks
    for (int i = 0; i < 15; i++) {
        process_tick(&charger, TICK, 0.0f);
    }

    printf("\nFinal state: %s\n", state_name(charger.state));
    return 0;
}