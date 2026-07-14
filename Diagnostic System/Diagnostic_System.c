/*
Tesla's diagnostics system logs fault codes as they occur, but only needs to retain the most recent 5 faults (older ones can be overwritten). Faults come in one at a time from various car subsystems.
Implement a fixed-size circular buffer that:

Stores incoming fault codes, overwriting the oldest entry once full
Can print all currently stored faults in the order they occurred (oldest → newest)
*/

#include <stdio.h>
#include <stdbool.h>

#define LOG_CAPACITY 5

typedef enum {
    FAULT_NONE,
    FAULT_BATTERY_OVERTEMP,
    FAULT_MOTOR_OVERCURRENT,
    FAULT_SENSOR_DISCONNECT,
    FAULT_LOW_COOLANT,
    FAULT_BRAKE_PRESSURE_LOW
} fault_code_t;

typedef struct {
    fault_code_t buffer[LOG_CAPACITY];
    int head;       // index of oldest entry
    int count;      // how many valid entries currently stored (caps at LOG_CAPACITY)
} fault_log_t;

// Adds a new fault to the log. If full, overwrites the oldest entry.
void log_fault(fault_log_t *log, fault_code_t code) {
    // TODO
}

// Prints all currently stored faults, oldest to newest
void print_faults(const fault_log_t *log) {
    // TODO
}

int main(void) {
    fault_log_t log = { .head = 0, .count = 0 };

    log_fault(&log, FAULT_BATTERY_OVERTEMP);
    log_fault(&log, FAULT_SENSOR_DISCONNECT);
    log_fault(&log, FAULT_LOW_COOLANT);
    log_fault(&log, FAULT_MOTOR_OVERCURRENT);
    log_fault(&log, FAULT_BRAKE_PRESSURE_LOW);

    // This 6th fault should overwrite the oldest one (FAULT_BATTERY_OVERTEMP)
    log_fault(&log, FAULT_SENSOR_DISCONNECT);

    print_faults(&log);

    return 0;
}

