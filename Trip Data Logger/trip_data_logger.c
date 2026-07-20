/*
 * ============================================================
 * Trip Data Logger (Dynamic Array)
 * ============================================================
 *
 * Tesla logs telemetry points during a drive without knowing the
 * trip length in advance. Implement a dynamic array that doubles
 * its capacity via realloc whenever it fills up.
 *
 * --- What to implement ---
 *   1. logger_init()      - allocates initial buffer (capacity
 *                            given by INITIAL_CAPACITY), zeroes count
 *   2. logger_add_point()  - appends a data point; if count == capacity,
 *                            DOUBLE the capacity via realloc BEFORE
 *                            inserting the new point. Must correctly
 *                            handle realloc failure (return false,
 *                            don't crash or leak).
 *   3. logger_average_speed() - computes average speed across all
 *                            logged points using pointer arithmetic
 *                            to walk the buffer (not array indexing)
 *   4. logger_free()       - frees the buffer and resets fields to
 *                            a safe empty state (no dangling pointer)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 4

typedef struct {
    int   odometer_km;
    float speed_kmh;
    int   battery_percent;
} trip_point_t;

typedef struct {
    trip_point_t *points;      // heap-allocated buffer
    int           count;       // number of points currently stored
    int           capacity;    // current allocated capacity
} trip_logger_t;

// Allocates the initial buffer at INITIAL_CAPACITY, sets count to 0.
// Returns true on success, false if allocation fails.
bool logger_init(trip_logger_t *logger) {
    logger->count = 0;
    trip_point_t* buffer_ptr;
    buffer_ptr = (trip_point_t*)calloc(INITIAL_CAPACITY, sizeof(trip_point_t));
    logger->points = buffer_ptr;
    logger->capacity = INITIAL_CAPACITY;

    if (logger->count == 0 && logger->points != NULL){
        return true;
    } 
    else {
        return false;
    }
}

// Appends a new point to the logger. If the buffer is full
// (count == capacity), double the capacity via realloc BEFORE
// adding the new point. If realloc fails, leave the logger in its
// previous valid state and return false (don't lose existing data).
bool logger_add_point(trip_logger_t *logger, int odometer_km, float speed_kmh, int battery_percent) {
    trip_point_t point = {odometer_km, speed_kmh, battery_percent};

    if (logger->count == logger->capacity){
        trip_point_t temp = *logger->points;
        int* reallocated = realloc(logger->points, 2*logger->capacity*sizeof(trip_point_t));
        if (reallocated == NULL){
            logger->points = &temp;
            return false;
        }
        else {
            logger->points[logger->count] = point;
            logger->count++;
            logger->capacity = 2*logger->capacity;
            return true;
        }
            }
    // If not full
    logger->points[logger->count] = point;
    logger->count++;
    return true;
}

// Computes the average speed across all logged points.
// MUST walk the buffer using pointer arithmetic, not logger->points[i].
// Returns 0.0f if count == 0.
float logger_average_speed(const trip_logger_t *logger) {
    if (logger->count == 0){
        return 0.0f;
    }
    trip_point_t* first_index;
    float speed = 0.0, average_speed;    
    for (first_index = logger->points; first_index < logger->points + logger->count; ++first_index){
        printf("%f\n", first_index->speed_kmh);
        speed += first_index->speed_kmh;
    }
    average_speed = speed / logger->count;
    return average_speed;
}

// Frees the buffer and resets the logger to a safe empty state
// (points = NULL, count = 0, capacity = 0) so it can't be
// accidentally used after freeing.
void logger_free(trip_logger_t *logger) {
    free(logger->points);

    logger->points = NULL;
    logger->count = 0;
    logger-> capacity = 0;

    return;
}

int main(void) {
    trip_logger_t logger;
    if (!logger_init(&logger)) {
        printf("Init failed\n");
        return 1;
    }

    // Add more points than INITIAL_CAPACITY to force at least one resize
    logger_add_point(&logger, 100, 45.0f, 90);
    logger_add_point(&logger, 105, 52.0f, 89);
    logger_add_point(&logger, 112, 60.0f, 87);
    logger_add_point(&logger, 120, 58.0f, 85);
    logger_add_point(&logger, 128, 65.0f, 83);   // triggers resize
    logger_add_point(&logger, 135, 40.0f, 81);

    printf("Logged %d points (capacity now %d)\n", logger.count, logger.capacity);
    printf("Average speed: %.2f km/h\n", logger_average_speed(&logger));
    logger_free(&logger);
    printf("Freed. count=%d capacity=%d points=%p\n",
           logger.count, logger.capacity, (void *)logger.points);

    return 0;
}