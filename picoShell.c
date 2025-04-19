#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>

int built_in_command(char* buf);
int linux_cmd(char* buf);

int main() {
    size_t len = 0;
    char* buf = NULL;

    while(1) {
        printf("I am a simple shell, enter your command $ ");

        if(getline(&buf, &len, stdin) == -1) {
            fprintf(stderr, "error in getline\n");
            exit(-1);
        }

        buf[strlen(buf)-1] = '\0';
        if(strlen(buf) == 0) {
            continue;
        }

        int return_value = built_in_command(buf);
        if(return_value == 0) {  // Built-in command handled
            continue;
        }
        else if(return_value == -1) {  // Not a built-in, try linux command
            int return_value2 = linux_cmd(buf);
            if(return_value2 == -1) {
                printf("Invalid command\n");
            }
        }
    }

    free(buf);
    return 0;
}

int built_in_command(char* buf) {
    char* token = NULL;
    const char* delimiters = " \t\n";
    char* args[32];
    int arg_count = 0;

    char* command = strtok(buf, delimiters);
    if(command == NULL) {
        return -1;  // Empty command
    }
    args[arg_count++] = command;

    while(arg_count < 31 && (token = strtok(NULL, delimiters)) != NULL) {
        args[arg_count++] = token;
    }
    args[arg_count] = NULL;


    char* cmd1 = "echo";
    char* cmd2 = "exit";
    char* cmd3 = "pwd";
    char* cmd4 = "cd";

    if(strncmp(command, cmd1, 4) == 0) {
        for(int i=1;i<arg_count;i++){
            printf("%s",args[i]);

        }
        printf("\n");
        return 0;
    }
    else if(strncmp(command, cmd2, 4) == 0) {
        printf("Good Bye :)\n");
        exit(0);
    }
    else if(strncmp(command, cmd3, 3) == 0) {
        char buf2[PATH_MAX];
        char* work_dir = getcwd(buf2, PATH_MAX);
        printf("%s\n", work_dir);
        return 0;
    }
    else if(strncmp(command, cmd4, 2) == 0) {
        // int i = 0;
        // while(buf[i] == ' ') i++;  // Skip leading spaces
        // i += 2;  // Skip past "cd"
        // while(buf[i] == ' ') i++;  // Skip spaces after "cd"

        if(chdir(args[1]) == -1) {
            perror("chdir failed");
        }
        return 0;
    }

    return -1;  // Not a built-in command
}

int linux_cmd(char* buf) {
    char* token = NULL;
    const char* delimiters = " \t\n";
    char* args[32];
    int arg_count = 0;

    char* command = strtok(buf, delimiters);
    if(command == NULL) {
        return -1;  // Empty command
    }
    args[arg_count++] = command;

    while(arg_count < 31 && (token = strtok(NULL, delimiters)) != NULL) {
        args[arg_count++] = token;
    }
    args[arg_count] = NULL;

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork failed");
        return -1;
    }
    else if(pid == 0) {
        execvp(command, args);

        return -1;
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
}
