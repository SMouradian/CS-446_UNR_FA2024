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


// FUNCTION DECLARATIONS
int parseInput(int, char* [], char*, int*);
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
    char filename[500];
    int threadCount;
    double elapsedTime;

    parseInput(argc, argv, filename, &threadCount);

    int dataArray[1000000];
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

    pthread_mutex_t lockVar;
    pthread_mutex_init(&lockVar, NULL); // created locking variable for all threads to use

    thread_data_t threadObjArr[threadCount];

    int indexSize = (dataCount + threadCount - 1) / threadCount; // calculates index size

    if(strcmp(filename, "oneThousand.txt") == 0){
        gettimeofday(&startTime, NULL); // initiated summation time

        for(int i = 0; i < threadCount; i++){ // starts the loop through the array of thread_data_t objects
            threadObjArr[i].data = dataArray;
            threadObjArr[i].startInd = i * indexSize;
            threadObjArr[i].endInd = (i + 1) * indexSize > dataCount ? dataCount : (i + 1) * indexSize;
            threadObjArr[i].lock = &lockVar;
            threadObjArr[i].totalSum = &totalSum;
        }

        pthread_t threadArr[threadCount]; // created an array of pthread_t objects

        // pthread_create portion of code
        for(int i = 0; i < threadCount; i++){
            pthread_create(&threadArr[i], NULL, arraySum, (void*)&threadObjArr[i]);
        }

        // pthread_join portion of code
        for(int i = 0; i < threadCount; i++){
            pthread_join(threadArr[i], NULL);
        }

        gettimeofday(&endTime, NULL); // End summation time
    }
    else if(strcmp(filename, "oneMillion.txt") == 0){
        for(int i = 0; i < threadCount; i++){ // starts the loop through the array of thread_data_t objects
            threadObjArr[i].data = dataArray;
            threadObjArr[i].startInd = i * indexSize;
            threadObjArr[i].endInd = (i + 1) * indexSize > dataCount ? dataCount : (i + 1) * indexSize;
            threadObjArr[i].lock = &lockVar;
            threadObjArr[i].totalSum = &totalSum;
        }

        pthread_t threadArr[threadCount]; // created an array of pthread_t objects

        // pthread_create portion of code
        for(int i = 0; i < threadCount; i++){
            pthread_create(&threadArr[i], NULL, arraySum, (void*)&threadObjArr[i]);
        }

        // pthread_join portion of code
        for(int i = 0; i < threadCount; i++){
            pthread_join(threadArr[i], NULL);
        }

        gettimeofday(&endTime, NULL); // End summation time
    }
    else{
        printf("Error: File not found...\n");
    }

    elapsedTime = ((endTime.tv_sec - startTime.tv_sec) * 1000.0) + ((endTime.tv_usec - startTime.tv_usec) / 1000.0); // calculate elapsed time

    printf("Final Sum: %lld\n", totalSum);
    printf("Execution Time: %f ms\n", elapsedTime);

    return 0;
}


//FUNCTION DEFINITIONS
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


int readFile(char fileArray[], int intArray[]){
    FILE *file = fopen(fileArray, "r");
    if(!file){
        printf("Error: File not found...");
        return -1;
    }

    int count = 0, value;

    while(fscanf(file, "%d", &value) == 1){
        if(count >= 1000000){
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