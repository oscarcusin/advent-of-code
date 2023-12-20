#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

struct galaxy_coords {
    int x;
    int y;
    long x_older;
    long y_older;
};

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * galaxies = list_new();
    int line_number = 0;
    int max[2] = {-1, -1};
    while (getline(&line, &len, stdin) != -1) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '#') {
                struct galaxy_coords * coords = malloc(sizeof(struct galaxy_coords));
                coords->x = i;
                coords->y = line_number;
                coords->x_older = i;
                coords->y_older = line_number;
                list_add(galaxies, coords);
                if (i > max[0]) max[0] = i;
                if (line_number > max[1]) max[1] = line_number;
            }
        }
        line_number++;
    }
    free(line);
    for (int row_column_flag = 0; row_column_flag < 2; row_column_flag++) {
        for (int i = max[row_column_flag]; i >= 0; i--) {
            int contains_galaxy = 0;
            for (int galaxy = 0; galaxy < list_size(galaxies); galaxy++) {
                if (!row_column_flag && ((struct galaxy_coords *) list_get(galaxies, galaxy))->x == i) contains_galaxy = 1;
                if (row_column_flag && ((struct galaxy_coords *) list_get(galaxies, galaxy))->y == i) contains_galaxy = 1;
            }
            if (!contains_galaxy) {
                for (int galaxy = 0; galaxy < list_size(galaxies); galaxy++) {
                    struct galaxy_coords * coords = list_get(galaxies, galaxy);
                    if (!row_column_flag && coords->x > i) {
                        coords->x += 1;
                        coords->x_older += 999999;
                    }
                    if (row_column_flag && coords->y > i) {
                        coords->y += 1;
                        coords->y_older += 999999;
                    }
                }
            }
        }
    }
    long total_distance = 0;
    long total_distance_older = 0;
    for (int i = 0; i < list_size(galaxies); i++) {
        for (int j = i + 1; j < list_size(galaxies); j++) {
            struct galaxy_coords * coords1 = list_get(galaxies, i);
            struct galaxy_coords * coords2 = list_get(galaxies, j);
            total_distance += abs(coords1->x - coords2->x) + abs(coords1->y - coords2->y);
            total_distance_older += labs(coords1->x_older - coords2->x_older) + labs(coords1->y_older - coords2->y_older);
        }
    }
    list_free_items(galaxies);
    list_free(galaxies);
    printf("Part one: %ld\n", total_distance);
    printf("Part one: %ld\n", total_distance_older);
}
