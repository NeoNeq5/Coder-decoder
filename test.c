#include <stdio.h>
#include "file1.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 32
#define MAX_CHANGE 10

void print_array(int decodingTab[]) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", decodingTab[i]);
    }
    printf("\n");
}

int decode_and_print(int decodingTab[], int indices[], int num_indices, int new_value) {
    int original_values[num_indices];
    int decode_result;

    // Zapisywanie oryginalnych wartości
    for (int i = 0; i < num_indices; i++) {
        original_values[i] = decodingTab[indices[i]];
        decodingTab[indices[i]] = new_value;
    }

    // Dekodowanie zmienionej tablicy
    decode_result = decodeTab(decodingTab);

    // Wypisanie zmienionej tablicy
    //print_array(decodingTab);

    // Przywrócenie oryginalnych wartości
    for (int i = 0; i < num_indices; i++) {
        decodingTab[indices[i]] = original_values[i];
    }

    return decode_result;
}

int generate_combinations(int n, int k, int indices[], int decodingTab[]) {
    int originalDecodingTab[ARRAY_SIZE];
    memcpy(originalDecodingTab, decodingTab, sizeof(originalDecodingTab));
    int count = 0;
    int successful_decodes = 0;

    if (k == 2) {
        // Pełne generowanie kombinacji dla dwóch elementów
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                indices[0] = i;
                indices[1] = j;

                if (decode_and_print(decodingTab, indices, 2, -1)) {
                    successful_decodes++;
                }
                count++;

                // Przywracanie pełnego stanu tablicy bazowej po każdej kombinacji
                memcpy(decodingTab, originalDecodingTab, sizeof(originalDecodingTab));
            }
        }
    } else {
        // Generowanie kombinacji dla większej liczby elementów
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                indices[0] = i;
                indices[1] = j;

                // Losowanie pozostałych elementów
                for (int l = 2; l < k; l++) {
                    indices[l] = rand() % n;
                }

                // Wypisanie indeksów dla przejrzystości
                // printf("Wybrane pozycje: ");
                // for (int l = 0; l < k; l++) {
                //     printf("%d ", indices[l]);
                // }
                // printf("\n");

                if (decode_and_print(decodingTab, indices, k, -1)) {
                    successful_decodes++;
                }
                count++;

                // Przywracanie pełnego stanu tablicy bazowej po każdej kombinacji
                memcpy(decodingTab, originalDecodingTab, sizeof(originalDecodingTab));
            }
        }
    }

    printf("Liczba udanych dekodowań dla %d elementów: %d\n", k, successful_decodes);
    return count;
}

void main1() {
    int tabEx[11];
    int decodingTab[ARRAY_SIZE];
    scanTab(tabEx, 11);
    codeTab(tabEx);
    scanTab(decodingTab, ARRAY_SIZE);

    // Wypisanie początkowej tablicy
    printf("Początkowa tablica:\n");
    print_array(decodingTab);

    // Ustawienie ziarna losowości
    srand(time(NULL));

    // Tworzenie kombinacji
    for (int k = 2; k <= MAX_CHANGE; k++) {
        printf("\nKombinacje dla %d elementów:\n", k);
        int indices[k];
        int possibilities = generate_combinations(ARRAY_SIZE, k, indices, decodingTab);
        printf("\nLiczba wypisanych możliwości dla %d elementów: %d\n", k, possibilities);
    }
}
