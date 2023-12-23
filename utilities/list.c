#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define INITIAL_CAPACITY 16

list * list_new() {
    list * l = malloc(sizeof(list));
    l->items = malloc(INITIAL_CAPACITY * sizeof(void *));
    l->size = 0;
    l->capacity = INITIAL_CAPACITY;
    return l;
}

void list_free(list * l) {
    if (l == NULL) return;
    free(l->items);
    free(l);
}

void list_free_items(list * l) {
    if (l == NULL) return;
    for (int i = 0; i < l->size; i++) {
        free(l->items[i]);
    }
}

void list_add(list * l, void * item) {
    if (l->size >= l->capacity) {
        l->capacity *= 2;
        l->items = realloc(l->items, l->capacity * sizeof(void *));
        if (l->items == NULL) {
            printf("Failed to allocate memory for list.\n");
            return;
        }
    }
    l->items[l->size++] = item;
}

void list_remove(list * l, int index) {
    if (index < 0 || index >= l->size) {
        printf("Index %d out of bounds for length %d.\n", index, l->size);
        return;
    }
    for (int i = index; i < l->size - 1; i++) {
        l->items[i] = l->items[i + 1];
    }
    l->size--;
}

int list_set(list * l, int index, void * item) {
    if (index < 0 || index >= l->size) {
        printf("Index %d out of bounds for length %d.\n", index, l->size);
        return 0;
    }
    l->items[index] = item;
    return 1;
}

void * list_get(list * l, int index) {
    if (index < 0 || index >= l->size) {
        printf("Index %d out of bounds for length %d.\n", index, l->size);
        return NULL;
    }
    return l->items[index];
}

void list_append(list * l1, list * l2) {
    for (int i = 0; i < l2->size; i++) {
        list_add(l1, l2->items[i]);
    }
}

int list_contains(list * l, void * item) {
    for (int i = 0; i < l->size; i++) {
        if (l->items[i] == item) return 1;
    }
    return 0;
}

int list_index(list * l, void * item) {
    for (int i = 0; i < l->size; i++) {
        if (l->items[i] == item) return i;
    }
    return -1;
}

int list_size(list * l) {
    return l->size;
}

int list_capacity(list * l) {
    return l->capacity;
}

list * list_copy(list * l) {
    list * copy = list_new(l->capacity);
    for (int i = 0; i < l->size; i++) {
        list_add(copy, l->items[i]);
    }
    return copy;
}

list * list_intersection(list * l1, list * l2) {
    list * intersection_list = list_new(l1->size < l2->size ? l1->size : l2->size);
    for (int i = 0; i < l1->size; i++) {
        if (list_contains(l2, l1->items[i])) {
            list_add(intersection_list, l1->items[i]);
        }
    }
    return intersection_list;
}

list * list_union(list * l1, list * l2) {
    list * union_list = list_new(l1->size + l2->size);
    for (int i = 0; i < l1->size; i++) {
        list_add(union_list, l1->items[i]);
    }
    for (int i = 0; i < l2->size; i++) {
        if (!list_contains(union_list, l2->items[i])) {
            list_add(union_list, l2->items[i]);
        }
    }
    return union_list;
}

list * list_difference(list * l1, list * l2) {
    list * difference_list = list_new(l1->size);
    for (int i = 0; i < l1->size; i++) {
        if (!list_contains(l2, l1->items[i])) {
            list_add(difference_list, l1->items[i]);
        }
    }
    return difference_list;
}
