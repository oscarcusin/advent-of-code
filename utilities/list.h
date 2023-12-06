#ifndef LIST_H
#define LIST_H

typedef struct {
    void ** data;
    int size;
    int capacity;
} list;

list * list_new();
void list_delete(list * list);
void list_add(list * list, void * item);
int list_set(list * list, int index, void * item);
int list_set_resize(list * list, int index, void * item);
void * list_get(list * list, int index);
void * list_get_resize(list * list, int index);
int list_contains(list * list, void * item);
int list_index(list * list, void * item);
int list_size(list * list);
int list_capacity(list * list);
list * list_copy(list * list);
list * list_intersection(list * list1, list * list2);
list * list_union(list * list1, list * list2);
list * list_difference(list * list1, list * list2);

#endif
