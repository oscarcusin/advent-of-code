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
        int card_id = atoi(&line[index]);
        list_set_resize(cards, card_id-1, (void *) ((long) list_get_resize(cards, card_id-1) + 1));
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
            list_set_resize(cards, card_id-1 + i, (void *) ((long) list_get_resize(cards, card_id-1 + i) + (long) list_get(cards, card_id-1)));
        }
    }
    printf("Part one: %d\n", points);
    int total_cards = 0;
    for (int i = 0; i < cards->size; i++) total_cards += (long) list_get(cards, i);
    printf("Part two: %d\n", total_cards);
}
