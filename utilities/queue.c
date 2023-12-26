#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue * queue_new() {
    queue * q = malloc(sizeof(queue));
    if (q == NULL) {
        fprintf(stderr, "queue_new(): Failed to allocate %lu bytes for queue struct.\n", sizeof(queue));
        return NULL;
    }
    q->items = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (q->items == NULL) {
        fprintf(stderr, "queue_new(): Failed to allocate %lu bytes for internal array.\n", INITIAL_CAPACITY * sizeof(void *));
        free(q);
        return NULL;
    }
    q->front = 0;
    q->size = 0;
    q->capacity = INITIAL_CAPACITY;
    return q;
}

void queue_free(queue * q) {
    if (q == NULL) return;
    free(q->items);
    q->items = NULL;
    free(q);
    q = NULL;
}

int queue_enqueue(queue * q, void * item) {
    if (q == NULL) return -1;
    if (q->size >= q->capacity) {
        q->capacity *= 2;
        void ** new_items = malloc(q->capacity * sizeof(void *));
        if (new_items == NULL) {
            fprintf(stderr, "queue_enqueue(): Failed to allocate %lu bytes for internal array.\n", q->capacity * sizeof(void *));
            return -1;
        }
        memcpy(new_items, q->items + q->front, (q->size - q->front) * sizeof(void *));
        memcpy(new_items + (q->size - q->front), q->items, q->front * sizeof(void *));
        free(q->items);
        q->items = new_items;
        q->front = 0;
    }
    q->items[(q->front + q->size) % q->capacity] = item;
    q->size++;
    return 0;
}

void * queue_dequeue(queue * q) {
    if (q == NULL || q->size == 0) return NULL;
    void * item = q->items[q->front];
    q->size--;
    if (q->size == 0) q->front = 0;
    else q->front = (q->front + 1) % q->capacity;
    return item;
}

void * queue_peek(queue * q) {
    if (q == NULL || q->size == 0) return NULL;
    return q->items[q->front];
}

int queue_contains(queue * q, void * item) {
    if (q == NULL) return 0;
    for (int i = 0; i < q->size; i++) {
        if (q->items[(q->front + i) % q->capacity] == item) return 1;
    }
    return 0;
}

size_t queue_size(queue * q) {
    return q->size;
}
