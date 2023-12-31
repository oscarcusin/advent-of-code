#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/list.h"

struct node {
    char name[4];
    char left[4];
    char right[4];
};

int main(int argc, char * argv[]) {
    char * instructions = NULL;
    size_t buff_len = 0;
    getline(&instructions, &buff_len, stdin);
    char node[4];
    char left[4];
    char right[4];
    list * nodes = list_new();
    struct node * curr_node;
    list * curr_node_ghosts = list_new();
    while (scanf("%3s = (%3s, %3s)\n", node, left, right) == 3) {
        struct node * new_node = malloc(sizeof(struct node));
        strcpy(new_node->name, node);
        strcpy(new_node->left, left);
        strcpy(new_node->right, right);
        list_add(nodes, new_node);
        if (strcmp(node, "AAA") == 0) curr_node = new_node;
        if (node[2] == 'A') list_add(curr_node_ghosts, new_node);
    }
    int steps = 0;
    if (strcmp(curr_node->name, "AAA") == 0) {
        while (strcmp(curr_node->name, "ZZZ") != 0) {
            for (int instruction = 0; instruction < strlen(instructions)-1; instruction++) {
                int direction = instructions[instruction] == 'L' ? 1 : 0;
                for (int node_index = 0; node_index < list_size(nodes); node_index++) {
                    struct node * next_node = list_get(nodes, node_index);
                    if (strcmp(next_node->name, direction ? curr_node->left : curr_node->right) == 0) {
                        curr_node = next_node;
                        break;
                    }
                }
            }
            steps += strlen(instructions)-1;
        }
    }
    int iterations_to_end[list_size(curr_node_ghosts)];
    for (int i = 0; i < list_size(curr_node_ghosts); i++) iterations_to_end[i] = -1;
    int iterations = 0;
    int reached_end = 0;
    while (reached_end < list_size(curr_node_ghosts)) {
        for (int instruction = 0; instruction < strlen(instructions)-1; instruction++) {
            int direction = instructions[instruction] == 'L' ? 1 : 0;
            for (int ghost_index = 0; ghost_index < list_size(curr_node_ghosts); ghost_index++) {
                if (iterations_to_end[ghost_index] != -1) continue;
                struct node * curr_node_ghost = list_get(curr_node_ghosts, ghost_index);
                for (int node_index = 0; node_index < list_size(nodes); node_index++) {
                    struct node * next_node = list_get(nodes, node_index);
                    if (strcmp(next_node->name, direction ? curr_node_ghost->left : curr_node_ghost->right) == 0) {
                        list_set(curr_node_ghosts, ghost_index, next_node);
                        break;
                    }
                }
            }
        }
        iterations++;
        for (int ghost_index = 0; ghost_index < list_size(curr_node_ghosts); ghost_index++) {
            if (iterations_to_end[ghost_index] != -1) continue;
            struct node * curr_node_ghost = list_get(curr_node_ghosts, ghost_index);
            if (curr_node_ghost->name[2] == 'Z') {
                iterations_to_end[ghost_index] = iterations;
                reached_end++;
            }
        }
    }
    long lcm = iterations_to_end[0];
    for (int i = 1; i < list_size(curr_node_ghosts); i++) {
        long a = lcm;
        long b = iterations_to_end[i];
        while (b != 0) {
            long temp = b;
            b = a % b;
            a = temp;
        }
        lcm = (lcm * iterations_to_end[i]) / a;
    }
    list_free_items(nodes);
    list_free(nodes);
    list_free(curr_node_ghosts);
    printf("%d\n", steps);
    printf("%ld\n", lcm*(strlen(instructions)-1));
    free(instructions);
}
