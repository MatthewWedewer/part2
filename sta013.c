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


void test_I2C(uint8_t * array_name)
{
	
	uint8_t i, error;

	i = timeout_val;
	do{
		error = I2C_Write(0x43,1,array_name);
		i--;
		}while( (error!=NO_ERRORS) && (i!=0) );
	i = timeout_val;
	do{
		error = I2C_Read(0x43,1,array_name);
		i--;
		}while( (error!=NO_ERRORS) && (i!=0) );
	printf("Received Value = %2.2bX\n\r", array_name[0]);
}