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
 *		statA  - send a message to the cog running the cogA code requesting status
 *		statB  - send a message to the cog running the cogB code requesting status
 *		statC  - send a message to the cog running the cogC code requesting status
 *		exit   - terminate application
 *		
 * The code loaded in to the cogs is identical except for the message 
 * returned when quired for status.
 * 
 */

#include <stdio.h>
#include <unistd.h>
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

/**************************** cog control routines ******************************/

/* start cogA */
int start_cogA(volatile void *parptr)
{ 
    /* beginning and ending addresses of the code for the A cog */
    extern unsigned int         _load_start_cogA_cog[];
    extern unsigned int         _load_stop_cogA_cog[];

    if(parA.A.cog != -1)
        cogstop(parA.A.cog);  //stop the cog

    int size  = (_load_stop_cogA_cog - _load_start_cogA_cog)*4;	//code size in bytes
    printf("  cogA code size %i bytes - ",size);
    unsigned int code[size];  				  //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogA_cog, size); //copy the code to HUB memory
    parA.A.cog = cognew(code, parptr);        //start the cog
    return parA.A.cog;
}

/* start cogB */
int start_cogB(volatile void *parptr)
{ 
    /* beginning and ending addresses of the code for the B cog */
    extern unsigned int         _load_start_cogB_cog[];
    extern unsigned int         _load_stop_cogB_cog[]; 

    if(parB.B.cog != -1)
        cogstop(parB.B.cog);  //stop the cog

    int size  = (_load_stop_cogB_cog - _load_start_cogB_cog)*4; //code size in bytes    
    printf("  cogB code size %i bytes - ",size);
    unsigned int code[size];                  //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogB_cog, size); //copy the code to HUB memory  
    parB.B.cog = cognew(code, parptr);        //start the cog
    return parB.B.cog;
}

/* start cogC */
int start_cogC(volatile void *parptr)
{ 
    /* beginning and ending addresses of the code for the C cog */
    extern unsigned int         _load_start_cogC_cog[];
    extern unsigned int         _load_stop_cogC_cog[]; 

    if(parC.C.cog != -1)
        cogstop(parC.C.cog);  //stop the cog

    int size  = (_load_stop_cogC_cog - _load_start_cogC_cog)*4; //code size in bytes    
    printf("  cogC code size %i bytes - ",size);
    unsigned int code[size];                  //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_cogC_cog, size); //copy the code to HUB memory 
    parC.C.cog = cognew(code, parptr);        //start the cog
    return parC.C.cog;
}

/************************* command processor routines *************************/
int process(char *input)
{
    int         i;

    for(i=0;i<COMMANDS;i++)
    {
        if(strcmp(input,command[i])==0)
            return i;
    }
    return -1;

}
/****************************** start main routine ******************************/

int main(void)
{
    char        input_buffer[INPUT_BUFFER];
    // int         cmd_num;

/* display startup message */
    waitcnt(500000 + _CNT);              //wait until initialization is complete
    printf("\nXMMC-cogs demo started\n"); 

/* set all cogs to not running */
    parA.A.cog = -1; 
    parB.B.cog = -1; 
    parC.C.cog = -1; 


/* loop forever */
    while(1) 
    {
        printf("\nenter command > ");
        gets(input_buffer);
        switch(process(input_buffer))
        {
            case 0: //startA
                if(start_cogA(&parA)== -1)
                    printf("  problem starting cogA\n");
                else
                    printf("  cogA started\n");
                break;
            case 1: //startB
                if(start_cogB(&parB)== -1)
                    printf("  problem starting cogB\n");
                else
                    printf("  cogB started\n");
                break;
            case 2: //startC
                if(start_cogC(&parC)== -1)
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
                printf("after call <%i>\n",parA.A.query_flag);
                waitcnt(50000000 + _CNT);              //wait until initialization is complete
                printf("after call <%i>\n",parA.A.query_flag);

                break;
            case 9: //status
                printf("  cog A is ");
                if(parA.A.cog == -1)
                    printf("not running\n");
                else
                    printf("running on cog %i, query count %i\n",parA.A.cog,parA.A.query_ctr);
                printf("  cog B is ");
                if(parB.B.cog == -1)
                    printf("not running\n");
                else
                    printf("running on cog %i, query count %i\n",parB.B.cog,parB.B.query_ctr);
                printf("  cog C is ");
                if(parC.C.cog == -1)
                    printf("not running\n");
                else
                    printf("running on cog %i, query count %i\n",parC.C.cog,parC.C.query_ctr);
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



