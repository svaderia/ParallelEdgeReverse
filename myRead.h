#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _TOKEN
#define _TOKEN
#define MAX_WORD_SIZE 140

void init_values(char* path, int** csrRowPtr, int** csrColIdx, int** csrVal, int *dim, int *nnz);

char* getWord(FILE* fp, char* b, int k, int* offset, bool* eof);

void print_matrix(int** mat, int dim);

#endif