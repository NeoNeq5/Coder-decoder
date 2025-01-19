#include <stdio.h>
#include "file1.h"

// Definicje funkcji GF (ciało Galois) - zakładamy, że są zaimplementowane w file1.h:
// - gf_add(a, b): Dodawanie w GF(2^m).
// - gf_multiply(a, b): Mnożenie w GF(2^m).
// - gf_divide(a, b): Dzielenie w GF(2^m).
// - gf_power(a, b): Potęgowanie w GF(2^m).

typedef struct {
    int polynomial[31];
    int degree;
} Polynomial;


void improvedDecoder() {
    // Inicjalizacja danych wejściowych
    int decodedPolynomial[31] = {1, 14, 1, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int s[20];
    calculateSyndromes1(s, decodedPolynomial);

    Polynomial cx = {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 0};
    Polynomial bx = {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 0};
    int l = 0, m = 1, b = 0;

    // Algorytm Berlekampa-Masseya
    for (int n = 0; n < 20; n++) {
        int d = s[n];
        for (int i = 1; i <= l; i++) {
            d = gf_add(d, gf_multiply(cx.polynomial[i], s[n - i]));
        }
        if (d == -1) { // Brak błędu
            m++;
            continue;
        } else if (2 * l <= n) { // Wykrycie nowego błędu
            Polynomial tx = cx;
            for (int i = 0; i <= bx.degree; i++) {
                cx.polynomial[i + m] = gf_add(cx.polynomial[i + m], gf_multiply(gf_divide(d, b), bx.polynomial[i]));
            }
            cx.degree += m;
            l = n + 1 - l;
            bx = tx;
            b = d;
            m = 1;
        } else { // Aktualizacja wielomianu korekty
            for (int i = 0; i <= bx.degree; i++) {
                cx.polynomial[i + m] = gf_add(cx.polynomial[i + m], gf_multiply(gf_divide(d, b), bx.polynomial[i]));
            }
            m++;
        }
    }

    printf("cx.degree = %d, a l = %d\n", cx.degree, l);

    // Brute-Force do lokalizacji błędów
    int errorLocators[31];
    int errorValues[31];
    int errorCount = 0;

    printf("\n");
    for (int i = 0; i < 31; i++) {
        int errorLocator = -1;
        for (int j = 0; j <= l; j++) {
            errorLocator = gf_add(errorLocator, gf_multiply(cx.polynomial[j], gf_power(i, l - j)));
        }
        if (errorLocator == -1) {
            printf("%d, ", i);
            errorLocators[errorCount] = i;
            errorCount++;
        }
    }

    printf("\nAktualny wielomian korekty: ");
    for (int i = 0; i <= l; i++) {
        printf("%d ", cx.polynomial[i]);
    }
    printf("\n%d\n",errorCount);


    // Obliczanie wartości błędów
    for (int i = 0; i < errorCount; i++) {
        int Xi_inv = gf_power(errorLocators[i], 30); // Odwrócenie Xi w GF(2^m)
        int numerator = -1, denominator = -1;

        // Oblicz licznik: S(Xi^-1)
        for (int j = 0; j < 20; j++) {
            numerator = gf_add(numerator, gf_multiply(s[j], gf_power(Xi_inv, 19 - j)));
            printf("%d  ", numerator);
        }
        printf("\n");
        // Oblicz mianownik: Λ'(Xi^-1)
        for (int j = 1; j <= l; j += 2) { // Tylko wyrazy o nieparzystym stopniu
            denominator = gf_add(denominator, gf_multiply(cx.polynomial[j], gf_power(Xi_inv, j - 1)));
            printf("%d  ", denominator);
        }
        printf("\n l = %d cs.degree = %d\n", l, cx.degree + 1);
        errorValues[i] = gf_divide(numerator, denominator); // Wartość błędu

    }

    // Korekcja błędów
    for (int i = 0; i < errorCount; i++) {
        int position = 31 - 1 - errorLocators[i]; // Indeks w dekodowanym wielomianie
        printf("errorValue = %d\n", errorValues[i]);
        printf("place to add = %d\n", decodedPolynomial[position]);;
        decodedPolynomial[position] = gf_add(decodedPolynomial[position], errorValues[i]);
    }

    // Wyświetlanie wyniku
    printf("Korygowany wielomian: \n");
    for (int i = 0; i < 31; i++) {
        printf("%d ", decodedPolynomial[i]);
    }
    printf("\n");
}
