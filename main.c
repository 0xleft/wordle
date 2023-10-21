#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define GRAY "\033[1;30m"
#define RESET "\033[0m"
#define REPLACE "\033[1A\033[2K"

#define MAX_TRIES 5

int is_char_in_string(char c, char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (c == str[i]) {
            return 1;
        }
    }

    return 0;
}

void game_loop(char *words[1000], int word_len) {
    int word = rand() % word_len;
    char *word_to_guess = words[word];

    int word_to_guess_len = strlen(word_to_guess) - 1;
    int won = 0;

    for (size_t i = 0; i < MAX_TRIES; i++) {
        char input[100];
        fgets(input, 100, stdin);

        if (strlen(input) - 1 < word_to_guess_len) {
            printf(REPLACE);
            i--;
            continue;
        }

        printf(REPLACE);

        int correct = 0;

        for (size_t j = 0; j < word_to_guess_len; j++) {
            if (tolower(input[j]) == word_to_guess[j]) {
                correct++;
                printf(GREEN "%c" RESET, tolower(input[j]));
            } else if (is_char_in_string(tolower(input[j]), word_to_guess)) {
                printf(YELLOW "%c" RESET, tolower(input[j]));
            } else {
                printf(GRAY "%c" RESET, tolower(input[j]));
            }
        }

        if (correct == word_to_guess_len) {
            printf("\nYou won! The word was %s\n", word_to_guess);
            return;
        }

        printf("\n");
    }

    if (!won) {
        printf("\nYou lost! The word was %s\n", word_to_guess);
    }
}

int main() {

    srand(time(NULL));

    FILE *words_file;
    char *words[1000];

    words_file = fopen("words.txt", "r");
    if (words_file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int word_len = 0;
    char buffer[100];
    while (fgets(buffer, 100, words_file) != NULL) {
        if (strcmp(buffer, "\n") == 0) {
            continue;
        }

        // replace \n with \0 and \r\n with \0 probably better solution
        if (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        words[word_len] = malloc(sizeof(char) * 100);

        for (size_t i = 0; i < strlen(buffer); i++) {
            buffer[i] = tolower(buffer[i]);
        }

        strcpy(words[word_len], buffer);

        word_len++;
    }

    fclose(words_file);

    game_loop(words, word_len);

    for (size_t j = 0; j < word_len; j++) {
        // printf("%s", words[j]); // debug
        free(words[j]);
    }
    return 0;
}