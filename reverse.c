#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"

struct tuple{
    int csrRowIdx;
    int csrCollIdx;
    short weight;
};
typedef struct tuple tuple;
struct csr{
    int n;  //size of matrix
    int nnz; // num of non-zero elements (edges)
    int *csrRowPtr; // Cumulative Sum of num of nnz in each row 
	tuple** tuple; // weights of edges and their row Index and coll Idx
};
typedef struct csr csr;

csr* getCsr(int n, int nnz){
    csr* Matrix = (csr*)malloc(sizeof(csr));
    Matrix -> n = n;
    Matrix -> nnz = nnz;
    Matrix -> csrRowPtr = (int*) calloc(n+1,sizeof(int) );
    Matrix -> tuple = (tuple**) calloc( nnz ,sizeof(tuple*));
    return Matrix;
}

int* sum(int n, int* arr1, int* arr2){
    int i = 0;
    //#pragma omp parallel for private(i)
    for(i =0; i < n+1; i++){
        arr1[i] += arr2[i]; 
    }
    return arr1;
}

void merge(csr* inMatrix, int start1, int mid, int end2)
{   
    tuple** a = inMatrix->tuple;    
    tuple** temp = (tuple**)malloc(sizeof(tuple*)*(end2-start1+1));    //array used for merging
    int i,j,k;
    i=start1;    //beginning of the first list
    j=mid+1;    //beginning of the second list
    k=0;
    
    while(i<=mid && j<=end2)    //while elements in both lists
    {
        if(a[i]->csrCollIdx < a[j]->csrCollIdx)
            temp[k++]=a[i++];
        else
            temp[k++]=a[j++];
    }
    
    while(i<=mid)    //copy remaining elements of the first list
        temp[k++]=a[i++];
        
    while(j<=end2)    //copy remaining elements of the second list
        temp[k++]=a[j++];
        
    //Transfer elements from temp[] back to a[]
    for(i=start1,j=0;i<=end2;i++,j++)
        a[i]=temp[j];
}

int *mergeTrans(csr* inMatrix, int i, int j)
{
    int mid;
    int* arr1;
    int* arr2;
    if(i<j)
    {
        mid=(int)(i+j)/2;
        //printf("i: %d, j:  %d mid: %d \n",i , j ,mid );
        
        //#pragma omp parallel sections 
        {
           #pragma omp task
            {
                arr1 = mergeTrans(inMatrix,i,mid);        //left recursion
            }
           #pragma omp task
            {
                 arr2 = mergeTrans(inMatrix,mid+1,j);    //right recursion
            }
        }
        merge(inMatrix, i,mid,j);    //merging of two sorted sub-arrays
        
        return sum( inMatrix->n, arr1, arr2 );
    }
    else
    {
    int * arr = (int*)calloc(inMatrix->n+1,sizeof(int));
    int  k = 0;
    for(k =inMatrix->tuple[i]-> csrCollIdx; k < inMatrix->n+1; k++){
        arr[k] = 1;
    }
    return arr;
    }
}


int main(int argc, char **argv)
{   
    csr* inMatrix = getCsr(5,15);
    /*
    char *filename = NULL;
    if(argc > 1)
    {
        filename = argv[1];
    }
    if(filename == NULL)
    {
        printf( "Error: No file provided!\n");
        exit();
    }
    printf("Matrix file name : %s" , filename);
    int *csrRowPtrA;
    int *csrColIdxA;
    */
    //toy input to be replaced
    int n = 5, nnz= 15;
    int csrRowPtr[] = {0 ,2 ,6 ,10 ,15, 15};
    int csrColIdx[] = {1, 3, 1, 1, 2, 3, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    int csrVal[] =    {4, 2, 2, 1, 4, 3, 2, 4, 3, 2, 1, 4, 3, 4, 3};
    int i = 0, j = 0, k =0;
    int tid;
    int chunk = 3;
    inMatrix->csrRowPtr = csrRowPtr;

    #pragma omp parallel for private(j)
    for(j = 0; j < inMatrix->nnz; j++){
        inMatrix->tuple[j] = (tuple*) calloc(1 ,sizeof(tuple));
        inMatrix->tuple[j]-> csrRowIdx = 0 ;
        inMatrix->tuple[j]-> csrCollIdx = csrColIdx[j];
        inMatrix->tuple[j]-> weight = csrVal[j];
    }
    
    #pragma omp parallel for  private(k,j)
    for(j = 0; j<inMatrix->n+1; j++){
        for(k = inMatrix->csrRowPtr[j-1]; k < inMatrix->csrRowPtr[j]; k++ ){
               inMatrix->tuple[k]-> csrRowIdx = j;        
        }
    }

    
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]-> csrRowIdx );
    }
    printf("\n");
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]->weight );
    }
    printf("\n");
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]-> csrCollIdx );
    }
    printf("\n");
    printf("\n");
    


    int * cscColPtr = mergeTrans(inMatrix, 0, nnz-1);

    
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]-> csrRowIdx );
    }
    printf("\n");
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]->weight );
    }
    printf("\n");
    for(j = 0; j<nnz; j++){
        printf("%d, ", inMatrix->tuple[j]-> csrCollIdx );
    }
    printf("\n");
    for(j = 0; j<n+1; j++){
        printf("%d, ", cscColPtr[j] );
    }
    printf("\n");
}