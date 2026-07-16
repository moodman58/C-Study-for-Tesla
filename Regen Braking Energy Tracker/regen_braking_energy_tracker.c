/*
When a Tesla driver lifts off the accelerator or brakes, the motor acts as a generator and recovers energy back into the battery. 
The car logs instantaneous regen power (in kW) at each control loop tick, and needs to:

Maintain a rolling average of the last WINDOW_SIZE power readings (for smoothing/display purposes)
Accumulate total energy recovered over the whole drive (in kWh), using each reading and the fixed tick interval

The code below is one engineer's attempt at implementing this. It compiles and runs, but there's a bug in it. Write your own test cases in main() to find and fix it.
Expected behavior of each function, for reference:

add_power_sample(tracker, power_kw): adds a new reading to the circular buffer (overwriting the oldest once full), 
and updates total energy recovered using power_kw * TICK_INTERVAL_HOURS.
get_average_power(tracker): returns the average of only the samples currently stored — if fewer than WINDOW_SIZE readings have been added so far, 
it should average just those, not divide by the full window size.
get_total_energy_kwh(tracker): returns cumulative energy recovered since the tracker was created.
*/

#include <stdio.h>

#define WINDOW_SIZE          5
#define TICK_INTERVAL_HOURS  (1.0f / 3600.0f)   // 1 tick = 1 second

typedef struct {
    float power_samples[WINDOW_SIZE];
    int   write_index;
    int   count;
    float running_sum;
    float total_energy_kwh;
} regen_tracker_t;

void init_tracker(regen_tracker_t *t) {
    t->write_index = 0;
    t->count = 0;
    t->running_sum = 0.0f;
    t->total_energy_kwh = 0.0f;
}

void add_power_sample(regen_tracker_t *t, float power_kw) {
    // Checking if full [1, 2, 3, 4, 5], seems to be working properly
    if (t->count == WINDOW_SIZE) {
        t->running_sum -= t->power_samples[t->write_index];
    }

    t->power_samples[t->write_index] = power_kw;
    t->running_sum += power_kw;
    t->write_index = (t->write_index + 1) % WINDOW_SIZE;

    if (t->count < WINDOW_SIZE) {
        t->count++;
    }

    t->total_energy_kwh += power_kw * TICK_INTERVAL_HOURS;
}

float get_average_power(const regen_tracker_t *t) {
    if (t->count == WINDOW_SIZE){
        return t->running_sum / WINDOW_SIZE;
    }
    else {
        return t-> running_sum / t->count;
    }
}

float get_total_energy_kwh(const regen_tracker_t *t) {
    return t->total_energy_kwh;
}

int main(void) {
    regen_tracker_t tracker;
    init_tracker(&tracker);

    add_power_sample(&tracker, 1.0);
    add_power_sample(&tracker, 2.0);
    add_power_sample(&tracker, 3.0);

    float avg_power = get_average_power(&tracker);
    printf("%f\n", avg_power);

    printf("%f\n", tracker.running_sum);

    for (int i = 0; i<5; ++i){
        printf("%f\n", tracker.power_samples[i]);
    }

    printf("%f\n", tracker.total_energy_kwh);

    return 0;
}