#include<string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 100

typedef struct Cmd{
    pid_t pid;
    char input[SIZE];
}Cmd;
Cmd commands[SIZE];
int isValid = 1;

int execute(char **user_cmd, int num_arg){
    pid_t pid;
    if( (pid = fork()) < 0){
        perror("{fork} failed");
        isValid =0;
    }
    else{
        if(pid == 0) {
            //child
            execvp(user_cmd[0], user_cmd);
            perror("{execvp} failed");
            isValid = 0;
        }
        waitpid(pid,NULL,WCONTINUED);
        //addCommand(num_arg, *user_cmd,pid);
    }
    return pid;
}

int parser(char* user_input, char* user_cmd[]){
    char *token;
    int i=0;
    token = strtok(user_input, " ");
    while( token != NULL ) {
        user_cmd[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    return i;
}

int main() {
    char user_input[SIZE];
    int num_cmd = -1;

    while (1) {
        printf("$ ");
        fflush(stdout);
        fgets(user_input, SIZE, stdin);
        num_cmd++;
        Cmd c;
       strcpy(c.input, user_input);

        char *user_cmd[SIZE];
        for (int i = 0; i < SIZE; i++) {
            user_cmd[i] = NULL;
        }
        user_input[strlen(user_input) - 1] = '\0';
        int num_arg = parser(user_input, user_cmd);

        //exit
        if (strcmp(user_cmd[0], "exit") == 0) {
            return 0;
        }

        //history
        else if (strcmp(user_cmd[0], "history") == 0) {
            c.pid = getpid();
            commands[num_cmd] = c;
            for (int i = 0; i <= num_cmd; i++) {
                printf("%d " "%s", commands[i].pid, commands[i].input);
            }
        }

        //cd
        else if(strcmp(user_cmd[0],"cd")==0) {
            char prev_path[SIZE];
            if (chdir(user_cmd[1])) {
                printf("{chdir} failed\n");
            } else {
                strcat(prev_path, user_cmd[1]);
                //printf("%s\n", getcwd(temp_path, SIZE));
                c.pid = getpid();
              //  strcpy(c.input, user_input);
                commands[num_cmd] = c;
            }
        }

        //other commands
        else {
            if(isValid == 1){
            pid_t temp_pid = execute(user_cmd, num_arg); ////
                if(temp_pid>=0){
                    c.pid = temp_pid;
                    //strcpy(c.input, user_input);
                    commands[num_cmd] = c;
                }
            }
        }
    }
}
