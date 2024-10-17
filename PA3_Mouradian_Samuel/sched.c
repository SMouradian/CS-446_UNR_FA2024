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


//PRINT_PROGRESS FUNCTION
#define TERM_GOTOXY(x, y) printf("\033[%d;%dH", (y), (x))
#define TERM_CLEAR() printf("\033[H\033[J")

#define ANSI_COLOR_RESET   "\033[0m"

#define ANSI_COLOR_GRAY    "\033[1;30m"
#define ANSI_COLOR_RED     "\033[1;31m"
#define ANSI_COLOR_GREEN   "\033[1;32m"
#define ANSI_COLOR_YELLOW  "\033[1;33m"
#define ANSI_COLOR_BLUE    "\033[1;34m"
#define ANSI_COLOR_MAGENTA "\033[1;35m"
#define ANSI_COLOR_CYAN    "\033[1;36m"
#define ANSI_COLOR_WHITE   "\033[1;37m"

void print_progress(pid_t, size_t);


//FUNCTION DECLARATIONS
void* arraySum(void*);
int parseInput(int, char* [], int*);


//OTHER REQUIRED DECLARATIONS
typedef struct _thread_data_t{
    int localTid;
    const int *data;
    int numVals;
    pthread_mutex_t *lock;
    long long int *totalSum;
} thread_data_t;


//MAIN
int main(int argc, char* argv[]){
    int threadCount;

    parseInput(argc, argv, &threadCount);  // checks argument passed by user

    int dataArray[2000000];  // once parseInput is successfully checked, a fix-sized array is created
    long long int totalSum = 0;  // creates and initialized totalSum index to 0
    pthread_mutex_t lockVar;  // creates pthread variable for thread locking
    thread_data_t threadObjArr[threadCount];  // creates array of objects with size equal to requested amount of threads

    for(int i = 0; i < threadCount; i++){  // iterates through thread_data_t object array
        threadObjArr[i].localTid = i;
        threadObjArr[i].data = dataArray;
        threadObjArr[i].numVals = 2000000;
        threadObjArr[i].lock = &lockVar;
        threadObjArr[i].totalSum = &totalSum;
    }

    pthread_t objectArr[threadCount];  // creates object array equal to number of threads requested
    
    for(int i = 0; i < threadCount; i++){  // iterates through pthread_t array and creates threads
        pthread_create(&objectArr[i], NULL, arraySum, &threadObjArr[i]);
    }
    for(int i = 0; i < threadCount; i++){  // iterates through pthread_t array and joins threads
        pthread_join(objectArr[i], NULL);
    }

    return 0;
}


//FUNCTION DEFINITIONS
void* arraySum(void* input){
    thread_data_t* thread_data = (thread_data_t *)input;
    
    while(1){
        long long int threadSum = 0;
        long long int latency_max = 0;

        for(int i = 0; i < thread_data->numVals; i++){
            struct timespec start, end;

            clock_gettime(_SC_MONOTONIC_CLOCK, &start);
            threadSum += thread_data->data[i];
            clock_gettime(_SC_MONOTONIC_CLOCK, &end);

            long int latency = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

            if(latency > latency_max){
                latency_max = latency;
            }
        }
        pthread_mutex_lock(thread_data->lock);
        *(thread_data->totalSum) += threadSum;
        pthread_mutex_unlock(thread_data->lock);

        print_progress(thread_data->localTid, latency_max);
    }

    return NULL;
}

int parseInput(int args, char* argv[], int* numThreads){
    if(args > 2){
        printf("Error: Too many arguments\n");
        return -1;
    }
    else if(args < 2){
        printf("Error: Too few arguments\n");
        return -1;
    }
    *numThreads = atoi(argv[1]);

    if(*numThreads <= 0){
        printf("Error: No threads present\n");
        return -1;
    }
    return 0;
}

void print_progress(pid_t localTid, size_t value){
    pid_t tid = (pid_t)pthread_self();

    TERM_GOTOXY(0, localTid + 1);

    char prefix[256];
    size_t bound = 100;
    sprintf(prefix, "%d: %ld (ns) \t[", tid, value);
    const char suffix[] = "]";
	const size_t prefix_length = strlen(prefix);
	const size_t suffix_length = sizeof(suffix) - 1;
	char *buffer = calloc(bound + prefix_length + suffix_length + 1, 1);
	size_t i = 0;

	strcpy(buffer, prefix);
	for(; i < bound; ++i){
	    buffer[prefix_length + i] = i < value/10000 ? '#' : ' ';
	}
	strcpy(&buffer[prefix_length + i], suffix);
        
        if (!(localTid % 7)) 
            printf(ANSI_COLOR_WHITE "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 6)) 
            printf(ANSI_COLOR_BLUE "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 5)) 
            printf(ANSI_COLOR_RED "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 4)) 
            printf(ANSI_COLOR_GREEN "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 3)) 
            printf(ANSI_COLOR_CYAN "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 2)) 
            printf(ANSI_COLOR_YELLOW "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else if (!(localTid % 1)) 
            printf(ANSI_COLOR_MAGENTA "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);  
        else
            printf("\b%c[2K\r%s\n", 27, buffer);

	fflush(stdout);
	free(buffer);
}