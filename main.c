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
#include "Directory_Functions.h"



	


extern uint32_t	idata RootDirSec;


void main(void)
{
	uint8_t error_flag;
	uint32_t block_number, return_entry, next_entry, next_sector;
	uint16_t number_of_entries;
	uint8_t xdata block_info[512];

	LED3=0;
	UART_Init();
	LCD_Init();
	SPI_master_init(400000); // Set clock rate to that speed in Hz
	error_flag = SDcard_init();
	printf("%-20s", "  Mounting Drive ");
	error_flag = mount_drive();
	number_of_entries = Print_Directory(RootDirSec, block_info);
	
	while(1)
	{
		if(error_flag!= NO_ERRORS)
		{
		printf("%2.2bX", error_flag);
			putchar(10);
			putchar(13);
		}
		while(error_flag == NO_ERRORS)
		{
			do
			{
				printf("%-35s", "Enter a Block Number");
				block_number = long_serial_input();
			}while(block_number > number_of_entries);
			return_entry = Read_Dir_Entry(block_number, 512, block_info);
			next_entry = return_entry | 0x0FFFFFFF;
			if(return_entry | 0x10000000)
			{			
				while(!(!(return_entry | 0x10000000) || (return_entry | 0x80000000) || block_number == 0));
				{
					
					next_sector = first_sector(next_entry);
					number_of_entries = Print_Directory(next_sector, block_info);
					do
					{
						printf("%-35s", "Enter a Block Number");
						block_number = long_serial_input();
					}while(block_number > number_of_entries);
					return_entry = Read_Dir_Entry(block_number, 512, block_info);
					next_entry = return_entry | 0x0FFFFFFF;
				}
			}
			if((return_entry |0x10000000) == 0 && block_number !=0)
			{
				Open_File(next_entry, block_info);
				next_entry = return_entry | 0x0FFFFFFF;
			}
			if(return_entry | 0x80000000)
			{
				error_flag = PRINT_ERROR;
			}
		}
	}
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

