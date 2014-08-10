/**************************************/
/*  rtc.h -                           */
/*  header file for rtc.cogc          */ 
/**************************************/

#ifndef _RTC_H_
#define _RTC_H_

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

/* rtc cog control block */
typedef volatile struct 
{
    int                 tdb_lock;   //lock ID for time date buffer
    int                 abt;        //!= 0 cog requests a system abort,value = error code
    int                 update;     //trigger update flag, 1=wait, 0=update 
    int                 new_day;
    int                 new_second;
    TD_BUF              td_buffer;  //time, date & dow stored as uint8_t 
}RTC_CB;
    
#endif // _RTC_H_