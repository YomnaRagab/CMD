/*
Author:Yomna Ragab.
CMD
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

typedef enum
{
     foreground=1,
     background=2
}enstatus; //to check the status of process if it's background or foreground .
enstatus status=1;
char phrase[256]; // to save the command from user
char *token[128]; // to save segmented words
void handler(int sig)
{
    FILE*f; // make a pointer to a file
    f=fopen("log_file.text","a"); //open a file in append mode
    pid_t p; //pointer of the process
    p=wait(NULL); // wait until it finishes
    fputs("child process was terminated!\n",f); // fputs to write the data in the file which address is holded in f
    fclose(f); //function to close file

}
void gettingPhrase(char *phrase);
/*
        function to get the command from the user and save it in the array of character.
        arguments: array of character "string" (save the i/p in it).
        return:None.
*/
void segmentation(char phrase[],char **token);
/*
        function: divide the command from the user into words and save it in the array of pointer to character.
                  Determine of the status of process.
        arguments: array of character (command) ,
                   array of pointer to character to save the instruction and modifiers in it.
        return:None.
*/
void execute (char **token,enstatus status);
/*
        function to execute the command from the user .
        arguments: array of pointer to character and the status of the process.
        return:None.
*/
void gettingPhrase(char *phrase)
{
    gets(phrase); //get the command from the user and save it in the phrase
}
void segmentation(char phrase[],char **token)
{
    int i=0;
    token[i]=strtok(phrase," "); // take the phrase and search for the delemeter which is tthe space and replace it with back slash 0 /0 and save the word in the token[i]

    while (token[i]!=NULL) // if the word is not null which means it's not the end of the command
    {
        i++;                // increase the counter to move to the next pointer
       token[i]=strtok(NULL," "); // from the last /0 it will complete the segmentation
    }
// chech about the status whether it background or foreground
    if (token[i-1][0]=='&') // if the first character of the last word is & so it's background
    {
        status=background;
    }
    else // else it's foreground
    {
        status=foreground;
    }
}


void execute (char **token,enstatus status)
{
     int i=0;
     pid_t pid; //address of the process
        pid=fork(); // creat a process
        if(pid<0) //fork failed
        {
            printf("Can't be executed\n");
        }
        else if(pid==0) //child process
        {
            execvp(token[0],token); // execute the instruction ond its modifiers
        }
        else //parent process
        {
            if(status==foreground) //foreground app
                {
                    waitpid(pid,&i,0); // wait for the child to execute if it's foreground
                }
        }
}
int main()
{

    while (1)
    {
        printf(">>");
        gettingPhrase(phrase); // call the func for getting the command
        segmentation(phrase,&token); // call the func for segment the command
        if(token[0]==NULL)
        {
            printf("No command/n"); // if the first word is null which means there's no command entered .. print no command
        }
        else if (strcmp(token[0],"cd")==0) // if the first word is cd it must be executed with different function called chdir
        {
            chdir(token[1]); // take the direction
        }
        else if (strcmp(token[0],"exit")==0)   // if the first word is exit it will break the while loop to exit
        {
            return 0;
        }
        else // else it's an normal instruction so it will be handled in execute function
        {
            execute(token,status);
        }
        signal(SIGCLD,handler); //call the interrupt , type of signal is SIGCLD which means Child is terminated and function of the signal is handler

    }
    return 0;
}
