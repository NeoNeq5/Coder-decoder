#include <stdio.h>

#include "file1.h"

//1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1
typedef struct {
    int polynomial[31];
    int degree;
} Polynomial;

void improvedDecoder() {
    int s[31] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1};
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
    for(int i = 0; i < count; i++){
        printf("%d, ", errorLocators[i]);
    }


}