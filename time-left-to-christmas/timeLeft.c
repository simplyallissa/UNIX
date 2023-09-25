#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define INTERVAL 3 /*between alarms*/

/*Counter to keep track of the number of times handlers are invoked*/
int i = 0;

/*Counter to keep track of number of ctrl-c by the user*/
int c = 0;

/*Counter to keep track of how many SIGALARMs have happened*/
int a = 1;

/*These are used by both signal handlers and are manipulated in the daysLeft function based on the target date*/
char buf[40];
int days, hours, minutes, seconds;
time_t diffInSeconds;

/*Calculate the number of days left until the target dates*/
int daysLeft(int signal)
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char targetDateStr[128];
    struct tm targetDate = {0, 0, 0, 0, 0, 0, 0, 0, -1};

    /*Based on the signal type make the target date either christmas or independence day*/
    if (signal == SIGALRM)
    {
        strftime(targetDateStr, 128, "12/25/2023", &targetDate);
    }
    else if (signal == SIGINT)
    {
        strftime(targetDateStr, 128, "07/04/2023", &targetDate);
    }

    /*Parse the target date string and convert it to a Unix timestamp*/
    strptime(targetDateStr, "%m/%d/%Y", &targetDate);
    time_t targetDateSeconds = mktime(&targetDate);

    /*Calculate the number of days, hours, minutes, and seconds until the target date*/
    diffInSeconds = targetDateSeconds - currentTime;
    days = diffInSeconds / 86400;
    hours = (diffInSeconds % 86400) / 3600;
    minutes = (diffInSeconds % 3600) / 60;
    seconds = diffInSeconds % 60;

    /*format and set the localTime*/
    strftime(buf, 40, "%Y-%m-%d %A %I:%M:%S %p, %Z", localTime);

    return 0;
}

/*Prints number of days til Christmas*/
void sigAlarmHandler(int signal)
{
    /*Increment the the number of times alarm handler is invoked*/
    a++;

    daysLeft(signal);
    if (a % 3 == 0)
    {
        alarm(8);
    }
    else
    {
        alarm(3);
    }

    printf("%d By SIGALARM:\n", i);
    printf("\tCurrent date and time: %s\n", buf);
    printf("\tUntil 2023 Christmas Day : %d Days, %d Hours, %d Minutes, %d Seconds\n", days, hours, minutes, seconds);
    printf("\tTotal number of seconds: %ld seconds\n", diffInSeconds);

    /*Increment the the number of times handlers are invoked*/
    i++;
}

/*Prints number of days til independence day*/
void sigIntHandler(int signal)
{
    //*Increment the number of ctrl-c by the user*/
    c++;
    /*calculate how many days left*/
    daysLeft(signal);

    printf("%d By SIGINT:\n", i);
    printf("\tCurrent date and time: %s\n", buf);
    printf("\tUntil 2023 Independence Day : %d Days, %d Hours, %d Minutes, %d Seconds\n", days, hours, minutes, seconds);
    printf("\tTotal number of seconds: %ld seconds\n", diffInSeconds);

    /*Increment the number of times handlers are invoked*/
    i++;

    /*If the user has clicked ctr-c less than 3 times, prompt them to see if they want to quit the program*/
    if (c < 3)
    {
        char userInput;
        do
        { /*Pause the alarm while asking the user if they want to quit or not*/
            alarm(0);
            printf("Quit (y/n)?\n");
            scanf(" %c", &userInput);
            /*if they answer y then exit*/
            if (userInput == 'y' || userInput == 'Y')
            {
                printf(" %c", userInput);
                exit(0);
            }
            else if (userInput == 'n' || userInput == 'N')
            {
                /*Set the 3 seconds again*/
                alarm(3);
                /*get outta this infinite loop*/
                break;
            }
            /*if they enter anything besides y or n, it will loop and prompt again*/

        } while (1);
    }
    /*If there are more than three kills then terminate*/
    else if (c == 3)
    {
        exit(0);
    }
}

int main()
{

    /*Set up the signal handlers for SIGALRM and SIGINT*/
    signal(SIGALRM, sigAlarmHandler);
    signal(SIGINT, sigIntHandler);
    /*Set initial 3 second timer*/
    alarm(3);

    /*Wait for the signals to be caught by looping*/
    while (1)
    {
        sleep(1);
    }

    return 0;
}
