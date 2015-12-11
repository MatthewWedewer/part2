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

uint8_t idata temp8;
uint32_t idata state_g, sector_offset, cluster_g, play_status;

uint16_t idata index1_g, index2_g;
uint32_t idata sector_base_g, return_entry, next_entry, block_number, sector;

uint8_t xdata buff1[512];
uint8_t xdata buff2[512];





void timer2_ISR(void) interrupt 5
{
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	

	TF2 = 0;
	TR0 = 0;
	TH0 = TIMER0H;
	TL0 = TIMER0L;
	TF0 = 0;
	TR0 = 1;
	
	
	if(state_g == LOAD_BUFFER_1)
	{
		if(sector_offset >= 64)
		{
			state_g = FIND_CLUSTER_1;
		}
	}

	if(state_g == LOAD_BUFFER_2)
	{
		if(sector_offset >= 64)
		{
			state_g = FIND_CLUSTER_2;
		}
	}


	switch(state_g)
	{
		// Data Send 1
		case DATA_SEND_1:
		{
			LED1 = 0;
			LED2 = 1;
			LED3 = 1;
			LED4 = 1;
			spi_en = 1;
			while((DATA_REQ == ACTIVE) && (TF0 == 0))  								
			{
				SPI_transfer_ISR(buff1[index1_g], & temp8); 							// What is temp8
				index1_g++;																								// Loosing last bit?
				if(index1_g > 511) // Buffer 1 empty
				{
					if(index2_g >511)
					{
						if(play_status == 3)																	
						{
							play_status = 0;
						}
						else
						{
							state_g = LOAD_BUFFER_2; // Buff 2 and Buff 1 empty
						}
					}																												
					else
					{
						state_g = DATA_SEND_2; // BUFF 1 empty  							//Seems like this is when buffer 1 still has stuff
					}
					TF0 = 1;
				}				// What does this do? is this the interupt for the whole system to keep pace
			}
			if((DATA_REQ == INACTIVE) && (state_g == DATA_SEND_1))
			{
				printf("INACTIVE\n\r");
				if(index2_g > 511) // Buffer 2 is empty
				{
					state_g = LOAD_BUFFER_2; // DR inactive and BUFF 2 empty
				}
				else
				{
					state_g = DATA_IDLE_1; // DR interupt
				}
			}
			spi_en = 0;
			break;
		}
		
		//Load Buffer 1
		case LOAD_BUFFER_1:
		{
			LED1 = 1;
			LED2 = 0;
			LED3 = 1;
			LED4 = 1;
			//printf("LOAD_BUFFER_1\n\r");
			sector = sector_base_g + sector_offset;
			read_sector_ISR(sector, buff1);
			sector_offset++;
			state_g = DATA_IDLE_2;
			index1_g = 0;
			break;
		}
		
		// Find Cluster 1
		case FIND_CLUSTER_1:
		{
			LED1 = 0;
			LED2 = 0;
			LED3 = 1;
			LED4 = 1;
			printf("FIND_CLUSTER_1\n\r");
			cluster_g = find_next_cluster_ISR(cluster_g, buff1);
			if(cluster_g == 0x0FFFFFFF) // Last cluster
			{
				printf("DONE\n\r");
				play_status = 3;
				state_g = DATA_IDLE_2;
			}
			else
			{
				sector_base_g = first_sector_ISR(cluster_g);
				sector_offset = 0;
				state_g = DATA_IDLE_2;
			}
			break;
		}
		
		case DATA_IDLE_1:
		{
			LED1 = 1;
	LED2 = 1;
	LED3 = 0;
	LED4 = 1;
			//printf("DATA_IDLE_1\n\r");
			if(DATA_REQ == ACTIVE)
			{
				state_g = DATA_SEND_1;
			}
			break;
		}
				
		// Data Send 2
		case DATA_SEND_2:
		{
			LED1 = 0;
	LED2 = 1;
	LED3 = 0;
	LED4 = 1;
			//printf("DATA_SEND_2\n\r");
			spi_en = 1;
			while((DATA_REQ == ACTIVE) && (TF0 == 0))  								// Can DATA_REQ go inactive while in the loop
			{
				SPI_transfer_ISR(buff1[index2_g], & temp8); 							// What is temp8
				index2_g++;
				if(index2_g > 511) // Buffer 2 empty
				{
					if(index1_g > 511) // Buffer 1 empty
					{
						if(play_status == 3)																	// Works only if after FIND_CLUSTER. why?
						{
							play_status = 0;
						}
						else
						{
							state_g = LOAD_BUFFER_1; // Buff 1 and Buff 2 empty
						}
					}																												// No FIND_CLUSTER
					else
					{
						state_g = DATA_SEND_1; // BUFF 2 empty  							//Seems like this is when buffer 2 still has stuff
					}
					TF0 = 1;																								// What does this do? is this the interupt for the whole system to keep pace
				}
			}
			if((DATA_REQ == INACTIVE) && (state_g == DATA_SEND_2))
			{
				if(index1_g > 511) // Buffer 1 is empty
				{
					state_g = LOAD_BUFFER_1; // DR inactive and BUFF 1 empty
				}
				else
				{
					state_g = DATA_IDLE_2; // DR interupt
				}
			}
			spi_en = 0;
			break;
		}
		
		//Load Buffer 2
		case LOAD_BUFFER_2:
		{
			LED1 = 1;
	LED2 = 0;
	LED3 = 0;
	LED4 = 1;
			//printf("LOAD_BUFFER_2\n\r");
			sector = sector_base_g + sector_offset;
			read_sector_ISR(sector, buff2);
			sector_offset++;
			state_g = DATA_IDLE_1;
			index2_g = 0;
			break;
		}
		
		// Find Cluster 2
		case FIND_CLUSTER_2:
		{
			LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 1;
			printf("FIND_CLUSTER_2\n\r");
			cluster_g = find_next_cluster_ISR(cluster_g, buff2);
			if(cluster_g == 0x0FFFFFFF)
			{
				play_status = 3;
				state_g = DATA_IDLE_1;
			}
			else
			{
				sector_base_g = first_sector_ISR(cluster_g);
				sector_offset = 0;
				state_g = DATA_IDLE_1;
			}
			break;
		}
		
		case DATA_IDLE_2:
		{
			LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 0;
			//printf("DATA_IDLE_2\n\r");
			if(DATA_REQ == ACTIVE)
			{
				state_g = DATA_SEND_2;
			}
			break;
		}						
	}
}








void main(void)
{
	uint8_t error_flag, array_name[2];
	
	
	uint16_t number_of_entries;

	trig = 1;
	LED1=0;
	UART_Init();
	LCD_Init();
	SPI_master_init(400000); // Set clock rate to that speed in Hz
	error_flag = SDcard_init();
	printf("%-20s", "Mounting Drive");
	putchar(10);
	putchar(13);
	error_flag = mount_drive(buff1);
	number_of_entries = Print_Directory(FirstRootDirSec, buff1);
	LCD_Clear();
	LCD_Write(COMMAND, LINE1);

	LCD_Print(9,"init done");
	LED2=0;
	array_name[0] = 0x01;
	array_name[1] = 0xAC;
	error_flag = test_I2C(array_name);
if (error_flag != NO_ERRORS)
	{
		LED4=0;
		printf("%-10s", "ERROR1! ");
		printf("%2.2bX", error_flag);
		putchar(10);
		putchar(13);
	}

	error_flag = config_file();
	if (error_flag != NO_ERRORS)
	{
		LED4=0;
		printf("%-10s", "config err ");
		printf("%2.2bX", error_flag);
		putchar(10);
		putchar(13);
	}
		

	LED3=0;
	

	if (error_flag != NO_ERRORS)
	{
		LED4=0;
		printf("%-10s", "test_i2c err ");
		printf("%2.2bX", error_flag);
		putchar(10);
		putchar(13);
	}

	TH2 = TIMER2H;
	TL2 = TIMER2L;
	T2CON = 0x80;
	TR0 = 0;
	TH0 = TIMER0H;
	TL0 = TIMER0L;
	TF0 = 0;	
	TMOD |= 0x01;
	
	ET2 = 1;                      /* Enable Timer 2 Interrupts */
	EA = 1;                       /* Global Interrupt Enable */












/************************
**        Loop         **
************************/

	
	while(1)
	{	
		sector_base_g = FirstRootDirSec;
		while(error_flag == NO_ERRORS)
		{
			do
			{
				printf("%-35s", "Enter a Block Number: ");
				block_number = long_serial_input();
				if(block_number > number_of_entries || block_number == 0)
				{
					printf("%-35s", "Number too large.\n\r");
				}
			}while(block_number > number_of_entries || block_number == 0);
			return_entry = Read_Dir_Entry(sector_base_g, block_number, buff1);
			printf("%-20s", "return_entry");
			printf("%8.8lX", return_entry);
			putchar(10);
			putchar(13);
			next_entry = return_entry & 0x0FFFFFFF;
			if(return_entry & 0x10000000)
			{			
				printf("%-20s", "was a directory");
				putchar(10);
				putchar(13);
				sector_base_g = first_sector(next_entry);
				printf("%-20s", "sector_base_g");
				printf("%8.8lX", sector_base_g);
				putchar(10);
				putchar(13);
				number_of_entries = Print_Directory(sector_base_g, buff1);
			}
			if((return_entry & 0x10000000) == 0 && block_number !=0)
			{
				printf("%-20s", "was a file");
				Open_File(next_entry, buff1);
				sector_base_g = first_sector(next_entry);
				sector = sector_base_g + sector_offset;
				read_sector_ISR(sector, buff1);
				sector_offset++;
				sector = sector_base_g + sector_offset;
				read_sector_ISR(sector, buff2);
				sector_offset++;
				state_g = DATA_SEND_1;


				
				play_status = 1; // Run
				TF2 = 0;
				TR2 = 1;
				index1_g = 0;
				index2_g = 0;
				while(play_status != 0)
				{	
					
					PCON = 1; // Idle mode???
					
				}
				TR2 = 0;
				
				number_of_entries = Print_Directory(sector_base_g, buff1);
			}
			if(return_entry & 0x80000000)
			{
				error_flag = PRINT_ERROR;
			}
		}
		if(error_flag!= NO_ERRORS)
		{
			LED4 = 0;
			putchar(10);
			putchar(13);
			printf("%-10s", "ERROR! ");
			printf("%2.2bX", error_flag);
			putchar(10);
			putchar(13);
			while(1);
		}
	}
}
		
		
		
		
		





		
		
		
		
		
//		
//		
//		if(state_g == LOAD_BUFFER_1)
//		{
//			if(sector_offset >= 64)
//			{
//				state_g = FIND_CLUSTER_1;
//			}
//		}

//		if(state_g == LOAD_BUFFER_2)
//		{
//			if(sector_offset >= 64)
//			{
//				state_g = FIND_CLUSTER_2;
//			}
//		}


//		switch(state_g)
//		{
//			// Data Send 1
//			case DATA_SEND_1:
//			{
//				spi_en = 1;
//				while((DATA_REQ == ACTIVE) && (TF0 == 0))  								// Can DATA_REQ go inactive while in the loop
//				{
//					SPI_transfer_ISR(buff1[index1_g], & temp8); 							// What is temp8
//					index1_g++;
//					if(index1_g > 511) // Buffer 1 empty
//					{
//						if(play_status == 3)																	// Works only if after FIND_CLUSTER. why?
//						{
//							play_status = 0;
//							// state_g = DATA_SEND_2; 													// I think this is where it goes
//						}
//						else
//						{
//							state_g = LOAD_BUFFER_2; // Buff 2 and Buff 1 empty
//						}
//					}																												// No FIND_CLUSTER
//					else
//					{
//						state_g = DATA_SEND_2; // BUFF 1 empty  							//Seems like this is when buffer 1 still has stuff
//					}
//					TF0 = 1;																								// What does this do? is this the interupt for the whole system to keep pace
//				}
//				if((DATA_REQ == INACTIVE) && (state_g == DATA_SEND_1))
//				{
//					if(index2_g > 511) // Buffer 2 is empty
//					{
//						state_g = LOAD_BUFFER_2; // DR inactive and BUFF 2 empty
//					}
//					else
//					{
//						state_g = DATA_IDLE_1; // DR interupt
//					}
//				}
//				spi_en = 0;
//				break;
//			}
//			
//			//Load Buffer 1
//			case LOAD_BUFFER_1:
//			{
//				sector = sector_base_g + sector_offset;
//				read_sector_ISR(sector, buff1);
//				sector_offset++;
//				state_g = DATA_IDLE_2;
//				break;
//			}
//			
//			// Find Cluster 1
//			case FIND_CLUSTER_1:
//			{
//				cluster_g = find_next_cluster_ISR(cluster_g, buff1);
//				if(cluster_g == 0x0FFFFFFF)
//				{
//					play_status = 3;
//					state_g = DATA_IDLE_2;
//				}
//				else
//				{
//					sector_base_g = first_sector_ISR(cluster_g);
//					sector_offset = 0;
//					state_g = DATA_IDLE_2;
//				}
//				break;
//			}
//			
//			case DATA_IDLE_1:
//			{
//				if(DATA_REQ == ACTIVE)
//				{
//					state_g = DATA_SEND_1;
//				}
//				break;
//			}
//					
//			// Data Send 2
//			case DATA_SEND_2:
//			{
//				spi_en = 1;
//				while((DATA_REQ == ACTIVE) && (TF0 == 0))  								// Can DATA_REQ go inactive while in the loop
//				{
//					SPI_transfer_ISR(buff1[index2_g], & temp8); 							// What is temp8
//					index2_g++;
//					if(index2_g > 511) // Buffer 2 empty
//					{
//						if(play_status == 3)																	// Works only if after FIND_CLUSTER. why?
//						{
//							play_status = 0;
//						}
//						else
//						{
//							state_g = LOAD_BUFFER_1; // Buff 1 and Buff 2 empty
//						}
//					}																												// No FIND_CLUSTER
//					else
//					{
//						state_g = DATA_SEND_1; // BUFF 2 empty  							//Seems like this is when buffer 2 still has stuff
//					}
//					TF0 = 1;																								// What does this do? is this the interupt for the whole system to keep pace
//				}
//				if((DATA_REQ == INACTIVE) && (state_g == DATA_SEND_2))
//				{
//					if(index1_g > 511) // Buffer 1 is empty
//					{
//						state_g = LOAD_BUFFER_1; // DR inactive and BUFF 1 empty
//					}
//					else
//					{
//						state_g = DATA_IDLE_2; // DR interupt
//					}
//				}
//				spi_en = 0;
//				break;
//			}
//			
//			//Load Buffer 2
//			case LOAD_BUFFER_2:
//			{
//				sector = sector_base_g + sector_offset;
//				read_sector_ISR(sector, buff2);
//				sector_offset++;
//				state_g = DATA_IDLE_1;
//				break;
//			}
//			
//			// Find Cluster 2
//			case FIND_CLUSTER_2:
//			{
//				cluster_g = find_next_cluster_ISR(cluster_g, buff2);
//				if(cluster_g == 0x0FFFFFFF)
//				{
//					play_status = 3;
//					state_g = DATA_IDLE_1;
//				}
//				else
//				{
//					sector_base_g = first_sector_ISR(cluster_g);
//					sector_offset = 0;
//					state_g = DATA_IDLE_1;
//				}
//				break;
//			}
//			
//			case DATA_IDLE_2:
//			{
//				if(DATA_REQ == ACTIVE)
//				{
//					state_g = DATA_SEND_2;
//				}
//				break;
//			}
//			
//		}		
//	}
//}	
//	










	
//	while(1)
//	{
//		sector_base_g = FirstRootDirSec;
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
//			return_entry = Read_Dir_Entry(sector_base_g, block_number, block_info);
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
//				sector_base_g = first_sector(next_entry);
//				printf("%-20s", "sector_base_g");
//				printf("%8.8lX", sector_base_g);
//				putchar(10);
//				putchar(13);
//				number_of_entries = Print_Directory(sector_base_g, block_info);
//			}
//			if((return_entry & 0x10000000) == 0 && block_number !=0)
//			{
//				printf("%-20s", "was a file");
//				Open_File(next_entry, block_info);
//				number_of_entries = Print_Directory(sector_base_g, block_info);
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

