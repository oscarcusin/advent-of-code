#ifndef LIST_H
#define LIST_H

#define INITIAL_CAPACITY 16

typedef struct {
    void ** items;
    size_t size;
    size_t capacity;
} list;

list * list_new();
void list_free(list * l);
void list_free_items(list * l);
int list_add(list * l, void * item);
int list_remove(list * l, size_t index);
int list_set(list * l, size_t index, void * item);
void * list_get(list * l, size_t index);
int list_contains(list * l, void * item);
size_t list_index_of(list * l, void * item);
size_t list_size(list * l);
int list_concat(list * l1, list * l2);
list * list_copy(list * l);
list * list_intersection(list * l1, list * l2);
list * list_union(list * l1, list * l2);
list * list_difference(list * l1, list * l2);

#endif
