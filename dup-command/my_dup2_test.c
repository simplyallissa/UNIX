#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void my_dup2_with_fcntl(int arg2, char *arg3)
{
    int originalFD;
    int cwdPath;
    int status;
    char newFdFilePath[1000];
    char symbolicLink;
    char fd[100];
    char buf[1000];
    char cwd[100000];

    //get the current working directory
    getcwd(cwd, sizeof(cwd));
    //add a backslash at the end of the cwd
    strcat(cwd, "/");
    //add the user input argument to create a file path the the file
    strcat(cwd, arg3);
    //grab the file descriptor using the file path
    originalFD = open(cwd, O_WRONLY);

    printf("old fd: %d%s", originalFD, "\n");
    printf("%s%i%s", "requested new duped fd: ", arg2, "\n");
    close(arg2);

    // fcntl - fcntl(fd, F_DUPPED, fd2);
    fcntl(originalFD, F_DUPFD, arg2);
 
    // used to convert string arg2 to int
    sprintf(fd, "%d", arg2);

    // this is the file path where references from the file descriptor to which files they represent are stored
    strcpy(newFdFilePath, "/proc/self/fd/");

    // put together the file path with the new file descriptor that was associated with the file from command line argument. For example: /proc/self/fd/5
    strcat(newFdFilePath, fd);

    //find the file name of the file descriptor
    symbolicLink = readlink(newFdFilePath, buf, sizeof(buf));
    printf("%s%s%i%s", newFdFilePath, ": new fd file path: ", buf, "\n");
}

int main(int argc, char *argv[])
{
    //pull arguments from the command line
    char *fd = argv[1];
    //convert the file descriptor from int to string
    int newFD = atoi(fd);
    int i;
    char *fn = argv[2];
    //check to make sure there are 3 arguments
    if (argc != 3)
    {
        printf("You must pass two arguments, the new file descriptor and the name of the file you wish to modify.");
        return 1;
    }
    //pass command line  arguments to the function
    else
    {
        my_dup2_with_fcntl(newFD, fn);
    };
}
