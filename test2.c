#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file1.h"

#define ARRAY_SIZE 31
#define MAX_ERRORS 20

// Funkcja do drukowania tablicy
void print_array2(int array[]) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// Funkcja do losowego zamieniania miejsc w tablicy
void introduce_errors(int array[], int error_count, int *decode_errors, int *decoded) {
    int original_array[ARRAY_SIZE];
    int error_positions[error_count];

    // Tworzenie kopii oryginalnej tablicy
    for (int i = 0; i < ARRAY_SIZE; i++) {
        original_array[i] = array[i];
    }

    // Losowanie unikalnych pozycji dla błędów
    for (int i = 0; i < error_count; i++) {
        int pos;
        int unique;
        do {
            unique = 1;
            pos = rand() % ARRAY_SIZE;
            for (int j = 0; j < i; j++) {
                if (error_positions[j] == pos) {
                    unique = 0;
                    break;
                }
            }
        } while (!unique);

        error_positions[i] = pos;
        array[pos] = -1; // Wstawienie błędu (-1 jako wartość testowa)
    }

    // Wyświetlenie tablicy z błędami
    // printf("Tablica z wprowadzonymi bledami:\n");
    // print_array2(array);

    // Dekodowanie zmienionej tablicy
    int decode_result = decodeTab(array);  // Assume decodeTab returns 1 for success and 0 for error

    // Wypisanie zdekodowanej tablicy
    // printf("Zdekodowana tablica:\n");
    // print_array2(array);

    // Jeśli dekodowanie się nie udało, zliczamy błąd
    if (decode_result == 0) {
        (*decode_errors)++;
        // printf("Decoding Error\n");
    }
    else {
        (*decoded)++;
    }

    // Przywrócenie oryginalnej tablicy
    for (int i = 0; i < error_count; i++) {
        array[error_positions[i]] = original_array[error_positions[i]];
    }

    // Wypisanie tablicy po przywróceniu oryginalnych wartości
    // printf("Tablica po przywróceniu do stanu oryginalnego:\n");
    // print_array2(array);
}


void main2() {
    int tabEx[11];
    int decodingTab[ARRAY_SIZE];
    scanTab(tabEx, 11);
    codeTab(tabEx);
    scanTab(decodingTab, ARRAY_SIZE);
    int decode_errors = 0;
    int decoded = 0;

    int test_count;

    // Ustawienie ziarna dla generatora losowego
    srand(time(NULL));

    printf("Podaj liczbe testow dla kazdej liczby bledow: ");
    scanf("%d", &test_count);

    for (int error_count = 1; error_count <= MAX_ERRORS; error_count++) {
        printf("\nLiczba bledow: %d\n", error_count);

        for (int t = 0; t < test_count; t++) {

            introduce_errors(decodingTab, error_count, &decode_errors, &decoded);
        }
        printf("\nilosc bledów zdekodowanych %d\n", decoded);
        printf("ilosc bledow %d\n", decode_errors);
        decoded = 0;
        decode_errors = 0;
    }
}
