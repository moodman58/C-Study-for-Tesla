/*
 * ============================================================
 * Cabin Climate Zone Controller
 * ============================================================
 *
 * Tesla's cabin has 4 climate zones (driver, passenger, rear-left,
 * rear-right). Each zone has a target temperature, current temperature,
 * and a fan speed setting. The system needs to compute, for each zone,
 * whether it should be HEATING, COOLING, or IDLE, based on how far the
 * current temp is from the target -- and set fan speed accordingly
 * (bigger gap = higher fan speed, capped at a max).
 *
 * Rules:
 * - If current_temp is within TOLERANCE_C of target_temp -> MODE_IDLE
 * - If current_temp is below target (outside tolerance)   -> MODE_HEATING
 * - If current_temp is above target (outside tolerance)   -> MODE_COOLING
 *
 * Fan speed:
 * - fan_speed = |current_temp - target_temp| rounded down to nearest int,
 *   capped at MAX_FAN_SPEED
 *   (e.g. gap of 0.5 -> speed 0, gap of 2.3 -> speed 2, gap of 8.0 -> speed 5)
 * - If mode is IDLE, fan_speed must always be 0, regardless of gap.
 *
 * Implement:
 *   1. determine_mode()   - decide HEATING / COOLING / IDLE for a zone
 *   2. compute_fan_speed()- compute fan speed from temp gap
 *   3. update_zone()      - apply both of the above to a single zone
 *   4. update_all_zones() - apply update_zone() to every zone, then
 *                           print each zone's resulting status
 * ============================================================
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NUM_ZONES     4
#define MAX_FAN_SPEED 5
#define TOLERANCE_C   0.5f   // if within this range of target, zone is IDLE

typedef enum {
    ZONE_DRIVER,
    ZONE_PASSENGER,
    ZONE_REAR_LEFT,
    ZONE_REAR_RIGHT
} zone_id_t;

typedef enum {
    MODE_IDLE,
    MODE_HEATING,
    MODE_COOLING
} climate_mode_t;

typedef struct {
    zone_id_t       id;
    float           current_temp;
    float           target_temp;
    climate_mode_t  mode;
    int             fan_speed;   // 0 = off, 1-5 = increasing speed
} climate_zone_t;

const char *zone_name(zone_id_t id) {
    static const char *names[NUM_ZONES] = {"Driver", "Passenger", "Rear-Left", "Rear-Right"};
    return names[id];
}

const char *mode_name(climate_mode_t m) {
    static const char *names[3] = {"IDLE", "HEATING", "COOLING"};
    return names[m];
}

// Given a zone's current and target temp, determine its mode:
// - within TOLERANCE_C of target -> MODE_IDLE
// - current below target (outside tolerance) -> MODE_HEATING
// - current above target (outside tolerance) -> MODE_COOLING
climate_mode_t determine_mode(float current_temp, float target_temp) {
    if (abs(target_temp - current_temp) < TOLERANCE_C){
        return MODE_IDLE;
    } 
    if (target_temp > current_temp){
        return MODE_HEATING;
    } else {
        return MODE_COOLING;
    }
}

// Given the temperature gap (absolute difference between current and
// target), returns a fan speed from 0-MAX_FAN_SPEED.
// Rule: fan_speed = gap rounded down to nearest int, capped at MAX_FAN_SPEED.
// (e.g. gap of 0.5 -> speed 0, gap of 2.3 -> speed 2, gap of 8.0 -> speed 5)
int compute_fan_speed(float temp_gap) {
    if (floor(temp_gap) >= MAX_FAN_SPEED){
        return MAX_FAN_SPEED;
    }
    return floor(temp_gap);
}

// Updates mode and fan_speed for a single zone based on its temps.
// If mode is IDLE, fan_speed should always be 0 regardless of gap.
void update_zone(climate_zone_t *zone) {
    zone->mode = determine_mode(zone->current_temp, zone->target_temp);
    if (zone->mode == MODE_IDLE){
        zone->fan_speed = 0;
    } else {
        float temp_gap = abs(zone->current_temp - zone->target_temp);
        zone->fan_speed = compute_fan_speed(temp_gap);
    }
}

// Updates all zones in the array, then prints each zone's full status.
void update_all_zones(climate_zone_t zones[NUM_ZONES]) {
    for (int i=0; i<NUM_ZONES; ++i){
        update_zone(&zones[i]);
    }
}

int main(void) {
    climate_zone_t zones[NUM_ZONES] = {
        { ZONE_DRIVER,      18.0f, 22.0f, MODE_IDLE, 0 },   // needs heating
        { ZONE_PASSENGER,   24.0f, 24.2f, MODE_IDLE, 0 },   // within tolerance
        { ZONE_REAR_LEFT,   30.0f, 21.0f, MODE_IDLE, 0 },   // needs strong cooling
        { ZONE_REAR_RIGHT,  19.5f, 20.0f, MODE_IDLE, 0 },   // within tolerance
    };

    update_all_zones(zones);
    for (int i=0; i<NUM_ZONES; ++i){
        climate_zone_t zone = zones[i];
        printf("%f %f %s %d \n", zone.current_temp, zone.target_temp, mode_name(zone.mode), zone.fan_speed);
    }

    return 0;
}