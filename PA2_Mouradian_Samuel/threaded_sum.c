// Assignment Name -> Homework 2
// Author -> Samuel Mouradian
// Course -> Computer Science 446 - Section 1001
// Professor -> Christos Papachristos
// Due Date -> 10.03.2024


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


//DEFINE MACROS
#define MAXSIZE 100000000


// FUNCTION DECLARATIONS
int parseInput(int, char* [], char**, int*);
int readFile(char[], int[]);
void* arraySum(void*);

// ADDITIONAL DECLARATIONS
typedef struct _thread_data_t{
    const int* data;
    int startInd, endInd;
    pthread_mutex_t* lock;
    long long int* totalSum;
} thread_data_t;


// MAIN DRIVER
int main(int argc, char* argv[]){
    char* filename;
    int threadCount;
    double elapsedTime;

    if(parseInput(argc, argv, &filename, &threadCount) == -1){
        return -1;
    }

    int dataArray[MAXSIZE] = {0};
    int dataCount = readFile(filename, dataArray);
    if(dataCount <= 0){
        return -1;
    }
    
    if(threadCount > dataCount){
        printf("Error: Too many threads requested");
        return -1;
    }

    long long int totalSum = 0;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL); // initiated summation time

    pthread_mutex_t lockVar;
    pthread_mutex_init(&lockVar, NULL); // created locking variable for all threads to use

    thread_data_t threadObjArr[threadCount];

    int indexSize = (dataCount + threadCount - 1) / threadCount; // calculates index size

    for(int i = 0; i < threadCount; i++){ // starts the loop through the array of thread_data_t objects
        threadObjArr[i].data = dataArray;
        threadObjArr[i].startInd = i * indexSize;
        threadObjArr[i].endInd = (i + 1) * indexSize > dataCount ? dataCount : (i + 1) * indexSize;
        threadObjArr[i].lock = &lockVar;
        threadObjArr[i].totalSum = &totalSum;
    }

    pthread_t threadArr[threadCount]; // sreated an array of pthread_t objects

    // pthread_create portion of code
    for(int j = 0; j < threadCount; j++){
        pthread_create(&threadArr[j], NULL, arraySum, &threadObjArr[j]);
    }

    // pthread_join portion of code
    for(int k = 0; k < threadCount; k++){
        pthread_join(threadArr[k], NULL);
    }


    gettimeofday(&endTime, NULL); // End summation time

    elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000.0 + (endTime.tv_usec - startTime.tv_usec) / 1000.0; // calculate elapsed time

    printf("Final Sum: %lld\n", totalSum);
    printf("Execution Time: %f ms\n", elapsedTime);

    return 0;
}


//FUNCTION DEFINITIONS
int parseInput(int args, char* argv[], char** file, int* numThreads){
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

int readFile(char fileArray[], int intArray[]){
    FILE *file = fopen(fileArray, "r");
    if(!file){
        printf("Error: File not found...");
        return -1;
    }

    int count = 0, value;

    while(fscanf(file, "%d", &value) == 1){
        if(count >= MAXSIZE){
            printf("Error: Exceeded max data size\n");
            break;
        }
        intArray[count] = value;
        count++;
    }

    fclose(file);
    return count;
}

void* arraySum(void* input){
    thread_data_t* thread_data = (thread_data_t *)input;
    long long int threadSum = 0;

    for(int i = thread_data->startInd; i < thread_data->endInd; i++){
        threadSum += thread_data->data[i];
    }

    pthread_mutex_lock(thread_data->lock);
    *thread_data->totalSum += threadSum;
    pthread_mutex_unlock(thread_data->lock);

    return NULL;
}