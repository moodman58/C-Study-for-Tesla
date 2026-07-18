/*
 * ============================================================
 * Vehicle Fault Flag Register
 * ============================================================
 *
 * Tesla's diagnostics system packs multiple fault flags into a single
 * uint16_t bitmask, where each bit position represents a different fault:
 *
 *   Bit 0 (0x0001) - FAULT_BATTERY_OVERTEMP
 *   Bit 1 (0x0002) - FAULT_MOTOR_OVERCURRENT
 *   Bit 2 (0x0004) - FAULT_LOW_COOLANT
 *   Bit 3 (0x0008) - FAULT_BRAKE_PRESSURE_LOW
 *   Bit 4 (0x0010) - FAULT_SENSOR_DISCONNECT
 *
 * Implement the functions below using bitwise operators
 * (&, |, ^, ~, <<, >>) -- no arrays, no if/else chains checking
 * each fault individually by name.
 *
 * --- What to implement ---
 *   1. set_fault()      - sets (turns ON) a given fault bit
 *   2. clear_fault()    - clears (turns OFF) a given fault bit
 *   3. is_fault_set()   - returns true if a given fault bit is set
 *   4. toggle_fault()   - flips a given fault bit (on->off, off->on)
 *   5. count_active_faults() - returns how many fault bits are
 *                              currently set (hint: think about how
 *                              to check each bit in a loop, or look
 *                              up a bit-counting trick)
 * ============================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FAULT_BATTERY_OVERTEMP    (1 << 0)   // 0x0001
#define FAULT_MOTOR_OVERCURRENT   (1 << 1)   // 0x0002
#define FAULT_LOW_COOLANT         (1 << 2)   // 0x0004
#define FAULT_BRAKE_PRESSURE_LOW  (1 << 3)   // 0x0008
#define FAULT_SENSOR_DISCONNECT   (1 << 4)   // 0x0010

// Sets (turns ON) the given fault bit in the register
uint16_t set_fault(uint16_t register_val, uint16_t fault_bit) {
    register_val = register_val | fault_bit;
    return register_val;
}

// Clears (turns OFF) the given fault bit in the register
uint16_t clear_fault(uint16_t register_val, uint16_t fault_bit) {
    register_val = (!(fault_bit) & register_val);
}

// Returns true if the given fault bit is currently set
bool is_fault_set(uint16_t register_val, uint16_t fault_bit) {
    // TODO
}

// Flips the given fault bit (on -> off, off -> on)
uint16_t toggle_fault(uint16_t register_val, uint16_t fault_bit) {
    // TODO
}

// Returns the count of currently active (set) fault bits in the register
int count_active_faults(uint16_t register_val) {
    // TODO
}

void print_register(uint16_t register_val) {
    printf("Register: 0x%04X\n", register_val);
}

int main(void) {
    uint16_t reg = 0;

    reg = set_fault(reg, FAULT_MOTOR_OVERCURRENT);
    printf("%d\n", reg);
    reg = set_fault(reg, FAULT_LOW_COOLANT);
    printf("%d\n", reg);
    print_register(reg);   // expect 0x0006

    printf("Motor overcurrent active? %s\n",
           is_fault_set(reg, FAULT_MOTOR_OVERCURRENT) ? "yes" : "no");
    printf("Battery overtemp active? %s\n",
           is_fault_set(reg, FAULT_BATTERY_OVERTEMP) ? "yes" : "no");

    reg = clear_fault(reg, FAULT_MOTOR_OVERCURRENT);
    print_register(reg);   // expect 0x0004

    reg = toggle_fault(reg, FAULT_SENSOR_DISCONNECT);
    print_register(reg);   // expect 0x0014

    reg = toggle_fault(reg, FAULT_LOW_COOLANT);
    print_register(reg);   // expect 0x0010

    printf("Active fault count: %d\n", count_active_faults(reg));  // expect 1

    return 0;
}