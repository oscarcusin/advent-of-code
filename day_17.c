#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"
#include "utilities/queue.h"

struct item {int x, y, dir;};

int find_heat_loss(list * map, int start_x, int start_y, int end_x, int end_y, int min_step, int max_step) {
    int width = strlen(list_get(map, 0));
    int height = list_size(map);
    unsigned int dist[height][width][4];
    memset(dist, 0xff, sizeof(dist));
    dist[start_y][start_x][0] = dist[start_y][start_x][1] = dist[start_y][start_x][2] = dist[start_y][start_x][3] = 0;
    queue * q = queue_new();
    struct item * start = malloc(sizeof(struct item));
    start->x = start_x;
    start->y = start_y;
    start->dir = -1;
    queue_enqueue(q, start);
    unsigned int min_dist = 0x7FFFFFFF;
    while (queue_size(q)) {
        struct item * curr = queue_dequeue(q);
        if (curr->x == end_x && curr->y == end_y) {
            for (int dir = 0; dir < 4; dir++) if (dist[end_y][end_x][dir] < min_dist) min_dist = dist[end_y][end_x][dir];
        }
        for (int dir = 0; dir < 4; dir++) {
            if (dir == curr->dir || (dir + 2) % 4 == curr->dir) continue;
            for (int step = min_step; step <= max_step; step++) {
                if (dir == 0 && curr->x + step >= width) continue;
                if (dir == 1 && curr->y + step >= height) continue;
                if (dir == 2 && curr->x - step < 0) continue;
                if (dir == 3 && curr->y - step < 0) continue;
                int new_dist = dist[curr->y][curr->x][curr->dir == -1 ? dir : curr->dir];
                for (int distance = 1; distance <= step; distance++) {
                    switch (dir) {
                        case 0: new_dist += ((char *) list_get(map, curr->y))[curr->x + distance] - '0'; break;
                        case 1: new_dist += ((char *) list_get(map, curr->y + distance))[curr->x] - '0'; break;
                        case 2: new_dist += ((char *) list_get(map, curr->y))[curr->x - distance] - '0'; break;
                        case 3: new_dist += ((char *) list_get(map, curr->y - distance))[curr->x] - '0'; break;
                    }
                }
                if (new_dist >= min_dist) continue;
                unsigned int * curr_dist;
                switch (dir) {
                    case 0: curr_dist = &dist[curr->y][curr->x + step][dir]; break;
                    case 1: curr_dist = &dist[curr->y + step][curr->x][dir]; break;
                    case 2: curr_dist = &dist[curr->y][curr->x - step][dir]; break;
                    case 3: curr_dist = &dist[curr->y - step][curr->x][dir]; break;
                }
                if (new_dist < *curr_dist) {
                    *curr_dist = new_dist;
                    struct item * item = malloc(sizeof(struct item));
                    item->x = dir == 0 ? curr->x + step : dir == 2 ? curr->x - step : curr->x;
                    item->y = dir == 1 ? curr->y + step : dir == 3 ? curr->y - step : curr->y;
                    item->dir = dir;
                    queue_enqueue(q, item);
                }
            }
        }
        free(curr);
    }
    queue_free(q);
    return min_dist;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * map = list_new();
    while (getline(&line, &len, stdin) != -1) {
        char * line_copy = malloc(strlen(line) + 1);
        strcpy(line_copy, line);
        line_copy[strlen(line_copy) - 1] = '\0';
        list_add(map, line_copy);
    }
    free(line);
    printf("%d\n", find_heat_loss(map, 0, 0, strlen(list_get(map, 0)) - 1, list_size(map) - 1, 1, 3));
    printf("%d\n", find_heat_loss(map, 0, 0, strlen(list_get(map, 0)) - 1, list_size(map) - 1, 4, 10));
    list_free_items(map);
    list_free(map);
}
