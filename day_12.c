#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

long valid_arrangements(char * springs, list * groups, int curr_group, long * memo, int memo_shape[]) {
    long memo_val = memo[strlen(springs) * memo_shape[1] * memo_shape[2] + list_size(groups) * memo_shape[2] + curr_group];
    if (memo_val != -1) return memo_val;
    if (curr_group && !list_size(groups)) return 0;
    for (int i = 0; i < strlen(springs); i++) {
        switch (springs[i]) {
            case '#':
                if (!list_size(groups)) return 0;
                if (curr_group + 1 > (long) list_get(groups, 0)) return 0;
                curr_group++;
                break;
            case '.':
                if (curr_group && curr_group < (long) list_get(groups, 0)) return 0;
                if (curr_group && curr_group == (long) list_get(groups, 0)) {
                    list_remove(groups, 0);
                    curr_group = 0;
                }
                break;
            case '?':
                if (curr_group && curr_group < (long) list_get(groups, 0)) {
                    curr_group++;
                } else if (curr_group && curr_group == (long) list_get(groups, 0)) {
                    list_remove(groups, 0);
                    curr_group = 0;
                } else {
                    long arrangements_damaged = 0, arrangements_operational = 0;
                    list * groups_damaged = list_copy(groups);
                    list * groups_operational = list_copy(groups);
                    arrangements_damaged = valid_arrangements(springs + i + 1, groups_damaged, 1, memo, memo_shape);
                    arrangements_operational = valid_arrangements(springs + i + 1, groups_operational, 0, memo, memo_shape);
                    memo[strlen(springs + i + 1) * memo_shape[1] * memo_shape[2] + list_size(groups) * memo_shape[2] + 1] = arrangements_damaged;
                    memo[strlen(springs + i + 1) * memo_shape[1] * memo_shape[2] + list_size(groups) * memo_shape[2] + 0] = arrangements_operational;
                    list_free(groups_damaged);
                    list_free(groups_operational);
                    return arrangements_damaged + arrangements_operational;
                }
                break;
        }
    }
    if (list_size(groups) && curr_group == (long) list_get(groups, 0)) list_remove(groups, 0);
    if (list_size(groups)) return 0;
    return 1;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    long totals[2] = {0, 0};
    while (getline(&line, &len, stdin) != -1) {
        char * springs[2];
        springs[0] = malloc(strlen(line));
        sscanf(line, "%s", springs[0]);
        list * groups[2] = {list_new(), list_new()};
        char * token = strtok(line + strlen(springs[0]), " ,\n");
        while (token != NULL) {
            long group = atol(token);
            list_add(groups[0], (void *)  group);
            token = strtok(NULL, ",\n");
        }
        free(token);
        springs[1] = malloc((strlen(springs[0])) * 5 + 5);
        springs[1][0] = '\0';
        for (int i = 0; i < 5; i++) {
            strcat(springs[1], springs[0]);
            if (i < 4) strcat(springs[1], "?");
            list_concat(groups[1], groups[0]);
        }
        for (int i = 0; i < 2; i++) {
            int memo_shape[3];
            memo_shape[0] = strlen(springs[i]) + 1;
            memo_shape[1] = list_size(groups[i]) + 1;
            memo_shape[2] = 2;
            long * memo = malloc((memo_shape[0] * memo_shape[1] * memo_shape[2]) * sizeof(long));
            memset(memo, -1, (memo_shape[0] * memo_shape[1] * memo_shape[2]) * sizeof(long));
            long arrangements = valid_arrangements(springs[i], groups[i], 0, memo, memo_shape);
            totals[i] += arrangements;
            free(memo);
            free(springs[i]);
            list_free(groups[i]);
        }
    }
    free(line);
    printf("%ld\n", totals[0]);
    printf("%ld\n", totals[1]);
}
