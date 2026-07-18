/*
 * ============================================================
 * Drive Mode & Suspension Coordinator
 * ============================================================
 *
 * Tesla vehicles support 3 drive modes, each with its own target
 * suspension height (mm) and stiffness (0-100 scale):
 *
 *   COMFORT - height: 120mm, stiffness: 30
 *   SPORT   - height: 95mm,  stiffness: 65
 *   TRACK   - height: 80mm,  stiffness: 90
 *
 * The car has 4 suspension corners. Rear corners run 5mm LOWER and
 * 10 stiffness units STIFFER than the mode's base values (due to
 * rear weight bias), front corners use the base values exactly.
 *
 * Each corner also tracks its CURRENT height/stiffness (which may
 * differ from target immediately after a mode switch, since the
 * actuators take time to move -- they adjust by at most
 * MAX_HEIGHT_STEP_MM / MAX_STIFFNESS_STEP per tick).
 *
 * --- What to implement ---
 *   1. get_mode_settings()   - given a drive_mode_t, returns the
 *                              BASE target height/stiffness for
 *                              that mode (front corner values)
 *   2. set_drive_mode()      - given a new mode, computes and stores
 *                              the correct TARGET height/stiffness for
 *                              all 4 corners (applying the rear offset
 *                              where applicable), but does NOT change
 *                              current_height/current_stiffness yet
 *   3. step_actuators()      - moves each corner's current height and
 *                              stiffness one step closer to its target
 *                              (by at most MAX_HEIGHT_STEP_MM /
 *                              MAX_STIFFNESS_STEP per call), without
 *                              overshooting
 *   4. all_corners_at_target() - returns true if every corner's
 *                                 current values equal their targets
 * ============================================================
 */

#include <stdio.h>
#include <stdbool.h>

#define NUM_CORNERS           4
#define REAR_HEIGHT_OFFSET_MM   -5
#define REAR_STIFFNESS_OFFSET   10
#define MAX_HEIGHT_STEP_MM       3
#define MAX_STIFFNESS_STEP       5

typedef enum {
    COMFORT,
    SPORT,
    TRACK
} drive_mode_t;

typedef enum {
    CORNER_FRONT_LEFT,
    CORNER_FRONT_RIGHT,
    CORNER_REAR_LEFT,
    CORNER_REAR_RIGHT
} corner_id_t;

typedef struct {
    int height_mm;
    int stiffness;
} suspension_settings_t;

typedef struct {
    corner_id_t             id;
    int                      current_height_mm;
    int                      current_stiffness;
    suspension_settings_t    target;
} suspension_corner_t;

const char *corner_name(corner_id_t id) {
    static const char *names[NUM_CORNERS] = {"Front-Left", "Front-Right", "Rear-Left", "Rear-Right"};
    return names[id];
}

// Returns the BASE (front-corner) target settings for a given mode
suspension_settings_t get_mode_settings(drive_mode_t mode) {
    suspension_settings_t suspension_settings;
    switch(mode){
      case COMFORT:
        suspension_settings.height_mm = 120;
        suspension_settings.stiffness = 30;
        break;
      case SPORT:
        suspension_settings.height_mm = 95;
        suspension_settings.stiffness = 65;
        break;
      case TRACK:
        suspension_settings.height_mm = 80;
        suspension_settings.stiffness = 90;
        break; 
    }
    return suspension_settings;
}

// Computes and stores the correct target height/stiffness for all
// 4 corners based on the new mode. Rear corners get the offsets
// applied on top of the base mode settings; front corners use base
// values directly. Does NOT touch current_height_mm/current_stiffness.
void set_drive_mode(suspension_corner_t corners[NUM_CORNERS], drive_mode_t mode) {
    suspension_settings_t mode_setting = get_mode_settings(mode);
    for(corner_id_t c = 0; c <NUM_CORNERS; c++){
      if(c < CORNER_REAR_LEFT){
        corners[c].target = mode_setting;
      } else {
        corners[c].target.height_mm = mode_setting.height_mm + REAR_HEIGHT_OFFSET_MM;
        corners[c].target.stiffness = mode_setting.stiffness + REAR_STIFFNESS_OFFSET; 
      }
    }
}

// Moves each corner's current height/stiffness one step closer to
// its target, by at most MAX_HEIGHT_STEP_MM / MAX_STIFFNESS_STEP,
// without overshooting past the target.
void step_actuators(suspension_corner_t corners[NUM_CORNERS]) {
    for(corner_id_t c = 0; c <NUM_CORNERS; c++){
      suspension_corner_t * corner = &corners[c];
      
      if (corner->current_height_mm < corner->target.height_mm){
        corner->current_height_mm += MAX_HEIGHT_STEP_MM;
         if (corner->current_height_mm > corner->target.height_mm){
          corner->current_height_mm = corner->target.height_mm;
        }
        
      }
      else if (corner->current_height_mm > corner->target.height_mm){
        corner->current_height_mm -= MAX_HEIGHT_STEP_MM;
         if (corner->current_height_mm < corner->target.height_mm){
          corner->current_height_mm = corner->target.height_mm;
        }
         
      }
      if (corner->current_stiffness < corner->target.stiffness){
        corner->current_stiffness += MAX_STIFFNESS_STEP;
        if (corner->current_stiffness > corner->target.stiffness){
          corner->current_stiffness = corner->target.stiffness;
        }
      }
      else if (corner->current_stiffness > corner->target.stiffness){
         corner->current_stiffness -= MAX_STIFFNESS_STEP;
         if (corner->current_stiffness < corner->target.stiffness){
          corner->current_stiffness = corner->target.stiffness;
        }
      }

    
      
  }
}

// Returns true if every corner's current values match their targets
bool all_corners_at_target(const suspension_corner_t corners[NUM_CORNERS]) {
  for(corner_id_t i = 0; i<NUM_CORNERS; ++i){
    if((corners[i].target.height_mm != corners[i].current_height_mm) || (corners[i].target.stiffness != corners[i].current_stiffness)){
      return false;
    }
  }
  return true;
}

int main(void) {
    suspension_corner_t corners[NUM_CORNERS] = {
        { CORNER_FRONT_LEFT,  120, 30, {0, 0} },
        { CORNER_FRONT_RIGHT, 120, 30, {0, 0} },
        { CORNER_REAR_LEFT,   115, 40, {0, 0} },
        { CORNER_REAR_RIGHT,  115, 40, {0, 0} },
    };

    set_drive_mode(corners, TRACK);

    int tick = 0;
    while (!all_corners_at_target(corners)) {
        step_actuators(corners);
        tick++;
        printf("Tick %d:\n", tick);
        for (int i = 0; i < NUM_CORNERS; i++) {
            printf("  %-12s height=%3d (target %3d)  stiffness=%3d (target %3d)\n",
                   corner_name(corners[i].id),
                   corners[i].current_height_mm, corners[i].target.height_mm,
                   corners[i].current_stiffness, corners[i].target.stiffness);
        }
    }

    printf("\nAll corners reached target in %d ticks\n", tick);
    return 0;
}