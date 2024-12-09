#include <stdio.h>
#include "file1.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 31
#define MAX_CHANGE 20

void print_array(int decodingTab[]) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", decodingTab[i]);
    }
    printf("\n");
}

int decode_and_print(int decodingTab[], int indices[], int num_indices, int new_value, int *decode_errors) {
    int original_values[ARRAY_SIZE];
    int decode_result;

    memcpy(original_values, decodingTab, sizeof(original_values));

    // Zmiana wartości w zadanych indeksach
    for (int i = 0; i < num_indices; i++) {
        decodingTab[indices[i]] = new_value;
    }

    // Dekodowanie zmienionej tablicy
    //print_array(decodingTab);

    decode_result = decodeTab(decodingTab);
    for (int i = 0; i < num_indices; i++) {

        if ((decodingTab[i] != original_values[i]) && decode_result == 1) {
            decode_result = 0;
            (*decode_errors)++;
            printf("Decoding Error\n");
            break;
        }
    }


    for (int i = 0; i < num_indices; i++) {
        decodingTab[indices[i]] = original_values[indices[i]];
    }

    return decode_result;
}

int generate_combinations(int n, int k, int indices[], int decodingTab[], int *decode_errors) {
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

                if (decode_and_print(decodingTab, indices, 2, -1, decode_errors)) {
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
                int unique_indices[k]; // Tablica, aby przechować unikalne indeksy
                unique_indices[0] = i;
                unique_indices[1] = j;

                // Losowanie pozostałych elementów, unikając powtórzeń
                int idx = 2;
                while (idx < k) {
                    int rand_idx = rand() % n;

                    // Sprawdzanie, czy indeks się powtarza
                    int is_unique = 1;
                    for (int m = 0; m < idx; m++) {
                        if (unique_indices[m] == rand_idx) {
                            is_unique = 0;
                            break;
                        }
                    }

                    // Jeśli indeks jest unikalny, dodajemy go
                    if (is_unique) {
                        unique_indices[idx] = rand_idx;
                        idx++;
                    }
                }

                // Przypisanie unikalnych indeksów do finalnej tablicy
                for (int l = 0; l < k; l++) {
                    indices[l] = unique_indices[l];
                }

                if (decode_and_print(decodingTab, indices, k, -1, decode_errors)) {
                    successful_decodes++;
                }
                count++;

                // Przywracanie pełnego stanu tablicy bazowej po każdej kombinacji
                memcpy(decodingTab, originalDecodingTab, sizeof(originalDecodingTab));
            }
        }
    }

    printf("Liczba udanych dekodowań dla %d elementow: %d\n", k, successful_decodes);
    return count;
}

void main1() {
    int tabEx[11];
    int decodingTab[ARRAY_SIZE];
    scanTab(tabEx, 11);
    codeTab(tabEx);
    scanTab(decodingTab, ARRAY_SIZE);

    // Wypisanie początkowej tablicy
    printf("Poczatkowa tablica:\n");
    print_array(decodingTab);

    // Ustawienie ziarna losowości
    srand(time(NULL));

    // Tworzenie kombinacji
    for (int k = 2; k <= MAX_CHANGE; k++) {
        printf("\nKombinacje dla %d elementow:\n", k);
        int indices[k];
        int decode_errors = 0; // Resetowanie licznika błędów dekodowania

        int possibilities = generate_combinations(ARRAY_SIZE, k, indices, decodingTab, &decode_errors);
        printf("\nLiczba wypisanych mozliwosci dla %d elementow: %d\n", k, possibilities);
        printf("Liczba bledów dekodowania dla %d elementow: %d\n", k, decode_errors);
    }
}
