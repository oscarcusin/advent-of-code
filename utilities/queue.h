#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue {
    void ** items;
    int size;
    int capacity;
    int front;
} queue;

queue * queue_new();
void queue_free(queue * q);
void queue_enqueue(queue * q, void * item);
void * queue_dequeue(queue * q);
int queue_size(queue * q);

#endif
