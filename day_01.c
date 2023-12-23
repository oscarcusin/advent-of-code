#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char * digits_in_letters[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int main(int argc, char * argv[]) {
    char * line = NULL;
    size_t buff_len = 0;
    int digits_only_sum = 0;
    int combined_sum = 0;
    while (getline(&line, &buff_len, stdin) != -1) {
        char left_digit = 0, right_digit = 0, left_digits_only = 0, right_digits_only = 0;
        for (int l = 0; l < strlen(line); l++) {
            int r = strlen(line) - l - 1;
            if (isdigit(line[l]) && !left_digits_only) left_digits_only = line[l];
            if (isdigit(line[r]) && !right_digits_only) right_digits_only = line[r];
            if (isdigit(line[l]) && !left_digit) left_digit = line[l];
            if (isdigit(line[r]) && !right_digit) right_digit = line[r];
            for (int i = 0; i < 9; i++) {
                if (!memcmp(line + l, digits_in_letters[i], strlen(digits_in_letters[i])) && !left_digit) {
                    left_digit = i + 1 + '0';
                }
                if (!memcmp(line + r, digits_in_letters[i], strlen(digits_in_letters[i])) && !right_digit) {
                    right_digit = i + 1 + '0';
                }
            }
        }
        if (left_digits_only && right_digits_only) digits_only_sum += (left_digits_only - '0') * 10 + right_digits_only - '0';
        combined_sum += (left_digit - '0') * 10 + right_digit - '0';
    }
    free(line);
    printf("%d\n", digits_only_sum);
    printf("%d\n", combined_sum);
}
