#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

int main(int argc, char * argv[]) {
    int game_id_sum = 0;
    int power_sum = 0;
    int game_id = 0;
    while (scanf("Game %d:", &game_id) == 1) {
        int valid = 1;
        int cubes;
        int max_red = 0, max_green = 0, max_blue = 0;
        char * colour = malloc(6 * sizeof(char));
        while (scanf("%d %[a-z]", &cubes, colour) == 2) {
            if (!strcmp(colour, "red")) {
                if (cubes > MAX_RED) valid = 0;
                if (cubes > max_red) max_red = cubes;
            }
            if (!strcmp(colour, "green")) {
                if (cubes > MAX_GREEN) valid = 0;
                if (cubes > max_green) max_green = cubes;
            }
            if (!strcmp(colour, "blue")) {
                if (cubes > MAX_BLUE) valid = 0;
                if (cubes > max_blue) max_blue = cubes;
            }
            getchar();
        }
        free(colour);
        if (valid) game_id_sum += game_id;
        power_sum += max_red * max_green * max_blue;
    }
    printf("%d\n", game_id_sum);
    printf("%d\n", power_sum);
}
