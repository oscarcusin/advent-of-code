#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t buff_len = 0;
    int points = 0;
    list * cards = list_new();
    while (getline(&line, &buff_len, stdin) != -1) {
        list * winning_numbers = list_new();
        list * my_numbers = list_new();
        int index = 0;
        while (line[index++] != ' ');
        int card_id = atoi(&line[index]) - 1;
        if (card_id > list_size(cards)-1) {
            for (int i = 0; i < card_id - (list_size(cards)-1); i++) list_add(cards, (void *) 0);
        }
        list_set(cards, card_id, (void *) ((long) list_get(cards, card_id) + 1));
        while (line[index++] != ':');
        int flag = 0;
        while (index < strlen(line)) {
            if (line[index] == '|') flag = 1;
            if (isdigit(line[index])) {
                list_add((flag ? my_numbers : winning_numbers), (void *) atol(&line[index]));
                while (isdigit(line[index])) index++;
            }
            index++;
        }
        list * matches = list_intersection(winning_numbers, my_numbers);
        points += 1 << (list_size(matches) - 1);
        for (int i = 1; i <= list_size(matches); i++) {
            if (card_id + i > list_size(cards)-1) list_add(cards, (void *) 0);
            list_set(cards, card_id + i, (void *) ((long) list_get(cards, card_id + i) + (long) list_get(cards, card_id)));
        }
        list_delete(winning_numbers);
        list_delete(my_numbers);
        list_delete(matches);
    }
    printf("Part one: %d\n", points);
    long total_cards = 0;
    for (int i = 0; i < list_size(cards); i++) total_cards += (long) list_get(cards, i);
    printf("Part two: %ld\n", total_cards);
    free(line);
    list_delete(cards);
}
