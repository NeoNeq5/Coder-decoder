#include <stdio.h>

void calculateSyndromes(int *syndromes, int *decodedPolymonial);
int gf_power(int a, int b);
void scanTab(int *tab, int size);
int gf_multiply(int a, int b);
int gf_divide(int a, int b);
int gf_add(int a, int b);
int to_primitive_element(int *binary);
void to_binary(int *binary, int alpha);

int main(){
    int syndromes[20];
    int decodedPolymonial[31];
    scanTab(decodedPolymonial, 31);
    calculateSyndromes(syndromes, decodedPolymonial);
}

//tu zaczyna się nowy kod
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
//tu kończy się nowy kod

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