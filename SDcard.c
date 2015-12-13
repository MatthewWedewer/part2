#include "main.h"
#include "port.h"
#include "SDcard.h"
#include "spi.h"
#include "lcd.h"
#include "hardware_delay.h"
#include "print_bytes.h"
#include <stdio.h>


uint8_t SDcard_init()
{
	uint8_t return_value[5];
	uint8_t error_flag, index, dat;
	
	AUXR = 0x0C;
	//Buffer ?
	ncs = 1;
	for(index = 0; index < 10; index++)
	{
		SPI_transfer(0xFF, return_value);
	}
	
	
	// Send CMD0 //
	
	error_flag = trans_CMD0(return_value);
	
	
	for (index=0;index<5;index++)
    {
       dat=return_value[index];
       printf("%2.2bX ",dat);
    }
	putchar(10);
	putchar(13);
	delay_ms(100);
	


	// Send CMD8 //
	if(error_flag == NO_ERRORS)
	{
		error_flag = trans_CMD8(return_value);
	}
	


	for (index=0;index<5;index++)
    {
       dat=return_value[index];
       printf("%2.2bX ",dat);
    }
	putchar(10);
	putchar(13);
	delay_ms(100);

	// Send CMD58 //
	if(error_flag == NO_ERRORS)
	{
		error_flag = trans_CMD58(return_value);
		if(return_value[0] != 0x01)
		{
			error_flag = WRONG_RESPONSE;
			LED2 = 0;
		}
	}
	


	for (index=0;index<5;index++)
    {
       dat=return_value[index];
       printf("%2.2bX ",dat);
    }
	putchar(10);
	putchar(13);
	delay_ms(100);

	// Send ACMD41 //
	if(error_flag == NO_ERRORS)
	{
		error_flag = trans_ACMD41(return_value);
	}



	for (index=0;index<5;index++)
    {
       dat=return_value[index];
       printf("%2.2bX ",dat);
    }
	putchar(10);
	putchar(13);
	delay_ms(100);
	
	// Send CMD58 //
	if(error_flag == NO_ERRORS)
	{
		error_flag = trans_CMD58(return_value);
		if(return_value[1] & 0xC0) //bit 31 and 30 are set
		{
			//High Capacity
		}
		else if(return_value[1] & 0x80) // only bit 31 is set
		{
			//Standard Capacity
			error_flag = WRONG_SDCARD;
		}
		else
		{
			error_flag = WRONG_RESPONSE;
		}
	}


	for (index=0;index<5;index++)
    {
       dat=return_value[index];
       printf("%2.2bX ",dat);
    }
	putchar(10);
	putchar(13);
	delay_ms(100);
	
	if(error_flag != NO_ERRORS) // LED4 is the error light and this should also be redundent.
	{
		LED4 = 0;
	}
	ncs = 1; // should not be needed but just in case
	
	return error_flag; 
}
















uint8_t trans_CMD0(uint8_t *return_value)
{
	uint8_t error_flag;

	// Send CMD0
	ncs = 0;
	error_flag = send_command(0, 0);
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response(1, return_value);
	}
	if(error_flag == NO_ERRORS)
	{
		if(return_value[0] != 0x01)
		{
			error_flag = WRONG_RESPONSE;
		}
	}
	ncs = 1;
	
	return error_flag;
}





uint8_t trans_CMD8(uint8_t *return_value)
{
	uint8_t error_flag;
	// Send CMD8
	ncs = 0;
	error_flag = send_command(8, 0x000001AA);
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response(5, return_value);
	}
	if(return_value[0] == 0x05)
		error_flag = VERSION_1_SD;
	else if(return_value[0] != 0x01)
		error_flag = FAIL_SDINIT;
	ncs = 1;
	return error_flag;
}





uint8_t trans_CMD58(uint8_t *return_value)
{
	uint8_t error_flag;
	// SEND CMD58
	ncs = 0;
	error_flag = send_command(58, 0);
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response(5, return_value);
	}
	return error_flag;
}




uint8_t trans_ACMD41(uint8_t *return_value)
{
	uint8_t error_flag, timeout;
	uint32_t ACMD41_argum = 0x40000000;

	
	// Send ACMD41
	
	ncs = 0;
	

	timeout = 0;
	do
	{
		timeout++;
		error_flag = send_command(55, 0);
		if(error_flag == NO_ERRORS)
			error_flag = get_response(1, return_value);
		if(!(return_value[0] == 0x00 || return_value[0] == 0x01))
			error_flag = SPI_ERROR;
		if(error_flag == NO_ERRORS)
			error_flag = send_command(41, ACMD41_argum);
		if(error_flag == NO_ERRORS)
			error_flag = get_response(1, return_value);
		if(!(return_value[0] == 0x00 || return_value[0] == 0x01))
			error_flag = SPI_ERROR;
		timeout++;
	}while(return_value[0] != 0x00 && timeout != 0 && error_flag == NO_ERRORS);
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	ncs = 1;
	
	return error_flag;
}









uint8_t send_command(uint8_t cmd, uint32_t argum)
{
	uint8_t error_flag, send_val, return_val, index;
	if(cmd < 64)
	{
		send_val = 0x40 | cmd;
		error_flag = SPI_transfer(send_val, &return_val);
		index = 24;
		while((error_flag == NO_ERRORS)&(index<25))
		{
			send_val = (uint8_t)(argum >> index);
			error_flag = SPI_transfer(send_val, &return_val);
			index -= 8;
		}
		if(cmd == 0)
			send_val = 0x95;
		else if(cmd == 8)
			send_val = 0x87;
		else
			send_val = 0x01;
		if(error_flag == NO_ERRORS)
			error_flag = SPI_transfer(send_val, &return_val);
		if(error_flag != NO_ERRORS)
			error_flag = SPI_ERROR;
	}
	else
		error_flag = ILLEGAL_COMMAND;
	return error_flag;
}





uint8_t send_command_ISR(uint8_t cmd, uint32_t argum)
{
	uint8_t idata error_flag, send_val, return_val, index;
	if(cmd < 64)
	{
		send_val = 0x40 | cmd;
		error_flag = SPI_transfer_ISR(send_val, &return_val);
		index = 24;
		while((error_flag == NO_ERRORS)&(index<25))
		{
			send_val = (uint8_t)(argum >> index);
			error_flag = SPI_transfer_ISR(send_val, &return_val);
			index -= 8;
		}
		if(cmd == 0)
			send_val = 0x95;
		else if(cmd == 8)
			send_val = 0x87;
		else
			send_val = 0x01;
		if(error_flag == NO_ERRORS)
			error_flag = SPI_transfer_ISR(send_val, &return_val);
		if(error_flag != NO_ERRORS)
			error_flag = SPI_ERROR;
	}
	else
		error_flag = ILLEGAL_COMMAND;
	return error_flag;
}



uint8_t get_response(uint8_t num_bytes, uint8_t *array_out)
{
	uint8_t timeout, error_flag, recieve_value;
	uint8_t index;
	timeout = 0;
	error_flag = NO_ERRORS;
	do
	{
		error_flag = SPI_transfer(0xFF, &recieve_value);
		timeout++;
	}while((timeout!=0)&&(error_flag == NO_ERRORS)&&(recieve_value == 0xFF));
	*array_out = recieve_value;
	if(timeout == 0)
		error_flag = TIMEOUT_ERROR;
	else if(error_flag != NO_ERRORS)
		error_flag = SPI_ERROR;
	else if((recieve_value != 0x01)&&(recieve_value != 0x00))
		error_flag = SPI_ERROR;
	else if(num_bytes > 1)
	{
		for(index = 1; index < num_bytes; index++)
		{
			SPI_transfer(0xFF, &recieve_value);
			*(array_out+index) = recieve_value;
		}
	}
	if(error_flag == NO_ERRORS)
		error_flag = SPI_transfer(0xFF, &recieve_value);
	return error_flag;
}





uint8_t get_response_no_end(uint16_t num_bytes, uint8_t *array_out)
{
	uint8_t timeout, error_flag, recieve_value;
	uint16_t index;
	timeout = 0;
	error_flag = NO_ERRORS;
	for(index = 0; index < num_bytes; index++)
	{
		SPI_transfer(0xFF, &recieve_value);
		array_out[index] = recieve_value;
	}
	return error_flag;
}




uint8_t get_response_no_end_ISR(uint16_t num_bytes, uint8_t *array_out)
{
	uint8_t idata timeout, error_flag, recieve_value;
	uint16_t idata index;
	timeout = 0;
	error_flag = NO_ERRORS;
	for(index = 0; index < num_bytes; index++)
	{
		SPI_transfer_ISR(0xFF, &recieve_value);
		array_out[index] = recieve_value;
	}
	return error_flag;
}







uint8_t read_block(uint32_t block_number, uint8_t *block_info)
{
	uint8_t error_flag, timeout;
	uint8_t return_value[5];
	ncs = 0;
	timeout = 0;
	error_flag = send_command(17, block_number);
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0x00 && timeout != 0);


	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	timeout = 0;
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0xFE    && timeout != 0 && error_flag == NO_ERRORS);
	

	
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	if((return_value[0] & 0xF0 )== 0x00)
	{
		error_flag = SDCARD_ERROR;
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(512, block_info);
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(3, &return_value);
//		CRC16 = return_value[0] * 256 + return_value[1]; Check sum, dont care
	}
	ncs = 1;
	
	return error_flag;
	
}


uint8_t read_block_ISR(uint32_t block_number, uint8_t *block_info)
{
	uint8_t idata error_flag, timeout;
	uint8_t idata return_value[5];
	ncs = 0;
	timeout = 0;
	error_flag = send_command_ISR(17, block_number);
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end_ISR(1, &return_value);
	}while(return_value[0] != 0x00 && timeout != 0);


	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	timeout = 0;
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end_ISR(1, &return_value);
	}while(return_value[0] != 0xFE    && timeout != 0 && error_flag == NO_ERRORS);
	

	
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	if((return_value[0] & 0xF0 )== 0x00)
	{
		error_flag = SDCARD_ERROR;
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end_ISR(512, block_info);
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end_ISR(3, &return_value);
//		CRC16 = return_value[0] * 256 + return_value[1]; Check sum, dont care
	}
	ncs = 1;
	
	return error_flag;
	
}



uint8_t read_sector(uint32_t block_number, uint8_t *block_info)
{
	uint8_t error_flag, timeout;
	uint8_t return_value[5];
	ncs = 0;
	timeout = 0;
	error_flag = send_command(17, block_number);
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0x00 && timeout != 0);


	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	timeout = 0;
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0xFE    && timeout != 0 && error_flag == NO_ERRORS);
	

	
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	if((return_value[0] & 0xF0 )== 0x00)
	{
		error_flag = SDCARD_ERROR;
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(512, block_info);
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(3, &return_value);
//		CRC16 = return_value[0] * 256 + return_value[1]; Check sum, dont care
	}
	ncs = 1;
	
	return error_flag;
	
}





uint8_t read_sector_ISR(uint32_t block_number, uint8_t *block_info)
{
	uint8_t error_flag, timeout;
	uint8_t return_value[5];
	ncs = 0;
	timeout = 0;
	error_flag = send_command_ISR(17, block_number);
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end_ISR(1, &return_value);
	}while(return_value[0] != 0x00 && timeout != 0);


	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	timeout = 0;
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end_ISR(1, &return_value);
	}while(return_value[0] != 0xFE    && timeout != 0 && error_flag == NO_ERRORS);
	

	
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	if((return_value[0] & 0xF0 )== 0x00)
	{
		error_flag = SDCARD_ERROR;
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end_ISR(512, block_info);
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end_ISR(3, &return_value);
//		CRC16 = return_value[0] * 256 + return_value[1]; Check sum, dont care
	}
	ncs = 1;
	
	return error_flag;
	
}











