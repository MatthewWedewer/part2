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



uint8_t config_file()
{
		
	uint8_t * config_p, send_array[2] , get_array[1], error_flag;
	uint32_t index;
	
	gpio_rst = 1;
	
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
			error_flag = I2C_Write(STA013_Addr, 1, send_array);

			LED4=0;
	
		}
	}while(send_array[0] != 0xFF && error_flag == NO_ERRORS);
	if(error_flag != NO_ERRORS)	
	printf("%-10s", "co error1 ");
	printf("%4.4lX", index);
		putchar(10);
		putchar(13);
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
			error_flag = I2C_Write(STA013_Addr, 1, send_array);
			
				LED4=0;
		}
	}while(send_array[0] != 0xFF && error_flag == NO_ERRORS);
	if(error_flag != NO_ERRORS)
	printf("%-10s", "co error ");
	printf("%4.4lX", index);
		putchar(10);
		putchar(13);
	
	//reset
	gpio_rst = 0;
	gpio_rst = 1;
	
	//Set PCM_Div
	send_array[0] = 0x54;
	send_array[1] = 0x07;
	get_array[0] = send_array[0];
	I2C_Write(STA013_Addr, 1, send_array);
	error_flag = I2C_Read(STA013_Addr, 1, get_array);
	printf("%-10s", "send1 ");
	printf("%2.2bX", send_array[1]);
	printf("%-10s", "get1 ");
	printf("%2.2bX", get_array[0]);
	putchar(10);
	putchar(13);
	
	//Set PCM_conf
	send_array[0] = 0x55;
	send_array[1] = 0x10;
	get_array[0] = send_array[0];
	I2C_Write(STA013_Addr, 1, send_array);
	I2C_Read(STA013_Addr, 1, get_array);
	printf("%-10s", "send2 ");
	printf("%2.2bX", send_array[1]);
	printf("%-10s", "get2 ");
	printf("%2.2bX", get_array[0]);
	putchar(10);
	putchar(13);
	
	//Set PLL_conf
	send_array[0] = 6;
	send_array[1] = 9;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 11;
	send_array[1] = 2;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 97;
	send_array[1] = 5;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 80;
	send_array[1] = 6;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 101;
	send_array[1] = 0;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 82;
	send_array[1] = 184;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 100;
	send_array[1] = 0;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 81;
	send_array[1] = 0;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 5;
	send_array[1] = 161;
	I2C_Write(STA013_Addr, 1, send_array);	
	
	//Set SCLK_POL
	send_array[0] = 0x03;
	send_array[1] = 0x00;
	I2C_Write(STA013_Addr, 1, send_array);	
	
	//Data_req_EN
	send_array[0] = 0x18;
	send_array[1] = 0x04;
	I2C_Write(STA013_Addr, 1, send_array);	
	
	//Data_req_pol
	send_array[0] = 0x0C;
	send_array[1] = 0x05;
	I2C_Write(STA013_Addr, 1, send_array);	
	
	//Attinuation DLA_DRA
	send_array[0] = 0x46;
	send_array[1] = 0x08;
	I2C_Write(STA013_Addr, 1, send_array);	
	send_array[0] = 0x48;
	send_array[1] = 0x08;
	I2C_Write(STA013_Addr, 1, send_array);	
	
	// Run forest RUN!!!
	send_array[0] = 0x72;
	send_array[1] = 0x01;
	I2C_Write(STA013_Addr, 1, send_array);	
	// WHO RA!!!

	return error_flag;
}


uint8_t test_I2C(uint8_t * array_name)
{
	
	uint8_t i, error_flag;
	error_flag = NO_ERRORS;
	i = timeout_val;
	trig = 0;
	i = timeout_val;
	do
	{
		error_flag = I2C_Read(0x43,1,array_name);
		i--;
	}while( (error_flag!=NO_ERRORS) && (i!=timeout_val) );
	if(error_flag != NO_ERRORS)
	{
		printf("%-10s", "test_error ");
		putchar(10);
		putchar(13);
	}
	printf("Received Value = %2.2bX\n\r", array_name[0]);
	trig = 1;
	return error_flag;
}