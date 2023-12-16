#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

#define CYCLES 1000000000

void tilt(list * platform, int direction) {
    int width = strlen(list_get(platform, 0)) - 2;
    int height = list_size(platform) - 1;
    for (int line = 0; line <= (direction % 2 == 0 ? width : height); line++) {
        int rocks = 0;
        for (int lineB = 0; lineB <= (direction % 2 == 0 ? height : width); lineB++) {
            char * c;
            switch (direction) {
                case 0: c = &((char *) list_get(platform, height - lineB))[line]; break;
                case 1: c = &((char *) list_get(platform, line))[width - lineB]; break;
                case 2: c = &((char *) list_get(platform, lineB))[line]; break;
                case 3: c = &((char *) list_get(platform, line))[lineB]; break;
            }
            if (*c == '#') {
                for (int rock = 1; rock <= rocks; rock++) {
                    switch (direction) {
                        case 0: ((char *) list_get(platform, height - lineB + rock))[line] = 'O'; break;
                        case 1: ((char *) list_get(platform, line))[width - lineB + rock] = 'O'; break;
                        case 2: ((char *) list_get(platform, lineB - rock))[line] = 'O'; break;
                        case 3: ((char *) list_get(platform, line))[lineB - rock] = 'O'; break;
                    }
                }
                rocks = 0;
            } else if (*c == 'O') {
                rocks++;
                *c = '.';
            }
        }
        for (int rock = 0; rock < rocks; rock++) {
            switch (direction) {
                case 0: ((char *) list_get(platform, rock))[line] = 'O'; break;
                case 1: ((char *) list_get(platform, line))[rock] = 'O'; break;
                case 2: ((char *) list_get(platform, height - rock))[line] = 'O'; break;
                case 3: ((char *) list_get(platform, line))[width - rock] = 'O'; break;
            }
        }
    }
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * platform = list_new();
    list * platform_cycle = list_new();
    list * initial_platform = list_new();
    list * platform_history = list_new();
    while (getline(&line, &len, stdin) != -1) {
        char * line_copy_a = malloc(strlen(line) + 1);
        char * line_copy_b = malloc(strlen(line) + 1);
        char * line_copy_c = malloc(strlen(line) + 1);
        strcpy(line_copy_a, line);
        strcpy(line_copy_b, line);
        strcpy(line_copy_c, line);
        list_add(platform, line_copy_a);
        list_add(platform_cycle, line_copy_b);
        list_add(initial_platform, line_copy_c);
    }
    list_add(platform_history, initial_platform);
    free(line);
    long total_load = 0;
    tilt(platform, 0);
    long total_load_cycle = 0;
    long cycle = 0;
    int found_loop = 0;
    while (cycle < CYCLES) {
        for (int direction = 0; direction < 4; direction++) tilt(platform_cycle, direction);
        cycle++;
        if (found_loop) continue;
        for (int prev = 0; prev < list_size(platform_history); prev++) {
            int is_same = 1;
            for (int i = 0; i < list_size(platform_cycle); i++) {
                if (strcmp(list_get(platform_cycle, i), list_get(list_get(platform_history, prev), i))) is_same = 0;
            }
            if (is_same) {
                found_loop = 1;
                int period = cycle - prev;
                int full_periods = (CYCLES - cycle) / period;
                cycle += full_periods * period;
                break;
            }
        }
        list * prev_platform = list_new();
        for (int row = 0; row < list_size(platform_cycle); row++) {
            char * line_copy = malloc(strlen(list_get(platform_cycle, row)) + 1);
            strcpy(line_copy, list_get(platform_cycle, row));
            list_add(prev_platform, line_copy);
        }
        list_add(platform_history, prev_platform);
    }
    for (int prev_platform = 0; prev_platform < list_size(platform_history); prev_platform++) {
        list * platform = list_get(platform_history, prev_platform);
        for (int row = 0; row < list_size(platform); row++) free(list_get(platform, row));
        list_delete(platform);
    }
    list_delete(platform_history);
    for (int row = 0; row < list_size(platform); row++) {
        for (int i = 0; i < strlen(list_get(platform, row)); i++) {
            if (((char *) list_get(platform, row))[i] == 'O') total_load += list_size(platform) - row;
        }
        free(list_get(platform, row));
    }
    list_delete(platform);
    for (int row = 0; row < list_size(platform_cycle); row++) {
        for (int i = 0; i < strlen(list_get(platform_cycle, row)); i++) {
            if (((char *) list_get(platform_cycle, row))[i] == 'O') total_load_cycle += list_size(platform_cycle) - row;
        }
        free(list_get(platform_cycle, row));
    }
    list_delete(platform_cycle);
    printf("Part one: %ld\n", total_load);
    printf("Part two: %ld\n", total_load_cycle);
}
