//Assignment Name	->  Homework 1
//Author			->  Samuel Mouradian
//Course			->  Computer Science 446 - Section 1001
//Professor			-> 	Christos Papachristos
//Due Date          ->  09.19.2024

#include <errno.h>
#include <fcntl.h>
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
    char cwd[500], input[500];
    char tokenList[500][500];

    while(1){
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("@smouradian ➜ %s$", cwd);
        }

        if(fgets(input, sizeof(input), stdin) != NULL){
            input[strcspn(input, "\n")] = 0;
            int tokenCount = parseInput(input, tokenList, 500);
            
            if(tokenCount > 0 && strcmp(tokenList[0], "cd") == 0){
                changeDirectories(tokenList[1]);
            }
            else{
                char* args[500];
                for(int i = 0; i < tokenCount; i++){
                    args[i] = tokenList[i];
                }
                args[tokenCount] = NULL;

                int status = executeCommand(args[0], NULL, NULL);
                if(status != 0){
                    printf("Command execution failed\n");
                }
            }
        }
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

void changeDirectories(const char* path){
    if(path == NULL){
        printf("Path is empty!");
    }
    else if(chdir(path) == -1){
        printf("Path not formatted Correctly!");
    }
    else{
        char cwd[500];
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("@smouradian ➜ %s$", cwd);
        }
    }
}

int executeCommand(char const* enteredCommand, const char* infile, const char* outfile){
    pid_t pid = fork();
    int status;

    if(pid < 0){
        return -1;
    }
    else if(pid == 0){
        if(infile != NULL){
            int infileVal = open(infile, O_RDONLY);
            if(infileVal < 0){
                _exit(EXIT_FAILURE);
            }
            else if(infileVal == 0){
                dup2(infileVal, STDIN_FILENO);
                close(infileVal);
            }
        }

        char* const args[] = {(char*)enteredCommand, NULL};
        if(execvp(args[0], args) < 0){
            _exit(EXIT_FAILURE);
        }
        else{
            return 0;
        }

    }
    else{
        wait(&status);
        return status;
    }
}