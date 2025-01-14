#ifndef FILE1_H
#define FILE1_H

void codeTab(int tab[]);

int decodeTab(int tab[]);
void scanTab(int tab[], int size);
void main1();
void main2();
void mainPol();
void improvedDecoder();
int gf_multiply(int a, int b);
int gf_divide(int a, int b);
int gf_add(int a, int b);
int to_primitive_element(int *binary);
void to_binary(int *binary, int alpha);
int gf_power(int a, int b);
void calculateSyndromes1(int *syndromes, int *decodedPolynomnial);

#endif