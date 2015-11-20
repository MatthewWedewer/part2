#include "main.h"
#include "port.h"
#include <stdio.h>
#include "uart.h"
#include "print_bytes.h"
#include "hardware_delay.h"
#include "lcd.h"
#include "spi.h"
#include "SDcard.h"
#include "Long_Serial_In.h"
#include "Directory_Functions.h"
#include "i2c.h"
#include "sta013.h"

extern uint8_t code CONFIG;
extern uint8_t code CONFIG2;



void config()
{
	
	uint8_t * config_p, send_array[2], error_flag;
	uint32_t index;
	
	
	
	
	config_p =& CONFIG;
	index = 0;
	do 
	{
		send_array[0] = *(config_p + index);
		index ++;
		send_array[1] = *(config_p + index);
		index++;
		if (send_array[0] != 0xFF)
		{
			error_flag = I2C_Write(STA013_Addr, 2, send_array);
		}
	}while(send_array[0] != 0xFF);
	
	config_p =& CONFIG2;
	index = 0;
	do 
	{
		send_array[0] = *(config_p + index);
		index ++;
		send_array[1] = *(config_p + index);
		index++;
		if (send_array[0] != 0xFF)
		{
			error_flag = I2C_Write(STA013_Addr, 2, send_array);
		}
	}while(send_array[0] != 0xFF);
}


void test_I2C(uint8_t * array_name)
{
	
	uint8_t i, error_flag;

	i = timeout_val;
	do
	{
		error_flag = I2C_Write(0x43,1,array_name);
		i--;
	}while( (error_flag!=NO_ERRORS) && (i!=0) );
	i = timeout_val;
	do{
		error_flag = I2C_Read(0x43,1,array_name);
		i--;
		}while( (error_flag!=NO_ERRORS) && (i!=0) );
	printf("Received Value = %2.2bX\n\r", array_name[0]);
}