#ifndef MAIN_H_
#define MAIN_H_

#define STACK_A		30					//stack size for cog A
#define STACK_B		30					//stack size for cog B
#define STACK_C		30					//stack size for cog C

/* control block data structure */
typedef volatile struct 
{
    int		query_flag;	//!= 0, requests a counter update
    int     query_ctr;	//number of time cog has been queried   	

} CONTROL_BLOCK;

#endif
