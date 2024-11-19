#include <stdio.h>

void scanTab(int tab[]);
void codeTab(int tab[]);
void dividePolynomials(int *dividend, int dividendLength, int *divisor, int divisorLength, int *reminder, int *quotient);
int gf_multiply(int a, int b);
int gf_divide(int a, int b);
int gf_add(int a, int b);
int to_primitive_element(int *binary);
void to_binary(int *binary, int alpha);

int main() {
    int tabEx[11];
    scanTab(tabEx);
    codeTab(tabEx);
}

void scanTab(int *tab) {
    char tab1[21];
    int tab2[11];
    scanf("%20s",tab1);
    int i =0;
    while(tab1[i] != '\0') {
        tab2[i] = tab1[i] - '0';
        i++;
    }
    if(i < 11) {
        for(int j = 0; j <11-i; j++) {
            tab[j] = -1;
        }
        for(int j = 0; j < i; j++) {
            tab[11 - i + j] = tab2[j];
        }
    }

    // printf("%d \n",i);
    // for(int j=11-i;j<11;j++) {
    //     printf("%d",tab[j]);
    // }
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
    dividePolynomials(reminder, 31, tabPolynomial, 21, reminder, quotinet);

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
                printf("%d ",reminder[i+j]);
            }
        }
        //printf("\n");
    }
    for(int i = 0; i < 11; i++){
        reminder[i] = dividend[i];
    }
    printf("\n reszta z dzielenia : \n");
    for(int i=0;i<dividendLength;i++) {
        printf("%d, ",reminder[i]);
    }
    printf("\n wynik dzielenia : \n");

    for(int i=0;i<11;i++) {
        printf("%d, ",quotient[i]);
    }
    printf("\n");
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
        printf("warn");
        return -1;
    }
    if(b == -1) {
        printf("dzielenie przez 0");
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
