#ifndef QUEUE_H
#define QUEUE_H

#define INITIAL_CAPACITY 16

typedef struct queue {
    void ** items;
    size_t front;
    size_t size;
    size_t capacity;
} queue;

queue * queue_new();
void queue_free(queue * q);
int queue_enqueue(queue * q, void * item);
void * queue_dequeue(queue * q);
void * queue_peek(queue * q);
int queue_contains(queue * q, void * item);
size_t queue_size(queue * q);

#endif
