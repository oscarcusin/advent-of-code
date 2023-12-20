#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

struct pipe {
    char ch;
    int con[2][2];
} pipes[] = {
    {'|', {{0, -1}, {0, 1}}},
    {'-', {{1, 0}, {-1, 0}}},
    {'L', {{0, -1}, {1, 0}}},
    {'J', {{0, -1}, {-1, 0}}},
    {'7', {{0, 1}, {-1, 0}}},
    {'F', {{0, 1}, {1, 0}}}
};

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * lines = list_new();
    list * circuit = list_new();
    int start_pos[2];
    while (getline(&line, &len, stdin) != -1) {
        char * line_copy = malloc(strlen(line) + 1);
        char * line_circuit = malloc(strlen(line) + 1);
        strcpy(line_copy, line);
        memset(line_circuit, '0', strlen(line));
        line_circuit[strlen(line)] = '\0';
        if (strchr(line_copy, 'S') != NULL) {
            start_pos[0] = strchr(line_copy, 'S') - line_copy;
            start_pos[1] = list_size(lines);
        }
        list_add(lines, line_copy);
        list_add(circuit, line_circuit);
    }
    free(line);
    int found = 0;
    int con[2][2];
    int curr_pos[2], curr_pipe_type, curr_pipe_exit;
    for (int y_offset = -1; y_offset <= 1; y_offset++) {
        if (start_pos[1] + y_offset < 0 || start_pos[1] + y_offset >= list_size(lines)) continue;
        char * line = list_get(lines, start_pos[1]+y_offset);
        for (int x_offset = -1; x_offset <= 1; x_offset++) {
            if (start_pos[0] + x_offset < 0 || start_pos[0] + x_offset >= strlen(line)) continue;
            char c = line[start_pos[0] + x_offset];
            if (c == '.') continue;
            for (int pipe_type = 0; pipe_type < 6; pipe_type++) {
                if (pipes[pipe_type].ch == c) {
                    if ((pipes[pipe_type].con[0][0] == -x_offset && pipes[pipe_type].con[0][1] == -y_offset)
                    || (pipes[pipe_type].con[1][0] == -x_offset && pipes[pipe_type].con[1][1] == -y_offset)) {
                        if (!found) {
                            found = 1;
                            curr_pipe_type = pipe_type;
                            curr_pipe_exit = (pipes[pipe_type].con[0][0] == -x_offset && pipes[pipe_type].con[0][1] == -y_offset) ? 1 : 0;
                            con[0][0] = x_offset, con[0][1] = y_offset;
                            curr_pos[0] = start_pos[0] + x_offset, curr_pos[1] = start_pos[1] + y_offset;
                            ((char *) list_get(circuit, curr_pos[1]))[curr_pos[0]] = pipes[pipe_type].ch;
                        } else con[1][0] = x_offset, con[1][1] = y_offset;
                        break;
                    }
                }
            }
        }
    }
    for (int pipe_type = 0; pipe_type < 6; pipe_type++) {
        if ((con[0][0] == pipes[pipe_type].con[0][0] && con[0][1] == pipes[pipe_type].con[0][1]
        && con[1][0] == pipes[pipe_type].con[1][0] && con[1][1] == pipes[pipe_type].con[1][1])
        || (con[0][0] == pipes[pipe_type].con[1][0] && con[0][1] == pipes[pipe_type].con[1][1]
        && con[1][0] == pipes[pipe_type].con[0][0] && con[1][1] == pipes[pipe_type].con[0][1])) {
            ((char *) list_get(circuit, start_pos[1]))[start_pos[0]] = pipes[pipe_type].ch;
            ((char *) list_get(lines, start_pos[1]))[start_pos[0]] = pipes[pipe_type].ch;
            break;
        }
    }
    int steps = 1;
    while (curr_pos[0] != start_pos[0] || curr_pos[1] != start_pos[1]) {
        int x_offset = pipes[curr_pipe_type].con[curr_pipe_exit][0];
        int y_offset = pipes[curr_pipe_type].con[curr_pipe_exit][1];
        for (int pipe_type = 0; pipe_type < 6; pipe_type++) {
            if (pipes[pipe_type].ch == ((char *) list_get(lines, curr_pos[1]+y_offset))[curr_pos[0] + x_offset]) {
                if ((pipes[pipe_type].con[0][0] == -x_offset && pipes[pipe_type].con[0][1] == -y_offset)
                || (pipes[pipe_type].con[1][0] == -x_offset && pipes[pipe_type].con[1][1] == -y_offset)) {
                    curr_pipe_type = pipe_type;
                    curr_pipe_exit = (pipes[pipe_type].con[0][0] == -x_offset && pipes[pipe_type].con[0][1] == -y_offset) ? 1 : 0;
                    curr_pos[0] += x_offset, curr_pos[1] += y_offset;
                    ((char *) list_get(circuit, curr_pos[1]))[curr_pos[0]] = pipes[pipe_type].ch;
                    steps++;
                    break;
                }
            }
        }
    }
    printf("Part one: %d\n", steps / 2);
    int tiles = 0;
    for (int y = 0; y < list_size(circuit); y++) {
        int north_flag = 0, south_flag = 0; 
        char * line = list_get(circuit, y);
        for (int x = 0; x < strlen(line)-1; x++) {
            if (line[x] == '|') north_flag = !north_flag, south_flag = !south_flag;
            if (line[x] == 'L' || line[x] == 'J') north_flag = !north_flag;
            if (line[x] == 'F' || line[x] == '7') south_flag = !south_flag;
            if (line[x] == '0' && north_flag && south_flag) tiles++;
        }
    }
    printf("Part two: %d\n", tiles);
    list_free_items(lines);
    list_free_items(circuit);
    list_free(lines);
    list_free(circuit);
}
