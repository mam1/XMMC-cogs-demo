/**
*	schedule.h - version 2
*
*	header file for schedule.c
*
*/

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#define EEPROM_0	0x8000		
#define EEPROM_1		
#define EEPROM_2		
#define EEPROM_2		


uint32_t sch_addr(int,int,int);		//(page,channel,day) return a pointer to a schedule
int cpy_sch_2_eeprom(char *,int);	//copy schedules from a file on the sd card to high eeprom
int cpy_sch_2_sd(char *, int);		//write schedules from high eeprom to a file on the sd card 
void put_sch(char *,int,int,int);
char *get_sch(char *,int,int,int);

int init_sch(char *);				//initial system schedule

#endif