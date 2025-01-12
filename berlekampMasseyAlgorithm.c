#include <stdio.h>

#include "file1.h"

//1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1
typedef struct {
    int polynomial[31];
    int degree;
} Polynomial;

// Funkcja obliczająca wartości błędów
void calculateErrorValues(int *errorLocators, int errorCount, int *syndromes, int *errorValues) {
    for (int i = 0; i < errorCount; i++) {
        int Xi = gf_power(errorLocators[i], 1); // Xi = alfa^(-loc)
        // Licznik wielomianu Omega(Xi)
        int numerator = syndromes[0];
        for (int j = 1; j < errorCount; j++) {
            numerator = gf_add(numerator, gf_multiply(syndromes[j], gf_power(Xi, j)));
        }
        // Mianownik Omega'(Xi), czyli pochodna wielomianu lokalizatorów błędów
        int denominator = 1;
        for (int j = 0; j < errorCount; j++) {
            if (j != i) {
                denominator = gf_multiply(denominator, gf_add(1, gf_multiply(errorLocators[j], Xi)));
            }
        }
        // Obliczenie wartości błędu przez dzielenie w ciele GF
        errorValues[i] = gf_divide(numerator, denominator);
    }
}

// Funkcja korygująca błędy w otrzymanym ciągu
void correctErrors(int *receivedPolynomial, int *errorLocators, int *errorValues, int errorCount) {
    for (int i = 0; i < errorCount; i++) {
        // Pozycja błędu w ciągu (LSB ma indeks 30, dlatego 30 - lokalizator)
        int position = 30 - errorLocators[i];
        // Korekcja błędu przez dodanie (w ciele GF) wartości błędu
        receivedPolynomial[position] = gf_add(receivedPolynomial[position], errorValues[i]);
    }
}

void improvedDecoder() {
    int s[31] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    Polynomial cx = {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 0};
    Polynomial bx = {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, 0};
    int l = 0;
    int m = 1;
    int b = 0;

    for (int n = 0; n < 31; n++) {
        int d = s[n];
        for (int i = 1; i <= l; i++) {
            d = gf_add(d, gf_multiply(cx.polynomial[i], s[n - i]));
        }

        if (d==-1) {
            m++;
        }
        else if (2 * l <= n) {
            Polynomial tx = cx;

            for (int i = 0; i <= bx.degree; i++) {
                cx.polynomial[i+m] = gf_add(cx.polynomial[i+m], gf_multiply(gf_divide(d, b), bx.polynomial[i]));
            }
            cx.degree += m;

            l = n + 1 - l;
            bx = tx;
            b = d;
            m = 1;
        }
        else {
            for (int i = 0; i <= bx.degree; i++) {
                cx.polynomial[i+m] = gf_add(cx.polynomial[i+m], gf_multiply(gf_divide(d, b), bx.polynomial[i]));
            }
            m++;
        }
    }

    printf("\nMinimalny wielomian: ");
    for (int i = 0; i <= l; i++) {
        printf("%d ", cx.polynomial[i]);
    }
    printf("\nDlugosc rejestru: %d\n", l);

    int errorLocators[31];
    int errorValues[31];
    int count = 0;

    for(int i = 0; i <= 30; i++){
        int errorLocator = -1;
        for(int j = 1; j <= l+1; j++){
            int power = gf_power(i, l+1-j);
            printf("%d, ", power);
            int multiplication = gf_multiply(cx.polynomial[j-1], power);
            printf("%d, ", multiplication);
            errorLocator = gf_add(errorLocator, multiplication);
            printf("%d -> ", errorLocator);
        }
        printf("\nResult: %d\n", errorLocator);
        if (errorLocator == -1) {
            errorLocators[count] = i;
            printf("%d\n", errorLocators[count]);
            count++;
        }
    }
    printf("\nlokalizatory błędów: \n");
    for(int i = 0; i < count; i++){
        printf("%d, ", errorLocators[i]);
    }

    calculateErrorValues(errorLocators,count, s, errorValues);

    printf("\nWartości bledów:\n");
    for(int i = 0; i < count; i++) {
        printf("%d, ", errorValues[i]);
    }

    correctErrors(s, errorLocators, errorValues, count);

    printf("\nskorygowany ciąg:\n");
    for(int i = 0; i < 31; i++) {
        printf("%d, ", s[i]);
    }
}