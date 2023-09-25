#include "apue.h"
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

static void sig_int(int); /* sinal-catching function */

int main(void)
{
    char buf[MAXLINE]; /* from apue.h */
    pid_t pid;
    int status, fd, stdout_saved = -1;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% "); /* print prompt (printf requires %% to print %) */
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null */
        /* create a child process */
        if ((pid = fork()) < 0)
        {
            err_sys("fork error");
        }
        else if (pid == 0)
        { /* child */
            /* string from the command line */
            char *cmd_strs[182];
            /* strtok takes a string and turns all characters into tokens */
            char *token = strtok(buf, " ");
            int i = 0;
            /*loop through all the strings to check tokens */
            while (token != NULL)
            {
                /* set up file redirections */
                if (strcmp(token, ">") == 0 || strcmp(token, ">&") == 0)
                { /* redirecting stdout to the specified file */
                    token = strtok(NULL, "\t\n");

                    if ((fd = open(token, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR)) < 0)
                    {
                        printf("open error of %s: %d\n", token, errno);
                        exit(3);
                    }
                    /*create a new open file descriptor*/
                    stdout_saved = dup(1);
                    /*for the open file, the new file descriptor is 1. This is the same as stdout */
                    if (dup2(fd, 1) < 0)
                    {
                        printf("dup2 error of%s:%d\n", token, errno);
                        exit(4);
                    }
                    /* revert the redirected file descriptors to the saved ones */
                    if (stdout_saved > -1) {
                        close (1);
                        dup2(stdout_saved, 1);
                        close(stdout_saved);
                        stdout_saved = -1; /* reset stdout to the initial value */ 
                    }
                    close(fd); /* to clean up memory, otherwise the fd will stay open */
                } else if (strcmp(token, "<") == 0){
                    token = strtok(NULL, " \t\n");
                    if ((fd = open(token, O_RDONLY)) < 0) {
                        printf("open error of %s: %d\n", token, errno);
                    }
                    stdout_saved = dup(1);
                    if (dup2(fd, 1) < 0) {
                        printf("dup2 error of%s:%d\n", token, errno);
                        exit(4);
                    }
                    close(fd); /* to clean up memory, otherwise the fd will stay open */
                    /* revert the redirected file descriptors to the saved ones */
                    if (stdout_saved > -1) {
                        close (1);
                        dup2(stdout_saved, 1);
                        close(stdout_saved);
                        stdout_saved = -1; /* reset stdout to the initial value */
                    }
                }
                cmd_strs[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            /* solves bad address error caused by tokenized string */
            cmd_strs[i] = NULL;
            /* replace the first command str with an image of cmd_str */
            execvp(cmd_strs[0], cmd_strs);
            err_ret("couldn’t execute: %s", buf);
            exit(127);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        printf("%% ");

    }
    exit(0);
}

void sig_int(int signo)
{
    printf("interrupt\n%% ");
}
