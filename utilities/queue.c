#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define INITIAL_CAPACITY 16

queue * queue_new() {
    queue * q = malloc(sizeof(queue));
    q->items = malloc(INITIAL_CAPACITY * sizeof(void *));
    q->size = 0;
    q->capacity = INITIAL_CAPACITY;
    q->front = -1;
    return q;
}

void queue_free(queue * q) {
    if (q == NULL) return;
    free(q->items);
    free(q);
}

void queue_enqueue(queue * q, void * item) {
    if (q->size >= q->capacity) {
        q->capacity *= 2;
        void ** new_items = malloc(q->capacity * sizeof(void *));
        memcpy(new_items, q->items + q->front, (q->size - q->front) * sizeof(void *));
        memcpy(new_items + (q->size - q->front), q->items, q->front * sizeof(void *));
        free(q->items);
        q->items = new_items;
        q->front = 0;
    }
    if (q->front == -1) q->front = 0;
    q->items[(q->front + q->size) % q->capacity] = item;
    q->size++;
}

void * queue_dequeue(queue * q) {
    if (q->size == 0) return NULL;
    void * item = q->items[q->front];
    q->size--;
    if (q->size == 0) q->front = -1;
    else q->front = (q->front + 1) % q->capacity;
    return item;
}

int queue_size(queue * q) {
    return q->size;
}
