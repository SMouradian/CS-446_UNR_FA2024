//Assignment Name	->  Homework 1
//Author			->  Samuel Mouradian
//Course			->  Computer Science 446 - Section 1001
//Professor			-> 	Christos Papachristos
//Due Date          ->  09.19.2024

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//FUNCTION DECLARATIONS
int parseInput(char* input, char splitwords[][500], int maxWords);
void changeDirectories(const char* path);
int executeCommand(char const* enteredCommand, const char* infile, const char* outfile);


//MAIN DRIVER
int main(){
    char cwd[500];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("@smouradian ➜ %s$\n", cwd);
    }

    return 0;
}


//FUNCTION DEFINITIONS
int parseInput(char* input, char splitWords[][500], int maxWords){
    int count;
    const char* tokens;

    tokens = strtok(input, " ");
    while(tokens != NULL && count < maxWords){
        strcpy(splitWords[maxWords], tokens);
        splitWords[count][499] = '\0';
        count++;
        tokens = strtok(NULL, " ");
    }

    return count;
}

//void changeDirectories(const char* path){}

//int executeCommand(char const* enteredCommand, const char* infile, const char* outfile){}