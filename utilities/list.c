#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list * list_new() {
    list * l = malloc(sizeof(list));
    if (l == NULL) {
        fprintf(stderr, "list_new(): Failed to allocate %lu bytes for list struct.\n", sizeof(list));
        return NULL;
    }
    l->items = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (l->items == NULL) {
        fprintf(stderr, "list_new(): Failed to allocate %lu bytes for internal array.\n", INITIAL_CAPACITY * sizeof(void *));
        free(l);
        return NULL;
    }
    l->size = 0;
    l->capacity = INITIAL_CAPACITY;
    return l;
}

void list_free(list * l) {
    if (l == NULL) return;
    free(l->items);
    l->items = NULL;
    free(l);
    l = NULL;
}

void list_free_items(list * l) {
    if (l == NULL) return;
    for (int i = 0; i < l->size; i++) {
        free(l->items[i]);
    }
}

int list_add(list * l, void * item) {
    if (l == NULL) return -1;
    if (l->size >= l->capacity) {
        l->capacity *= 2;
        void ** new_items = realloc(l->items, l->capacity * sizeof(void *));
        if (new_items == NULL) {
            fprintf(stderr, "list_add(): Failed to allocate %lu bytes for internal array.\n", l->capacity * sizeof(void *));
            return -1;
        }
        l->items = new_items;
    }
    l->items[l->size++] = item;
    return 0;
}

int list_remove(list * l, size_t index) {
    if (l == NULL) return -1;
    if (index >= l->size) {
        printf("Index %lu out of bounds for length %lu.\n", index, l->size);
        return -1;
    }
    for (int i = index; i < l->size - 1; i++) {
        l->items[i] = l->items[i + 1];
    }
    l->size--;
    return 0;
}

int list_set(list * l, size_t index, void * item) {
    if (l == NULL) return -1;
    if (index >= l->size) {
        printf("Index %lu out of bounds for length %lu.\n", index, l->size);
        return -1;
    }
    l->items[index] = item;
    return 0;
}

void * list_get(list * l, size_t index) {
    if (l == NULL) return NULL;
    if (index >= l->size) {
        printf("Index %lu out of bounds for length %lu.\n", index, l->size);
        return NULL;
    }
    return l->items[index];
}

int list_contains(list * l, void * item) {
    if (l == NULL) return 0;
    for (int i = 0; i < l->size; i++) {
        if (l->items[i] == item) return 1;
    }
    return 0;
}

size_t list_index_of(list * l, void * item) {
    if (l == NULL) return -1;
    for (int i = 0; i < l->size; i++) {
        if (l->items[i] == item) return i;
    }
    return -1;
}

size_t list_size(list * l) {
    if (l == NULL) return 0;
    return l->size;
}

int list_concat(list * l1, list * l2) {
    if (l1 == NULL || l2 == NULL) return -1;
    for (int i = 0; i < l2->size; i++) {
        list_add(l1, l2->items[i]);
    }
    return 0;
}

list * list_copy(list * l) {
    if (l == NULL) return NULL;
    list * copy = list_new();
    if (copy == NULL) return NULL;
    for (int i = 0; i < l->size; i++) {
        list_add(copy, l->items[i]);
    }
    return copy;
}

list * list_intersection(list * l1, list * l2) {
    if (l1 == NULL || l2 == NULL) return NULL;
    list * intersection_list = list_new();
    if (intersection_list == NULL) return NULL;
    for (int i = 0; i < l1->size; i++) {
        if (list_contains(l2, l1->items[i])) {
            list_add(intersection_list, l1->items[i]);
        }
    }
    return intersection_list;
}

list * list_union(list * l1, list * l2) {
    if (l1 == NULL || l2 == NULL) return NULL;
    list * union_list = list_new();
    if (union_list == NULL) return NULL;
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
    if (l1 == NULL || l2 == NULL) return NULL;
    list * difference_list = list_new();
    if (difference_list == NULL) return NULL;
    for (int i = 0; i < l1->size; i++) {
        if (!list_contains(l2, l1->items[i])) {
            list_add(difference_list, l1->items[i]);
        }
    }
    return difference_list;
}
