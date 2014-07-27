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
#include "cog_test.h"
#include "simpletools.h"                      // Include simpletools library 


/* allocate control block & stack for cogA */
struct {
    unsigned stack[STACK_SIZE_A];
    volatile struct cogA_mailbox A;
} parA;

/* allocate control block & stack for cogB */
struct {
    unsigned stack[STACK_SIZE_B];
    volatile struct cogB_mailbox B;
} parB;

/* allocate control block & stack for cogC */
struct {
    unsigned stack[STACK_SIZE_C];
    volatile structm  cogC_mailbox C;
} parC;

/* start cogA */
int start_cogA(volatile void *parptr)
{ 
    int size = (_load_stop_rtc_cog - _load_start_rtc_cog)*4;	//code size in bytes
    printf("cogA code size %i bytes\n",size);
    unsigned int code[size];  				 //allocate enough HUB to hold the COG code
    memcpy(code, _load_start_rtc_cog, size); //assume xmmc
    return cognew(code, parptr);
}

/* start cogA */
void startA(volatile void *parptr)
{
    extern unsigned int _load_start_cogA_cog[];
#if defined(__PROPELLER_XMM__) || defined(__PROPELLER_XMMC__)
    load_cog_driver_xmm(_load_start_cogA_cog, 496, (uint32_t *)parptr);
#else
    cognew(_load_start_cogA_cog, parptr);
#endif
}

/* start cogB */
void startB(volatile void *parptr)
{
    extern unsigned int _load_start_cogB_cog[];
#if defined(__PROPELLER_XMM__) || defined(__PROPELLER_XMMC__)
    load_cog_driver_xmm(_load_start_cogB_cog, 496, (uint32_t *)parptr);
#else
    cognew(_load_start_cogB_cog, parptr);
#endif
}

/* start cogC */
void startC(volatile void *parptr)
{
    extern unsigned int _load_start_cogC_cog[];
#if defined(__PROPELLER_XMM__) || defined(__PROPELLER_XMMC__)
    load_cog_driver_xmm(_load_start_cogC_cog, 496, (uint32_t *)parptr);
#else
    cognew(_load_start_cogC_cog, parptr);
#endif
}

int main(void)
{
/* get some lock ids */
    parA.A.message_lock = locknew();
    parB.B.message_lock = locknew();
    parC.C.message_lock = locknew();
    sleep(2);  
/* start the A cog */
    printf("starting cogA\n");
    startA(&parA.A);
    while(lockset(parA.A.message_lock));
    printf("%s\n",parA.A.cog_message);
    lockclr(parA.A.message_lock);

/* start the B cog */
    printf("starting cogB\n");
    startB(&parB.B);
    while(lockset(parB.B.message_lock));
    printf("%s\n",parB.B.cog_message);
    lockclr(parB.B.message_lock);

/* start the C cog */
    printf("starting cogC\n");
    startC(&parC.C);
    while(lockset(parC.C.message_lock));
    printf("%s\n",parC.C.cog_message);
    lockclr(parC.C.message_lock);

/* loop forever */
    while(1) {

        sleep(2);
        printf("cogA: %s\n",parA.A.cog_message);
        printf("cogB: %s\n",parB.B.cog_message);
        printf("cogC: %s\n",parC.C.cog_message);
    }
    return 0;
}



