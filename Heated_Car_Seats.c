/*
A Tesla seat has 4 heating elements (front-left, front-right, rear-left, rear-right). 
Each element has a target temperature and a current temperature. 
Every control loop tick, each element should move 1 degree closer to its target (heat up or cool down by 1°C, not overshoot).
Implement the update logic and a helper to check if the whole seat has reached target.
*/

//  {20.0f, 25.0f},   
#include <stdio.h>
#include <stdbool.h>

#define NUM_ELEMENTS 4

typedef struct {
    float current_temp;
    float target_temp;
} heating_element_t;

// Move each element 1 degree closer to its target (don't overshoot)
void update_elements(heating_element_t elements[NUM_ELEMENTS]) {
   
    for (int i = 0; i < NUM_ELEMENTS; ++i){
        heating_element_t heating_element = elements[i];
        // current temp should go up
        if (elements[i].target_temp > elements[i].current_temp) { 
            elements[i].current_temp += 1;
            if (elements[i].current_temp + 1 <= elements[i].target_temp){
                elements[i].current_temp += 1;
            } else {
                elements[i].current_temp = elements[i].target_temp;
            }
        }
        // Current temp should go down
        if (elements[i].target_temp < elements[i].current_temp){
            if (elements[i].current_temp - 1 >= elements[i].target_temp){
                elements[i].current_temp -= 1;
            } else {
                elements[i].current_temp = elements[i].target_temp;
            }
        }
        // If its equal do nothing
        if (elements[i].current_temp == elements[i].target_temp){
            continue;
        }
    }
}

// Returns true if every element has reached its target temp
bool all_at_target(const heating_element_t elements[NUM_ELEMENTS]) {
    for (int i = 0; i < NUM_ELEMENTS; i++){
        heating_element_t element = elements[i];
        if (element.current_temp != element.target_temp){
            return false;
        } else {
            continue;
        }
    }
    return true;
}

int main(void) {
    heating_element_t seat[NUM_ELEMENTS] = {
        {20.0f, 25.0f},   // front-left: heating
        {30.0f, 22.0f},   // front-right: cooling
        {18.0f, 18.0f},   // rear-left: already there
        {15.0f, 20.0f}    // rear-right: heating
    };

    int tick = 0;
    while (!all_at_target(seat)) {
        update_elements(seat);
        tick++;
        printf("Tick %d: FL=%.1f FR=%.1f RL=%.1f RR=%.1f\n",
               tick, seat[0].current_temp, seat[1].current_temp,
               seat[2].current_temp, seat[3].current_temp);
    }

    printf("All elements reached target in %d ticks\n", tick);
    return 0;
}