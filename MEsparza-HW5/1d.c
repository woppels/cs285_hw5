
/* 
	Compile with gcc
	
Filename: 1d.c
Original Author: Tihamer
Modified by: Michael Esparza
Homework: Assignment 5
Question: 1

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


struct accessCtrl
{
    char username[20];
    char password[15];
    int isRoot;
    char secureQuestion1[15];
    char secureQuestion2[15];
    char secureQuestion3[15];	
};

// Used sample code from SO to check for valid input/input length
void getInput(char *question, char *inputBuffer, int bufferLength)
{
    printf("%s  (Max %d characters)\n", question, bufferLength - 1);
    fgets(inputBuffer, bufferLength, stdin);

    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n')
            dropped++;

        if (dropped > 0) // if they input exactly (bufferLength - 1) 
                         // characters, there's only the \n to chop off
        {
            printf("Your input was over the limit by %d characters. Try again\n", dropped );
            getInput(question, inputBuffer, bufferLength);
        }
    }
    else
    {
        inputBuffer[strlen(inputBuffer) -1] = '\0';      
    }
}

int main(void)
{
    char usrname[40];
    
    //fgets version of getlogin?
    getlogin_r(usrname,20);
    printf("%s \n",usrname);
    
    struct accessCtrl ac;
    uid_t uid=getuid(), euid=geteuid();
    //printf("%d\n %d\n",uid, euid);
    
    if (uid<0 || uid!=euid)
    {
        /* We might have elevated privileges beyond that of the user who invoked
         * the program, due to uid bit. Be very careful about trusting any data! */
        ac.isRoot = 1;
    }
    else
    {
        ac.isRoot = 0;
    }
   	
    // Get user input
    getInput("Enter username:\n", ac.username, 21);
    
    getInput("Enter the password:\n ", ac.password, 16);
	
    getInput("What is your favorite sport?\n", ac.secureQuestion1, 16);
    
    getInput("Who is your favorite actor?\n", ac.secureQuestion2, 16);

    getInput("Who is your favorite teacher?\n", ac.secureQuestion3, 16);
    
    
    if(
       ac.isRoot ||
       (!strcmp(ac.password, "coolpwd") && !strcmp(ac.username,usrname)&&!strcmp(ac.secureQuestion1,"baseball")&&!strcmp(ac.secureQuestion2,"Steven Seagal")&&!strcmp(ac.secureQuestion3,"Tihamer"))||
       (!strcmp(ac.password, "smoothieking") && !strcmp(ac.username,usrname)&&!strcmp(ac.secureQuestion1,"football")&&!strcmp(ac.secureQuestion2,"Matt Damon")&&!strcmp(ac.secureQuestion3,"Tihamer"))
       )
    {
        printf ("\n Access granted \n");
    }
    else
    {
        printf ("\n Incorrect Password \n");
    }
    
    printf("Is root?%d\n", ac.isRoot);
    
    return 0;
}
