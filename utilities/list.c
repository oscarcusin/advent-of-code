#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define INITIAL_CAPACITY 16

list * list_new() {
    list * list = malloc(sizeof(list));
    list->data = malloc(INITIAL_CAPACITY * sizeof(void*));
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void list_delete(list * list) {
    if (list == NULL) return;
    free(list->data);
    free(list);
}

void list_add(list * list, void * item) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(void*));
        if (list->data == NULL) {
            printf("Failed to allocate memory for list.\n");
            return;
        }
    }
    list->data[list->size++] = item;
}

int list_set(list * list, int index, void * item) {
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds.\n");
        return 0;
    }
    list->data[index] = item;
    return 1;
}

int list_set_resize(list * list, int index, void * item) {
    if (index < 0 || index >= __SIZE_MAX__ / sizeof(void*)) {
        printf("Index out of bounds.\n");
        return 0;
    }
    if (index >= list->size) {
        while (index >= list->capacity) list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(void*));
        if (list->data == NULL) {
            printf("Failed to allocate memory for list.\n");
            return 0;
        }
        list->size = index + 1;
    }
    list->data[index] = item;
    return 1;
}

void * list_get(list * list, int index) {
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds.\n");
        return NULL;
    }
    return list->data[index];
}


void * list_get_resize(list * list, int index) {
    if (index < 0 || index >= __SIZE_MAX__ / sizeof(void*)) {
        printf("Index out of bounds.\n");
        return NULL;
    }
    if (index >= list->size) {
        while (index >= list->capacity) list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(void*));
        if (list->data == NULL) {
            printf("Failed to allocate memory for list.\n");
            return NULL;
        }
        list->size = index + 1;
    }
    return list->data[index];
}

int list_contains(list * list, void * item) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == item) return 1;
    }
    return 0;
}

int list_index(list * list, void * item) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == item) return i;
    }
    return -1;
}

int list_size(list * list) {
    return list->size;
}

int list_capacity(list * list) {
    return list->capacity;
}

list * list_copy(list * old_list) {
    list * copy = list_new(old_list->capacity);
    for (int i = 0; i < old_list->size; i++) {
        list_add(copy, old_list->data[i]);
    }
    return copy;
}

list * list_intersection(list * list1, list * list2) {
    list * intersection_list = list_new(list1->size < list2->size ? list1->size : list2->size);
    for (int i = 0; i < list1->size; i++) {
        if (list_contains(list2, list1->data[i])) {
            list_add(intersection_list, list1->data[i]);
        }
    }
    return intersection_list;
}

list * list_union(list * list1, list * list2) {
    list * union_list = list_new(list1->size + list2->size);
    for (int i = 0; i < list1->size; i++) {
        list_add(union_list, list1->data[i]);
    }
    for (int i = 0; i < list2->size; i++) {
        if (!list_contains(union_list, list2->data[i])) {
            list_add(union_list, list2->data[i]);
        }
    }
    return union_list;
}

list * list_difference(list * list1, list * list2) {
    list * difference_list = list_new(list1->size);
    for (int i = 0; i < list1->size; i++) {
        if (!list_contains(list2, list1->data[i])) {
            list_add(difference_list, list1->data[i]);
        }
    }
    return difference_list;
}
