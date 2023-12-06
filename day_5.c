#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "utilities/list.h"

struct range {
    long dest_start;
    long source_start;
    long length;
};

void find_location(long seed, list * maps, long * location, long * range_size) {
    long curr_number = seed;
    long min_range_size = LONG_MAX;
    for (int map_index = 0; map_index < maps->size; map_index++) {
        list * map = list_get(maps, map_index);
        for (int range_index = 0; range_index < map->size; range_index++) {
            struct range * range = list_get(map, range_index);
            if (curr_number >= range->source_start && curr_number < range->source_start + range->length) {
                if (range->source_start + range->length - curr_number < min_range_size) {
                    min_range_size = range->source_start + range->length - curr_number;
                }
                curr_number = range->dest_start + (curr_number - range->source_start);
                break;
            }
        }
    }
    * location = curr_number;
    if (range_size != NULL) * range_size = min_range_size;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t buff_len = 0;
    getline(&line, &buff_len, stdin);
    list * seeds = list_new();
    for (int i = 0; i < strlen(line); i++) {
        if (isdigit(line[i]) && !isdigit(line[i-1])) {
            long * seed = malloc(sizeof(long));
            * seed = atol(&line[i]);
            list_add(seeds, seed);
        }
    }
    list * maps = list_new();
    while (getline(&line, &buff_len, stdin) != -1) {
        if (!isdigit(line[0]) && line[0] != '\n') {
            list * map = list_new();
            long dest_start, source_start, length;
            while (scanf("%ld %ld %ld\n", &dest_start, &source_start, &length) == 3) {
                struct range * range = malloc(sizeof(struct range));
                range->dest_start = dest_start;
                range->source_start = source_start;
                range->length = length;
                list_add(map, range);
            }
            list_add(maps, map);
        }
    }
    long min_location_single = LONG_MAX;
    long min_location_range = LONG_MAX;
    for (int i = 0; i < list_size(seeds); i++) {
        long location_single;
        find_location(* (long *) list_get(seeds, i), maps, &location_single, NULL);
        if (location_single < min_location_single) min_location_single = location_single;
        if (!(i % 2)) {
            long location_range, location_range_range_size;
            long seed = * (long *) list_get(seeds, i);
            while (seed < * (long *) list_get(seeds, i) + * (long *) list_get(seeds, i+1)) {
                find_location(seed, maps, &location_range, &location_range_range_size);
                if (location_range < min_location_range) min_location_range = location_range;
                seed += location_range_range_size;
            }
        }
    }
    printf("Part one: %ld\n", min_location_single);
    printf("Part two: %ld\n", min_location_range);
}
