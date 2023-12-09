#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utilities/list.h"

struct hand {
    char cards[5];
    int bid;
    int rank;
    int rank_joker;
};

char cards[] = "23456789TJQKA";
char cards_joker[] = "J23456789TQKA";

int hand_type(struct hand * hand) {
    int freq[6] = {0};
    for (int label = 0; label < 13; label++) {
        int count = 0;
        for (int i = 0; i < 5; i++) {
            if (hand->cards[i] == cards[label]) count++;
        }
        freq[count]++;
    }
    if (freq[5] > 0) return 0;
    if (freq[4] > 0) return 1;
    if (freq[3] > 0 && freq[2] > 0) return 2;
    if (freq[3] > 0) return 3;
    if (freq[2] > 1) return 4;
    if (freq[2] > 0) return 5;
    return 6;
}

int hand_type_joker(struct hand * hand) {
    int freq[6] = {0};
    int jokers = 0;
    for (int i = 0; i < 5; i++) {
        if (hand->cards[i] == 'J') jokers++;
    }
    for (int label = 1; label < 13; label++) {
        int count = 0;
        for (int i = 0; i < 5; i++) {
            if (hand->cards[i] == cards_joker[label]) count++;
        }
        freq[count]++;
    }
    for (int i = 0; i <= 5; i++) if (freq[5-i] > 0 && jokers >= i) return 0;
    for (int i = 0; i <= 4; i++) if (freq[4-i] > 0 && jokers >= i) return 1;
    if ((freq[3] > 0 && freq[2] > 0) || (freq[2] > 1 && jokers)) return 2;
    for (int i = 0; i <= 3; i++) if (freq[3-i] > 0 && jokers >= i) return 3;
    if (freq[2] > 1) return 4;
    if (freq[2] > 0 || jokers) return 5;
    return 6;
}

void rank(list * hands, list * types, int joker) {
    int rank = list_size(hands);
    for (int type = 0; type < 7; type++) {
        list * hands_of_type = list_get(types, type);
        int strength[list_size(hands_of_type)];
        for (int hand_index = 0; hand_index < list_size(hands_of_type); hand_index++) {
            struct hand * hand = list_get(hands_of_type, hand_index);
            strength[hand_index] = 0;
            for (int card = 0; card < 5; card++) {
                char * cards_ptr = joker ? cards_joker : cards;
                strength[hand_index] += (strchr(cards_ptr, hand->cards[card]) - cards_ptr) * (pow(13, 4 - card));
            }
        }
        for (int i = 0; i < list_size(hands_of_type); i++) {
            int max = -1;
            int max_index = 0;
            for (int j = 0; j < list_size(hands_of_type); j++) {
                if (strength[j] > max) {
                    max = strength[j];
                    max_index = j;
                }
            }
            strength[max_index] = -1;
            struct hand * hand = list_get(hands_of_type, max_index);
            if (joker) hand->rank_joker = rank--;
            else hand->rank = rank--;
        }
    }
}

int main(int argc, char * argv[]) {
    list * hands = list_new();
    char hand_input[6] = "";
    int bid_input = 0;
    while(scanf("%s %d\n", hand_input, &bid_input) == 2) {
        struct hand * h = malloc(sizeof(struct hand));
        strcpy(h->cards, hand_input);
        h->bid = bid_input;
        list_add(hands, h);
    }
    list * types = list_new();
    list * types_joker = list_new();
    for (int i = 0; i < 7; i++) list_add(types, list_new());
    for (int i = 0; i < 7; i++) list_add(types_joker, list_new());
    for (int hand_index = 0; hand_index < list_size(hands); hand_index++) {
        struct hand * hand = list_get(hands, hand_index);
        list_add(list_get(types, hand_type(hand)), hand);
        list_add(list_get(types_joker, hand_type_joker(hand)), hand);
    }
    rank(hands, types, 0);
    rank(hands, types_joker, 1);
    for (int i = 0; i < 7; i++) list_delete(list_get(types, i));
    for (int i = 0; i < 7; i++) list_delete(list_get(types_joker, i));
    list_delete(types);
    list_delete(types_joker);
    long winnings = 0;
    long winnings_joker = 0;
    for (int i = 0; i < list_size(hands); i++) {
        struct hand * hand = list_get(hands, i);
        winnings += hand->rank * hand->bid;
        winnings_joker += hand->rank_joker * hand->bid;
    }
    for (int i = 0; i < list_size(hands); i++) free(list_get(hands, i));
    list_delete(hands);
    printf("Part one: %ld\n", winnings);
    printf("Part two: %ld\n", winnings_joker);
}
