/*
Tesla's battery pack is organized into 6 modules, each containing 12 cells. 
To maximize pack life, cells need to be "balanced" — if any cell's voltage is significantly higher than the minimum voltage in its own module, 
that cell should be bled down slightly during charging.

A cell needs balancing if:
cell_voltage - module_min_voltage > BALANCE_THRESHOLD (0.02V)
Implement the functions below to:

Find the minimum voltage within each module
Mark which cells need balancing -> cell_voltage - module_min_voltage > BALANCE_THRESHOLD (0.02V)
Report the single most imbalanced cell in the entire pack (module index + cell index + how far it is above its module's min) -> biggest value away from balanced threshold
*/


// min voltage within a module is just the smallest value of the modules subarray
// If cell needs balancing then that cells voltage - min voltage of module > 0.02

#include <stdio.h>
#include <stdbool.h>
#include <float.h>

#define NUM_MODULES         6
#define CELLS_PER_MODULE     12
#define BALANCE_THRESHOLD    0.02f

typedef struct {
    float voltages[NUM_MODULES][CELLS_PER_MODULE];
    bool  needs_balancing[NUM_MODULES][CELLS_PER_MODULE];
} battery_pack_t;

typedef struct {
    int   module_idx;
    int   cell_idx;
    float imbalance;   // how far above module min
} worst_cell_t;

// Returns the minimum voltage found within a single module
float get_module_min(const battery_pack_t *pack, int module_idx) {
    const float *voltage = pack->voltages[module_idx];
    float minimum_voltage = 0.0;

    for (int i = 0; i < CELLS_PER_MODULE; ++i){
        if (voltage[i] < minimum_voltage){
            minimum_voltage = voltage[i];
        }
    }
    return minimum_voltage;
}

// Fills in pack->needs_balancing for every cell in the pack
void mark_cells_for_balancing(battery_pack_t *pack) {
    const float* voltage = pack->voltages;
    const float* balancing_module = pack->needs_balancing;
    float* cell_module;
    float* needs_balancing_module;
    float cell_voltage;

    for (int i = 0; i < NUM_MODULES; ++i){
        cell_module = &voltage[i];
        needs_balancing_module = &balancing_module[i]; 
        float module_min = get_module_min(pack, i);

        for (int j = 0; j < CELLS_PER_MODULE; ++j){
            cell_voltage = cell_module[j];
            if (cell_voltage - module_min > 0.02) {
                needs_balancing_module[j] = true;
            } else {
                needs_balancing_module[j] = false;
            }
        }
    }
}

// Scans the whole pack and returns the single cell with the largest
// imbalance (voltage above its own module's min)
worst_cell_t find_worst_cell(const battery_pack_t *pack) {
    const float* voltage = pack->voltages;
    const float* balancing_module = pack->needs_balancing;
    float* cell_module;
    float* needs_balancing_module;
    float cell_voltage;
    worst_cell_t worst_cell = {0, 0, 0.0};

    for (int i = 0; i < NUM_MODULES; ++i){
        cell_module = &voltage[i];
        needs_balancing_module = &balancing_module[i]; 
        float module_min = get_module_min(pack, i);
        for (int j = 0; j < CELLS_PER_MODULE; ++j){
            cell_voltage = cell_module[j];
            if (cell_voltage - module_min > worst_cell.imbalance) {
               worst_cell.imbalance = cell_voltage - module_min;
               worst_cell.module_idx = i;
               worst_cell.cell_idx = j;
            } 
        }
    }
    return worst_cell;
}


int main(void) {
    battery_pack_t pack = {0};

    // Simple test setup: module 0 has one clear outlier cell
    for (int m = 0; m < NUM_MODULES; m++) {
        for (int c = 0; c < CELLS_PER_MODULE; c++) {
            pack.voltages[m][c] = 3.70f;
        }
    }
    pack.voltages[0][5] = 3.75f;   // outlier, 0.05V above rest
    pack.voltages[3][9] = 3.73f;   // smaller outlier, 0.03V above rest

    mark_cells_for_balancing(&pack);

    worst_cell_t worst = find_worst_cell(&pack);
    printf("Worst cell: module %d, cell %d, imbalance %.3fV\n",
           worst.module_idx, worst.cell_idx, worst.imbalance);

    return 0;
}

