#ifndef MAIN_H_
#define MAIN_H_

#define VERSION				"1.6" 					//version number

#define STACK_A				30						//stack size for cog A
#define STACK_B				30						//stack size for cog B
#define STACK_C				30						//stack size for cog C

#define INPUT_BUFFER		128						//input buffer size
#define COMMANDS			11						//number of commands
#define CHANNELS 			8
#define DAYS_PER_WEEK 		7
#define SCHEDULES 			DAYS_PER_WEEK * CHANNELS
#define SCHEDULE_RECORDS 	11						//max number of schedule records plus 1
#define SCHEDULE_BYTES		4 * SCHEDULE_RECORDS 	//bytes per schedule

/* control block data structure */
typedef volatile struct 
{
    int		query_flag;	//!= 0, requests a counter update
    int     query_ctr;	//number of times cog has been queried 
    int		cog;		//number of the cog the code is runing on  	
} CONTROL_BLOCK;

#endif
