#include <stdio.h>

void scanTab(int tab[], int size);
void codeTab(int tab[]);
void decodeTab(int tab[]);
void dividePolynomials(int *dividend, int dividendLength, int *divisor, int divisorLength, int *reminder, int *quotient);
int gf_multiply(int a, int b);
int gf_divide(int a, int b);
int gf_add(int a, int b);
int to_primitive_element(int *binary);
void to_binary(int *binary, int alpha);

int tabPolynomial[21] = {0,22,30,9,0,12,25,20,5,23,1,5,17,30,5,25,29,22,12,25,24};
int const t = 10;

int main() {
    int tabEx[11];
    int decodingTab[31];
    //scanTab(tabEx, 11);
    //codeTab(tabEx);
    scanTab(decodingTab, 31);
    decodeTab(decodingTab);
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
    dividePolynomials(tabToCode, 31, tabPolynomial, 21, reminder, quotinet);
    for(int i = 0; i < 11; i++){
        reminder[i] = tabToCode[i];
    }
    printf("\nZakodowana wiadomość: ");
    for(int i=0;i<31;i++) {
        printf("%d, ",reminder[i]);
    }
}

void decodeTab(int tab[]){
    int reminder[31];
    int quotinet[31];
    int w = 0;
    int moveCounter;
    dividePolynomials(tab, 31, tabPolynomial, 21, reminder, quotinet);
    printf("\nSyndrom: ");
    for(int i=0;i<31;i++) {
        printf("%d, ",reminder[i]);
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
        printf("\nPrzesunięta wiadomość: ");
        for(int i = 0; i < 31; i++){
            printf("%d, ", tab[i]);
        }
        dividePolynomials(tab, 31, tabPolynomial, 21, reminder, quotinet);
        w = 0;
        printf("\nSyndrom: ");
        for(int i=0;i<31;i++) {
            printf("%d, ",reminder[i]);
            if(reminder[i] != -1){
                w++;
            }
        }
        printf("\nWaga: %d", w);
        moveCounter++;
    }
    if(moveCounter == 31){
        printf("\nBłędy niekorygowalne");
    }
    else{
        for(int i = 0; i < 31; i++){
            tab[i] = gf_add(tab[i], reminder[i]);
        }
        printf("\nZdekodowana przesunięta wiadomość: ");
        for(int i=0;i<31;i++) {
            printf("%d, ",tab[i]);
        }
        while(moveCounter > 0){
            int helper = tab[0];
            for(int i = 0; i < 30; i++){
                tab[i] = tab[i+1];
            }
            tab[30] = helper;
            moveCounter--;
        }
        printf("\nZdekodowana wiadomość: ");
        for(int i=0;i<31;i++) {
            printf("%d, ",tab[i]);
        }
    }
}

void dividePolynomials(int *dividend, int dividendLength, int *divisor, int divisorLength, int *reminder, int *quotient) {
    for(int i=0;i<dividendLength;i++) {
        reminder[i] = dividend[i];
    }
    for(int i=0;i<dividendLength;i++) {
        quotient[i] = -1;
    }
    for(int i=0;i<=dividendLength - divisorLength;i++) {
        if(reminder[i] != -1) {
            //printf("%d. ", i);
            int quotTemp = gf_divide(reminder[i], divisor[0]);
            quotient[i] = quotTemp;
            for(int j=0;j<divisorLength;j++) {
                if(divisor[j] != -1) {
                    int product = gf_multiply(quotTemp, divisor[j]);
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
        printf("warn");
        return -1;
    }
    //return (a * b) % 32;
    return (a + b) % 31;
}

int gf_add(int a, int b) {
    /*if(a == -1) return b;
    if(b == -1) return a;
    return a ^ b;*/
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
        printf("\nwarn");
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
