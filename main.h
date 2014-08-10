#ifndef MAIN_H_
#define MAIN_H_

#define VERSION				"1.6" 					//version number

#define STACK_A				30						//stack size for cog A
#define STACK_B				30						//stack size for cog B
#define STACK_C				30						//stack size for cog C
#define _STACK_SIZE_RTC     30

#define INPUT_BUFFER		128						//input buffer size
#define COMMANDS			19						//number of commands
#define CHANNELS 			8
#define _DAYS_PER_WEEK 		7
#define SCHEDULES 			_DAYS_PER_WEEK * CHANNELS
#define SCHEDULE_RECORDS 	11						//max number of schedule records plus 1
#define SCHEDULE_BYTES		4 * SCHEDULE_RECORDS 	//bytes per schedule
#define _HIGH_EEPROM			0x8000					//start addressss of available eeprom
#define _SCHEDULE_BUFFER			(_BYTES_PER_SCH_RECORD * (_SCHEDULE_RECORDS + 1)) * _NUMBER_OF_CHANNELS
#define _BYTES_PER_SCH_RECORD		4
#define _SCHEDULE_RECORDS			10
#define _NUMBER_OF_CHANNELS			8
#define _SCHEDULE_FILE_NAME			"test04.it"

/* control block data structure */
typedef volatile struct 
{
    int		query_flag;	//!= 0, requests a counter update
    int     query_ctr;	//number of times cog has been queried 
    int		cog;		//number of the cog the code is runing on  	
} CONTROL_BLOCK_A;
/* control block data structure */
typedef volatile struct 
{
    int			query_flag;	//!= 0, requests a counter update
    int     	query_ctr;	//number of times cog has been queried 
    int			cog;		//number of the cog the code is runing on 
    char        eeprom_addr[2]; //
    uint32_t	schedule_buffer[_SCHEDULE_BUFFER/4];
    int 		error_flag;	//!=0 error,	
 	
} CONTROL_BLOCK_B;
/* control block data structure */
typedef volatile struct 
{
    int		query_flag;	//!= 0, requests a counter update
    int     query_ctr;	//number of times cog has been queried 
    int		cog;		//number of the cog the code is runing on  	
} CONTROL_BLOCK_C;
#endif
