#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#define DIR_PERM 0777
#define DIR_NAME "/tmp/foo"

//err_sys() code from the textbook 
void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}

int main(int argc, char **argv)
{
//check that the code is running
printf("running\n");
//make a new directory in tmp called foo
if (mkdir(DIR_NAME, DIR_PERM) < 0)
	err_sys("mkdir failed");
//change the current working directory to foo
if (chdir(DIR_NAME) < 0)
	err_sys("chdir failed");
//remove the foo directory
if (rmdir(DIR_NAME) < 0)
	err_sys("failed to remove folder\n");
else
	printf("folder removed successfully\n");
//open the closed foo directory using .
if (opendir(".") == NULL)
	printf("could not open directory using .\n");
else
	printf("opened directory with .\n");
//attempt to open the closed foo directory using ../foo
if (opendir("../foo") == NULL)
	printf("could not open directory using ../foo\n");
else
	printf("opened directory with ../foo\n");
//attempt to open the directory using /tmp/foo
if (opendir("/tmp/foo") == NULL)
	printf("could not open directory using /tmp/foo\n");
else
	printf("opened directory with /temp/foo\n");
}
