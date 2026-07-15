/*
A Tesla Supercharger station has a limited number of physical stalls, but cars can queue when all stalls are busy. 
Cars join the queue in a FIFO order (first come, first served) as a linked list.
Each car has a battery_percent and requests a charge. When a stall opens up, 
the next car in line should be dequeued and assigned to it — unless a car in the queue has battery_percent < 10 (critical/low battery), 
in which case that car should jump to the front of the line, overriding normal FIFO order.
Implement the linked-list queue operations below.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct car_node {
    char  vin[8];
    int   battery_percent;
    struct car_node *next;
} car_node_t;

typedef struct {
    car_node_t *head;   // front of queue (next to be served)
    car_node_t *tail;   // back of queue (last one to arrive)
} charge_queue_t;

int len_queue = 0;

// Creates a new car node (helper, already done for you)
car_node_t *create_car(const char *vin, int battery_percent) {
    car_node_t *node = malloc(sizeof(car_node_t));
    strncpy(node->vin, vin, sizeof(node->vin) - 1);
    node->vin[sizeof(node->vin) - 1] = '\0';
    node->battery_percent = battery_percent;
    node->next = NULL;
    return node;
}

// Adds a car to the back of the queue, UNLESS its battery is critical (<10%),
// in which case it should be inserted at the FRONT of the queue instead.
void enqueue_car(charge_queue_t *q, car_node_t *car) {
    if(q->head == NULL){
        q->head = car;
        q->tail = car;
        len_queue++;
        return;
    }
    if(car->battery_percent < 10){
        q->head->next = car;
        q->head = car;
    }else{
        car->next = q->tail;
        q->tail = car;
    }
    len_queue++;
    // TODO
}

// Removes and returns the car at the front of the queue.
// Returns NULL if the queue is empty.
car_node_t *dequeue_car(charge_queue_t *q) {
    car_node_t *node = q->tail;
    while(node->next->next != NULL){
        node = node->next;
    }
    node->next = NULL;
    len_queue--;
    return node;
    // TODO
}

// Prints the queue from front to back
void print_queue(const charge_queue_t *q) {
    car_node_t *node = q->tail;
    char vin_numbers[len_queue][8];

    // while(node->next != NULL){
    //     printf("%s", node->vin);
    //     node = node->next;
    // }
    // printf("%s", node->vin);

    // printf("%d", len_queue);
    for(int i=0; i<len_queue; i++){
        for(int j=0; j<8; j++){
            vin_numbers[i][j] = node->vin[j];
        }
        node = node->next;
    }
    for(int i=(len_queue -1); i>=0; i--){
        printf("%s\n", vin_numbers[i]);
    }
    // TODO
}

int main(void) {
    charge_queue_t q = { .head = NULL, .tail = NULL };

    enqueue_car(&q, create_car("VIN0001", 45));
    enqueue_car(&q, create_car("VIN0002", 60));
    enqueue_car(&q, create_car("VIN0003", 8));   // critical! should jump to front
    enqueue_car(&q, create_car("VIN0004", 30));
    enqueue_car(&q, create_car("VIN0005", 30));

    printf("Queue state:\n");
    print_queue(&q);

    car_node_t *next_up = dequeue_car(&q);
    printf("\nStall opened, serving: %s (%d%%)\n", next_up->vin, next_up->battery_percent);
    free(next_up);

    printf("\nQueue after dequeue:\n");
    print_queue(&q);

    return 0;
}