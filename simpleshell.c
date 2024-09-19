//Assignment Name -> Homework 1
//Author -> Samuel Mouradian
//Course -> Computer Science 446 - Section 1001
//Professor -> Christos Papachristos
//Due Date -> 09.19.2024

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
    char cwd[500], input[500], tokenList[500][500];
    int trigger = 1;

    while(trigger){
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("smouradian@SamsPC:~%s$ ", cwd);
        }

        fgets(input, 500, stdin);
        int numChar = parseInput(input, tokenList, 100);
        
        if(strcmp(tokenList[0], "cd") == 0){
            if(numChar > 1){
                changeDirectories(tokenList[1]);
            }
        }
        else if(strcmp(tokenList[0], "exit") == 0){
            return (trigger = 0);
        }
        else{
            char* args[500];
            for(int i = 0; i < numChar; i ++){
                args[i] = tokenList[i];
            }
            args[numChar] = NULL;
            executeCommand(tokenList[0], NULL, NULL);
        }
    }

    return 0;
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

void changeDirectories(const char* path){
    if(path == NULL || strlen(path) == 0){
        printf("Path is empty!\n");
    }
    
    if(chdir(path) == -1){
        printf("Path not formatted correctly\n");
    }

    char cwd[500];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
    }
    else{
        printf("Error rerouting directories: %s\n", strerror(errno));
    }
}

int executeCommand(char const* enteredCommand, const char* infile, const char* outfile){
    int status = fork();

    if(status < 0){
        printf("Fork failed: %s\n", strerror(errno));
        return -1;
    }
    else if(status == 0){
        if(infile != NULL){
            int inputVal = open(infile, O_RDONLY);
            if(inputVal < 0){
                printf("Error opening input file: %s\n", strerror(errno));
                exit(1);
            }
            dup2(inputVal, STDIN_FILENO);
            close(inputVal);
        }

        char* const args[] = {(char*)enteredCommand, NULL};
        if(execvp(args[0], args) < 0){
            printf("Execution failed: %s\n", strerror(errno));
            exit(1);
        }
    }
    else{
        int pwait;
        wait(&pwait);
        if(WIFEXITED(pwait) == 0){
            printf("Child finished with exit status: %d\n", WEXITSTATUS(pwait));
        }
    }
    return status;
}