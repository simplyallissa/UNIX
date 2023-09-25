#include "apue.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
static void sig_int(int);

int main(void)
{
    char buf[MAXLINE]; /* from apue.h */
    pid_t pid;
    int status;
    char *pch = strtok(buf, " \t\n"); /* 3 white spaces */
    int fd, stdin_saved = -1, stdout_saved = -1, stderr_saved = -1;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% "); /* print prompt (printf requires %% to print %) */
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null */

        if ((pid = fork()) < 0)
        {
            err_sys("fork error");
        }
        else if (pid == 0)
        { /* child */
            char *cmd_strs[182];
            char *token = strtok(buf, " ");
            int i = 0;
            while (token != NULL)
            {
                cmd_strs[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            cmd_strs[i] = NULL;
            execvp(cmd_strs[0], cmd_strs);
            err_ret("couldn’t execute: %s", buf);
            exit(127);
        }
        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        printf("%% ");
        if (strcmp(pch, ">") == 0)
        { // redirecting stdout to the specified file
            /* stdout redirection */
            pch = strtok(NULL, " \t\n");
            if ((an_fd = open(pch, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR)) < 0)
            {
                printf("open error of %s: %d\n", pch, errno);
                exit(3);
            }
            stdout_saved = dup(1);
            if (dup2(fd, 1) < 0)
            {
                printf("open error of %s: %d\n", pch, errno);
                exit(4);
            }
            close(fd); // no longer needed.  Why?
        }
        else if (strcmp(pch, "<") == 0)
        {
            [...]
        }
        /* revert the redirected file descriptors to the saved ones */
        if (stdout_saved > -1)
        {
            close(1);
            dup2(stdout_saved, 1);
            close(stdout_saved);
            stdout_saved = -1; // reset stdout to the initial value
        }
    }
    exit(0);
}

void sig_int(int signo)
{
    printf("interrupt\n%% ");
}
