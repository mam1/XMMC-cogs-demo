/**
*	schedule.h - version 2
*
*	header file for schedule.c
*
*/

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#define EEPROM_ADDR 0xA0
		


// uint32_t sch_addr(int,int,int);		//(page,channel,day) return a pointer to a schedule


int init_sch(char *);				//initialize system schedule

int cpy_sch_2_eeprom(char *,int);	//copy schedules from a file on the sd card to high eeprom (filen name, eeprom page)
int cpy_sch_2_sd(char *, int);		//write schedules from high eeprom to a file on the sd card (file name, eeprom page )

int load_sch_buf(int);				//fetch the working schedule for the requested day from eeprom (day)


/* routines that work with sdchedules */
void put_sch(char *,int,int,int);	//write a schedule to high eeprom
char *get_sch(char *,int,int,int);	//fetch a schedule from high eeprom



/* routines to work with individual schedule records */
 int  get_key(uint32_t);          			//extract key (lower 31 bits) from a uint32_t
 int  get_s(uint32_t);        			//extract state (high bit) from a uint32_t
 void put_key(volatile uint32_t *,int);    	//load key into lower 31 bits of the  uint31_t at the address specified
 void put_state(volatile uint32_t *,int);  	//load state into high bit of the uint31_t at the address specified



#endif