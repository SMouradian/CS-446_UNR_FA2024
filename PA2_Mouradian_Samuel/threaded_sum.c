//Assignment Name -> Homework 2
//Author -> Samuel Mouradian
//Course -> Computer Science 446 - Section 1001
//Professor -> Christos Papachristos
//Due Date -> 10.03.2024


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>


//FUNCTION DECLARATIONS
int parseInput(char, char* [], char**, int*);
int readFile(char[], int[]);
void* arraySum(void*);

//ADDITIONAL DECLARATIONS
typedef struct _thread_data_t{
    const int* data;
    int startInd, endInt;
    pthread_mutex_t* lock;
    long long int* totalSum;
} _thread_data_t;


//MAIN DRIVER
int main(int argc, char* argv[]){
    char* filename;
    int threadCount;

    if(parseInput(argc, argv, &filename, &threadCount) == -1){
        return -1;
    }
    else{
        //call readFile function
    }

    return 0;
}


//FUNCTION DEFINITIONS
int parseInput(char args, char* argv[], char** file, int* numThreads){
    if(args != 3){
        printf("Error: Too few arguments\n");
        return -1;
    }
    else{
        *file = argv[1];
        *numThreads = atoi(argv[2]);

        if(*numThreads <= 0){
            printf("Error: No threads present\n");
            return -1;
        }
    }

    return 0;
}

int readFile(char filename[], int intArray[]){
    FILE *file = fopen(filename, "r");
    if(!file){
        printf("Error: File failed to open");
        return -1;
    }

    int count = 0, value;

    while(fscanf(file, "%d", &value) == 1){
        intArray[count] = value;
        count++;
    }

    fclose(file);
    return count;
}

/*void* arraySum(void* thread_data_t){}*/