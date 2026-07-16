/*
During charging, the BMS needs to decide which cells to actively bleed-balance first. Rather than balancing all imbalanced cells simultaneously (which draws more current than the balancing circuit can handle), the system builds a priority queue (implemented as a sorted linked list) of cells that need balancing, ordered by imbalance magnitude, highest first. Each control loop tick, the top of the queue is dequeued and balanced for one cycle; if it still needs more balancing afterward, it gets re-inserted into the queue at its new (likely lower) priority.
The code below is one engineer's attempt at implementing this sorted priority queue as a singly linked list. It compiles and runs, but there's a bug in it.
Expected behavior of each function, for reference:

insert_sorted(queue, cell_id, imbalance): inserts a new node into the list such that the list remains sorted in descending order of imbalance (highest imbalance at the head).
dequeue_highest(queue): removes and returns the node at the head of the list (highest imbalance). 
Returns NULL if the queue is empty. The queue must be left in a valid, consistent state afterward — including if this empties the queue completely.
print_queue(queue): prints all cells currently in the queue, in order from head to tail.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct balance_node {
    int   cell_id;
    float imbalance;
    struct balance_node *next;
} balance_node_t;

typedef struct {
    balance_node_t *head;
    balance_node_t *tail;
    int              size;
} balance_queue_t;

void init_queue(balance_queue_t *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

balance_node_t *create_node(int cell_id, float imbalance) {
    balance_node_t *node = malloc(sizeof(balance_node_t));
    node->cell_id = cell_id;
    node->imbalance = imbalance;
    node->next = NULL;
    return node;
}

void insert_sorted(balance_queue_t *q, int cell_id, float imbalance) {
    balance_node_t *new_node = create_node(cell_id, imbalance);

    if (q->head == NULL) {
        q->head = new_node;
        q->tail = new_node;
        q->size++;
        return;
    }

    if (imbalance > q->head->imbalance) {
        new_node->next = q->head;
        q->head = new_node;
        q->size++;
        return;
    }

    balance_node_t *current = q->head;
    while (current->next != NULL && current->next->imbalance > imbalance) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;

    if (new_node->next == NULL) {
        q->tail = new_node;
    }

    q->size++;
}

balance_node_t *dequeue_highest(balance_queue_t *q) {
    if (q->head == NULL) {
        return NULL;
    }

    balance_node_t *top = q->head;
    if(q->head->next == NULL){
        q->tail = NULL;
    }
    q->head = q->head->next;
    q->size--;

    return top;
}

void print_queue(const balance_queue_t *q) {
    balance_node_t *current = q->head;
    printf("Queue (size=%d): ", q->size);
    while (current != NULL) {
        printf("[cell %d, imbalance %.3f] ", current->cell_id, current->imbalance);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    balance_queue_t queue;
    balance_node_t node;
    init_queue(&queue);
    insert_sorted(&queue, 1, 5);
    insert_sorted(&queue, 2, 4);
    insert_sorted(&queue, 3, 6);
    insert_sorted(&queue, 4, 6);
    print_queue(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    insert_sorted(&queue, 1, 5);
    insert_sorted(&queue, 2, 6);
    insert_sorted(&queue, 3, 7);
    print_queue(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    dequeue_highest(&queue);
    if(queue.tail != NULL){
        printf("id: %d\n", queue.tail->cell_id);
    }
    insert_sorted(&queue, 1, 5);
    print_queue(&queue);
    // TODO: write your own test cases here.
    // Try inserting several cells with different imbalances in various orders,
    // verify sorted order with print_queue after each insert.
    // Then dequeue everything (including past the point where the queue is empty)
    // and verify the queue remains in a valid state throughout —
    // including what happens if you insert again after fully emptying it.

    return 0;
}