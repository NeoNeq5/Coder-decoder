#include <stdio.h>

#include "file1.h"


void scanTab(int tab[], int size);
void codeTab(int tab[]);

void calculateSyndromes(int *syndromes, int *decodedPolymonial);
int decodeTab(int tab[]);
void dividePolynomials(int *dividend, int dividendLength, int *reminder, int *quotient);
int gf_multiply(int a, int b);
int gf_divide(int a, int b);
int gf_add(int a, int b);
int to_primitive_element(int *binary);
void to_binary(int *binary, int alpha);
void polynomialCalc();
int gf_power(int a, int b);

int tabPolynomial[21] = {0, 22, 30, 5, 29, 10, 13, 11, 13, 29, 23, 19, 24, 12, 4, 22, 0, 28, 12, 25, 24};
int polynomialLength = 21;
int const t = 10;

int main() {
    //polynomialCalc();
    //printf("%d",gf_add(4, gf_multiply(21,gf_power(1, 14))));
    //printf(" %d",gf_multiply(27,5));
    improvedDecoder();
    //mainPol();
    //main2();
    int tabEx[11];
    int decodingTab[31];
    // //polynomialCalc();
    scanTab(tabEx, 11);
    codeTab(tabEx);
    int mode = 1;
    scanf("jezeli chcesz dekoder uproszczony -1 jezeli chcesz dekoder rozszerzony -2 %d", mode);
    if(mode == 1) {

        scanTab(decodingTab, 31);
        decodeTab(decodingTab);
    }
    else {
        int syndromes[20];
        int decodedPolymonial[31];
        scanTab(decodedPolymonial, 31);
        calculateSyndromes(syndromes, decodedPolymonial);
    }

}

void calculateSyndromes(int *syndromes, int *decodedPolymonial){
    for(int i = 1; i <= 20; i++){
        int syndrome = -1;
        for(int j = 1; j <= 31; j++){
            int power = gf_power(i, 31-j);
            printf("%d, ", power);
            int multiplication = gf_multiply(decodedPolymonial[j-1], power);
            printf("%d, ", multiplication);
            syndrome = gf_add(syndrome, multiplication);
            printf("%d -> ", syndrome);
        }
        printf("\nSyndrom: %d ", syndrome);
        syndromes[i-1] = syndrome;
        printf("%d\n", syndromes[i-1]);
    }
    for(int i = 0; i < 20; i++){
        printf("%d, ", syndromes[i]);
    }
}

int gf_power(int a, int b){
    if(a == -1) {
        printf("warn");
        return -1;
    }
    if(b == 0){
        return 0;
    }
    return (a * b) % 31;
}

void scanTab(int *tab, int size) {
    char tab1[21];
    int tab2[size];
    int i = 0;
    /*scanf("%20s",tab1);
    while(tab1[i] != '\0') {
        tab2[i] = tab1[i] - '0';
        i++;
    }*/
    printf("\nPodaj wielomian:\n");
    while(i < size){
        printf("%d. ", i+1);
        scanf("%d", &tab2[i]);
        i++;
    }
    for(int j = 0; j <size-i; j++) {
        tab[j] = -1;        
    }
    for(int j = 0; j < i; j++) {
        tab[size - i + j] = tab2[j];
    }
    printf("\nWprowadzony wielomian: ");
    //printf("%d \n",i);
    for(int j=size-i;j<size;j++) {
        printf("%d, ",tab[j]);
    }
}

void scanTab2(int *tab, int size, const char *filename) {
    int temp[size];
    int count = 0;

    // Otwórz plik do odczytu
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Nie można otworzyć pliku");
        return;
    }

    // Wczytaj liczby z pliku
    while (count < size && fscanf(file, "%d", &temp[count]) == 1) {
        count++;
    }

    fclose(file);

    // Jeśli mniej liczb niż wymagany rozmiar, uzupełnij 1
    for (int i = count; i < size; i++) {
        temp[i] = 1;
    }

    // Przekopiuj dane do wynikowej tablicy (uwzględniając ewentualne nadmiarowe dane)
    for (int i = 0; i < size; i++) {
        tab[i] = temp[i];
    }

    // Wyświetl wczytaną tablicę
    printf("\nWczytany wielomian: ");
    for (int i = 0; i < size; i++) {
        printf("%d, ", tab[i]);
    }
    printf("\n");
}

void codeTab(int tab[]) {
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
    dividePolynomials(tabToCode, 31, reminder, quotinet);
    for(int i = 0; i < 11; i++){
        reminder[i] = tabToCode[i];
    }
    printf("\nZakodowana wiadomosc: ");
    for(int i=0;i<31;i++) {
        printf("%d, ",reminder[i]);
    }
}

int decodeTab(int tab[]){
    int reminder[31];
    int quotinet[31];
    int w = 0;
    int moveCounter = 0;
    dividePolynomials(tab, 31, reminder, quotinet);
    //printf("\nSyndrom: ");
    for(int i=0;i<31;i++) {
        //printf("%d, ",reminder[i]);
        if(reminder[i] != -1){
            w++;
        }
    }
    while(w > t && moveCounter != 31){
        int helper = tab[30];
        for(int i = 30; i > 0; i--){
            tab[i] = tab[i-1];
        }
        tab[0] = helper;
        //printf("\nPrzesunięta wiadomosc: ");
        for(int i = 0; i < 31; i++){
            //printf("%d, ", tab[i]);
        }
        dividePolynomials(tab, 31, reminder, quotinet);
        w = 0;
        //printf("\nSyndrom: ");
        for(int i=0;i<31;i++) {
            //printf("%d, ",reminder[i]);
            if(reminder[i] != -1){
                w++;
            }
        }
        //printf("\nWaga: %d", w);
        moveCounter++;
    }
    if(moveCounter == 31){
        //printf("\nBledy niekorygowalne");
        return 0;
    }
    else{
        for(int i = 0; i < 31; i++){
            tab[i] = gf_add(tab[i], reminder[i]);
        }
        // printf("\nZdekodowana przesunięta wiadomosc: ");
        // for(int i=0;i<31;i++) {
        //     printf("%d, ",tab[i]);
        // }
        while(moveCounter > 0){
            int helper = tab[0];
            for(int i = 0; i < 30; i++){
                tab[i] = tab[i+1];
            }
            tab[30] = helper;
            moveCounter--;
        }
        printf("\nZdekodowana wiadomosc: ");
        for(int i=0;i<31;i++) {
        printf("%d, ",tab[i]);
        }
        return 1;
    }
}

void dividePolynomials(int *dividend, int dividendLength, int *reminder, int *quotient) {
    for(int i=0;i<dividendLength;i++) {
        reminder[i] = dividend[i];
    }
    for(int i=0;i<dividendLength;i++) {
        quotient[i] = -1;
    }
    for(int i=0;i<=dividendLength - polynomialLength;i++) {
        if(reminder[i] != -1) {
            //printf("%d. ", i);
            int quotTemp = gf_divide(reminder[i],tabPolynomial[0]);
            quotient[i] = quotTemp;
            //printf("\n");
            for(int j=0;j<polynomialLength;j++) {
                if(tabPolynomial[j] != -1) {
                    int product = gf_multiply(quotTemp,tabPolynomial[j]);
                    //printf("%d + %d = ", reminder[i+j], product);
                    reminder[i+j] = gf_add(reminder[i+j], product);
                    //printf("%d, ", reminder[i+j]);
                }
                //printf("%d, ",reminder[i+j]);
            }
        }
        //printf("\n");
    }
    /*for(int i = 0; i < 11; i++){
        reminder[i] = dividend[i];
    }
    printf("\n");
    for(int i=0;i<dividendLength;i++) {
        printf("%d, ",reminder[i]);
    }*/
}

int gf_multiply(int a, int b) {
    if(a == -1 || b == -1) {
        return -1;
    }
    return (a + b) % 31;
}

int gf_add(int a, int b) {
    int tabA[5];
    to_binary(tabA, a);
    int tabB[5];
    to_binary(tabB, b);
    int tabSum[5];
    for(int i = 0; i < 5; i++){
        tabSum[i] = tabA[i]^tabB[i];
    }
    return to_primitive_element(tabSum);
}

int gf_divide(int a, int b) {
    if(a == -1 ) {
        printf("\nwarn dzielenie");
        return -1;
    }
    if(b == -1) {
        printf("\ndzielenie przez 0");
        return -1;
    }
    //return (a - b + 32) % 32;
    return (a - b + 31) % 31;
}

int to_primitive_element(int *binary){
    int tabOfAlphas[32][6] = 
    {{0,0,0,0,0,-1},
    {0,0,0,0,1,0},
    {0,0,0,1,0,1},
    {0,0,0,1,1,18},
    {0,0,1,0,0,2},
    {0,0,1,0,1,5},
    {0,0,1,1,0,19},
    {0,0,1,1,1,11},
    {0,1,0,0,0,3},
    {0,1,0,0,1,29},
    {0,1,0,1,0,6},
    {0,1,0,1,1,27},
    {0,1,1,0,0,20},
    {0,1,1,0,1,8},
    {0,1,1,1,0,12},
    {0,1,1,1,1,23},
    {1,0,0,0,0,4},
    {1,0,0,0,1,10},
    {1,0,0,1,0,30},
    {1,0,0,1,1,17},
    {1,0,1,0,0,7},
    {1,0,1,0,1,22},
    {1,0,1,1,0,28},
    {1,0,1,1,1,26},
    {1,1,0,0,0,21},
    {1,1,0,0,1,25},
    {1,1,0,1,0,9},
    {1,1,0,1,1,16},
    {1,1,1,0,0,13},
    {1,1,1,0,1,14},
    {1,1,1,1,0,24},
    {1,1,1,1,1,15}};
    int tester = 0;
    int alpha;
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 5; j++){
            if(tabOfAlphas[i][j] != binary[j]){
                tester = 0;
                break;
            }
            else{
                tester++;
            }
        }
        if(tester == 5){
            alpha = i;
            break;
        }
    }
    return tabOfAlphas[alpha][5];
}

void to_binary(int *binary, int alpha){
    int tabOfAlphas[32][6] = 
    {{0,0,0,0,0,-1},
    {0,0,0,0,1,0},
    {0,0,0,1,0,1},
    {0,0,0,1,1,18},
    {0,0,1,0,0,2},
    {0,0,1,0,1,5},
    {0,0,1,1,0,19},
    {0,0,1,1,1,11},
    {0,1,0,0,0,3},
    {0,1,0,0,1,29},
    {0,1,0,1,0,6},
    {0,1,0,1,1,27},
    {0,1,1,0,0,20},
    {0,1,1,0,1,8},
    {0,1,1,1,0,12},
    {0,1,1,1,1,23},
    {1,0,0,0,0,4},
    {1,0,0,0,1,10},
    {1,0,0,1,0,30},
    {1,0,0,1,1,17},
    {1,0,1,0,0,7},
    {1,0,1,0,1,22},
    {1,0,1,1,0,28},
    {1,0,1,1,1,26},
    {1,1,0,0,0,21},
    {1,1,0,0,1,25},
    {1,1,0,1,0,9},
    {1,1,0,1,1,16},
    {1,1,1,0,0,13},
    {1,1,1,0,1,14},
    {1,1,1,1,0,24},
    {1,1,1,1,1,15}};
    for(int i = 0; i < 32; i++){
        if(tabOfAlphas[i][5] == alpha){
            for(int j = 0; j < 5; j++){
                binary[j] = tabOfAlphas[i][j];
            }
            break;
        }
    }
}

void polynomialCalc(){
    int polynomial[21];
    int nowSize = 2;
    polynomial[0] = 1;
    polynomial[1] = 0;
    for(int i = 2; i < 21; i++){
        polynomial[i] = -1;
    }
    for(int i = 2; i <= 20; i++){
        int tempPolynomial[nowSize + 1];
        int multiplier[2] = {nowSize, 0};
        for(int j = 0; j < nowSize + 1; j++){
            tempPolynomial[j] = -1;
        }
        for(int j = 0; j < nowSize; j++){
            for(int k = 0; k < 2; k++){
                int tempNumber = gf_multiply(polynomial[j], multiplier[k]);
                printf("%d\n", tempNumber);
                printf("%d + %d = %d\n", tempPolynomial[j+k], tempNumber, gf_add(tempPolynomial[j+k],tempNumber));
                tempPolynomial[j+k] = gf_add(tempPolynomial[j+k],tempNumber);
            }
        }
        nowSize++;
        for(int j = 0; j < nowSize; j++){
            polynomial[j] = tempPolynomial[j];
        }
    }
    for(int i = nowSize-1; i >= 0; i--){
        printf("%d, ", polynomial[i]);
    }
}