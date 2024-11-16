#include <stdio.h>

int main() {
    char tab[21];
    int tab2[20];
    scanf("%20s",tab);
    int i =0;
    while(tab[i] != '\0') {
        tab2[i] = tab[i] - '0';
        i++;
    }
    printf("%d \n",i);
    for(int j=0;j<i;j++) {
        printf("%d",tab2[j]);
    }
}