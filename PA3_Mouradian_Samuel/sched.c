// Assignment Name - Homework 3
// Author - Samuel Mouradian
// Computer Science 446 - Section 1001
// Professor - Christos Papachristos
// Due Date - 10.17.2024


//LIBRARIES
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


//FUNCTION DECLARATIONS
void* arraySum(void*);
int parseInput(int, char* [], char*, int*);


//OTHER REQUIRED DECLARATIONS
typedef struct _thread_data_t{
    int localTid;
    const int *data;
    int numVals;
    pthread_mutex_t *lock;
    long long int *totalSum;
} thread_data_t;


//MAIN
int main(int argc, char* argv){

    return 0;
}


//FUNCTION DEFINITIONS
void* arraySum(void* input){
    return NULL;
}

int parseInput(int args, char* argv[], char* file, int* numThreads){
    if(args != 3){
        printf("Error: Too few arguments\n");
        return -1;
    }
    strcpy(file, argv[1]);
    *numThreads = atoi(argv[2]);

    if(*numThreads <= 0){
        printf("Error: No threads present\n");
        return -1;
    }
    return 0;
}