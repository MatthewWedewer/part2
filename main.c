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
#include "read_sector.h"
void main(void)
{
	uint8_t error_flag;
	uint32_t block_number;
	uint8_t xdata block_info[512];

	LED3=0;
	UART_Init();
	LCD_Init();
	SPI_master_init(400000); // Set clock rate to that speed in Hz
	error_flag = SDcard_init();
	
	while(1)
	{
		if(error_flag != NO_ERRORS)
		{
			LED4 = 0;
			LCD_Clear();
			LCD_Write(COMMAND, LINE1);
			LCD_Print(1, &error_flag);
			
			delay_ms(1000);	
   			printf("%2.2bX ",error_flag);   		
			putchar(10);
			putchar(13);
			delay_ms(1000);
		}
		else
		{
			delay_ms(1000);
			LCD_Clear();
			LCD_Write(COMMAND, LINE1);
			LCD_Print(13, "Enter a Block");
			LCD_Write(COMMAND, LINE2);
			LCD_Print(8, "Number: ");
			//delay_ms(3000);
			
			printf("%-35s", "Enter a Block Number");
		  
			block_number = long_serial_input();		// returns uint32_t 
			LCD_Clear();
			
			error_flag = read_block(block_number, block_info);
			LCD_Print(13, "Read Success!");
			print_memory(block_info,512);
			
		}

		
		
		
		
//		LCD_Write(COMMAND, LINE1);
//		LCD_Print(16, "Coltons and Matt");
//		LCD_Write(COMMAND, LINE2);
//		LCD_Print(15, "hews LCD Works!");
//		delay_ms(3000);
//		LCD_Clear();
	}

}

