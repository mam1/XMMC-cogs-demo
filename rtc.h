
/**************************************/
/*  rtc.h -                           */
/*  header file for rtc.cogc          */ 
/**************************************/

#ifndef _RTC_H_
#define _RTC_H_

#define _rtcClockPin    28              //i2c - DS3231 clock
#define _rtcDataPin     29              //i2c - DS3231 data

/* set i2c high by allowing the pin to float high, set low by forcing it low */
#define i2c_float_scl_high  DIRA &= ~(1<<scl)
#define i2c_set_scl_low     DIRA |= (1<<scl)
#define i2c_float_sda_high  DIRA &= ~(1<<sda)
#define i2c_set_sda_low     DIRA |= (1<<sda)

/*  DS3231 stuff */
#define DS3231_Address              0x68                                                              
#define DS3231_Read_addr            ((DS3231_Address << 1) | 0x01) 
#define DS3231_Write_addr           ((DS3231_Address << 1) & 0xFE)
#define ACK                         0
#define NACK                        1

/* rtc cog control block definitions */

typedef volatile struct 
{
    int                 	tdb_lock;   	//lock ID for time date buffer
    uint8_t                 cog;            //cog ID of the cog the rtc.cog code is running on, -1 = not running
    uint8_t                 abt;        	//!= 0 cog requests a system abort,value = error code
    uint8_t                 update_sec;     //trigger update, 1=wait, 0=update 
    uint8_t                 update_day; 	//trigger update, 1=wait, 0=update 
    uint8_t     			sec;
    uint8_t     			min;
    uint8_t     			hour;
    uint8_t     			day;
    uint8_t     			month;
    uint8_t     			year;
    uint8_t     			dow;
}RTC_CB;
    
#endif // _RTC_H_