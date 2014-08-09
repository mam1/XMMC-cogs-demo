/**
 * This is the main cog_test program file. It is a skeleton of an
 * application comprised of  a xmmc process which starts and stops processes
 * running on separate cogs.  The xmmc routine is a simple command
 * processor.  It supports the following commands:
 *      startA - load cogA code into a cog and start it
 *      startB - load cogB code into a cog and start it
 *      startC - load cogC code into a cog and start it
 *      stopA  - stop the cog running cogA code
 *      stopB  - stop the cog running cogB code
 *      stopC  - stop the cog running cogC code
 *      queryA - set flag telling the cogA code to increment query counter for the cog
 *      queryB - set flag telling the cogB code to increment query counter for the cog
 *      queryC - set flag telling the cogC code to increment query counter for the cog
 *      status - display the state of all cogs and counters
 *      exit   - terminate application
 *      
 * The code loaded in to the cogs is identical.  On startup it zeros the 
 * query counter then loops, checking shared memory for a flag requesting
 * that the query counter be incremented. 
 * 
 */
#include <stdio.h>
#include <propeller.h>
#include "main.h"
#include "schedule.h"



/* allocate control block & stack for cogA */
struct {
    unsigned            stack[STACK_A];
    CONTROL_BLOCK_A       A;
} parA;
/* allocate control block & stack for cogB */
struct {
    unsigned            stack[STACK_B];
    CONTROL_BLOCK_B       B;
} parB;
/* allocate control block & stack for cogC */
struct {
    unsigned            stack[STACK_C];
    CONTROL_BLOCK_C       C;
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
/* 10 */    "exit",
/* 11 */    "addrSch",
/* 12 */    "Rschedule",
/* 13 */    "Wschedule",
/* 14 */    "in",
/* 15 */    "disp",
/* 16 */    "gets",
/* 17 */    "puts"};



/************************* command processor routines *************************/
/* start cogA */
int start_cogA(volatile void *parptr)
{ 
    extern unsigned int _load_start_cogA_cog[]; //start address for cog code
    extern unsigned int _load_stop_cogA_cog[];  //end address for cog code 
    if(parA.A.cog != -1)                        //see if the cog is running
        cogstop(parA.A.cog);                    //stop the cog
    int size  = (_load_stop_cogA_cog - _load_start_cogA_cog)*4; //code size in bytes
    printf("  cogA code size %i bytes - ",size);
    unsigned int code[size];                    //allocate enough HUB to hold the COG code
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

void disp_sch_addrs(void)
{
    int         c,d,ii;
    ii=0;
    for(c=0;c<CHANNELS;c++)
        for(d=1;d<_DAYS_PER_WEEK+1;d++)
        {
            // printf("%i - c=%i d=%i  address = %x\n",ii++,c,d,sch_addr(0,c,d));      
        }
    printf("\n************************\n");

    for(c=0;c<CHANNELS;c++)
        for(d=1;d<_DAYS_PER_WEEK+1;d++)
        {
            // printf("%i - c=%i d=%i  address = %x\n",ii++,c,d,sch_addr(1,c,d));
        }
    printf("\n************************\n");
    for(c=0;c<CHANNELS;c++)
        for(d=1;d<_DAYS_PER_WEEK+1;d++)
        {
            // printf("%i - c=%i d=%i  address = %x\n",ii++,c,d,sch_addr(2,c,d));      
        }
}



/****************************** start main routine ******************************/
int main(void)
{
    char        input_buffer[INPUT_BUFFER]; //buffer for user input
    char        *schedule_file_name = {_SCHEDULE_FILE_NAME};
    int         i;
    char        sb[128];

    waitcnt(500000 + _CNT);                 //wait until initialization is complete
    printf("%s","\033c\n");

    printf("XMMC-cogc demo v%s\n\n",VERSION);   //display startup message 

    printf("%i channels per day, %i days per week, %i schedules\n%i records per schedule, 4 bytes per record, %i bytes per schedule\n",
            CHANNELS,_DAYS_PER_WEEK,SCHEDULES,SCHEDULE_RECORDS,SCHEDULE_BYTES);
    printf("schedule array %i bytes\n\n",SCHEDULE_BYTES * SCHEDULES);
    printf("size of working schedule buffer %i bytes, %i ints\n",sizeof(parB.B.schedule_buffer),sizeof(parB.B.schedule_buffer)/4);

/* set all cogs to not running */
    parA.A.cog = -1; 
    parB.B.cog = -1; 
    parC.C.cog = -1; 

/* initialize schedules */
    if(init_sch(schedule_file_name))
    {
        printf("* error initializing system schedule\n program terminated\n");
        return 1;
    }
    printf("system schedule initilaized\n");




/* loop forever */
    while(1) 
    {
        printf("\nenter command > ");                //prompt user
        fgets(input_buffer,INPUT_BUFFER,stdin);      //get a line of input
        input_buffer[strlen(input_buffer)-1] = '\0'; //get rid of trailing new line character
        switch(process(input_buffer))                //test input,take appropriate action
        {
            case 0:     //startA
                if(start_cogA(&parA.A)== -1)
                    printf("  problem starting cogA\n");
                else
                    printf("  cogA started\n");
                break;
            case 1:     //startB
                if(start_cogB(&parB.B)== -1)
                    printf("  problem starting cogB\n");
                else
                    printf("  cogB started\n");
                break;
            case 2:     //startC
                if(start_cogC(&parC.C)== -1)
                    printf("  problem starting cogC\n");
                else
                    printf("  cogC started\n");
                break;
            case 3:     //stopA
                cogstop(parA.A.cog);
                printf("  cog A stopped\n");
                parA.A.cog = -1;
                break;
            case 4:     //stopB
                cogstop(parB.B.cog);
                printf("  cog B stopped\n");
                parB.B.cog = -1;
                break;
            case 5:     //stopC
                cogstop(parC.C.cog);
                printf("  cog C stopped\n");
                parC.C.cog = -1;
                break;
            case 6:     //queryA
                if(parA.A.cog == -1)
                    printf("cog A is not running\n");              
                else
                    parA.A.query_flag = 1;
                break;
            case 7:     //queryB
                if(parB.B.cog == -1)
                    printf("cog B is not running\n");              
                else
                    parB.B.query_flag = 1;
                break;
            case 8:     //queryC
                if(parC.C.cog == -1)
                    printf("cog C is not running\n");              
                else
                    parC.C.query_flag = 1;
                break;
            case 9:     //status
                status();
                break;
            case 10:    //exit
                printf("exiting program\n");
                return 0;
            case 11:    //schAddrs
                disp_sch_addrs();
                break;
            case 12:    //Rschedule
                cpy_sch_2_eeprom(_SCHEDULE_FILE_NAME, 0);
                break;
            case 13:    //Wschedule
                cpy_sch_2_sd(_SCHEDULE_FILE_NAME, 0);
                break; 
            case 14:    //in
                printf("enter string >");
                fgets(input_buffer,INPUT_BUFFER,stdin);      //get a line of input
                input_buffer[strlen(input_buffer)-1] = '\0'; //get rid of trailing new line character
                printf("buffer befor <%s>\n",parB.B.schedule_buffer); 
                strcpy(parB.B.schedule_buffer,input_buffer); 
                printf("buffer after <%s>\n",parB.B.schedule_buffer); 
                break;
            case 15:    //disp
                printf("<");
                for(i=0;i<20;i++)
                {
                    putc(sb[i],stdout);
                    sb[i] = ' ';
                }
                printf(">");
                break;
            case 16: //gets
                printf("buffer before <%s>\n",sb);
                get_sch(sb,1,1,1); 
                printf("buffer after <%s>\n",sb);
                break;
            case 17: //puts
                printf("enter string >");
                fgets(input_buffer,INPUT_BUFFER,stdin);      //get a line of input
                input_buffer[strlen(input_buffer)-1] = '\0'; //get rid of trailing new line character
                printf("buffer before <%s>\n",sb);
                strcpy(sb,input_buffer); 
                printf("buffer after <%s>\n",sb);
                put_sch(sb,1,1,1); 
                break;
            default:
                printf("<%s> is not a valid command\n",input_buffer);

        }
    }
    return 0;
}



