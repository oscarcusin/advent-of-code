#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
    char dir;
    int dist;
    char color_code[7];
    int x_pos = 0;
    int perimeter = 0;
    int area = 0;
    long x_pos_color = 0;
    long perimeter_color = 0;
    long area_color = 0;
    while (scanf("%c %d (#%[0-9a-z])\n", &dir, &dist, color_code) != EOF) {
        switch (dir) {
            case 'R': x_pos += dist; break;
            case 'L': x_pos -= dist; break;
            case 'D': area += x_pos * dist; break;
            case 'U': area += x_pos * -dist; break;
        }
        perimeter += dist;
        char dir_color;
        switch (color_code[5]) {
            case '0': dir_color = 'R'; break;
            case '1': dir_color = 'D'; break;
            case '2': dir_color = 'L'; break;
            case '3': dir_color = 'U'; break;
        }
        color_code[5] = '\0';
        long dist_color = strtol(color_code, NULL, 16);
        switch (dir_color) {
            case 'R': x_pos_color += dist_color; break;
            case 'L': x_pos_color -= dist_color; break;
            case 'D': area_color += x_pos_color * dist_color; break;
            case 'U': area_color += x_pos_color * -dist_color; break;
        }
        perimeter_color += dist_color;
    }
    printf("%d\n", area + perimeter / 2 + 1);
    printf("%ld\n", area_color + perimeter_color / 2 + 1);
}
