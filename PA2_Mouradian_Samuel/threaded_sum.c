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
int parseInput(char* input, char splitwords[][500], int maxWords);
int readFile(char[], int[]);
void* arraySum(void*);


//MAIN DRIVER
int main(){

}


//FUNCTION DEFINITIONS
int parseInput(char* input, char splitWords[][500], int maxWords){
    int count = 0;
    const char* delim = " \t\n";
    char* tokens = strtok(input, delim);

    while(tokens != NULL && count < maxWords){
        strcpy(splitWords[count], tokens);
        tokens = strtok(NULL, delim);
        count++;
    }

    return count;
}