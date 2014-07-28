/**
 * This is the main cog_test program file. It is a skeleton of an
 * application has a xmmc process which starts and stops processes
 * running on separate cogs.  The xmmc routine is a simple command
 * processor.  It supports the following commands:
 *		startA - load cogA code into a cog and start it
 *		startB - load cogB code into a cog and start it
 *		startC - load cogC code into a cog and start it
 *		stopA  - stop the cog running cogA code
 *		stopB  - stop the cog running cogB code
 *		stopC  - stop the cog running cogC code
 *		queryA - set flag telling the cogA code to increment query counter for the cog
 *		queryB - set flag telling the cogB code to increment query counter for the cog
 *		queryC - set flag telling the cogC code to increment query counter for the cog
 *		status - display the state of all cogs and counters
 *      exit   - terminate application
 *		
 * The code loaded in to the cogs is identical.  On startup it zeros the 
 * query counter then loops, checking shared memory for a flag requesting
 * that the query counter be incremented. 
 * 
 **/

#include <stdio.h>
#include <propeller.h>
#include "main.h"

/* allocate control block & stack for cogA */
struct {
    unsigned            stack[STACK_A];
    CONTROL_BLOCK       A;
} parA;

/* allocate control block & stack for cogB */
struct {
    unsigned            stack[STACK_B];
    CONTROL_BLOCK       B;
} parB;

/* allocate control block & stack for cogC */
struct {
    unsigned            stack[STACK_C];
    CONTROL_BLOCK       C;
} parC;

/* command processor command list */
char    *command[COMMANDS] = {
/*  0 */    "startA",
/*  1 */    "startB",
/*  2 */    "startC",
/*  3 */    "stopA",
/*  4 */    "stopB",
/*  5 */    "stopC",
/*  6 */    "queryA",
/*  7 */    "queryB",
/*  8 */    "queryC",
/*  9 */    "status",
/* 10 */    "exit"};

/************************* command processor routines *************************/
/* start cogA */
int start_cogA(volatile void *parptr)
{ 
    extern unsigned int _load_start_cogA_cog[]; //start address for cog code
    extern unsigned int _load_stop_cogA_cog[];  //end address for cog code 
    if(parA.A.cog != -1)                        //see if the cog is running
        cogstop(parA.A.cog);                    //stop the cog
    int size  = (_load_stop_cogA_cog - _load_start_cogA_cog)*4;	//code size in bytes
    printf("  cogA code size %i bytes - ",size);
    unsigned int code[size];  				    //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogA_cog, size);   //copy the code to HUB memory
    parA.A.cog = cognew(code, parptr);          //start the cog
    return parA.A.cog;
}

/* start cogB */
int start_cogB(volatile void *parptr)
{ 
    extern unsigned int  _load_start_cogB_cog[]; //start address for cog code
    extern unsigned int  _load_stop_cogB_cog[];  //end address for cog code 
    if(parB.B.cog != -1)                         //see if the cog is running
        cogstop(parB.B.cog);                     //stop the cog
    int size  = (_load_stop_cogB_cog - _load_start_cogB_cog)*4; //code size in bytes    
    printf("  cogB code size %i bytes - ",size);
    unsigned int code[size];                     //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogB_cog, size);    //copy the code to HUB memory  
    parB.B.cog = cognew(code, parptr);           //start the cog
    return parB.B.cog;
}

/* start cogC */
int start_cogC(volatile void *parptr)
{ 
    extern unsigned int _load_start_cogC_cog[]; //start address for cog code
    extern unsigned int _load_stop_cogC_cog[];  //end address for cog code  
    if(parC.C.cog != -1)                        //see if the cog is running
        cogstop(parC.C.cog);                    //stop the cog
    int size  = (_load_stop_cogC_cog - _load_start_cogC_cog)*4; //code size in bytes    
    printf("  cogC code size %i bytes - ",size);
    unsigned int code[size];                    //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogC_cog, size);   //copy the code to HUB memory 
    parC.C.cog = cognew(code, parptr);          //start the cog
    return parC.C.cog;
}

/* convert user input to command number */
int process(char *input)
{
    int         i;
    for(i=0;i<COMMANDS;i++)             //loop through the command list
    {
        if(strcmp(input,command[i])==0) //see if the user entered a valid command
            return i;                   //return the command number
    }
    return -1;                          //user entered an invalid command
}

/* display the status of all cogs and query counters */
void status(void)
{
    printf("  cog A is ");
    if(parA.A.cog == -1)
        printf("not running, query count %i\n",parA.A.query_ctr);
    else
        printf("running on cog %i, query count %i\n",parA.A.cog,parA.A.query_ctr);
    printf("  cog B is ");
    if(parB.B.cog == -1)
        printf("not running, query count %i\n", parB.B.query_ctr);
    else
        printf("running on cog %i, query count %i\n",parB.B.cog,parB.B.query_ctr);
    printf("  cog C is ");
    if(parC.C.cog == -1)
        printf("not running, query count %i\n",parC.C.query_ctr);
    else
        printf("running on cog %i, query count %i\n",parC.C.cog,parC.C.query_ctr);
    return;
}

/****************************** start main routine ******************************/
int main(void)
{
    char        input_buffer[INPUT_BUFFER]; //buffer for user input

/* display startup message */
    waitcnt(500000 + _CNT);                 //wait until initialization is complete
    printf("\nXMMC-cogs demo version 1.0\n"); 

/* set all cogs to not running */
    parA.A.cog = -1; 
    parB.B.cog = -1; 
    parC.C.cog = -1; 

/* loop forever */
    while(1) 
    {
        printf("\nenter command > ");
        fgets(input_buffer,INPUT_BUFFER,stdin);
        switch(process(input_buffer))   //test user input,take appropriate action
        {
            case 0: //startA
                if(start_cogA(&parA.A)== -1)
                    printf("  problem starting cogA\n");
                else
                    printf("  cogA started\n");
                break;
            case 1: //startB
                if(start_cogB(&parB.B)== -1)
                    printf("  problem starting cogB\n");
                else
                    printf("  cogB started\n");
                break;
            case 2: //startC
                if(start_cogC(&parC.C)== -1)
                    printf("  problem starting cogC\n");
                else
                    printf("  cogC started\n");
                break;
            case 3: //stopA
                cogstop(parA.A.cog);
                printf("  cog A stopped\n");
                parA.A.cog = -1;
                break;
            case 4: //stopB
                cogstop(parB.B.cog);
                printf("  cog B stopped\n");
                parB.B.cog = -1;
                break;
            case 5: //stopC
                cogstop(parC.C.cog);
                printf("  cog C stopped\n");
                parC.C.cog = -1;
                break;
            case 6: //queryA
                parA.A.query_flag = 1;
                break;
            case 7: //queryB
                parB.B.query_flag = 1;
                break;
            case 8: //queryC
                parC.C.query_flag = 1;
                break;
            case 9: //status
                status();
                break;
            case 10:    //exit
                printf("exiting program\n");
                return 0;
            default:
                printf("<%s> is not a valid command\n",input_buffer);

        }
    }
    return 0;
}



