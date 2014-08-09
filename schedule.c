/**
*	schedule.c  - version 2.0
*
* 	collection of routines to maintain schedules
*
*/
#include <propeller.h>
#include <stdio.h>
#include <stdlib.h>
#include <i2c.h>
#include "schedule.h"
#include "main.h"

/*************************** drivers  ********************************/
extern _Driver _FullDuplexSerialDriver;
extern _Driver _FileDriver;
_Driver *_driverlist[] = {
  &_FullDuplexSerialDriver,
  &_FileDriver,
  NULL
};

/* globals */
static I2C_COGDRIVER i2c;
static I2C* pI2c;
static int scl = 28;
static int sda = 29;

struct
{
  uint8_t       b0;
  uint8_t       b1;
  uint8_t       buf[SCHEDULE_BYTES];
} record_buffer;

uint32_t            *srec;
/* control block & stack for cogB */
extern struct {
    unsigned            stack[STACK_B];
    CONTROL_BLOCK_B       B;
} parB;


#define EEPROM_ADDR 0xA0

/*


uint8_t EEPROM_ReadByte(int addr)
{
     uint8_t  data[2] = { addr >> 8, addr & 0xFF };
     uint8_t  value;
     chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, data, 2, 0));
     chk("i2cRead", i2cRead(pI2c, EEPROM_ADDR, &value, 1, 1)); 
     return value;
}

void EEPROM_WriteByte(int addr, uint8_t value)
{
     uint8_t  data[3] = { addr >> 8, addr & 0xFF, value };
     // chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, 3, 1));
}

void EEPROM_WriteBuffer(int addr, uint8_t value)
{
     uint8_t  data[3] = { addr >> 8, addr & 0xFF, value };
     chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, data, 3, 1));
}
uint8_t show_byte(int addr)
{
    uint8_t byte = EEPROM_ReadByte(addr);
    printf("[%04x]: %02x\n", addr, byte);
    return byte;
}

uint32_t key_calc(int c,int d,int h,int m)
{
    return (c*7*24*60)+(d*24*60)+(h*60)+m;
}



int EEPROM_ReadSchedule(int c,int d,uint32_t *s)
{
     // uint8_t  data[2] = { addr >> 8, addr & 0xFF };
     uint8_t  value;
     // chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, data, 2, 0));
     // chk("i2cRead", i2cRead(pI2c, EEPROM_ADDR, &value, 1, 1)); 
     return value;
}

int EEPROM_WriteSchedule(uint32_t *s)
{
    // uint8_t  data[3] = { addr >> 8, addr & 0xFF, value };
    // chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, data, 3, 1)); 
    return 0;  
}

int sd2eeprom(FILE *f, uint32_t *b)
{
	int 		eeprom_addr,i;
	printf("sd2eeprom called\n");
  eeprom_addr = _HIGH_EEPROM;

	for(i=0;i<_DAYS_PER_WEEK;i++)
	{
		// fread()
		// EEPROM_WriteSchedule(_ACTIVE, );
	}
	return;
}
*/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/

void chk(char *fcn, int sts)
{
    if (sts != 0) {
        printf("%s failed: %d\n", fcn, sts);
    exit(1);
    }
}
/************************************* schedule routines ***************************************/
/* get schedule - copy eeprom to buffer */
char *get_sch(char *buffer,int day,int channel,int eeprom_page)
{
  int         eeprom_addr;
  eeprom_addr = _HIGH_EEPROM;
  eeprom_addr += _SCHEDULE_BUFFER*day+SCHEDULE_BYTES*channel;
  record_buffer.b0 = (eeprom_addr >> 8) & 0xFF;
  record_buffer.b1 =  eeprom_addr & 0xFF;
  chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, &record_buffer.b0, 2, 0));
  chk("i2cRead", i2cRead(pI2c, EEPROM_ADDR, &record_buffer.buf, SCHEDULE_BYTES, 1));     
  printf("  day %i, channel %i, page %i, copied from eeprom starting at %x to buffer\n",day,channel,eeprom_page,eeprom_addr);  
  return buffer;
}

/* put schedule - copy buffer to eeprom  */
void put_sch(char *buffer,int day,int channel,int eeprom_page)
{
  int         eeprom_addr;
  eeprom_addr = _HIGH_EEPROM;
  eeprom_addr += _SCHEDULE_BUFFER*day+SCHEDULE_BYTES*channel;
  record_buffer.b0 = (eeprom_addr >> 8) & 0xFF;
  record_buffer.b1 =  eeprom_addr & 0xFF;
  chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, &record_buffer.b0, SCHEDULE_BYTES+2, 1));
  printf("  day %i, channel %i, page %i, copied from buffer to eeprom starting at %x\n",day,channel,eeprom_page,eeprom_addr);  
  return;
}






/********************************* schedule buffer routines *************************************/
/* load schedule buffer from high eeprom */
int load_sch_buf(int day)
{
  int         eeprom_addr;
  eeprom_addr = _HIGH_EEPROM;
  eeprom_addr += sizeof(parB.B.schedule_buffer)*day;
  parB.B.eeprom_addr[0] = (eeprom_addr >> 8) & 0xFF;
  parB.B.eeprom_addr[1] =  eeprom_addr & 0xFF;
  chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, parB.B.eeprom_addr, sizeof(parB.B.schedule_buffer), 1));         
  printf("  day %i,channel %i, page %i, copied to eeprom starting at %x\n",day,eeprom_addr);
}



/********************************* schedule file routines *************************************/
/* copy schedules from a file on the sd card to high eeprom */
int cpy_sch_2_eeprom(char *file_name,int eeprom_page)
{
  int         d,ret;
  FILE        *fptr;
  int         eeprom_addr;
  printf("  loading eeprom from %s\n",file_name);
  eeprom_addr = _HIGH_EEPROM;
  if((fptr = fopen(file_name,"r")) == 0)
  {
    printf("* error in cpy_sch_2_eeprom, can't open <%s>\n",file_name);
    return 1;
  }
  for(d=0;d<_DAYS_PER_WEEK;d++)
  {
      /* load buffer from the sd card */
      ret = fread(parB.B.schedule_buffer,sizeof(parB.B.schedule_buffer),1,fptr);
      if(ret != 1)
      {
          printf("* error in cpy_sch_2_eeprom, bad read, ret = <%i>\n",ret);
          fclose(fptr);
          return 1;
      }
      /*  copy the buffer to high eeprom */
      parB.B.eeprom_addr[0] = (eeprom_addr >> 8) & 0xFF;
      parB.B.eeprom_addr[1] =  eeprom_addr & 0xFF;
      chk("i2cWrite", i2cWrite(pI2c, EEPROM_ADDR, parB.B.eeprom_addr, sizeof(parB.B.schedule_buffer), 1));      
      printf("  day %i copied from sd card to eeprom stariting at %x\n",d,eeprom_addr);
      eeprom_addr += sizeof(parB.B.schedule_buffer);
  }
  fclose(fptr);
  return 0;
}
/* write schedules from high eeprom to a file on the sd card */
int cpy_sch_2_sd(char *file_name, int eeprom_page)
{
  int         d,ret;
  FILE        *fptr;
  int         eeprom_addr;
  printf("  writing %s from eeprom\n",file_name);
  eeprom_addr = _HIGH_EEPROM;
  if((fptr = fopen(file_name,"w")) == 0)
  {
    printf("* error in cpy_sch_2_sd, can't open <%s>\n",file_name);
    return 1;
  }
  for(d=0;d<_DAYS_PER_WEEK;d++)
  {
      /* load buffer from eeprom */
      load_sch_buf(d);

      /*  copy the buffer to file on the sd card */
      ret = fwrite(parB.B.schedule_buffer,sizeof(parB.B.schedule_buffer),1,fptr);
      if(ret != 1)
      {
          printf("* error in cpy_sch_2_sd, bad write, ret = <%i>\n",ret);
          fclose(fptr);
          return 1;
      }
  }
  fclose(fptr);
  return 0;
}
int EEPROM_Init(void)
{
     pI2c = i2cOpen(&i2c, scl, sda, 400000);
     if ( pI2c == 0 ) {
         return -1;
     }
     return 0;
}

int init_sch(char *file_name)
{
	int 			      ret,i,ii,c,d,size;
  uint8_t         *buf_ptr;
  int             eeprom_addr;
	FILE 			       *fptr;

	printf("init_sch called\n");
  if(EEPROM_Init())
  {
      printf("** error in init_sch, bad i2c open\n");
      return 1;  
  }
	size = sizeof(parB.B.schedule_buffer)/4;
	printf("size = %i\n",size);
	for(i=0;i<size;i++)
		parB.B.schedule_buffer[i] = 0;
	printf("schedule buffer set to zero\nattempt to open <%s>\n",file_name);
	fptr = fopen(file_name,"r");
	if(fptr)	//found a schedule file
  {
		printf("found existing schedule file\n");
    fclose(fptr);
  }
  else
  {
    printf("schedule file <%s> not found\nattempt to create schedule file\n",file_name);
  	fptr = fopen(file_name,"w");
  	if(fptr != NULL)
  	{
      printf("<%s> opened\n",file_name);
  		for(i=0;i<_DAYS_PER_WEEK;i++)
  		{
  			ret = fwrite(parB.B.schedule_buffer,sizeof(parB.B.schedule_buffer),1,fptr);
  			if(ret != 1)
  			{
  				printf("** error in init_sch, bad write\n");
  				fclose(fptr);
  				return 1;
  			}
  		}
  		fclose(fptr);
  		printf("%s initialized\n",file_name);
  	}
  	else
  	{
  		printf("** error in init_sch, bad w open, returned %i\n",fptr);
  		fclose(fptr);
  		return 1;
  	}
  }
  printf("load eeprom from %s\n",file_name);
  if(cpy_sch_2_eeprom(file_name,0))
  {
    printf("* error bad return from cpy_sch_2_eeprom\n");
    return 1;
  }
  printf("  eeprom loaded\n");
  return 0;
}
