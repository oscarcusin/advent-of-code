#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilities/list.h"

#define MAX_RATING 4000

typedef struct condition {
    int rating;
    char operator;
    int value;
    char target[4];
} condition;

typedef struct workflow {
    char name[4];
    list * conditions;
    char final_target[4];
} workflow;

int part_is_accepted(int ratings[], list * workflows, char * start) {
    workflow * curr;
    for (int i = 0; i < list_size(workflows); i++) {
        curr = list_get(workflows, i);
        if (strcmp(curr->name, start) == 0) break;
    }
    while (1) {
        char next_target[4];
        int found = 0;
        for (int i = 0; i < list_size(curr->conditions); i++) {
            condition * condition = list_get(curr->conditions, i);
            if ((condition->operator == '>' && ratings[condition->rating] > condition->value)
            || (condition->operator == '<' && ratings[condition->rating] < condition->value)) {
                strcpy(next_target, condition->target);
                found = 1;
                break;
            }
        }
        if (!found) strcpy(next_target, curr->final_target);
        if (strcmp(next_target, "A") == 0) return 1;
        if (strcmp(next_target, "R") == 0) return 0;
        for (int i = 0; i < list_size(workflows); i++) {
            workflow * next = list_get(workflows, i);
            if (strcmp(next->name, next_target) == 0) {
                curr = next;
                break;
            }
        }
    }
}

long distinct_combinations(int range[4][2], list * workflows, char * target)  {
    if (strcmp(target, "R") == 0) return 0;
    if (strcmp(target, "A") == 0) {
        return ((long) range[0][1] - range[0][0] + 1)
        * ((long) range[1][1] - range[1][0] + 1)
        * ((long) range[2][1] - range[2][0] + 1)
        * ((long) range[3][1] - range[3][0] + 1);
    }
    workflow * wf;
    for (int i = 0; i < list_size(workflows); i++) {
        wf = list_get(workflows, i);
        if (strcmp(wf->name, target) == 0) break;
    }
    long combinations = 0;
    for (int i = 0; i < list_size(wf->conditions); i++) {
        condition * cond = list_get(wf->conditions, i);
        if ((cond->operator == '<' && range[cond->rating][1] < cond->value)
        || (cond->operator == '>' && range[cond->rating][0] > cond->value)) {
            combinations += distinct_combinations(range, workflows, cond->target);
        }
        else if ((cond->operator == '<' && range[cond->rating][0] < cond->value)
        || (cond->operator == '>' && range[cond->rating][1] > cond->value)) {
            int new_ranges[4][2];
            memcpy(new_ranges, range, 8 * sizeof(int));
            new_ranges[cond->rating][cond->operator == '<'] = cond->operator == '<' ? cond->value - 1 : cond->value + 1;
            combinations += distinct_combinations(new_ranges, workflows, cond->target);
            range[cond->rating][cond->operator == '>'] = cond->value;
        }
    }
    combinations += distinct_combinations(range, workflows, wf->final_target);
    return combinations;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * workflows = list_new();
    while (getline(&line, &len, stdin) != -1) {
        if (line[0] == '\n') break;
        workflow * wf = malloc(sizeof(workflow));
        wf->conditions = list_new();
        char name[4];
        char string[strlen(line)];
        sscanf(line, "%[a-z]{%[a-zA-Z0-9<>:,]}\n", name, string);
        strcpy(wf->name, name);
        char * token = strtok(string, ",");
        while (token != NULL) {
            if (token[1] != '<' && token[1] != '>') {
                strcpy(wf->final_target, token);
                break;
            }
            condition * cond = malloc(sizeof(condition));
            switch (token[0]) {
                case 'x': cond->rating = 0; break;
                case 'm': cond->rating = 1; break;
                case 'a': cond->rating = 2; break;
                case 's': cond->rating = 3; break;
            }
            cond->operator = token[1];
            cond->value = atoi(token + 2);
            char * colon = strchr(token, ':');
            strcpy(cond->target, colon + 1);
            list_add(wf->conditions, cond);
            token = strtok(NULL, ",");
        }
        list_add(workflows, wf);
    }
    free(line);
    long total = 0;
    int ratings[4] = {0};
    while (scanf("{x=%d,m=%d,a=%d,s=%d}\n", &ratings[0], &ratings[1], &ratings[2], &ratings[3]) == 4) {
        if (part_is_accepted(ratings, workflows, "in")) {
            total += ratings[0] + ratings[1] + ratings[2] + ratings[3];
        }
    }
    printf("%ld\n", total);
    long combinations = 0;
    int range[4][2];
    for (int i = 0; i < 4; i++) {
        range[i][0] = 1;
        range[i][1] = 4000;
    }
    combinations = distinct_combinations(range, workflows, "in");
    printf("%ld\n", combinations);
    for (int i = 0; i < list_size(workflows); i++) {
        workflow * wf = list_get(workflows, i);
        for (int j = 0; j < list_size(wf->conditions); j++) free(list_get(wf->conditions, j));
        list_free(wf->conditions);
        free(wf);
    }
    list_free(workflows);
}
