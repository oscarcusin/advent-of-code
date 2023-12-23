#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

int simulate_beam(list * layout, list * tiles, int x, int y, int dir, char * computed_beams) {
    int energized_tiles = 0;
    while (x >= 0 && x < strlen(list_get(layout, 0)) && y >= 0 && y < list_size(layout)) {
        if (computed_beams[y * strlen(list_get(layout, 0)) + x] & (1 << dir)) return energized_tiles;
        if (((char *) list_get(tiles, y))[x] == '.') {
            ((char *) list_get(tiles, y))[x] = '#';
            energized_tiles++;
        }
        computed_beams[y * strlen(list_get(layout, 0)) + x] |= (1 << dir);
        switch (((char *) list_get(layout, y))[x]) {
            case '/':
                switch (dir) {
                    case 0: dir = 3; break;
                    case 1: dir = 2; break;
                    case 2: dir = 1; break;
                    case 3: dir = 0; break;
                }
                break;
            case '\\':
                switch (dir) {
                    case 0: dir = 1; break;
                    case 1: dir = 0; break;
                    case 2: dir = 3; break;
                    case 3: dir = 2; break;
                }
                break;
            case '|':
                if (!(dir % 2)) {
                    energized_tiles += simulate_beam(layout, tiles, x, y + 1, 1, computed_beams);
                    energized_tiles += simulate_beam(layout, tiles, x, y - 1, 3, computed_beams);
                    return energized_tiles;
                }
                break;
            case '-':
                if (dir % 2) {
                    energized_tiles += simulate_beam(layout, tiles, x + 1, y, 0, computed_beams);
                    energized_tiles += simulate_beam(layout, tiles, x - 1, y, 2, computed_beams);
                    return energized_tiles;
                }
                break;
        }
        switch (dir) {
            case 0: x++; break;
            case 1: y++; break;
            case 2: x--; break;
            case 3: y--; break;
        }
    }
    return energized_tiles;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * layout = list_new();
    while (getline(&line, &len, stdin) != -1) {
        char * line_copy = malloc(strlen(line));
        memcpy(line_copy, line, strlen(line) - 1);
        line_copy[strlen(line) - 1] = '\0';
        list_add(layout, line_copy);
    }
    free(line);
    int width = strlen(list_get(layout, 0)), height = list_size(layout);
    int energized_tiles_top_left = 0;
    int max_energized_tiles = 0;
    for (int i = 0; i < (width > height ? width : height); i++) {
        for (int dir = 0; dir < 4; dir++) {
            char * computed_beams = calloc(height * width, sizeof(char));
            list * tiles = list_new();
            for (int i = 0; i < height; i++) {
                char * line = malloc(width + 1);
                memset(line, '.', width);
                line[width] = '\0';
                list_add(tiles, line);
            }
            if (dir % 2 && i < width) {
                int energized_tiles = 0;
                energized_tiles = simulate_beam(layout, tiles, i, (dir == 1 ? 0 : list_size(layout) - 1), dir, computed_beams);
                if (energized_tiles > max_energized_tiles) max_energized_tiles = energized_tiles;
            }
            else if (!(dir % 2) && i < height) {
                int energized_tiles = 0;
                energized_tiles = simulate_beam(layout, tiles, (dir == 0 ? 0 : strlen(list_get(layout, 0)) - 1), i, dir, computed_beams);
                if (energized_tiles > max_energized_tiles) max_energized_tiles = energized_tiles;
                if (dir == 0 && i == 0) energized_tiles_top_left = energized_tiles;
            }
            free(computed_beams);
            list_free_items(tiles);
            list_free(tiles);
        }
    }
    list_free_items(layout);
    list_free(layout);
    printf("%d\n", energized_tiles_top_left);
    printf("%d\n", max_energized_tiles);
}
