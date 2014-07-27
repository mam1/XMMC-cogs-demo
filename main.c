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

/**************************** cog control routines ******************************/

/* start cogA */
int start_cogA(volatile void *parptr)
{ 
    int size = (_load_stop_cogA_cog - _load_start_cogA_cog)*4;	//code size in bytes
    printf("cogA code size %i bytes\n",size);
    unsigned int code[size];  				 //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_rtc_cog, size); //assume xmmc
    return cognew(code, parptr);
}

/* start cogB */
int start_cogB(volatile void *parptr)
{ 
    int size = (_load_stop_cogB_cog - _load_start_cogB_cog)*4;  //code size in bytes
    printf("cogB code size %i bytes\n",size);
    unsigned int code[size];                 //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_rtc_cog, size); //assume xmmc
    return cognew(code, parptr);
}

/* start cogC */
int start_cogC(volatile void *parptr)
{ 
    int size = (_load_stop_cogC_cog - _load_start_cogC_cog)*4;  //code size in bytes
    printf("cogC code size %i bytes\n",size);
    unsigned int code[size];                 //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_rtc_cog, size); //assume xmmc
    return cognew(code, parptr);

}

/************************* command processor routines *************************/

int 

/****************************** start main routine ******************************/

int main(void)
{
    int         cog_num_A, cog_num_B, cog_numC; //cog number where the code is running
    char        input-bufffer[]
/* display startup message */
    sleep(1);
    print("XMMC-cogs demo started\n");  


/* loop forever */
    while(1) 
    {
        readStr();


    }
    return 0;
}



