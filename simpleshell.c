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
int parseInput(char* input, char splitwords[] [500], int maxWords);
void changeDirectories(const char* path);
int executeCommand(char const* enteredCommand, const char* infile, const char* outfile);


//MAIN DRIVER
int main(){

}


//FUNCTION DEFINITIONS
int parseInput(char* input, char splitwords[] [500], int maxWords){
}

void changeDirectories(const char* path){
    fork();
}

int executeCommand(char const* enteredCommand, const char* infile, const char* outfile){
    fork();
    dup2(infile, outfile);              //appropriately redirects stdin & stdout
    execvp(infile, enteredCommand);     //launches new child process
}