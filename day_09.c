#include <stdio.h>
#include <stdlib.h>
#include "utilities/list.h"

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    long total_sum_last = 0;
    long total_sum_first = 0;
    while (getline(&line, &len, stdin) != -1) {
        long number;
        int offset = 0, chars;
        list * numbers = list_new();
        while (sscanf(line + offset, "%ld%n", &number, &chars) == 1) {
            list_add(numbers, (void *) number);
            offset += chars;
        }
        list * last_numbers = list_new();
        list * first_numbers = list_new();
        list_add(last_numbers, list_get(numbers, list_size(numbers) - 1));
        list_add(first_numbers, list_get(numbers, 0));
        int zeroes = 0;
        while (zeroes != list_size(numbers)) {
            list * diffs = list_new();
            zeroes = 0;
            for (int i = 0; i < list_size(numbers) - 1; i++) {
                long diff = (long) list_get(numbers, i + 1) - (long) list_get(numbers, i);
                if (i == list_size(numbers) - 2) list_add(last_numbers, (void *) diff);
                if (i == 0) list_add(first_numbers, (void *) diff);
                if (diff == 0) zeroes++;
                list_add(diffs, (void *) diff);
            }
            list_free(numbers);
            numbers = diffs;
        }
        list_free(numbers);
        for (int i = list_size(last_numbers) - 2; i >= 0; i--) {
            list_set(last_numbers, i, (void *) ((long) list_get(last_numbers, i) + (long) list_get(last_numbers, i+1)));
            list_set(first_numbers, i, (void *) ((long) list_get(first_numbers, i) - (long) list_get(first_numbers, i+1)));
        }
        total_sum_last += (long) list_get(last_numbers, 0);
        total_sum_first += (long) list_get(first_numbers, 0);
        list_free(last_numbers);
        list_free(first_numbers);
    }
    free(line);
    printf("Part one: %ld\n", total_sum_last);
    printf("Part two: %ld\n", total_sum_first);
}
