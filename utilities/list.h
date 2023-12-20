#ifndef LIST_H
#define LIST_H

typedef struct {
    void ** data;
    int size;
    int capacity;
} list;

list * list_new();
void list_free(list * l);
void list_free_items(list * l);
void list_add(list * l, void * item);
void list_remove(list * l, int index);
int list_set(list * l, int index, void * item);
void * list_get(list * l, int index);
void list_append(list * l1, list * l2);
int list_contains(list * l, void * item);
int list_index(list * l, void * item);
int list_size(list * l);
int list_capacity(list * l);
list * list_copy(list * l);
list * list_intersection(list * l1, list * l2);
list * list_union(list * l1, list * l2);
list * list_difference(list * l1, list * l2);

#endif
