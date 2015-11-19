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




	

extern uint8_t code CONFIG, CONFIG2;
extern uint32_t	idata FirstRootDirSec;


void main(void)
{
	uint8_t error_flag, array_name[2];
	
	uint16_t number_of_entries;
	uint8_t xdata block_info[512];

	LED3=0;
	UART_Init();
	LCD_Init();
	SPI_master_init(400000); // Set clock rate to that speed in Hz
	error_flag = SDcard_init();
	printf("%-20s", "Mounting Drive");
	putchar(10);
	putchar(13);
	error_flag = mount_drive(block_info);
	number_of_entries = Print_Directory(FirstRootDirSec, block_info);
	LCD_Clear();
	LCD_Write(COMMAND, LINE1);

	LCD_Print(9,"init done");
	
test_I2C(array_name);
config();
	
//	while(1)
//	{
//		current_sector = FirstRootDirSec;
//		while(error_flag == NO_ERRORS)
//		{
//			do
//			{
//				printf("%-35s", "Enter a Block Number: ");
//				block_number = long_serial_input();
//				if(block_number > number_of_entries || block_number == 0)
//				{
//					printf("%-35s", "Number too large.\n\r");
//				}
//			}while(block_number > number_of_entries || block_number == 0);
//			return_entry = Read_Dir_Entry(current_sector, block_number, block_info);
//			printf("%-20s", "return_entry");
//			printf("%8.8lX", return_entry);
//			putchar(10);
//			putchar(13);
//			next_entry = return_entry & 0x0FFFFFFF;
//			if(return_entry & 0x10000000)
//			{			
//				printf("%-20s", "was a directory");
//				putchar(10);
//				putchar(13);
//				current_sector = first_sector(next_entry);
//				printf("%-20s", "current_sector");
//				printf("%8.8lX", current_sector);
//				putchar(10);
//				putchar(13);
//				number_of_entries = Print_Directory(current_sector, block_info);
//			}
//			if((return_entry & 0x10000000) == 0 && block_number !=0)
//			{
//				printf("%-20s", "was a file");
//				Open_File(next_entry, block_info);
//				number_of_entries = Print_Directory(current_sector, block_info);
//			}
//			if(return_entry & 0x80000000)
//			{
//				error_flag = PRINT_ERROR;
//			}
//		}
//		if(error_flag!= NO_ERRORS)
//		{
//			LED4 = 0;
//			putchar(10);
//			putchar(13);
//			printf("%-10s", "ERROR! ");
//			printf("%2.2bX", error_flag);
//			putchar(10);
//			putchar(13);
//			while(1);
//		}
//	}
}

		
		
		
		
		
		
		
		
		
		
		
//		if(error_flag != NO_ERRORS)
//		{
//			LED4 = 0;
//			LCD_Clear();
//			LCD_Write(COMMAND, LINE1);
//			LCD_Print(1, &error_flag);
//			
//			delay_ms(1000);	
//   			printf("%2.2bX ",error_flag);   		
//			putchar(10);
//			putchar(13);
//			delay_ms(1000);
//		}
//		else
//		{
//			delay_ms(1000);
//			LCD_Clear();
//			LCD_Write(COMMAND, LINE1);
//			LCD_Print(13, "Enter a Block");
//			LCD_Write(COMMAND, LINE2);
//			LCD_Print(8, "Number: ");
//			//delay_ms(3000);
//			
//			printf("%-35s", "Enter a Block Number");
//		  
//			block_number = long_serial_input();		// returns uint32_t 
//			LCD_Clear();
//			
//			error_flag = read_block(block_number, block_info);
//			LCD_Print(13, "Read Success!");
//			print_memory(block_info,512);
//			
//		}

//		
//		
//		
//		
////		LCD_Write(COMMAND, LINE1);
////		LCD_Print(16, "Coltons and Matt");
////		LCD_Write(COMMAND, LINE2);
////		LCD_Print(15, "hews LCD Works!");
////		delay_ms(3000);
////		LCD_Clear();
//	}

//}

