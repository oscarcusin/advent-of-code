#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t buff_len = 0;
    list * lines = list_new();
    while (getline(&line, &buff_len, stdin) != -1) {
        char * line_copy = malloc(strlen(line) + 1);
        strcpy(line_copy, line);
        list_add(lines, line_copy);
    }
    free(line);
    long parts_sum = 0;
    long gears_sum = 0;
    for (int line_index = 0; line_index < list_size(lines); line_index++) {
        char * line = list_get(lines, line_index);
        for (int char_index = 0; char_index < strlen(line) - 1; char_index++) {
            if (!isdigit(line[char_index]) && line[char_index] != '.') {
                list * numbers = list_new();
                for (int i = (line_index - 1 > 0 ? line_index - 1 : 0); i <= (line_index + 1 < list_size(lines) - 1 ? line_index + 1 : list_size(lines) - 1); i++) {
                    int found_number = 0;
                    int start_index = -1;
                    int end_index = -1;
                    for (int j = (char_index - 1 > 0 ? char_index - 1 : 0); j <= (char_index + 1 < strlen(line) - 1 ? char_index + 1 : strlen(line) - 1); j++) {
                        if (end_index > j) continue;
                        char * line2;
                        if (i == line_index) line2 = line;
                        else line2 = list_get(lines, i);
                        if (isdigit(line2[j]) && !found_number) {
                            found_number = 1;
                            start_index = j;
                            end_index = j;
                            while (start_index >= 0 && isdigit(line2[start_index])) start_index--;
                            start_index++;
                            while (end_index < strlen(line2) - 1 && isdigit(line2[end_index])) end_index++;
                            char * number = strndup(line2 + start_index, end_index - start_index);
                            list_add(numbers, (void *) atol(number));
                            free(number);
                        }
                        if (!isdigit(line2[j])) found_number = 0;
                    }
                }
                for (int i = 0; i < list_size(numbers); i++) {
                    parts_sum += (long) list_get(numbers, i);
                }
                if (list_size(numbers) == 2) {
                    gears_sum += (long) list_get(numbers, 0) * (long) list_get(numbers, 1);
                }
                list_free(numbers);
            }
        }
    }
    list_free_items(lines);
    list_free(lines);
    printf("%ld\n", parts_sum);
    printf("%ld\n", gears_sum);
}