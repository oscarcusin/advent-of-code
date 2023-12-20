#include <stdio.h>
#include <stdlib.h>
#include "utilities/list.h"

int main(int argc, char * argv[]) {
    list * times = list_new();
    list * distances = list_new();
    long time_input = 0, distance_input = 0;
    scanf("Time:");
    while (scanf("%ld", &time_input) == 1) list_add(times, (void *) time_input);
    scanf("Distance:");
    while (scanf("%ld", &distance_input) == 1) list_add(distances, (void *) distance_input);
    int total_ways = 1;
    for (int race = 0; race < list_size(times); race++) {
        long time = (long) list_get(times, race);
        long distance = (long) list_get(distances, race);
        int start = 0, end = 0;
        for (start = 0; start < time; start++) if ((time - start) * start > distance) break;
        for (end = time - 1; end >= 0; end--) if ((time - end) * end > distance) break;
        total_ways *= (end - start + 1);
    }
    char * time_concat = malloc(list_size(times) * 20 * sizeof(char));
    char * distance_concat = malloc(list_size(times) * 20 * sizeof(char));
    int time_concat_chars = 0, distance_concat_chars = 0;
    for (int i = 0; i < list_size(times); i++) {
        time_concat_chars += sprintf(time_concat + time_concat_chars, "%ld", (long) list_get(times, i));
        distance_concat_chars += sprintf(distance_concat + distance_concat_chars, "%ld", (long) list_get(distances, i));
    }
    list_free(times);
    list_free(distances);
    long time = atol(time_concat);
    long distance = atol(distance_concat);
    free(time_concat);
    free(distance_concat);
    int start = 0, end = 0;
    for (start = 0; start < time; start++) if ((time - start) * start > distance) break;
    for (end = time - 1; end >= 0; end--) if ((time - end) * end > distance) break;
    int ways_concat = end - start + 1;
    printf("Part one: %d\n", total_ways);
    printf("Part one: %d\n", ways_concat);
}
