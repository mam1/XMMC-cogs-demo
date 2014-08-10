#ifndef MAIN_H_
#define MAIN_H_

#define VERSION			"1.2" 			//version number

#define STACK_A			30				//stack size for cog A
#define STACK_B			30				//stack size for cog B
#define STACK_C			30				//stack size for cog C
#define _STACK_SIZE_RTC		30				//stack size for rtc cog


#define INPUT_BUFFER	128				//input buffer size
#define COMMANDS		12				//number of commands 

/* control block data structure */
typedef volatile struct 
{
    int		query_flag;	//!= 0, requests a counter update
    int     query_ctr;	//number of times cog has been queried 
    int		cog;		//number of the cog the code is runing on  	
} CONTROL_BLOCK;

#endif
