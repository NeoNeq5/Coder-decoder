#include <stdio.h>

void scanTab(int tab[]);
void codeTab(int tab[]);
void dividePolynomials(int *dividend, int dividendLength, int *divisor, int divisorLength, int *reminder, int *quotient);
int gf_multiply(int coef, int dividend);
int gf_divide(int coef, int dividend);
int gf_add(int coef, int dividend);


int main() {
    int tabEx[11] = {-1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 30};
    codeTab(tabEx);
}

void scanTab(int tab[]) {
    char tab1[21];
    int tab2[20];
    scanf("%20s",tab1);
    int i =0;
    while(tab1[i] != '\0') {
        tab2[i] = tab1[i] - '0';
        i++;
    }
    printf("%d \n",i);
    for(int j=0;j<i;j++) {
        printf("%d",tab2[j]);
    }
}

void codeTab(int tab[]) {
    int tabPolynomial[21] = {0,22,30,9,0,12,25,20,5,23,1,5,17,30,5,25,29,22,12,25,24};
    int tabToCode[31];
    int reminder[31];
    int quotinet[31];
    for(int i=0;i<31;i++) {
        if(i<11) {
            tabToCode[i] =tab[i];
        }
        else {
            tabToCode[i] = -1;
        }
    }
    dividePolynomials(tabToCode, 31, tabPolynomial, 21, reminder, quotinet);
}

void dividePolynomials(int *dividend, int dividendLength, int *divisor, int divisorLength, int *reminder, int *quotient) {
    for(int i=0;i<dividendLength;i++) {
        reminder[i] = dividend[i];
    }
    for(int i=0;i<dividendLength;i++) {
        quotient[i] = -1;
    }
    for(int i=0;i<dividendLength - divisorLength;i++) {
        if(reminder[i] != -1) {
            int quotTemp = gf_divide(reminder[i], divisor[0]);
            quotient[i] = quotTemp;

            for(int j=1;j<divisorLength;j++) {
                if(divisor[j] != -1) {
                    int product = gf_multiply(quotTemp, divisor[j]);
                    reminder[i+j] = gf_add(reminder[i+j], product);
                }
            }
        }
    }
    for(int i=0;i<dividendLength;i++) {
        printf("%d\n",reminder[i]);
    }
}

int gf_multiply(int a, int b) {
    if(a == -1 || b == -1) return -1;
    return (a * b) % 32;
}
int gf_add(int a, int b) {
    if(a == -1) return b;
    if(b == -1) return a;
    return a ^ b;
}
int gf_divide(int a, int b) {
    if(a == -1 ) return -1;
    if(b == -1) {
        printf("dzielenie przez 0");
        return -1;
    }
    return (a - b + 32) % 32;
}
