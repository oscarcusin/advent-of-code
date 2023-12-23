#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

struct lens {
    char * label;
    int focal_length;
};

int hash(char * str) {
    int hash_value = 0;
    for (int i = 0; i < strlen(str); i++) hash_value = ((hash_value + str[i]) * 17) % 256;
    return hash_value;
}

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    char * step;
    step = strtok(line, ",\n");
    long hash_sum = 0, focusing_power = 0;
    list * boxes[256];
    for (int i = 0; i < 256; i++) boxes[i] = list_new();
    while (step != NULL) {
        hash_sum += hash(step);
        int focal_length = -1;
        char label[16];
        sscanf(step, "%[a-z] %*[-=] %d", label, &focal_length);
        if (focal_length == -1) {
            list * box = boxes[hash(label)];
            for (int i = 0; i < list_size(box); i++) {
                struct lens * lens = list_get(box, i);
                if (strcmp(lens->label, label) == 0) {
                    free(lens->label);
                    free(lens);
                    list_remove(box, i);
                    break;
                }
            }
        } else {
            int found = 0;
            for (int i = 0; i < list_size(boxes[hash(label)]); i++) {
                struct lens * lens = list_get(boxes[hash(label)], i);
                if (strcmp(lens->label, label) == 0) {
                    lens->focal_length = focal_length;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                struct lens * lens = malloc(sizeof(struct lens));
                lens->label = malloc(strlen(label) + 1);
                strcpy(lens->label, label);
                lens->focal_length = focal_length;
                list_add(boxes[hash(label)], lens);
            }
        }
        step = strtok(NULL, ",\n");
    }
    for (int box_index = 0; box_index < 256; box_index++) {
        list * box = boxes[box_index];
        for (int lens_index = 0; lens_index < list_size(box); lens_index++) {
            struct lens * lens = list_get(box, lens_index);
            focusing_power += (box_index + 1) * (lens_index + 1) * lens->focal_length;
            free(lens->label);
            free(lens);
        }
        list_free(box);
    }
    free(line);
    printf("%ld\n", hash_sum);
    printf("%ld\n", focusing_power);
}
