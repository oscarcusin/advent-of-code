#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    list * patterns = list_new();
    list_add(patterns, list_new());
    while (getline(&line, &len, stdin) != -1) {
        if (line[0] == '\n') list_add(patterns, list_new());
        else {
            char * line_copy = malloc(strlen(line) + 1);
            strcpy(line_copy, line);
            list_add(list_get(patterns, list_size(patterns) - 1), line_copy);
        }
    }
    free(line);
    long total_sum = 0;
    long total_sum_smudge = 0;
    for (int pattern_index = 0; pattern_index < list_size(patterns); pattern_index++) {
        list * pattern = list_get(patterns, pattern_index);
        int width = strlen(list_get(pattern, 0)) - 1;
        int height = list_size(pattern);
        for (int is_horizontal = 0; is_horizontal <= 1; is_horizontal++) {
            int diffs;
            for (int reflection_line = 1; reflection_line < (is_horizontal ? height : width); reflection_line++) {
                diffs = 0;
                int dist = 0;
                while (reflection_line - 1 - dist >= 0 && reflection_line + dist < (is_horizontal ? height : width)) {
                    char a, b;
                    for (int i = 0; i < (is_horizontal ? width : height); i++) {
                        if (is_horizontal) {
                            a = ((char *) list_get(pattern, reflection_line - 1 - dist))[i];
                            b = ((char *) list_get(pattern, reflection_line + dist))[i];
                        } else {
                            a = ((char *) list_get(pattern, i))[reflection_line -1 - dist];
                            b = ((char *) list_get(pattern, i))[reflection_line + dist];
                        }
                        if (a != b) diffs++;
                    }
                    dist++;
                }
                if (diffs == 0) {
                    if (is_horizontal) total_sum += 100 * reflection_line;
                    else total_sum += reflection_line;
                } else if (diffs == 1) {
                    if (is_horizontal) total_sum_smudge += 100 * reflection_line;
                    else total_sum_smudge += reflection_line;
                }
            }
        }
        for (int i = 0; i < list_size(pattern); i++) free(list_get(pattern, i));
        list_delete(pattern);
    }
    list_delete(patterns);
    printf("Part one: %ld\n", total_sum);
    printf("Part two: %ld\n", total_sum_smudge);
}
