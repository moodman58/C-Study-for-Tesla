/*
The cabin has many sensors, each identified by a sensor_id (0–999). Faults stream in as (sensor_id, severity) events, arbitrarily and out of order. 
You need to track, per sensor, the count of faults and the highest severity seen — without scanning a giant list every time.
Rather than a full hash map, use a fixed-size direct-lookup array (since IDs are bounded 0–999) where each slot holds a struct — 
this is a common embedded-systems substitute for a hash table when key ranges are known and small.
*/

#include <stdio.h>
#include <stdbool.h>

#define MAX_SENSOR_ID 1000

typedef struct {
    int  fault_count;
    int  max_severity;   // 0 = none seen yet
    bool seen;           // has this sensor ever reported a fault?
} sensor_stats_t;

typedef struct {
    sensor_stats_t table[MAX_SENSOR_ID];
} fault_registry_t;

// Records a new fault event for a given sensor
void record_fault(fault_registry_t *reg, int sensor_id, int severity) {
    // TODO
}

// Returns the sensor_id with the highest fault_count across the whole registry.
// If no faults have been recorded at all, return -1.
int find_most_frequent_faulty_sensor(const fault_registry_t *reg) {
    // TODO
}

// Prints stats only for sensors that have reported at least one fault
void print_active_faults(const fault_registry_t *reg) {
    // TODO
}

int main(void) {
    fault_registry_t reg = {0};

    record_fault(&reg, 42, 3);
    record_fault(&reg, 42, 5);
    record_fault(&reg, 17, 2);
    record_fault(&reg, 42, 1);
    record_fault(&reg, 900, 4);

    print_active_faults(&reg);

    int worst_id = find_most_frequent_faulty_sensor(&reg);
    printf("\nMost frequently faulting sensor: %d\n", worst_id);

    return 0;
}